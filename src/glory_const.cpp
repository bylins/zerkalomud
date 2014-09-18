// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2010 Krodo
// Part of Bylins http://www.mud.ru

#include <list>
#include <map>
#include <string>
#include <iomanip>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/array.hpp>
#include "pugixml.hpp"
#include <boost/algorithm/string.hpp>

#include "glory_const.hpp"
#include "structs.h"
#include "screen.h"
#include "char.hpp"
#include "comm.h"
#include "db.h"
#include "genchar.h"
#include "handler.h"
#include "char_player.hpp"
#include "glory_misc.hpp"

extern void add_karma(CHAR_DATA * ch, char const * punish , const char * reason);
extern void check_max_hp(CHAR_DATA *ch);

namespace GloryConst
{

enum
{
	GLORY_STR = 0, // +����� G_STR..G_CHA
	GLORY_DEX,
	GLORY_INT,
	GLORY_WIS,
	GLORY_CON,
	GLORY_CHA, // -//-
	GLORY_HIT, // +��
	GLORY_SUCCESS, //����
	GLORY_WILL, //����
	GLORY_STABILITY, //���������
	GLORY_REFLEX, //�������
	GLORY_MIND, //�����
	GLORY_TOTAL
};

struct glory_node
{
	glory_node() : free_glory(0) {};
	// ��������� �����
	int free_glory;
	// ��� ���� ��� ���� �������������
	std::string name;
	// ������ ������ � ���������� ������
	std::map<int /* ����� ���� �� enum */, int /* ������� ����� ����� �������*/> stats;
};

// ����� ������ ��������� � ��������� �����
typedef boost::shared_ptr<glory_node> GloryNodePtr;
typedef std::map<long /* ��� ���� */, GloryNodePtr> GloryListType;
GloryListType glory_list;
// ��������� ��������� � ���� ������ ���-�� �����
int total_charge = 0;
// ����������� � ���������
int total_spent = 0;

struct glory_olc
{
	glory_olc() : olc_free_glory(0), olc_was_free_glory(0)
	{
		for (int i = 0; i < GLORY_TOTAL; ++i)
		{
			stat_cur[i] = 0;
			stat_add[i] = 0;
			stat_was[i] = 0;
		}
	};

	boost::array<int, GLORY_TOTAL> stat_cur;
	boost::array<int, GLORY_TOTAL> stat_add;
	boost::array<int, GLORY_TOTAL> stat_was;

