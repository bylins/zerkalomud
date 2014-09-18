// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#include <map>
#include <list>
#include <string>
#include <sstream>
#include <iomanip>
#include "parcel.hpp"
#include "db.h"
#include "interpreter.h"
#include "comm.h"
#include "char.hpp"
#include "handler.h"
#include "auction.h"
#include "screen.h"
#include "char_player.hpp"
#include "mail.h"
#include "name_list.hpp"
#include "room.hpp"
#include "objsave.h"

extern CHAR_DATA *get_player_of_name(const char *name);
extern int get_buf_line(char **source, char *target);
extern OBJ_DATA *read_one_object_new(char **data, int *error);
extern void olc_update_object(int robj_num, OBJ_DATA *obj, OBJ_DATA *olc_proto);

namespace Parcel
{

const int KEEP_TIMER = 60 * 24 * 3; // 3 ����� ���� �� ����� (� �������)
const int SEND_COST = 100; // � ����� ������ ��������� �� ������� ������
const int RESERVED_COST_COEFF = 3; // ���� ����� �� 3 ���
const int MAX_SLOTS = 25; // ������� ������ ����� ���������� � �������� �� ������ ������
const int RETURNED_TIMER = -1; // ��� ��������� ������� ���� ������� ������ ������ ��� ������� �����
const char * FILE_NAME = LIB_DEPOT"parcel.db";

// ��� �������� ������� �����������
const bool RETURN_WITH_MONEY = 1;
const bool RETURN_NO_MONEY = 0;

// ������������ � ������ �������
static int was_sended = 0;

// ��� ��������� ������� ������ (����� �� ������� �� ������)
static std::string send_buffer;
static int send_cost_buffer = 0;
static int send_reserved_buffer = 0;

class Node
{
public:
	Node (int money, OBJ_DATA *obj) : money_(money), timer_(0), obj_(obj) {};
	Node () : money_(0), timer_(0), obj_(0) {};
	int money_; // ��������������� ��������
	int timer_; // ������� ����� ������ ��� ���� ���������� (��� �������� ���� KEEP_TIMER ������������ �����������)
	OBJ_DATA *obj_; // ������ (����� �� ������� ������, ��� ����� � ���� - ���� � ������� ����������� �������)
};

class LoadNode
{
public:
	Node obj_node;
	long sender;
	long target;
};

typedef std::map<long /* ��� ����������� */, std::list<Node> > SenderListType;
typedef std::map<long /* ��� ���������� */,  SenderListType> ParcelListType;

ParcelListType parcel_list; // ������ �������
SenderListType return_list; // ��������� ������ �� �������

/**
* ��������� ��� ��� �������.
*/
void parcel_log(const char *format, ...)
{
	const char *filename = "../log/parcel.log";
	static FILE *file = 0;
	if (!file)
	{
		file = fopen(filename, "a");
		if (!file)
		{
			log("SYSERR: can't open %s!", filename);
			return;
		}
		opened_files.push_back(file);
	}
	else if (!format)
		format = "SYSERR: // parcel_log received a NULL format.";

	write_time(file);
	va_list args;
	va_start(args, format);
	vfprintf(file, format, args);
	va_end(args);
	fprintf(file, "\n");
	fflush(file);
}

/**
* ����������� ���� (���� �� �������) � ����� �������.
*/
void invoice(long uid)
{
	DESCRIPTOR_DATA *d = DescByUID(uid);
	if (d)
	{
		if (!has_parcel(d->character))
		{
			send_to_char(d->character, "%s��� ������ �������, ������� �� ����� � �����������!%s\r\n",
					CCWHT(d->character, C_NRM), CCNRM(d->character, C_NRM));
		}
	}
}

/**
* ���������� ������ � ������ �������.
*/
void add_parcel(long target, long sender, const Node &tmp_node)
{
	invoice(target);
	ParcelListType::iterator it = parcel_list.find(target);
	if (it != parcel_list.end())
	{
		SenderListType::iterator it2 = it->second.find(sender);
		if (it2 != it->second.end())
		{
			it2->second.push_back(tmp_node);
		}
		else
		{
			std::list<Node> tmp_list;
			tmp_list.push_back(tmp_node);
			it->second.insert(std::make_pair(sender, tmp_list));
		}
	}
	else
	{
		std::list<Node> tmp_list;
		tmp_list.push_back(tmp_node);
		SenderListType tmp_map;
		tmp_map.insert(std::make_pair(sender, tmp_list));
		parcel_list.insert(std::make_pair(target, tmp_map));
	}
}

/**
* ������� ����� ��������� ��� ���������� ������ ���������� (��� �����������).
*/
int total_sended(CHAR_DATA *ch)
{
	int sended = 0;
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		SenderListType::const_iterator it2 = it->second.find(GET_UNIQUE(ch));
		if (it2 != it->second.end())
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				++sended;
			}
		}
	}
	return sended;
}

/**
* �������� ����������� ��������� ������ ������.
*/
bool can_send(CHAR_DATA *ch, CHAR_DATA *mailman, OBJ_DATA *obj)
{
	if (IS_OBJ_STAT(obj, ITEM_NODROP)
			|| IS_OBJ_STAT(obj, ITEM_NORENT)
			|| OBJ_FLAGGED(obj, ITEM_ZONEDECAY)
			|| OBJ_FLAGGED(obj, ITEM_REPOP_DECAY)
			|| OBJ_FLAGGED(obj, ITEM_DECAY)
			|| OBJ_FLAGGED(obj, ITEM_NORENT)
			|| GET_OBJ_TYPE(obj) == ITEM_KEY
			|| GET_OBJ_RENT(obj) < 0
			|| GET_OBJ_RNUM(obj) <= NOTHING)
	{
		snprintf(buf, MAX_STRING_LENGTH,
				"$n ������$g ��� : '%s - �� �� ���������� ����� ����!'\r\n",
				OBJ_PAD(obj, 0));
		act(buf, FALSE, mailman, 0, ch, TO_VICT);
		return 0;
	}
	else if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER && obj->contains)
	{
		snprintf(buf, MAX_STRING_LENGTH,
				"$n ������$g ��� : '� %s ���-�� �����.'\r\n", OBJ_PAD(obj, 5));
		act(buf, FALSE, mailman, 0, ch, TO_VICT);
		return 0;
	}
	else if (SetSystem::is_big_set(obj))
	{
		snprintf(buf, MAX_STRING_LENGTH,
				"$n ������$g ��� : '%s �������� ������ �������� ������ ���������.'\r\n",
				OBJ_PAD(obj, 0));
		act(buf, FALSE, mailman, 0, ch, TO_VICT);
		return 0;
	}
	return 1;
}

/**
* �������� �������� (������/�������������� �����, ����� �� ������ ���������).
*/
void send_object(CHAR_DATA *ch, CHAR_DATA *mailman, long vict_uid, OBJ_DATA *obj)
{
	if (!ch || !mailman || !vict_uid || !obj)
	{
		log("Parcel: ������� ������� ��������: %d, %d, %d, %d (%s %s %d)",
				ch ? 1 : 0, mailman ? 1 : 0, vict_uid ? 1 : 0, obj ? 1 : 0, __FILE__, __func__, __LINE__);
		return;
	}

	if (!can_send(ch, mailman, obj)) return;

	const int reserved_cost = get_object_low_rent(obj) * RESERVED_COST_COEFF;
	const int total_cost = reserved_cost + SEND_COST;

	if (ch->get_total_gold() < total_cost)
	{
		act("$n ������$g ��� : '�� � ���� ���� ��� ������� �����!'", FALSE, mailman, 0, ch, TO_VICT);
		return;
	}
	if (total_sended(ch) >= MAX_SLOTS)
	{
		act("$n ������$g ��� : '�� ��� � ��� �������� ���� �����! �������, ���� �� ������� ��������!'",
				FALSE, mailman, 0, ch, TO_VICT);
		return;
	}

	std::string name = GetNameByUnique(vict_uid);
	if (name.empty())
	{
		act("$n ������$g ��� : '������ � ����� ����������, �������� �����!'", FALSE, mailman, 0, ch, TO_VICT);
		return;
	}
	name_convert(name);

	if (send_buffer.empty())
		send_buffer += "�������: " + name + ", ����������:\r\n";

	snprintf(buf, sizeof(buf), "%s%s%s\r\n", CCWHT(ch, C_NRM), GET_OBJ_PNAME(obj, 0), CCNRM(ch, C_NRM));
	send_buffer += buf;

	Node tmp_node(reserved_cost, obj);
	add_parcel(vict_uid, GET_UNIQUE(ch), tmp_node);

	send_reserved_buffer += reserved_cost;
	send_cost_buffer += SEND_COST;

	ch->remove_both_gold(total_cost);
	obj_from_char(obj);
	ch->save_char();

	check_auction(NULL, obj);
	OBJ_DATA *temp;
	REMOVE_FROM_LIST(obj, object_list, next);
//	ObjectAlias::remove(obj);
}