	int olc_free_glory;
	int olc_was_free_glory;
};

// ������� �������� �� ��������� ����� ��� ���������
const int STAT_RETURN_FEE = 10;
// ����� �� ������� ����� (��������)
const int TRANSFER_FEE = 5;
// ����������� ����� �� ������� ����� (�� ��������)
const int MIN_TRANSFER_TAX = 50;
// ����������� ���-�� ����� ��� �������� (>= MIN_TRANSFER_TAX)
const int MIN_TRANSFER_AMOUNT = 100;

//���-�� ������ �����, ����������� �� ���
const int HP_FACTOR=50;

//����
const int SUCCESS_FACTOR=10;

//������
const int SAVE_FACTOR = 15;

//�������
const int RESIST_FACTOR = 7;

const char *olc_stat_name[] =
{
	"����",
	"��������",
	"��",
	"��������",
	"��������",
	"�������",
	"����.�����",
	"�����.����������",
	"����",
	"���������",
	"�������",
	"�����"
};

void transfer_log(const char *format, ...)
{
	const char *filename = "../log/glory_transfer.log";

	FILE *file = fopen(filename, "a");
	if (!file)
	{
		log("SYSERR: can't open %s!", filename);
		return;
	}

	if (!format)
		format = "SYSERR: imm_log received a NULL format.";

	write_time(file);
	va_list args;
	va_start(args, format);
	vfprintf(file, format, args);
	va_end(args);
	fprintf(file, "\n");

	fclose(file);
}

/**
* ������ ������� ������� GET_GLORY().
*/
int get_glory(long uid)
{
	int glory = 0;
	GloryListType::iterator it = glory_list.find(uid);
	if (it != glory_list.end())
	{
		glory = it->second->free_glory;
	}
	return glory;
}

/**
* ���������� ����� ����, �������� ����� ������ ��� �������������, �����������, ���� ��� ������.
*/
void add_glory(long uid, int amount)
{
	if (uid <= 0 || amount <= 0)
	{
		return;
	}

	GloryListType::iterator it = glory_list.find(uid);
	if (it != glory_list.end())
	{
		it->second->free_glory += amount;
	}
	else
	{
		GloryNodePtr temp_node(new glory_node);
		temp_node->free_glory = amount;
		glory_list[uid] = temp_node;
	}

	DESCRIPTOR_DATA *d = DescByUID(uid);
	if (d)
	{
		send_to_char(d->character, "%s�� ��������� %d %s ���������� �����!%s\r\n",
			CCGRN(d->character, C_NRM),
			amount, desc_count(amount, WHAT_POINT),
			CCNRM(d->character, C_NRM));
	}
	save();
}

/**
* ���������� '����� ����������'.
*/
void print_glory(CHAR_DATA *ch, GloryListType::iterator &it)
{
	*buf = '\0';
	for (std::map<int, int>::const_iterator i = it->second->stats.begin(), iend = it->second->stats.end(); i != iend; ++i)
	{
		if ((i->first >= 0) && (i->first < (int)sizeof(olc_stat_name))) {
			//out << olc_stat_name[i->first] << ": +";
			sprintf(buf+strlen(buf), "%-16s: +%d\r\n", olc_stat_name[i->first], i->second);
		}
		else
		{
			log("Glory: ������������ ����� ����� %d (uid: %ld)", i->first, it->first);
		}
	}
	sprintf(buf+strlen(buf), "��������� ����� : %d\r\n", it->second->free_glory);
	send_to_char(buf, ch);
	return;
}

/**
* ����� ��������� � ��������� ����� � ���� (glory ���).
*/
void print_to_god(CHAR_DATA *ch, CHAR_DATA *god)
{
	GloryListType::iterator it = glory_list.find(GET_UNIQUE(ch));
	if (it == glory_list.end())
	{
		send_to_char(god, "� %s ������ �� �����.\r\n", GET_PAD(ch, 1));
		return;
	}

	send_to_char(god, "���������� �� ����� ����� %s:\r\n", GET_PAD(ch, 1));
	print_glory(god, it);
	return;
}

int add_stat_cost(int stat, boost::shared_ptr<GloryConst::glory_olc> olc)
{
	if (stat < 0 || stat >= GLORY_TOTAL)
	{
		log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
		return 0;
	}

	int glory = 0;
	switch(stat)
	{
		case GLORY_STR:
		case GLORY_DEX:
		case GLORY_INT:
		case GLORY_WIS:
		case GLORY_CON:
		case GLORY_CHA:
			glory = (olc->stat_add[stat] * 200) + 1000;
			break;
		case GLORY_HIT:
			glory = (olc->stat_add[stat]/HP_FACTOR * 200) + 1000;
			break;
		case GLORY_SUCCESS:
			glory = (olc->stat_add[stat]/SUCCESS_FACTOR * 200) + 1000;
			break;
		case GLORY_WILL:
		case GLORY_STABILITY:
		case GLORY_REFLEX:
			glory = (olc->stat_add[stat]/SAVE_FACTOR * 200) + 1000;
			break;
		case GLORY_MIND:
			glory = (olc->stat_add[stat]/RESIST_FACTOR * 200) + 1000;
			break;
		default:
			log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
	}
	if (olc->stat_was[stat] - olc->stat_add[stat] > 0)
		glory -= glory * STAT_RETURN_FEE / 100;
	return glory;
}

int remove_stat_cost(int stat, boost::shared_ptr<GloryConst::glory_olc> olc)
{
	if (stat < 0 || stat >= GLORY_TOTAL)
	{
		log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
		return 0;
	}
	if (!olc->stat_add[stat])
	{
		return 0;
	}

	int glory = 0;
	switch(stat)
	{
		case GLORY_STR:
		case GLORY_DEX:
		case GLORY_INT:
		case GLORY_WIS:
		case GLORY_CON:
		case GLORY_CHA:
			glory = ((olc->stat_add[stat] -1) * 200) + 1000;
			break;
		case GLORY_HIT:
			glory = ((olc->stat_add[stat] -1)/HP_FACTOR * 200) + 1000;
			break;
		case GLORY_SUCCESS:
			glory = ((olc->stat_add[stat] -1)/SUCCESS_FACTOR * 200) + 1000;
			break;
		case GLORY_WILL:
		case GLORY_STABILITY:
		case GLORY_REFLEX:
			glory = ((olc->stat_add[stat] -1)/SAVE_FACTOR * 200) + 1000;
			break;
		case GLORY_MIND:
			glory = ((olc->stat_add[stat] -1)/RESIST_FACTOR * 200) + 1000;
			break;
		default:
			log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
	}
	if (olc->stat_was[stat] - olc->stat_add[stat] >= 0)
		glory -= glory * STAT_RETURN_FEE / 100;
	return glory;
}

const char *olc_del_name[] =
{
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
};

const char *olc_add_name[] =
{
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
	"�",
};

std::string olc_print_stat(CHAR_DATA *ch, int stat)
{
	if (stat < 0 || stat >= GLORY_TOTAL)
	{
		log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
		return "";
	}

	return boost::str(boost::format("  %-16s :  %s(+%5d)%s  (%s%s%s) %2d (%s%s%s)  %s(-%5d)  | %d%s\r\n")
		% olc_stat_name[stat]
		% CCINRM(ch, C_NRM)
		% remove_stat_cost(stat, ch->desc->glory_const)
		% CCNRM(ch, C_NRM)
		% CCIGRN(ch, C_NRM) % olc_del_name[stat] % CCNRM(ch, C_NRM)
		% (ch->desc->glory_const->stat_cur[stat] + ch->desc->glory_const->stat_add[stat])
		% CCIGRN(ch, C_NRM) % olc_add_name[stat] % CCNRM(ch, C_NRM)
		% CCINRM(ch, C_NRM)
		% add_stat_cost(stat, ch->desc->glory_const)
		% (ch->desc->glory_const->stat_add[stat] > 0 ? std::string("+")
			+ boost::lexical_cast<std::string>(ch->desc->glory_const->stat_add[stat]) : "")
		% CCNRM(ch, C_NRM));
}

/**
* ���������� ��� ����.
*/
void spend_glory_menu(CHAR_DATA *ch)
{
	std::ostringstream out;
	out << "\r\n                         -      +\r\n";

	out << olc_print_stat(ch, GLORY_STR)
		<< olc_print_stat(ch, GLORY_DEX)
		<< olc_print_stat(ch, GLORY_INT)
		<< olc_print_stat(ch, GLORY_WIS)
		<< olc_print_stat(ch, GLORY_CON)
		<< olc_print_stat(ch, GLORY_CHA)
		<< olc_print_stat(ch, GLORY_HIT)
		<< olc_print_stat(ch, GLORY_SUCCESS)
		<< olc_print_stat(ch, GLORY_WILL)
		<< olc_print_stat(ch, GLORY_STABILITY)
		<< olc_print_stat(ch, GLORY_REFLEX)
		<< olc_print_stat(ch, GLORY_MIND);

	out << "\r\n  ��������� �����: " << ch->desc->glory_const->olc_free_glory << "\r\n\r\n";

	if (ch->desc->glory_const->olc_free_glory != ch->desc->glory_const->olc_was_free_glory)
	{
		out << "  " << CCIGRN(ch, C_SPR) << "�" << CCNRM(ch, C_SPR)
			<< ") ��������� ����������\r\n";
	}
	out << "  " << CCIGRN(ch, C_SPR) << "�" << CCNRM(ch, C_SPR)
		<< ") ����� ��� ����������\r\n"
		<< "  ��� �����: ";
	send_to_char(out.str(), ch);
}

void olc_del_stat(CHAR_DATA *ch, int stat)
{
	int REMOVE_AMOUNT = 0;
	switch(stat)
	{
		case GLORY_STR:
		case GLORY_DEX:
		case GLORY_INT:
		case GLORY_WIS:
		case GLORY_CON:
		case GLORY_CHA:
			REMOVE_AMOUNT = 1;
			break;
		case GLORY_HIT:
			REMOVE_AMOUNT = HP_FACTOR;
			break;
		case GLORY_SUCCESS:
			REMOVE_AMOUNT = SUCCESS_FACTOR;
			break;
		case GLORY_WILL:
		case GLORY_STABILITY:
		case GLORY_REFLEX:
			REMOVE_AMOUNT = SAVE_FACTOR;
			break;
		case GLORY_MIND:
			REMOVE_AMOUNT = RESIST_FACTOR;
			break;
		default:
			log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
	}
	if (ch->desc->glory_const->stat_add[stat] > 0)
	{
		ch->desc->glory_const->olc_free_glory +=
			remove_stat_cost(stat, ch->desc->glory_const);
		ch->desc->glory_const->stat_add[stat] -= REMOVE_AMOUNT;
	}
}

void olc_add_stat(CHAR_DATA *ch, int stat)
{
	int need_glory = add_stat_cost(stat, ch->desc->glory_const);
	switch(stat)
	{
		case GLORY_CON:
		{
			if (ch->desc->glory_const->olc_free_glory >= need_glory)
			{
				ch->desc->glory_const->olc_free_glory -= need_glory;
				ch->desc->glory_const->stat_add[stat] += 1;
			}
			break;
		}
		case GLORY_STR:
		case GLORY_DEX:
		case GLORY_INT:
		case GLORY_WIS:
		case GLORY_CHA:
		{
			if (ch->desc->glory_const->olc_free_glory >= need_glory
				&& ch->desc->glory_const->stat_cur[stat]
					+ ch->desc->glory_const->stat_add[stat] < 50)
			{
				ch->desc->glory_const->olc_free_glory -= need_glory;
				ch->desc->glory_const->stat_add[stat] += 1;
			}
			break;
		}
		case GLORY_HIT:
		if (ch->desc->glory_const->olc_free_glory >= need_glory)
		{
			ch->desc->glory_const->olc_free_glory -= need_glory;
			ch->desc->glory_const->stat_add[stat] += HP_FACTOR;
		}
		break;
		case GLORY_SUCCESS:
		if (ch->desc->glory_const->olc_free_glory >= need_glory)
		{
			ch->desc->glory_const->olc_free_glory -= need_glory;
			ch->desc->glory_const->stat_add[stat] += SUCCESS_FACTOR;
		}
		break;
		case GLORY_WILL:
		case GLORY_STABILITY:
		case GLORY_REFLEX:
		if (ch->desc->glory_const->olc_free_glory >= need_glory)
		{
			ch->desc->glory_const->olc_free_glory -= need_glory;
			ch->desc->glory_const->stat_add[stat] += SAVE_FACTOR;
		}
		break;
		case GLORY_MIND:
		if (ch->desc->glory_const->olc_free_glory >= need_glory)
		{
			ch->desc->glory_const->olc_free_glory -= need_glory;
			ch->desc->glory_const->stat_add[stat] += RESIST_FACTOR;
		}
		break;
		default:
			log("SYSERROR : bad stat %d (%s:%d)", stat, __FILE__, __LINE__);
	}
}

int olc_real_stat(CHAR_DATA *ch, int stat)
{
	return ch->desc->glory_const->stat_cur[stat]
		+ ch->desc->glory_const->stat_add[stat];
}

int calculate_glory_in_stats(GloryListType::const_iterator &i)
{
	int total = 0;
	for (std::map<int, int>::const_iterator k = i->second->stats.begin(),
		kend = i->second->stats.end(); k != kend; ++k)
	{
		int factor = 0;
		switch(k->first)
		{
			case GLORY_STR:
			case GLORY_DEX:
			case GLORY_INT:
			case GLORY_WIS:
			case GLORY_CON:
			case GLORY_CHA:
				factor = 1;
				break;
			case GLORY_HIT:
				factor = HP_FACTOR;
				break;
			case GLORY_SUCCESS:
				factor = SUCCESS_FACTOR;
				break;
			case GLORY_WILL:
			case GLORY_STABILITY:
			case GLORY_REFLEX:
				factor = SAVE_FACTOR;
				break;
			case GLORY_MIND:
				factor = RESIST_FACTOR;
				break;
				}
		for (int m = 0; m < k->second; m+=factor)
			total += (m/factor * 200) + 1000;
	}
	return total;
}

bool parse_spend_glory_menu(CHAR_DATA *ch, char *arg)
{
	switch (*arg)
	{
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_STR);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_DEX);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_INT);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_WIS);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_CON);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_CHA);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_HIT);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_SUCCESS);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_WILL);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_STABILITY);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_REFLEX);
			break;
		case '�':
		case '�':
			olc_del_stat(ch, GLORY_MIND);
			break;
		case '�':
			olc_add_stat(ch, GLORY_STR);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_DEX);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_INT);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_WIS);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_CON);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_CHA);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_HIT);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_SUCCESS);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_WILL);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_STABILITY);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_REFLEX);
			break;
		case '�':
		case '�':
			olc_add_stat(ch, GLORY_MIND);
			break;
			case '�':
		case '�':
		{
			// ������������ �����
			ch->set_str(olc_real_stat(ch, GLORY_STR));
			ch->set_dex(olc_real_stat(ch, GLORY_DEX));
			ch->set_int(olc_real_stat(ch, GLORY_INT));
			ch->set_wis(olc_real_stat(ch, GLORY_WIS));
			ch->set_con(olc_real_stat(ch, GLORY_CON));
			ch->set_cha(olc_real_stat(ch, GLORY_CHA));
			// ���������� ������ � ������ �����
			GloryListType::const_iterator it = glory_list.find(GET_UNIQUE(ch));
			if (glory_list.end() == it)
			{
				log("SYSERROR : ��� ������ ���� ��� ������ �� ��� ���������� ����� name=%s (%s:%d)",
					ch->get_name(), __FILE__, __LINE__);
				send_to_char("������ ����������, �������� �����!\r\n", ch);
				ch->desc->glory_const.reset();
				STATE(ch->desc) = CON_PLAYING;
				return 1;
			}
			// ����� ����� ��������������� (��� ������� ������)
			int was_glory = it->second->free_glory + calculate_glory_in_stats(it);
			// ���������� ��������� ������
			it->second->free_glory = ch->desc->glory_const->olc_free_glory;
			it->second->stats.clear();
			for (int i = 0; i < GLORY_TOTAL; ++i)
			{
				if (ch->desc->glory_const->stat_add[i] > 0)
				{
					it->second->stats[i] = ch->desc->glory_const->stat_add[i];
				}
			}
			// ������ ������� ������
			int now_glory = it->second->free_glory + calculate_glory_in_stats(it);
			if (was_glory < now_glory)
			{
				log("SYSERROR : �������� ���������� ����� ����� �������������� � ��� (%d -> %d) name=%s (%s:%d)",
					was_glory, now_glory, ch->get_name(), __FILE__, __LINE__);
			}
			else
			{
				total_charge += was_glory - now_glory;
			}
			// ����� �� ��� � ����������� ��
			ch->desc->glory_const.reset();
			STATE(ch->desc) = CON_PLAYING;
			check_max_hp(ch);
			send_to_char("���� ��������� ���������.\r\n", ch);
			ch->save_char();
			save();
			return 1;
		}
		case '�':
		case '�':
			ch->desc->glory_const.reset();
			STATE(ch->desc) = CON_PLAYING;
			send_to_char("�������������� ��������.\r\n", ch);
			return 1;
		default:
			break;
	}
	return 0;
}