/**
* �������� ��������, ��������� �� �������� ����� ('��������� ��� �������)'.
*/
void send(CHAR_DATA *ch, CHAR_DATA *mailman, long vict_uid, char *arg)
{
	if (IS_NPC(ch)) return;

	if (IS_IMMORTAL(ch))
	{
		send_to_char("�� �� ��...\r\n" , ch);
		return;
	}
	if (GET_UNIQUE(ch) == vict_uid)
	{
		act("$n ������$g ��� : '�� �������� ���������� �����!'", FALSE, mailman, 0, ch, TO_VICT);
		return;
	}
	if (RENTABLE(ch))
	{
		act("$n ������$g ��� : '�� � ���� ���� �� ������ � �����, ����������!'", FALSE, mailman, 0, ch, TO_VICT);
		return;
	}

	OBJ_DATA *obj, *next_obj;
	char tmp_arg[MAX_INPUT_LENGTH];
	char tmp_arg2[MAX_INPUT_LENGTH];

	two_arguments(arg, tmp_arg, tmp_arg2);

	if (is_number(tmp_arg))
	{
		int amount = atoi(tmp_arg);
		if (!strn_cmp("coin", tmp_arg2, 4) || !strn_cmp("���", tmp_arg2, 5) || !str_cmp("�����", tmp_arg2))
		{
			act("$n ������$g ��� : '��� �������� ����� �������������� �������� �����.'", FALSE, mailman, 0, ch, TO_VICT);
			return;
		}
		else if (!str_cmp("���", tmp_arg2) || !str_cmp("all", tmp_arg2))
		{
			if (!ch->carrying)
			{
				send_to_char("� ��� ���� ������ ���.\r\n", ch);
				return;
			}
			for (obj = ch->carrying; obj && amount; obj = next_obj)
			{
				--amount;
				next_obj = obj->next_content;
				send_object(ch, mailman, vict_uid, obj);
			}
		}
		else if (!*tmp_arg2)
		{
			send_to_char(ch, "���� %d �� ������ ��������� ?\r\n", amount);
		}
		else if (!(obj = get_obj_in_list_vis(ch, tmp_arg2, ch->carrying)))
		{
			send_to_char(ch, "� ��� ��� '%s'.\r\n", tmp_arg2);
		}
		else
		{
			while (obj && amount--)
			{
				next_obj = get_obj_in_list_vis(ch, tmp_arg2, obj->next_content);
				send_object(ch, mailman, vict_uid, obj);
				obj = next_obj;
			}
		}
	}
	else
	{
		int dotmode = find_all_dots(tmp_arg);
		if (dotmode == FIND_INDIV)
		{
			if (!(obj = get_obj_in_list_vis(ch, tmp_arg, ch->carrying)))
			{
				send_to_char(ch, "� ��� ��� '%s'.\r\n", tmp_arg);
			}
			send_object(ch, mailman, vict_uid, obj);
		}
		else
		{
			if (dotmode == FIND_ALLDOT && !*tmp_arg)
			{
				send_to_char("��������� \"���\" ������ ���� ��������� ?\r\n", ch);
				return;
			}
			if (!ch->carrying)
				send_to_char("� ��� ���� ������ ���.\r\n", ch);
			else
			{
				bool has_items = false;
				for (obj = ch->carrying; obj; obj = next_obj)
				{
					next_obj = obj->next_content;
					if (CAN_SEE_OBJ(ch, obj) && ((dotmode == FIND_ALL || isname(tmp_arg, obj->name))))
					{
						send_object(ch, mailman, vict_uid, obj);
						has_items = true;
					}
				}
				if (!has_items)
					send_to_char(ch, "� ��� ��� '%s'.\r\n", tmp_arg);
			}
		}
	}

	if (!send_buffer.empty())
	{
		snprintf(buf, sizeof(buf), "� ��� �������� %d %s � ��� %d %s ��������������� �� 3 ��� ��������.\r\n",
			send_cost_buffer, desc_count(send_cost_buffer, WHAT_MONEYa),
			send_reserved_buffer, desc_count(send_reserved_buffer, WHAT_MONEYa));
		send_buffer += buf;
		send_to_char(ch, send_buffer.c_str());

		send_buffer = "";
		send_cost_buffer = 0;
		send_reserved_buffer = 0;
	}
}

/**
* ��������� �� �������� ����� ('�����'). ���������� ������������ �������, ������� ��� �� ����������.
*/
void print_sending_stuff(CHAR_DATA *ch)
{
	std::stringstream out;
	out << "\r\n���� ������� �������:";
	bool print = false;
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		SenderListType::const_iterator it2 = it->second.find(GET_UNIQUE(ch));
		if (it2 != it->second.end())
		{
			print = true;
			std::string name = GetNameByUnique(it->first);
			name_convert(name);
			out << "\r\n�������: " << name << ", ����������:\r\n" << CCWHT(ch, C_NRM);

			int money = 0;
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				out << GET_OBJ_PNAME(it3->obj_, 0) << "\r\n";
				money += it3->money_;
			}
			out << CCNRM(ch, C_NRM)
				<< money << " " << desc_count(money, WHAT_MONEYa) << " ��������������� �� 3 ��� ��������.\r\n";
		}
	}
	if (print)
		send_to_char(out.str(), ch);
}

/**
* ��� ���������� ��������� �� ����� � �������.
*/
int print_spell_locate_object(CHAR_DATA *ch, int count, std::string name)
{
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				if (number(1, 100) > (40 + MAX((GET_REAL_INT(ch) - 25) * 2, 0)))
					continue;
				if (!isname(name.c_str(), it3->obj_->name))
					continue;

				snprintf(buf, MAX_STRING_LENGTH, "%s �����%s�� � ��������� ������ � ���������.\r\n",
						it3->obj_->short_description, GET_OBJ_POLY_1(ch, it3->obj_));
				CAP(buf);
				send_to_char(buf, ch);

				if (--count <= 0)
					return count;
			}
		}
	}
	return count;
}

/**
* ���� �� �� ���� �����-���� �������.
*/
bool has_parcel(CHAR_DATA *ch)
{
	ParcelListType::const_iterator it = parcel_list.find(GET_UNIQUE(ch));
	if (it != parcel_list.end())
		return true;
	else
		return false;
}

/**
* ������� ����������������� ����� �����������.
*/
void return_money(std::string const &name, int money, bool add)
{
	if (money <= 0)
	{
		log("WARNING: money=%d (%s %s %d)", money, __FILE__, __func__, __LINE__);
		return;
	}

	CHAR_DATA *vict = 0;
	if ((vict = get_player_of_name(name.c_str())))
	{
		if (add)
		{
			vict->add_bank(money);
			send_to_char(vict, "%s�� �������� %d %s ���������� ��������� �� �������� ������%s.\r\n",
					CCWHT(vict, C_NRM), money, desc_count(money, WHAT_MONEYu), CCNRM(vict, C_NRM));
		}
	}
	else
	{
		vict = new Player; // TODO: ���������� �� ����
		if (load_char(name.c_str(), vict) < 0)
		{
			delete vict;
			return;
		}
		vict->add_bank(money);
		vict->save_char();
		delete vict;
	}
}