const char *GLORY_CONST_FORMAT =
	"������: �����2\r\n"
	"        �����2 ����������\r\n"
	"        �����2 ��������� <���> <���-��>\r\n";

ACMD(do_spend_glory)
{
	GloryListType::iterator it = glory_list.find(GET_UNIQUE(ch));
	// �� ����������� ����
	//send_to_char("�������� ���������...\r\n", ch);
	//return;
	if (glory_list.end() == it || IS_IMMORTAL(ch))
	{
		send_to_char("��� ��� �� �����...\r\n", ch);
		return;
	}

	std::string buffer = argument, buffer2;
	GetOneParam(buffer, buffer2);

	if (buffer2.empty())
	{
		if (it->second->free_glory < 1000 && it->second->stats.empty())
		{
			send_to_char("� ��� ������������ ����� ����� ��� ������������� ���� �������.\r\n", ch);
			return;
		}

		boost::shared_ptr<glory_olc> tmp_glory_olc(new glory_olc);
		tmp_glory_olc->stat_cur[GLORY_STR] = ch->get_inborn_str();
		tmp_glory_olc->stat_cur[GLORY_DEX] = ch->get_inborn_dex();
		tmp_glory_olc->stat_cur[GLORY_INT] = ch->get_inborn_int();
		tmp_glory_olc->stat_cur[GLORY_WIS] = ch->get_inborn_wis();
		tmp_glory_olc->stat_cur[GLORY_CON] = ch->get_inborn_con();
		tmp_glory_olc->stat_cur[GLORY_CHA] = ch->get_inborn_cha();
		tmp_glory_olc->stat_cur[GLORY_HIT] = it->second->stats[GLORY_HIT];
		tmp_glory_olc->stat_cur[GLORY_SUCCESS] = it->second->stats[GLORY_SUCCESS];
		tmp_glory_olc->stat_cur[GLORY_WILL] = it->second->stats[GLORY_WILL];
		tmp_glory_olc->stat_cur[GLORY_STABILITY] = it->second->stats[GLORY_STABILITY];
		tmp_glory_olc->stat_cur[GLORY_REFLEX] = it->second->stats[GLORY_REFLEX];
		tmp_glory_olc->stat_cur[GLORY_MIND] = it->second->stats[GLORY_MIND];

		for (std::map<int, int>::const_iterator i = it->second->stats.begin(), iend = it->second->stats.end(); i != iend; ++i)
		{
			if (i->first < GLORY_TOTAL && i->first >= 0)
			{
				tmp_glory_olc->stat_cur[i->first] -= i->second;
				tmp_glory_olc->stat_add[i->first] = tmp_glory_olc->stat_was[i->first] = i->second;
			}
			else
			{
				log("Glory: ������������ ����� ����� %d (uid: %ld)", i->first, it->first);
			}
		}
		tmp_glory_olc->olc_free_glory = tmp_glory_olc->olc_was_free_glory = it->second->free_glory;

		ch->desc->glory_const = tmp_glory_olc;
		STATE(ch->desc) = CON_GLORY_CONST;
		spend_glory_menu(ch);
	}
	else if (CompareParam(buffer2, "����������"))
	{
		send_to_char("���������� � ��������� ���� ����� �����:\r\n", ch);
		print_glory(ch, it);
	}
	else if (CompareParam(buffer2, "���������"))
	{
		if (it->second->free_glory < MIN_TRANSFER_AMOUNT)
		{
			send_to_char(ch,
				"� ��� ������������ ��������� ����� ���������� ����� (������� %d).\r\n",
				MIN_TRANSFER_AMOUNT);
			return;
		}

		std::string name;
		GetOneParam(buffer, name);
		// buffer = ���-��
		boost::trim(buffer);

		Player p_vict;
		CHAR_DATA *vict = &p_vict;
		if (load_char(name.c_str(), vict) < 0)
		{
			send_to_char(ch, "%s - ������������ ��� ���������.\r\n", name.c_str());
			return;
		}
		if (str_cmp(GET_EMAIL(ch), GET_EMAIL(vict)))
		{
			send_to_char(ch, "��������� ����� ������ email ������.\r\n");
			return;
		}

		int amount = 0;
		try
		{
			amount = boost::lexical_cast<int>(buffer);
		}
		catch(...)
		{
			send_to_char(ch, "%s - ������������ ���������� ��� ��������.\r\n", buffer.c_str());
			send_to_char(GLORY_CONST_FORMAT, ch);
			return;
		}
		if (amount < MIN_TRANSFER_AMOUNT || amount > it->second->free_glory)
		{
			send_to_char(ch,
				"%d - ������������ ���������� ��� ��������.\r\n"
				"�� ������ ��������� �� %d �� %d ���������� �����.\r\n",
				amount, MIN_TRANSFER_AMOUNT, it->second->free_glory);
			return;
		}

		int tax = MAX(MIN_TRANSFER_TAX, amount / 100 * TRANSFER_FEE);
		int total_amount = amount - tax;

		remove_glory(GET_UNIQUE(ch), amount);
		add_glory(GET_UNIQUE(vict), total_amount);

		snprintf(buf, MAX_STRING_LENGTH,
			"Transfer %d const glory from %s", total_amount, GET_NAME(ch));
		add_karma(vict, buf, "��������");

		snprintf(buf, MAX_STRING_LENGTH,
			"Transfer %d const glory to %s", amount, GET_NAME(vict));
		add_karma(ch, buf, "��������");

		total_charge += tax;
		transfer_log("%s -> %s transfered %d (%d tax)", GET_NAME(ch), GET_NAME(vict), total_amount, tax);

		ch->save_char();
		vict->save_char();
		save();

		send_to_char(ch, "%s ���������� %d ���������� ����� (%d ��������).\r\n",
			GET_PAD(vict, 2), total_amount, tax);

		// TODO: �� ���� � �����-��� ��� �����, �� ���� ���������/�����
		// �� �������� �������, ����� �� ���������.
		// � ��� ��������� ����� ����� ��� ������� ������ �� ���� �����
	}
	else
	{
		send_to_char(GLORY_CONST_FORMAT, ch);
	}
}

/**
* �������� ����� � ���� (���������), ���� ���� ���� �� �����.
* \return 0 - ������ �� �������, ����� > 0 - ������� ������� �������
*/
int remove_glory(long uid, int amount)
{
	if (uid <= 0 || amount <= 0)
	{
		return 0;
	}

	int real_removed = amount;

	GloryListType::iterator i = glory_list.find(uid);
	if (glory_list.end() != i)
	{
		// ���� ���� ��-��� �� ��������
		if (i->second->free_glory >= amount)
		{
			i->second->free_glory -= amount;
		}
		else
		{
			real_removed = i->second->free_glory;
			i->second->free_glory = 0;
		}
		// ������ ������ ������ ��������� �� ������ ���
		if (!i->second->free_glory && i->second->stats.empty())
		{
			glory_list.erase(i);
		}
		save();
	}
	else
	{
		real_removed = 0;
	}

	return real_removed;
}

bool reset_glory(CHAR_DATA *ch)
{
	GloryListType::iterator i = glory_list.find(GET_UNIQUE(ch));
	if (glory_list.end() != i)
	{
		glory_list.erase(i);
		save();
		check_max_hp(ch);
		GloryMisc::recalculate_stats(ch);
		ch->save_char();
		return true;
	}
	return false;
}

ACMD(do_glory)
{
	if (!*argument)
	{
		send_to_char("������ ������� : \r\n"
			"   glory <���> (���������� �� ���������� ���������)\r\n"
			"   glory <���> +|-<���-�� �����> �������\r\n"
			"   glory <���> reset ������� (��������� ��������� � ��������� �����)\r\n", ch);
		return;
	}

	enum {SHOW_GLORY, ADD_GLORY, SUB_GLORY, RESET_GLORY};

	char num[MAX_INPUT_LENGTH];
	int mode = 0;

	char *reason = two_arguments(argument, arg, num);
	skip_spaces(&reason);

	if (!*num)
	{
		mode = SHOW_GLORY;
	}
	else if (*num == '+')
	{
		mode = ADD_GLORY;
	}
	else if (*num == '-')
	{
		mode = SUB_GLORY;
	}
	else if (is_abbrev(num, "reset"))
	{
		mode = RESET_GLORY;
	}
	// ����� �������, ����� ����� ������ ��������
	skip_dots(&reason);

	if (mode != SHOW_GLORY && (!reason || !*reason))
	{
		send_to_char("������� ������� ��������� ����� ?\r\n", ch);
		return;
	}

	CHAR_DATA *vict = get_player_vis(ch, arg, FIND_CHAR_WORLD);
	if (vict && vict->desc && STATE(vict->desc) == CON_GLORY_CONST)
	{
		send_to_char("�������� � ������ ������ ����������� ���� �����.\r\n", ch);
		return;
	}
	Player t_vict; // TODO: �����
	if (!vict)
	{
		if (load_char(arg, &t_vict) < 0)
		{
			send_to_char("������ ��������� �� ����������.\r\n", ch);
			return;
		}
		vict = &t_vict;
	}

	switch (mode)
	{
		case ADD_GLORY:
		{
			int amount = atoi((num + 1));
			add_glory(GET_UNIQUE(vict), amount);
			send_to_char(ch, "%s ��������� %d �.�. ���������� ����� (�����: %d �.�.).\r\n",
				GET_PAD(vict, 2), amount, get_glory(GET_UNIQUE(vict)));
			// ������ � �����, ����
			sprintf(buf, "(GC) %s sets +%d const glory to %s.", GET_NAME(ch), amount, GET_NAME(vict));
			mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), SYSLOG, TRUE);
			imm_log(buf);
			sprintf(buf, "Change const glory +%d by %s", amount, GET_NAME(ch));
			add_karma(vict, buf, reason);
			GloryMisc::add_log(mode, amount, std::string(buf), std::string(reason), vict);
			break;
		}
		case SUB_GLORY:
		{
			int amount = remove_glory(GET_UNIQUE(vict), atoi((num + 1)));
			if (amount <= 0)
			{
				send_to_char(ch, "� %s ��� ��������� ���������� �����.\r\n", GET_PAD(vict, 1));
				break;
			}
			send_to_char(ch, "� %s ������� %d �.�. ���������� ����� (�����: %d �.�.).\r\n",
				GET_PAD(vict, 1), amount, get_glory(GET_UNIQUE(vict)));
			// ������ � �����, ����
			sprintf(buf, "(GC) %s sets -%d const glory to %s.", GET_NAME(ch), amount, GET_NAME(vict));
			mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), SYSLOG, TRUE);
			imm_log(buf);
			sprintf(buf, "Change const glory -%d by %s", amount, GET_NAME(ch));
			add_karma(vict, buf, reason);
			GloryMisc::add_log(mode, amount, std::string(buf), std::string(reason), vict);
			break;
		}
		case RESET_GLORY:
		{
			if (reset_glory(vict))
			{
				send_to_char(ch, "%s - ������� ������ ���������� �����.\r\n", vict->get_name());
				// ������ � �����, ����
				sprintf(buf, "(GC) %s reset const glory to %s.", GET_NAME(ch), GET_NAME(vict));
				mudlog(buf, NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), SYSLOG, TRUE);
				imm_log(buf);
				sprintf(buf, "Reset stats and const glory by %s", GET_NAME(ch));
				add_karma(vict, buf, reason);
				GloryMisc::add_log(mode, 0, std::string(buf), std::string(reason), vict);
			}
			else
			{
				send_to_char(ch, "%s - ������ ���������� ����� � ��� ������.\r\n", vict->get_name());
			}
			break;
		}
		default:
			GloryConst::print_to_god(vict, ch);
	}
	vict->save_char();
}