/**
* ������-�������� �� ����� ������� ��� ���������.
*/
void fill_ex_desc(CHAR_DATA *ch, OBJ_DATA *obj, std::string sender)
{
	CREATE(obj->ex_description, EXTRA_DESCR_DATA, 1);
	obj->ex_description->keyword = str_dup("������� ��������� ����� ���� parcel box case chest");
	obj->ex_description->next = 0;

	int size = MAX(strlen(GET_NAME(ch)), sender.size());
	std::stringstream out;
	out.setf(std::ios_base::left);

	out << "   ��������������� �� ������ ���� � ������ ����������\r\n"
			"���������� ������ - ������� ������ ������������ ����...\r\n"
			"�� �������� ����� ����� �������:\r\n\r\n";
	out << std::setw(size + 16) << std::setfill('-') << " " << std::setfill(' ') << "\r\n";
	out << "| �����������: " << std::setw(size) << sender
		<< " |\r\n|  ����������: " << std::setw(size) << GET_NAME(ch) << " |\r\n";
	out << std::setw(size + 16) << std::setfill('-') << " " << std::setfill(' ') << "\r\n";

	obj->ex_description->description = str_dup(out.str().c_str());
}

/**
* ������ ��������� ����� �� �������, ���� �� ����� �� �����.
*/
int calculate_timer_cost(std::list<Node>::iterator const &it)
{
	return static_cast<int>((get_object_low_rent(it->obj_) / (24.0 * 60.0)) * it->timer_);
}

/**
 * ������� ��� ��������� �������.
 */
OBJ_DATA * create_parcel()
{
	OBJ_DATA *obj = create_obj();
	obj->name = str_dup("������� ��������� ����� ���� parcel box case chest");
	obj->short_description = str_dup("�������");
	obj->description = str_dup("���-�� ����� ����� ���� �������.");
	obj->PNames[0] = str_dup("�������");
	obj->PNames[1] = str_dup("�������");
	obj->PNames[2] = str_dup("�������");
	obj->PNames[3] = str_dup("�������");
	obj->PNames[4] = str_dup("��������");
	obj->PNames[5] = str_dup("�������");
	GET_OBJ_SEX(obj) = SEX_FEMALE;
	GET_OBJ_TYPE(obj) = ITEM_CONTAINER;
	GET_OBJ_WEAR(obj) = ITEM_WEAR_TAKE;
	GET_OBJ_WEIGHT(obj) = 1;
	GET_OBJ_COST(obj) = 1;
	GET_OBJ_RENT(obj) = 1;
	GET_OBJ_RENTEQ(obj) = 1;
	obj->set_timer(24 * 60);
	SET_BIT(GET_OBJ_EXTRA(obj, ITEM_NOSELL), ITEM_NOSELL);
	SET_BIT(GET_OBJ_EXTRA(obj, ITEM_DECAY), ITEM_DECAY);
	return obj;
}

/**
* ��������� ������� �� �����, ��������� �� �������� �����. ('��������').
*/
void receive(CHAR_DATA *ch, CHAR_DATA *mailman)
{
	ParcelListType::iterator it = parcel_list.find(GET_UNIQUE(ch));
	if (it != parcel_list.end())
	{
		for (SenderListType::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			std::string name = GetNameByUnique(it2->first);
			name_convert(name);

			OBJ_DATA *obj = create_parcel();
			fill_ex_desc(ch, obj, name);

			int money = 0;
			for (std::list<Node>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				money += it3->money_ - calculate_timer_cost(it3);
				// ��������� � ����.������ � ������ � �������
				it3->obj_->next = object_list;
				object_list = it3->obj_;
//				ObjectAlias::add(it3->obj_);
				obj_to_obj(it3->obj_, obj);
			}
			return_money(name, money, RETURN_WITH_MONEY);

			obj_to_char(obj, ch);
			snprintf(buf, MAX_STRING_LENGTH, "$n ���$g ��� ������� (����������� %s).", name.c_str());
			act(buf, FALSE, mailman, 0, ch, TO_VICT);
			act("$N ���$G $n2 �������.", FALSE, ch, 0, mailman, TO_ROOM);
			++was_sended;
		}
		parcel_list.erase(it);
	}
}

/**
* �������� ��������� ����� ������, � ������������ ����, ���� ��� ������.
*/
void create_mail(long to, long from, char *text)
{
	store_mail(to, from, text);
	DESCRIPTOR_DATA* i = get_desc_by_id(to);
	if (i)
		send_to_char(i->character, "%s��� ������ ������, ������� �� ����� � �����������!%s\r\n",
				CCWHT(i->character, C_NRM), CCNRM(i->character, C_NRM));
}