void save()
{
	pugi::xml_document doc;
	doc.append_child().set_name("glory_list");
	pugi::xml_node char_list = doc.child("glory_list");

	for (GloryListType::const_iterator i = glory_list.begin(), iend = glory_list.end(); i != iend; ++i)
	{
		pugi::xml_node char_node = char_list.append_child();
		char_node.set_name("char");
		char_node.append_attribute("uid") = (int)i->first;
		char_node.append_attribute("glory") = i->second->free_glory;

		for (std::map<int, int>::const_iterator k = i->second->stats.begin(),
			kend = i->second->stats.end(); k != kend; ++k)
		{
			pugi::xml_node stat = char_node.append_child();
			stat.set_name("stat");
			stat.append_attribute("num") = k->first;
			stat.append_attribute("amount") = k->second;
		}
	}

	pugi::xml_node charge_node = char_list.append_child();
	charge_node.set_name("total_charge");
	charge_node.append_attribute("amount") = total_charge;

	pugi::xml_node spent_node = char_list.append_child();
	spent_node.set_name("total_spent");
	spent_node.append_attribute("amount") = total_spent;

	doc.save_file(LIB_PLRSTUFF"glory_const.xml");
}

void load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(LIB_PLRSTUFF"glory_const.xml");
	if (!result)
	{
		snprintf(buf, MAX_STRING_LENGTH, "SYSERR: error reading glory_const.xml: %s", result.description());
		perror(buf);
		return;
	}
    pugi::xml_node char_list = doc.child("glory_list");
	for (pugi::xml_node node = char_list.child("char"); node; node = node.next_sibling("char"))
	{
		long uid = boost::lexical_cast<long>(node.attribute("uid").value());
		std::string name = GetNameByUnique(uid);
		if (name.empty())
		{
			log("GloryConst: UID %ld - ��������� �� ����������.", uid);
			continue;
		}
		if (glory_list.find(uid) != glory_list.end())
		{
			log("SYSERROR : �������� ������ uid=%ld, name=%s (%s:%d)",
				uid, name.c_str(), __FILE__, __LINE__);
			continue;
		}

		GloryNodePtr tmp_node(new glory_node);

		long free_glory = boost::lexical_cast<int>(node.attribute("glory").value());
		tmp_node->free_glory = free_glory;

		for (pugi::xml_node stat = node.child("stat"); stat; stat = stat.next_sibling("stat"))
		{
			int stat_num = boost::lexical_cast<int>(stat.attribute("num").value());
			int stat_amount = boost::lexical_cast<int>(stat.attribute("amount").value());
			if (stat_num >= GLORY_TOTAL && stat_num < 0)
			{
				log("SYSERROR : ���������� ����� ������� ����� num=%d, name=%s (%s:%d)",
					stat_num, name.c_str(), __FILE__, __LINE__);
				continue;
			}
			if (tmp_node->stats.find(stat_num) != tmp_node->stats.end())
			{
				log("SYSERROR : �������� ������� ����� num=%d, name=%s (%s:%d)",
					stat_num, name.c_str(), __FILE__, __LINE__);
				continue;
			}
			tmp_node->stats[stat_num] = stat_amount;
		}
		glory_list[uid] = tmp_node;
    }
    pugi::xml_node charge_node = char_list.child("total_charge");
    if (charge_node)
    {
		total_charge = boost::lexical_cast<int>(charge_node.attribute("amount").value());
    }
    pugi::xml_node spent_node = char_list.child("total_spent");
    if (spent_node)
    {
		total_spent = boost::lexical_cast<int>(spent_node.attribute("amount").value());
    }
}