/**
* ������������ ���������� ������ ������������ ��������� (�� ��������� �������).
*/
void prepare_return(const long uid, const std::list<Node>::iterator &it)
{
	Node tmp_node(0, it->obj_);
	tmp_node.timer_ = RETURNED_TIMER;

	SenderListType::iterator it2 = return_list.find(uid);
	if (it2 != return_list.end())
	{
		it2->second.push_back(tmp_node);
	}
	else
	{
		std::list<Node> tmp_list;
		tmp_list.push_back(tmp_node);
		return_list.insert(std::make_pair(uid, tmp_list));
	}
}

/**
* ������� ��������� �� ���������� ������ (������������� �� � �������� ������ �������).
*/
void return_parcel()
{
	for (SenderListType::iterator it = return_list.begin(); it != return_list.end(); ++it)
	{
		for (std::list<Node>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			Node tmp_node(it2->money_, it2->obj_);
			tmp_node.timer_ = RETURNED_TIMER;
			add_parcel(it->first, it->first, tmp_node);
		}
	}
	return_list.clear();
}

/**
* ����� �������� �� ����� � ����������� �� ���� ����������� � ���������� ����� ������.
*/
void extract_parcel(const long sender_uid, const long target_uid, const std::list<Node>::iterator &it)
{
	long sender_id = get_id_by_uid(sender_uid);
	long target_id = get_id_by_uid(target_uid);
	snprintf(buf, MAX_STRING_LENGTH, "� ����������� �������� ���: %s ��������%s � ����.\r\n",
			it->obj_->short_description, GET_OBJ_SUF_2(it->obj_));

	char *tmp = str_dup(buf);
	// -1 � �������� �� ����������� ��� ��������� ��������� � ��� �������� ������
	create_mail(sender_id, -1, tmp);
	create_mail(target_id, -1, tmp);
	free(tmp);

	// ������� ���������� ����������������� ��� ����������� (� ����������� ��� ����)
	if (it->money_ && it->timer_ != RETURNED_TIMER)
	{
		int money_return = it->money_ - calculate_timer_cost(it);
		std::string name = GetNameByUnique(sender_uid);
		return_money(name, money_return, RETURN_WITH_MONEY);
	}

	extract_obj(it->obj_);
}

/**
* ��������� ������ � �������� �������.
*/
void return_invoice(long uid, OBJ_DATA *obj)
{
	long target_id = get_id_by_uid(uid);
	snprintf(buf, MAX_STRING_LENGTH, "������� ���������� �����������: %s.\r\n", obj->short_description);

	char *tmp = str_dup(buf);
	create_mail(target_id, -1, tmp);
	free(tmp);
}

/**
* ������ ������, ���������� ���� ����� � ��� ������� ����� ���.
*/
LoadNode parcel_read_one_object(char **data, int *error)
{
	LoadNode tmp_node;

	*error = 1;
	// ������ �� ������ �������� (#)
	for (; **data != '#'; (*data)++)
		if (!**data || **data == '$')
			return tmp_node;

	// ��������� #
	(*data)++;
	char buffer[MAX_STRING_LENGTH];

	*error = 2;
	// �����������
	if (!get_buf_line(data, buffer))
		return tmp_node;
	*error = 3;
	if ((tmp_node.target = atol(buffer)) <= 0)
		return tmp_node;

	*error = 4;
	// ����������
	if (!get_buf_line(data, buffer))
		return tmp_node;
	*error = 5;
	if ((tmp_node.sender = atol(buffer)) <= 0)
		return tmp_node;

	*error = 6;
	// ����������������� �����
	if (!get_buf_line(data, buffer))
		return tmp_node;
	*error = 7;
	if ((tmp_node.obj_node.money_ = atoi(buffer)) < 0)
		return tmp_node;

	*error = 8;
	// ������ �������� �� �����
	if (!get_buf_line(data, buffer))
		return tmp_node;
	*error = 9;
	if ((tmp_node.obj_node.timer_ = atoi(buffer)) < -1)
		return tmp_node;

	*error = 0;
	tmp_node.obj_node.obj_ = read_one_object_new(data, error);

	if (*error)
		*error = 10;

	return tmp_node;
}

/**
* �������� ��������� � ���.���������� (��� ������).
*/
void load()
{
	FILE *fl;
	if (!(fl = fopen(LIB_DEPOT"parcel.db", "r")))
	{
		log("SYSERR: Error opening parcel database.");
		return;
	}

	fseek(fl, 0L, SEEK_END);
	int fsize = ftell(fl);

	char *data, *readdata;
	CREATE(readdata, char, fsize + 1);
	fseek(fl, 0L, SEEK_SET);
	if (!fread(readdata, fsize, 1, fl) || ferror(fl))
	{
		fclose(fl);
		log("SYSERR: Memory error or cann't read parcel database file.");
		free(readdata);
		return;
	};
	fclose(fl);

	data = readdata;
	*(data + fsize) = '\0';

	for (fsize = 0; *data && *data != '$'; fsize++)
	{
		int error;
		LoadNode node = parcel_read_one_object(&data, &error);

		if (!node.obj_node.obj_)
		{
			log("SYSERR: Error #%d reading parcel database file.", error);
			return;
		}

		if (error)
		{
			log("SYSERR: Error #%d reading item from parcel database.", error);
			return;
		}
		add_parcel(node.target, node.sender, node.obj_node);
		// �� ����������� ������ �������
		OBJ_DATA *temp;
		REMOVE_FROM_LIST(node.obj_node.obj_, object_list, next);
//		ObjectAlias::remove(node.obj_node.obj_);
	}

	free(readdata);
}

/**
* ���������� ��������� � ���.���������� (��� ������� ��������).
*/
void save()
{
	std::stringstream out;
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				out << "#" << it->first << "\n" << it2->first << "\n" << it3->money_ << "\n" << it3->timer_ << "\n\n";
				write_one_object(out, it3->obj_, 0);
				out << "\n";
			}
		}
	}
	out << "$\n$\n";

	// ��������� � ����
	std::ofstream file(FILE_NAME);
	if (!file.is_open())
	{
		log("SYSERR: error opening file: %s! (%s %s %d)", FILE_NAME, __FILE__, __func__, __LINE__);
		return;
	}
	file << out.rdbuf();
	file.close();

	return;
}

/**
* ���������� �������� � ��������� + �������� �������� �� �����. ����/������� �� ����������.
*/
void update_timers()
{
	for (ParcelListType::iterator it = parcel_list.begin(); it != parcel_list.end(); /* empty */)
	{
		for (SenderListType::iterator it2 = it->second.begin(); it2 != it->second.end(); /* empty */)
		{
			std::list<Node>::iterator tmp_it;
			for (std::list<Node>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3 = tmp_it)
			{
				tmp_it = it3;
				++tmp_it;

				it3->obj_->dec_timer();
				if (it3->obj_->get_timer() <= 0)
				{
					extract_parcel(it2->first, it->first, it3);
					it2->second.erase(it3);
				}
				else
				{
					if (it3->timer_ == RETURNED_TIMER)
					{
						// ������ ��� ���������� �����������, ����� �� ������, �� ������ ���� ��� ����
						it3->obj_->dec_timer();
						if (it3->obj_->get_timer() <= 0)
						{
							extract_parcel(it2->first, it->first, it3);
							it2->second.erase(it3);
						}
					}
					else
					{
						++it3->timer_;
						if (it3->timer_ >= KEEP_TIMER)
						{
							return_invoice(it->first, it3->obj_);
							prepare_return(it2->first, it3);
							// ��� ���� ������ ��������� �������� � ������
							std::string name = GetNameByUnique(it2->first);
							return_money(name, it3->money_, RETURN_NO_MONEY);
							// � ������� ������ (��� ����� � �������� ����� � return_parcel)
							it2->second.erase(it3);
						}
					}
				}
			}
			if (it2->second.empty())
				it->second.erase(it2++);
			else
				++it2;
		}
		if (it->second.empty())
			parcel_list.erase(it++);
		else
			++it;
	}
	return_parcel();
	save();
}

/**
* ������� 'show stats' �������� � ���������� ����.
*/
void show_stats(CHAR_DATA *ch)
{
	int targets = 0, returned = 0, objs = 0, reserved_money = 0;
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		++targets;
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				++objs;
				reserved_money += it3->money_;
				if (it3->timer_ == RETURNED_TIMER)
					++returned;
			}
		}
	}
	send_to_char(ch, "  �����: ��������� � �������� %d, ���������� � ������ %d\r\n", objs, was_sended);
}