void set_stats(CHAR_DATA *ch)
{
	GloryListType::iterator i = glory_list.find(GET_UNIQUE(ch));
	if (glory_list.end() == i)
	{
		return;
	}

	for (std::map<int, int>::const_iterator k = i->second->stats.begin(),
		kend = i->second->stats.end(); k != kend; ++k)
	{
		switch(k->first)
		{
			case G_STR:
				ch->inc_str(k->second);
				break;
			case G_DEX:
				ch->inc_dex(k->second);
				break;
			case G_INT:
				ch->inc_int(k->second);
				break;
			case G_WIS:
				ch->inc_wis(k->second);
				break;
			case G_CON:
				ch->inc_con(k->second);
				break;
			case G_CHA:
				ch->inc_cha(k->second);
				break;
			default:
				log("Glory: ������������ ����� ����� %d (uid: %d)", k->first, GET_UNIQUE(ch));
		}
	}
}

/**
 * ���������� ��������� ������ (������ �� ����� 6 ��������).
 */
int main_stats_count(CHAR_DATA *ch)
{
	GloryListType::iterator i = glory_list.find(GET_UNIQUE(ch));
	if (glory_list.end() == i)
	{
		return 0;
	}

	int count = 0;
	for (std::map<int, int>::const_iterator k = i->second->stats.begin(),
		kend = i->second->stats.end(); k != kend; ++k)
	{
		switch(k->first)
		{
			case G_STR:
			case G_DEX:
			case G_INT:
			case G_WIS:
			case G_CON:
			case G_CHA:
				count += k->second;
				break;
		}
	}
	return count;
}