/**
* �������� ������ ������ �� ����� � ������ ���������� ����� ����� ���.
*/
void renumber_obj_rnum(int rnum)
{
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				if (GET_OBJ_RNUM(it3->obj_) >= rnum)
					GET_OBJ_RNUM(it3->obj_)++;
			}
		}
	}
}

/**
* ������� '���' ��� ����� ��������� �� �����.
*/
int print_imm_where_obj(CHAR_DATA *ch, char *arg, int num)
{
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				if (isname(arg, it3->obj_->name))
				{
					std::string target = GetNameByUnique(it->first);
					std::string sender = GetNameByUnique(it2->first);

					send_to_char(ch, "O%3d. %-25s - �����%s�� �� ����� (�����������: %s, ����������: %s).\r\n",
							num++, it3->obj_->short_description, GET_OBJ_POLY_1(ch, it3->obj_), sender.c_str(), target.c_str());
				}
			}
		}
	}
	return num;
}

/**
* ���������� ����� �������� ��� ��������� �� ��������� ����� ���.
*/
void olc_update_from_proto(int robj_num, OBJ_DATA *olc_proto)
{
	for (ParcelListType::const_iterator it = parcel_list.begin(); it != parcel_list.end(); ++it)
	{
		for (SenderListType::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			for (std::list<Node>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				if (GET_OBJ_RNUM(it3->obj_) == robj_num)
					olc_update_object(robj_num, it3->obj_, olc_proto);
			}
		}
	}
}

/**
* ����� ���� ��� ����� �������.
*/
OBJ_DATA * locate_object(const char *str)
{
	for (ParcelListType::const_iterator i = parcel_list.begin(); i != parcel_list.end(); ++i)
	{
		for (SenderListType::const_iterator k = i->second.begin(); k != i->second.end(); ++k)
		{
			for (std::list<Node>::const_iterator o = k->second.begin(); o != k->second.end(); ++o)
			{
				if (isname(str, o->obj_->name))
				{
					return o->obj_;
				}
			}
		}
	}
	return 0;
}

/**
 * ������� ���� ������ ������� �� �����������.
 */
void bring_back(CHAR_DATA *ch, CHAR_DATA *mailman)
{
	int money = 0;
	bool empty = true;
	for (ParcelListType::iterator i = parcel_list.begin(); i != parcel_list.end(); /* empty */)
	{
		SenderListType::iterator k = i->second.find(GET_UNIQUE(ch));
		if (k == i->second.end())
		{
			++i;
			continue;
		}
		empty = false;
		OBJ_DATA *obj = create_parcel();
		fill_ex_desc(ch, obj, std::string("����� ���������"));
		for (std::list<Node>::iterator l = k->second.begin(); l != k->second.end(); ++l)
		{
			money += l->money_ - calculate_timer_cost(l);
			l->obj_->next = object_list;
			object_list = l->obj_;
//			ObjectAlias::add(l->obj_);
			obj_to_obj(l->obj_, obj);
		}
		obj_to_char(obj, ch);
		snprintf(buf, MAX_STRING_LENGTH, "$n ���$g ��� �������.");
		act(buf, FALSE, mailman, 0, ch, TO_VICT);
		act("$N ���$G $n2 �������.", FALSE, ch, 0, mailman, TO_ROOM);

		i->second.erase(k);
		if (i->second.empty())
		{
			parcel_list.erase(i++);
		}
		else
		{
			++i;
		}
	}
	if (!empty && money > 0)
	{
		act("$n ������$g ��� : '�� ���������� ������� ������� � ��� �������� �������� ����������������� ���.'",
				FALSE, mailman, 0, ch, TO_VICT);
		std::string name = GET_NAME(ch);
		return_money(name, money/2, RETURN_WITH_MONEY);
	}
	else if (empty)
	{
		act("$n ������$g ��� : '� ��� ��� ������� ����� �������!'", FALSE, mailman, 0, ch, TO_VICT);
	}
}

} // namespace Parcel