/**
* ����� ���� � show stats.
*/
void show_stats(CHAR_DATA *ch)
{
	int free_glory = 0, spend_glory = 0;
	for (GloryListType::const_iterator i = glory_list.begin(), iend = glory_list.end();  i != iend; ++i)
	{
		free_glory += i->second->free_glory;
		spend_glory += calculate_glory_in_stats(i);
	}
	send_to_char(ch,
		"  �����2: ������� %d, �������� %d, ����� %d, �������� %d\r\n"
		"  ����� ��������� ����� � ���������: %d\r\n",
		spend_glory, free_glory, free_glory + spend_glory, total_charge, total_spent);
}

void add_total_spent(int amount)
{
	if (amount > 0)
	{
		total_spent += amount;
	}
}

void apply_modifiers(CHAR_DATA *ch)
{
	GloryListType::iterator it = glory_list.find(GET_UNIQUE(ch));
	if (it==glory_list.end())
		return;
	for (std::map<int, int>::const_iterator i = it->second->stats.begin(); i != it->second->stats.end(); ++i)
	{
		int location = 0;
		bool add = true;
	switch (i->first)
		{
			case GLORY_HIT:
				location = APPLY_HIT;
				break;
			case GLORY_SUCCESS:
				location = APPLY_CAST_SUCCESS;
				break;
			case GLORY_WILL:
				location = APPLY_SAVING_WILL;
				add = false;
				break;
			case GLORY_STABILITY:
				location = APPLY_SAVING_STABILITY;
				add = false;
				break;
			case GLORY_REFLEX:
				location = APPLY_SAVING_REFLEX;
				add = false;
				break;
			case GLORY_MIND:
				location = APPLY_RESIST_MIND;
				break;
			default:
				break;
		}
		if (location)
			affect_modify(ch, location, i->second, 0, add);
		}
		}

} // namespace GloryConst
