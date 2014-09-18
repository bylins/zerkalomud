/*************************************************************************
*   File: item.creation.cpp                            Part of Bylins    *
*   Item creation from magic ingidients                                  *
*                           *
*  $Author$                                                       *
*  $Date$                                          *
*  $Revision$                                                     *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "screen.h"
#include "spells.h"
#include "skills.h"
#include "constants.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "olc.h"
#include "im.h"
#include "features.hpp"
#include "item.creation.hpp"
#include "char.hpp"
#include "modify.h"
#include "room.hpp"

#define SpINFO   spell_info[spellnum]

extern int material_value[];

int slot_for_char(CHAR_DATA * ch, int i);
void char_dam_message(int dam, CHAR_DATA * ch, CHAR_DATA * victim, int attacktype, int mayflee);
void die(CHAR_DATA * ch, CHAR_DATA * killer);

struct create_item_type created_item[] = { {300, 0x7E, 15, 40, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
		(ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{301, 0x7E, 12, 40, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{302, 0x7E, 8, 25, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{303, 0x7E, 5, 13, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{304, 0x7E, 10, 35, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{305, 0, 8, 15, {{0, 0, 0}}, 0,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{306, 0, 8, 20, {{0, 0, 0}}, 0,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS | ITEM_WEAR_WIELD)},
	{307, 0x3A, 10, 20, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BODY)},
	{308, 0x3A, 4, 10, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_ARMS)},
	{309, 0x3A, 6, 12, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_LEGS)},
	{310, 0x3A, 4, 10, {{COAL_PROTO, 0, 0}}, SKILL_TRANSFORMWEAPON,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_HEAD)},
	{312, 0, 4, 40, {{WOOD_PROTO, TETIVA_PROTO, 0}}, SKILL_CREATEBOW,
	 (ITEM_WEAR_TAKE | ITEM_WEAR_BOTHS)}
};

const char *create_item_name[] = { "��������",
								   "���",
								   "�����",
								   "���",
								   "�����",
								   "�����",
								   "������",
								   "��������",
								   "������",
								   "������",
								   "����",
								   "���",
								   "\n"
								 };

const struct make_skill_type make_skills[] =
{
//  { "���������� �����","������", SKILL_MAKE_STAFF },
	{"���������� ���", "����", SKILL_MAKE_BOW},
	{"�������� ������", "������", SKILL_MAKE_WEAPON},
	{"�������� ������", "������", SKILL_MAKE_ARMOR},
	{"����� ������", "������", SKILL_MAKE_WEAR},
	{"���������� ��������", "�����.", SKILL_MAKE_JEWEL},
//  { "������� �����","������", SKILL_MAKE_POTION },
	{"\n", 0}		// ����������
};

const char *create_weapon_quality[] = { "RESERVED",
										"RESERVED",
										"RESERVED",
										"RESERVED",
										"RESERVED",
										"������������ �������� ��������",	/* <3 */
										"�������� ��������",	/* 3 */
										"����� ��� ������� ��������",
										"������� ��������",	/* 4 */
										"����� ��� ��������������� ��������",
										"��������������� ��������",	/* 5 */
										"����� ��� ������� ��������",
										"������� ��������",	/* 6 */
										"���� ��� �������� ��������",
										"�������� ��������",	/* 7 */
										"����� ��� �������� ��������",
										"��������� ��������",	/* 8 */
										"����� ��� ��������� ��������",
										"�������� ��������",	/* 9 */
										"����� ��� �������� ��������",
										"������������� ��������",	/* 10 */
										"����� ��� ������������� ��������",
										"������������� ��������",	/* 11 */
										"����� ��� ������������� ��������",
										"��������, ���������� �������� �������",	/* 12 */
										"������� ��������, ��� ������ ������� �������",
										"��������, ���������� �������",	/* 13 */
										"������� ��������, ��� ������ �������",
										"��������, ���������� �������� �������",	/* 14 */
										"������� ��������, ��� ������ ������� �������",
										"��������, ���������� ���� ���� ���������",	/* 15 */
										"��������, ����� ��� ���������� ���� ���� ���������",
										"��������, ���������� ���� �������",	/* 16 */
										"��������, ����� ��� ���������� ���� �������",
										"��������, ���������� ���� ���� �������",	/* 17 */
										"��������, ����� ��� ���������� ���� ���� �������",
										"��������, ���������� ���� �������",	/* 18 */
										"��������, ����� ��� ���������� ���� �������",
										"��������, ���������� ���� �������",	/* 19 */
										"��������, ����� ��� ���������� ���� �������",
										"��������, ���������� ���� �����",	/* 20 */
										"��������, ����� ��� ���������� ���� �����",
										"��������, ���������� ���� �����",	/* 21 */
										"��������, ����� ��� ���������� ���� �����",
										"��������, ���������� ���� �������� �����",	/* 22 */
										"��������, ����� ��� ���������� ���� �������� �����",
										"��������, ������� ����� ������ ������� ������",	/* 23 */
										"��������, ������� ��� ��, ������� ����� ������� ������",
										"��������, ���������� ������� �����",	/* 24 */
										"��������, ����� ��� ���������� ������� �����",
										"��������, ���������� �����",	/* 25 */
										"��������, ����� ��� ���������� �����",
										"��������, �������� ��������� ��������", // 26
										"��������, �������� ��������� ��������",
										"��������, �������� ��������� ��������", // 27
										"��������, �������� ��������� ��������",
										"��������, ���������� ������� �����", // >= 28
										"\n"
									  };

MakeReceptList make_recepts;

/* ������� ������ � ����� */
CHAR_DATA *& operator<<(CHAR_DATA * &ch, string p)
{
	send_to_char(p.c_str(), ch);
	return ch;
}

void init_make_items()
{
	char tmpbuf[MAX_INPUT_LENGTH];

	sprintf(tmpbuf, "Loading making recepts.");
	mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);

	make_recepts.load();
}

// ������ ���� ������������ � ���� ������ �������
void mredit_parse(DESCRIPTOR_DATA * d, char *arg)
{
	string sagr = string(arg);
	char tmpbuf[MAX_INPUT_LENGTH];
	string tmpstr;
	MakeRecept *trec = OLC_MREC(d);
	int i;

	switch (OLC_MODE(d))
	{
	case MREDIT_MAIN_MENU:
		// ���� �������� ����.
		if (sagr == "1")
		{
			send_to_char("������� VNUM ���������������� �������� : ", d->character);
			OLC_MODE(d) = MREDIT_OBJ_PROTO;
			return;
		}
		if (sagr == "2")
		{
			// �������� ������ ������ ... ��� ������ ������� �������.
			tmpstr = "\r\n������ ��������� ������:\r\n";
			i = 0;
			while (make_skills[i].num != 0)
			{
				sprintf(tmpbuf, "%s%d%s) %s.\r\n", grn, i + 1, nrm, make_skills[i].name);

				tmpstr += string(tmpbuf);

				i++;
			}
			tmpstr += "������� ����� ������ : ";
			send_to_char(tmpstr.c_str(), d->character);

			OLC_MODE(d) = MREDIT_SKILL;
			return;
		}
		if (sagr == "3")
		{
			send_to_char("����������� ������ ?(y/n) : ", d->character);
			OLC_MODE(d) = MREDIT_LOCK;
			return;
		}
		for (i = 0; i < MAX_PARTS; i++)
		{
			if (atoi(sagr.c_str()) - 4 == i)
			{
				OLC_NUM(d) = i;
				mredit_disp_ingr_menu(d);
				return;
			}
		}
		if (sagr == "d")
		{
			send_to_char("������� ������ ?(y/n) :", d->character);
			OLC_MODE(d) = MREDIT_DEL;
			return;
		}

		if (sagr == "s")
		{
			// ��������� ������� � ����
			make_recepts.save();
			send_to_char("������� ���������.\r\n", d->character);
			mredit_disp_menu(d);
			OLC_VAL(d) = 0;
			return;
		}
		if (sagr == "q")
		{
			// ��������� �� ������������� �� ���������
			if (OLC_VAL(d))
			{
				send_to_char("�� ������� ��������� ��������� � ������� ?(y/n) : ", d->character);
				OLC_MODE(d) = MREDIT_CONFIRM_SAVE;
				return;
			}
			else
			{
				// ��������� ������� �� �����
				// ��� ����������� ������� ��������� ���.
				make_recepts.load();
				// ������� ��������� OLC ������� � ���������� ����� ������
				cleanup_olc(d, CLEANUP_ALL);
				return;
			}
		}
		send_to_char("�������� ����.\r\n", d->character);
		mredit_disp_menu(d);
		break;

	case MREDIT_OBJ_PROTO:
		i = atoi(sagr.c_str());
		if (real_object(i) < 0)
		{
			send_to_char("�������� ���������� ���� ������� �� ����������.\r\n", d->character);
		}
		else
		{
			trec->obj_proto = i;
			OLC_VAL(d) = 1;
		}
		mredit_disp_menu(d);
		break;

	case MREDIT_SKILL:
		int skill_num;

		skill_num = atoi(sagr.c_str());
		i = 0;

		while (make_skills[i].num != 0)
		{
			if (skill_num == i + 1)
			{
				trec->skill = make_skills[i].num;
				OLC_VAL(d) = 1;
				mredit_disp_menu(d);
				return;
			}
			i++;
		}
		send_to_char("������� ������������ ������.\r\n", d->character);

		mredit_disp_menu(d);
		break;

	case MREDIT_DEL:
		if (sagr == "Y" || sagr == "y")
		{
			send_to_char("������ ������. ������� ���������.\r\n", d->character);

			make_recepts.del(trec);

			make_recepts.save();

			make_recepts.load();

			// ������� ��������� OLC ������� � ���������� ����� ������
			cleanup_olc(d, CLEANUP_ALL);
			return;

		}
		else if (sagr == "N" || sagr == "n")
		{
			send_to_char("������ �� ������.\r\n", d->character);

		}
		else
		{
			send_to_char("�������� ����.\r\n", d->character);
		}
		mredit_disp_menu(d);
		break;

	case MREDIT_LOCK:
		if (sagr == "Y" || sagr == "y")
		{
			send_to_char("������ ������������ �� �������������.\r\n", d->character);
			trec->locked = true;
			OLC_VAL(d) = 1;
		}
		else if (sagr == "N" || sagr == "n")
		{
			send_to_char("������ ������������� � ����� ��������������.\r\n", d->character);
			trec->locked = false;
			OLC_VAL(d) = 1;

		}
		else
		{
			send_to_char("�������� ����.\r\n", d->character);
		}
		mredit_disp_menu(d);
		break;

	case MREDIT_INGR_MENU:
		// ���� ���� ������������.
		if (sagr == "1")
		{
			send_to_char("������� VNUM ����������� : ", d->character);
			OLC_MODE(d) = MREDIT_INGR_PROTO;
			return;
		}
		if (sagr == "2")
		{
			send_to_char("������� ���.��� ����������� : ", d->character);
			OLC_MODE(d) = MREDIT_INGR_WEIGHT;
			return;
		}
		if (sagr == "3")
		{
			send_to_char("������� ���.���� ����������� : ", d->character);
			OLC_MODE(d) = MREDIT_INGR_POWER;
			return;
		}
		if (sagr == "q")
		{
			mredit_disp_menu(d);
			return;
		}
		send_to_char("�������� ����.\r\n", d->character);
		mredit_disp_ingr_menu(d);
		break;

	case MREDIT_INGR_PROTO:

		i = atoi(sagr.c_str());
		if (real_object(i) < 0)
		{
			send_to_char("�������� ���������� ���� ����������� �� ����������.\r\n", d->character);
		}
		else
		{
			trec->parts[OLC_NUM(d)].proto = i;
			OLC_VAL(d) = 1;
		}
		mredit_disp_ingr_menu(d);

		break;

	case MREDIT_INGR_WEIGHT:

		i = atoi(sagr.c_str());

		trec->parts[OLC_NUM(d)].min_weight = i;
		OLC_VAL(d) = 1;

		mredit_disp_ingr_menu(d);

		break;

	case MREDIT_INGR_POWER:

		i = atoi(sagr.c_str());

		trec->parts[OLC_NUM(d)].min_power = i;

		OLC_VAL(d) = 1;

		mredit_disp_ingr_menu(d);

		break;
	case MREDIT_CONFIRM_SAVE:
		if (sagr == "Y" || sagr == "y")
		{
			send_to_char("������� ���������.\r\n", d->character);

			make_recepts.save();

			make_recepts.load();
			// ������� ��������� OLC ������� � ���������� ����� ������
			cleanup_olc(d, CLEANUP_ALL);
			return;
		}
		else if (sagr == "N" || sagr == "n")
		{
			send_to_char("������ �� ��� ��������.\r\n", d->character);
			cleanup_olc(d, CLEANUP_ALL);
			return;
		}
		else
		{
			send_to_char("�������� ����.\r\n", d->character);
		}
		mredit_disp_menu(d);
		break;
	}
}

// ������ � ����� �������������� �������� ��� ���������.
ACMD(do_edit_make)
{
	string tmpstr;
	DESCRIPTOR_DATA *d;

	char tmpbuf[MAX_INPUT_LENGTH];

	int i;

	MakeRecept *trec;

	// ��������� �� ������ �� ���-�� ������� ��� ���������� ����������
	for (d = descriptor_list; d; d = d->next)
		if (d->olc && STATE(d) == CON_MREDIT)
		{
			sprintf(tmpbuf, "������� � ��������� ������ ������������� %s.\r\n", GET_PAD(d->character, 4));
			send_to_char(tmpbuf, ch);
			return;
		}

	argument = one_argument(argument, tmpbuf);

	if (!*tmpbuf)
	{
		// ����� �� ����� ������� ����� ������.
		trec = new(MakeRecept);
		// ���������� ������ �� ������ ��� �� ������������.

		make_recepts.add(trec);

		ch->desc->olc = new olc_data;
		// ������ � ��������� ������ �������.
		STATE(ch->desc) = CON_MREDIT;

		OLC_MREC(ch->desc) = trec;

		OLC_VAL(ch->desc) = 0;

		mredit_disp_menu(ch->desc);

		return;
	}

	i = atoi(tmpbuf);

	if ((i > make_recepts.size()) || (i <= 0))
	{
		send_to_char("���������� ������� �� ����������.", ch);

		return;
	}

	i -= 1;

	ch->desc->olc = new olc_data;

	STATE(ch->desc) = CON_MREDIT;

	OLC_MREC(ch->desc) = make_recepts[i];;

	mredit_disp_menu(ch->desc);

	return;
}

// ����������� ���� ���������� �����������.
void mredit_disp_ingr_menu(DESCRIPTOR_DATA * d)
{
	// ������ ���� ...
	MakeRecept *trec;
	string objname, ingrname, tmpstr;
	char tmpbuf[MAX_INPUT_LENGTH];

	int index = OLC_NUM(d);
	trec = OLC_MREC(d);
	get_char_cols(d->character);

	const OBJ_DATA *tobj = read_object_mirror(trec->obj_proto);
	if (trec->obj_proto && tobj)
		objname = tobj->PNames[0];
	else
		objname = "���";

	tobj = read_object_mirror(trec->parts[index].proto);
	if (trec->parts[index].proto && tobj)
		ingrname = tobj->PNames[0];
	else
		ingrname = "���";

	sprintf(tmpbuf,
#if defined(CLEAR_SCREEN)
			"[H[J"
#endif
			"\r\n\r\n"
			"-- ������ - %s%s%s (%d) \r\n"
			"%s1%s) ���������� : %s%s (%d)\r\n"
			"%s2%s) ���. ���   : %s%d\r\n"
			"%s3%s) ���. ����  : %s%d\r\n",
			grn, objname.c_str(), nrm, trec->obj_proto,
			grn, nrm, yel, ingrname.c_str(), trec->parts[index].proto,
			grn, nrm, yel, trec->parts[index].min_weight, grn, nrm, yel, trec->parts[index].min_power);


	tmpstr = string(tmpbuf);

	tmpstr += string(grn) + "q" + string(nrm) + ") �����\r\n";
	tmpstr += "��� ����� : ";

	send_to_char(tmpstr.c_str(), d->character);

	OLC_MODE(d) = MREDIT_INGR_MENU;
}


// ����������� �������� ����.
void mredit_disp_menu(DESCRIPTOR_DATA * d)
{
	// ������ ���� ...
	MakeRecept *trec;
	char tmpbuf[MAX_INPUT_LENGTH];
	string tmpstr, objname, skillname;

	trec = OLC_MREC(d);
	get_char_cols(d->character);

	const OBJ_DATA *tobj = read_object_mirror(trec->obj_proto);
	if (trec->obj_proto && tobj)
		objname = tobj->PNames[0];
	else
		objname = "���";

	int i = 0;
	//
	skillname = "���";
	while (make_skills[i].num != 0)
	{
		if (make_skills[i].num == trec->skill)
		{
			skillname = string(make_skills[i].name);
			break;
		}
		i++;
	}

	sprintf(tmpbuf,
#if defined(CLEAR_SCREEN)
			"[H[J"
#endif
			"\r\n\r\n"
			"-- ������ --\r\n"
			"%s1%s) �������    : %s%s (%d)\r\n"
			"%s2%s) ������     : %s%s (%d)\r\n"
			"%s3%s) ���������� : %s%s \r\n",
			grn, nrm, yel, objname.c_str(), trec->obj_proto,
			grn, nrm, yel, skillname.c_str(), trec->skill, grn, nrm, yel, (trec->locked ? "��" : "���"));

	tmpstr = string(tmpbuf);

	for (int i = 0; i < MAX_PARTS; i++)
	{
		tobj = read_object_mirror(trec->parts[i].proto);
		if (trec->parts[i].proto && tobj)
			objname = tobj->PNames[0];
		else
			objname = "���";

		sprintf(tmpbuf,
				"%s%d%s) ��������� %d: %s%s (%d)\r\n",
				grn, i + 4, nrm, i + 1, yel, objname.c_str(), trec->parts[i].proto);
		tmpstr += string(tmpbuf);
	};

	tmpstr += string(grn) + "d" + string(nrm) + ") �������\r\n";
	tmpstr += string(grn) + "s" + string(nrm) + ") ���������\r\n";
	tmpstr += string(grn) + "q" + string(nrm) + ") �����\r\n";

	tmpstr += "��� ����� : ";

	send_to_char(tmpstr.c_str(), d->character);

	OLC_MODE(d) = MREDIT_MAIN_MENU;
}

ACMD(do_list_make)
{
	string tmpstr, skill_name, obj_name;
	char tmpbuf[MAX_INPUT_LENGTH];

	MakeRecept *trec;

	if (make_recepts.size() == 0)
	{
		send_to_char("������� � ���� ���� �� ����������.", ch);
		return;
	}

	// ������ ������ �������� ���� �������� ��� � ��������.
	tmpstr = "###  �  ������  �������             ������������                         \r\n";
	tmpstr += "------------------------------------------------------------------------------\r\n";

	for (int i = 0; i < make_recepts.size(); i++)
	{
		int j = 0;
		skill_name = "���";
		obj_name = "���";

		trec = make_recepts[i];

		const OBJ_DATA *obj = read_object_mirror(trec->obj_proto);
		if (obj)
			obj_name = string(obj->PNames[0]).substr(0, 11);

		while (make_skills[j].num != 0)
		{
			if (make_skills[j].num == trec->skill)
			{
				skill_name = string(make_skills[j].short_name);
				break;
			}
			j++;
		}

		sprintf(tmpbuf, "%3d  %-1s  %-6s  %-12s(%5d) :",
				i + 1, (trec->locked ? "*" : " "), skill_name.c_str(), obj_name.c_str(), trec->obj_proto);


		tmpstr += string(tmpbuf);


		for (int j = 0; j < MAX_PARTS; j++)
		{
			if (trec->parts[j].proto != 0)
			{

				obj = read_object_mirror(trec->parts[j].proto);
				if (obj)
					obj_name = string(obj->PNames[0]).substr(0, 11);
				else
					obj_name = "���";

				sprintf(tmpbuf, " %-12s(%5d)", obj_name.c_str(), trec->parts[j].proto);

				if (j > 0)
				{
					tmpstr += ",";
					if (j % 2 == 0)
					{
						// ��������� ������� ���� ������ ������ 2;
						tmpstr += "\r\n";
						tmpstr += "                                    :";
					}

				};

				tmpstr += string(tmpbuf);
			}
			else
				break;

		}

		tmpstr += "\r\n";

	}

	page_string(ch->desc, tmpstr);
	return;
}

// �������� ������ �������� �� ����������.
ACMD(do_make_item)
{
	// ��� ������ �������.

	// ���� �������� ��� ��������� �� ������� ������ ���� ��������
	// ��������� ��� ������������ ��������� �� ��� ������

	// ��������� ����� ���, ����� , � ��������(������ ���������)

	// ��� ������� make

	// �������� ����� ������ � ������ (���) ������ ������. �������� ����

	// ������� �����

	// ����� ������

	string tmpstr;
	MakeReceptList *canlist;
	MakeRecept *trec;
	char tmpbuf[MAX_INPUT_LENGTH];
	int i, used_skill;

	used_skill = subcmd;
	argument = one_argument(argument, tmpbuf);
	canlist = new MakeReceptList;

	// ��������� � ����������� �� ���� ��� ������� ... ������ ��������
	switch (subcmd)
	{
	case(MAKE_POTION):
		// ����� �����.
		tmpstr = "�� ������ �������:\r\n";
		make_recepts.can_make(ch, canlist, SKILL_MAKE_POTION);

		break;

	case(MAKE_WEAR):
		// ���� ������.
		tmpstr = "�� ������ �����:\r\n";
		make_recepts.can_make(ch, canlist, SKILL_MAKE_WEAR);

		break;

	case(MAKE_METALL):

		tmpstr = "�� ������ ��������:\r\n";

		make_recepts.can_make(ch, canlist, SKILL_MAKE_WEAPON);

		make_recepts.can_make(ch, canlist, SKILL_MAKE_ARMOR);

		break;

	case(MAKE_CRAFT):

		tmpstr = "�� ������ ����������:\r\n";

		make_recepts.can_make(ch, canlist, SKILL_MAKE_STAFF);

		make_recepts.can_make(ch, canlist, SKILL_MAKE_BOW);

		make_recepts.can_make(ch, canlist, SKILL_MAKE_JEWEL);
		break;
	}

	if (canlist->size() == 0)
	{
		// ��� �� ����� ������� ������.
		send_to_char("�� ������ �� ������ �������.\r\n", ch);
		return;
	}

	if (!*tmpbuf)
	{
		// ������� ��� ������ ��������� ������� ����� �������.
		for (i = 0; i < canlist->size(); i++)
		{
			const OBJ_DATA *tobj = read_object_mirror((*canlist)[i]->obj_proto);
			if (!tobj)
				return;
			sprintf(tmpbuf, "%d) %s\r\n", i + 1, tobj->PNames[0]);
			tmpstr += string(tmpbuf);
		};
		send_to_char(tmpstr.c_str(), ch);
		return;
	}
	// ���������� �� ������ ���� �� ������, ���� �� �������� � �������.

	tmpstr = string(tmpbuf);

	i = atoi(tmpbuf);

	if ((i > 0) && (i <= canlist->size())
			&& (tmpstr.find(".") > tmpstr.size()))
	{
		trec = (*canlist)[i - 1];
	}
	else
	{
		trec = canlist->get_by_name(tmpstr);
		if (trec == NULL)
		{
			tmpstr = "������ � ��� ���������� ������.\r\n";
			send_to_char(tmpstr.c_str(), ch);
			free(canlist);
			return;
		}
	};

	trec->make(ch);
	free(canlist);

	return;
}

void go_create_weapon(CHAR_DATA * ch, OBJ_DATA * obj, int obj_type, int skill)
{
	OBJ_DATA *tobj;
	char txtbuff[100];
	const char *to_char = NULL, *to_room = NULL;
	int prob, percent, ndice, sdice, weight;
	float average;

	if (obj_type == 5 || obj_type == 6)
	{
		weight = number(created_item[obj_type].min_weight, created_item[obj_type].max_weight);
		percent = 100;
		prob = 100;
	}
	else
	{
		if (!obj)
			return;
		skill = created_item[obj_type].skill;
		percent = number(1, skill_info[skill].max_percent);
		prob = train_skill(ch, skill, skill_info[skill].max_percent, 0);
		weight = MIN(GET_OBJ_WEIGHT(obj) - 2, GET_OBJ_WEIGHT(obj) * prob / percent);
	}

	if (weight < created_item[obj_type].min_weight)
		send_to_char("� ��� �� ������� ���������.\r\n", ch);
	else if (prob * 5 < percent)
		send_to_char("� ��� ������ �� ����������.\r\n", ch);
	else if (!(tobj = read_object(created_item[obj_type].obj_vnum, VIRTUAL)))
		send_to_char("������� ��� ������������ ������.\r\n", ch);
	else
	{
		GET_OBJ_WEIGHT(tobj) = MIN(weight, created_item[obj_type].max_weight);
		GET_OBJ_COST(tobj) = 2 * GET_OBJ_COST(obj) / 3;
		GET_OBJ_OWNER(tobj) = GET_UNIQUE(ch);

// ����� �������� �� �������.
// ��� 5+ ������ ����� ���� ������� ���� � 3 �������: ������� 2% � �� 0.5% �� ����
// ��� 2 ������ ������� ���� 5%, 1% �� ������ ����
// ��� 1 ����� ������ 20% � 4% �� ������ ����
// �������. ����������. ������ �� ����� ����� � ����� ����.
		if (skill == SKILL_TRANSFORMWEAPON)
		{
			if (ch->get_skill(skill) >= 105 && number(1, 100) <= 2 + (ch->get_skill(skill) - 105) / 10)
				SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_WITH3SLOTS), ITEM_WITH3SLOTS);
			else if (number(1, 100) <= 5 + MAX((ch->get_skill(skill) - 80), 0) / 5)
				SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_WITH2SLOTS), ITEM_WITH2SLOTS);
			else if (number(1, 100) <= 20 + MAX((ch->get_skill(skill) - 80), 0) / 5 * 4)
				SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_WITH1SLOT), ITEM_WITH1SLOT);
		}

		switch (obj_type)
		{
		case 0:	/* smith weapon */
		case 1:
		case 2:
		case 3:
		case 4:
		case 11:
		{
			// �������. ������ ������ �������� �� ������� ���������.
			// ������� MAX(<�������>, <��������>/100*<������� �����>-<������ �� 0 �� 25% ���������>)
			// � �������� ���� ���� �����, � ��������� ������ �� ���������
			int timer = MAX(ONE_DAY,
					tobj->get_timer() / 100 * ch->get_skill(skill)
							- number(0, tobj->get_timer() / 100 * 25));
			tobj->set_timer(timer);
			sprintf(buf, "���� ������� ����������� �������� %d ����\n", tobj->get_timer() / 24 / 60);
			act(buf, FALSE, ch, tobj, 0, TO_CHAR);
			GET_OBJ_MATER(tobj) = GET_OBJ_MATER(obj);
			// �������. ��� ��������.
			// ���� GET_OBJ_MAX(tobj) = MAX(50, MIN(300, 300 * prob / percent));
			// ������� MAX(<�������>, <��������>/100*<������� �����>-<������ �� 0 �� 25% ���������>)
			// ��� ������� ����� �������� �� 100, ����� �������������� ������� �� 100. ��� �� �������� �������.
			GET_OBJ_MAX(tobj) = MAX(20000, 35000 / 100 * ch->get_skill(skill) - number(0, 35000 / 100 * 25)) / 100;
			GET_OBJ_CUR(tobj) = GET_OBJ_MAX(tobj);
			percent = number(1, skill_info[skill].max_percent);
			prob = calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
			ndice = MAX(2, MIN(4, prob / percent));
			ndice += GET_OBJ_WEIGHT(tobj) / 10;
			percent = number(1, skill_info[skill].max_percent);
			prob = calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
			sdice = MAX(2, MIN(5, prob / percent));
			sdice += GET_OBJ_WEIGHT(tobj) / 10;
			GET_OBJ_VAL(tobj, 1) = ndice;
			GET_OBJ_VAL(tobj, 2) = sdice;
			SET_BIT(tobj->obj_flags.wear_flags, created_item[obj_type].wear);
			if (skill != SKILL_CREATEBOW)
			{
				if (GET_OBJ_WEIGHT(tobj) < 14 && percent * 4 > prob)
					SET_BIT(tobj->obj_flags.wear_flags, ITEM_WEAR_HOLD);
				to_room = "$n �������$g $o3.";
				average = (((float) sdice + 1) * (float) ndice / 2.0);
				if (average < 3.0)
					sprintf(txtbuff, "�� �������� $o3 %s.", create_weapon_quality[(int)(2.5 * 2)]);
				else if (average <= 27.5)
					sprintf(txtbuff, "�� �������� $o3 %s.",
							create_weapon_quality[(int)(average * 2)]);
				else
					sprintf(txtbuff, "�� �������� $o3 %s!", create_weapon_quality[56]);
				to_char = (char *) txtbuff;
			}
			else
			{
				to_room = "$n ���������$g $o3.";
				to_char = "�� ���������� $o3.";
			}
			break;
		}
		case 5:	/* mages weapon */
		case 6:
			tobj->set_timer(ONE_DAY);
			GET_OBJ_MAX(tobj) = 50;
			GET_OBJ_CUR(tobj) = 50;
			ndice = MAX(2, MIN(4, GET_LEVEL(ch) / number(6, 8)));
			ndice += (GET_OBJ_WEIGHT(tobj) / 10);
			sdice = MAX(2, MIN(5, GET_LEVEL(ch) / number(4, 5)));
			sdice += (GET_OBJ_WEIGHT(tobj) / 10);
			GET_OBJ_VAL(tobj, 1) = ndice;
			GET_OBJ_VAL(tobj, 2) = sdice;
			SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_NORENT), ITEM_NORENT);
			SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_DECAY), ITEM_DECAY);
			SET_BIT(GET_OBJ_EXTRA(tobj, ITEM_NOSELL), ITEM_NOSELL);
			SET_BIT(tobj->obj_flags.wear_flags, created_item[obj_type].wear);
			to_room = "$n ������$g $o3.";
			to_char = "�� ������� $o3.";
			break;
		case 7:	/* smith armor */
		case 8:
		case 9:
		case 10:
		{
			// �������. ������ ������ �������� �� ������� ���������.
			// ������� MAX(<�������>, <��������>/100*<������� �����>-<������ �� 0 �� 25% ���������>)
			// � �������� ���� ���� �����, � ��������� ������ �� ���������
			int timer = MAX(ONE_DAY,
					tobj->get_timer() / 100 * ch->get_skill(skill)
							- number(0, tobj->get_timer() / 100 * 25));
			tobj->set_timer(timer);
			sprintf(buf, "���� ������� ����������� �������� %d ����\n", tobj->get_timer() / 24 / 60);
			act(buf, FALSE, ch, tobj, 0, TO_CHAR);
			GET_OBJ_MATER(tobj) = GET_OBJ_MATER(obj);
			// �������. ��� ��������.
			// ���� GET_OBJ_MAX(tobj) = MAX(50, MIN(300, 300 * prob / percent));
			// ������� MAX(<�������>, <��������>/100*<������� �����>-<������ �� 0 �� 25% ���������>)
			// ��� ������� ����� �������� �� 100, ����� �������������� ������� �� 100. ��� �� �������� �������.
			GET_OBJ_MAX(tobj) = MAX(20000, 10000 / 100 * ch->get_skill(skill) - number(0, 15000 / 100 * 25)) / 100;
			GET_OBJ_CUR(tobj) = GET_OBJ_MAX(tobj);
			percent = number(1, skill_info[skill].max_percent);
			prob = calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
			ndice = MAX(2, MIN((105 - material_value[GET_OBJ_MATER(tobj)]) / 10, prob / percent));
			percent = number(1, skill_info[skill].max_percent);
			prob = calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
			sdice = MAX(1, MIN((105 - material_value[GET_OBJ_MATER(tobj)]) / 15, prob / percent));
			GET_OBJ_VAL(tobj, 0) = ndice;
			GET_OBJ_VAL(tobj, 1) = sdice;
			SET_BIT(tobj->obj_flags.wear_flags, created_item[obj_type].wear);
			to_room = "$n �������$g $o3.";
			to_char = "�� �������� $o3.";
			break;
		}
		} // switch
		if (to_char)
			act(to_char, FALSE, ch, tobj, 0, TO_CHAR);
		if (to_room)
			act(to_room, FALSE, ch, tobj, 0, TO_ROOM);

		if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
		{
			send_to_char("�� �� ������� ������ ������� ���������.\r\n", ch);
			obj_to_room(tobj, IN_ROOM(ch));
			obj_decay(tobj);
		}
		else if (IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(tobj) > CAN_CARRY_W(ch))
		{
			send_to_char("�� �� ������� ������ ����� ���.\r\n", ch);
			obj_to_room(tobj, IN_ROOM(ch));
			obj_decay(tobj);
		}
		else
			obj_to_char(tobj, ch);
	}
	if (obj)
	{
		obj_from_char(obj);
		extract_obj(obj);
	}
}


ACMD(do_transform_weapon)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	OBJ_DATA *obj = NULL, *coal, *proto[MAX_PROTO];
	int obj_type, i, found, rnum;

	if (IS_NPC(ch) || !ch->get_skill(subcmd))
	{
		send_to_char("��� ����� ����� �� ������.\r\n", ch);
		return;
	}

	argument = one_argument(argument, arg1);
	if (!*arg1)
	{
		switch (subcmd)
		{
		case SKILL_TRANSFORMWEAPON:
			send_to_char("�� ��� �� ������ ���������� ?\r\n", ch);
			break;
		case SKILL_CREATEBOW:
			send_to_char("��� �� ������ ���������� ?\r\n", ch);
			break;
		}
		return;
	}
	if ((obj_type = search_block(arg1, create_item_name, FALSE)) == -1)
	{
		switch (subcmd)
		{
		case SKILL_TRANSFORMWEAPON:
			send_to_char("���������� ����� � :\r\n", ch);
			break;
		case SKILL_CREATEBOW:
			send_to_char("���������� ����� :\r\n", ch);
			break;
		}
		for (obj_type = 0; *create_item_name[obj_type] != '\n'; obj_type++)
			if (created_item[obj_type].skill == subcmd)
			{
				sprintf(buf, "- %s\r\n", create_item_name[obj_type]);
				send_to_char(buf, ch);
			}
		return;
	}
	if (created_item[obj_type].skill != subcmd)
	{
		switch (subcmd)
		{
		case SKILL_TRANSFORMWEAPON:
			send_to_char("������ ������� �������� ������.\r\n", ch);
			break;
		case SKILL_CREATEBOW:
			send_to_char("������ ������� ���������� ������.\r\n", ch);
			break;
		}
		return;
	}

	for (i = 0; i < MAX_PROTO; proto[i++] = NULL);

	argument = one_argument(argument, arg2);
	if (!*arg2)
	{
		send_to_char("��� ������ ������������.\r\n", ch);
		return;
	}
	if (!(obj = get_obj_in_list_vis(ch, arg2, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� '%s'.\r\n", arg2);
		send_to_char(buf, ch);
		return;
	}
	if (obj->contains)
	{
		act("� $o5 ���-�� �����.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}

	if (GET_OBJ_TYPE(obj) == ITEM_INGRADIENT)
	{
		for (i = 0, found = FALSE; i < MAX_PROTO; i++)
			if (GET_OBJ_VAL(obj, 1) == created_item[obj_type].proto[i])
			{
				if (proto[i])
					found = TRUE;
				else
				{
					proto[i] = obj;
					found = FALSE;
					break;
				}
			}
		if (i >= MAX_PROTO)
		{
			if (found)
				act("������, �� ��� ���-�� ����������� ������ $o1.", FALSE, ch, obj, 0, TO_CHAR);
			else
				act("������, $o �� �������� ��� �����.", FALSE, ch, obj, 0, TO_CHAR);
			return;
		}
	}
	else
		if (created_item[obj_type].material_bits &&
				!IS_SET(created_item[obj_type].material_bits, (1 << GET_OBJ_MATER(obj))))
		{
			act("$o ������$G �� ������������� ���������.", FALSE, ch, obj, 0, TO_CHAR);
			return;
		}
	switch (subcmd)
	{
	case SKILL_TRANSFORMWEAPON:

		// ��������� �������� �� ���� ������ ������
		// ����� �� ���� ������ � ���������� �� ����.
		if (created_item[obj_type].material_bits &&
				!IS_SET(created_item[obj_type].material_bits, (1 << GET_OBJ_MATER(obj))))
		{
			act("$o ������$G �� ������������� ���������.", FALSE, ch, obj, 0, TO_CHAR);
			return;
		}

		if (!IS_IMMORTAL(ch))
		{
			if (!ROOM_FLAGGED(IN_ROOM(ch), ROOM_SMITH))
			{
				send_to_char("��� ����� ������� � ������� ��� �����.\r\n", ch);
				return;
			}

			for (coal = ch->carrying; coal; coal = coal->next_content)
				if (GET_OBJ_TYPE(coal) == ITEM_INGRADIENT)
				{
					for (i = 0; i < MAX_PROTO; i++)
						if (proto[i] == coal)
							break;
						else if (!proto[i]
								 && GET_OBJ_VAL(coal, 1) == created_item[obj_type].proto[i])
						{
							proto[i] = coal;
							break;
						}
				}
			for (i = 0, found = TRUE; i < MAX_PROTO; i++)
				if (created_item[obj_type].proto[i] && !proto[i])
				{
					if ((rnum = real_object(created_item[obj_type].proto[i])) < 0)
						act("� ��� ��� ������������ �����������.", FALSE, ch, 0, 0, TO_CHAR);
					else
						act("� ��� �� ������� $o1 ��� �����.", FALSE, ch,
							obj_proto[rnum], 0, TO_CHAR);
					found = FALSE;
				}
			if (!found)
				return;
		}
		for (i = 0; i < MAX_PROTO; i++)
		{
			if (proto[i] && proto[i] != obj)
			{
				GET_OBJ_COST(obj) += GET_OBJ_COST(proto[i]);
				extract_obj(proto[i]);
			}
		}
		go_create_weapon(ch, obj, obj_type, SKILL_TRANSFORMWEAPON);
		break;
	case SKILL_CREATEBOW:
		for (coal = ch->carrying; coal; coal = coal->next_content)
			if (GET_OBJ_TYPE(coal) == ITEM_INGRADIENT)
			{
				for (i = 0; i < MAX_PROTO; i++)
					if (proto[i] == coal)
						break;
					else if (!proto[i]
							 && GET_OBJ_VAL(coal, 1) == created_item[obj_type].proto[i])
					{
						proto[i] = coal;
						break;
					}
			}
		for (i = 0, found = TRUE; i < MAX_PROTO; i++)
			if (created_item[obj_type].proto[i] && !proto[i])
			{
				if ((rnum = real_object(created_item[obj_type].proto[i])) < 0)
					act("� ��� ��� ������������ �����������.", FALSE, ch, 0, 0, TO_CHAR);
				else
					act("� ��� �� ������� $o1 ��� �����.", FALSE, ch, obj_proto[rnum], 0, TO_CHAR);
				found = FALSE;
			}
		if (!found)
			return;
		for (i = 1; i < MAX_PROTO; i++)
			if (proto[i])
			{
				GET_OBJ_WEIGHT(proto[0]) += GET_OBJ_WEIGHT(proto[i]);
				GET_OBJ_COST(proto[0]) += GET_OBJ_COST(proto[i]);
				extract_obj(proto[i]);
			}
		go_create_weapon(ch, proto[0], obj_type, SKILL_CREATEBOW);
		break;
	}
}

int ext_search_block(const char *arg, const char **list, int exact)
{
	register int i, l = strlen(arg), j, o;

	if (exact)
	{
		for (i = j = 0, o = 1; j != 1 && **(list + i); i++)	// shapirus: ������� � ��� ������ ����
			if (**(list + i) == '\n')
			{
				o = 1;
				switch (j)
				{
				case 0:
					j = INT_ONE;
					break;
				case INT_ONE:
					j = INT_TWO;
					break;
				case INT_TWO:
					j = INT_THREE;
					break;
				default:
					j = 1;
					break;
				}
			}
			else if (!str_cmp(arg, *(list + i)))
				return (j | o);
			else
				o <<= 1;
	}
	else
	{
		if (!l)
			l = 1;	/* Avoid "" to match the first available
				 * string */
		for (i = j = 0, o = 1; j != 1 && **(list + i); i++)	// shapirus: ������� � ��� ������ ����
			if (**(list + i) == '\n')
			{
				o = 1;
				switch (j)
				{
				case 0:
					j = INT_ONE;
					break;
				case INT_ONE:
					j = INT_TWO;
					break;
				case INT_TWO:
					j = INT_THREE;
					break;
				default:
					j = 1;
					break;
				}
			}
			else if (!strn_cmp(arg, *(list + i), l))
				return (j | o);
			else
				o <<= 1;
	}

	return (0);
}

// *****************************
MakeReceptList::MakeReceptList()
{
	// ������������� ������ ����� .
}

MakeReceptList::~MakeReceptList()
{
	// ���������� ������.
	clear();
}

int
MakeReceptList::load()
{
	// ������ ��� ���� � ��������.

	// ���� �������� ���������� !!!

	char tmpbuf[MAX_INPUT_LENGTH];
	string tmpstr;

	// ������ ������ �������� �� ������ ������.
	clear();

	MakeRecept *trec;

	ifstream bifs(LIB_MISC "makeitems.lst");
	if (!bifs)
	{
		sprintf(tmpbuf, "MakeReceptList:: Unable open input file !!!");
		mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);
		return (FALSE);
	}
	while (!bifs.eof())
	{
		bifs.getline(tmpbuf, MAX_INPUT_LENGTH, '\n');

		tmpstr = string(tmpbuf);

		// ���������� ����������������� �������.
		if (tmpstr.substr(0, 2) == "//")
			continue;

//    cout << "Get str from file :" << tmpstr << endl;

		if (tmpstr.size() == 0)
			continue;

		trec = new(MakeRecept);

		if (trec->load_from_str(tmpstr))
		{
			recepts.push_back(trec);
		}
		else
		{
			delete trec;
			// ������ ������������ �� ������ ��������
			sprintf(tmpbuf, "MakeReceptList:: Fail get recept from line.");
			mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);
		}

	}
	return TRUE;
}

int MakeReceptList::save()
{
	// ����� ��� ���� � ��������.
	// ������� ������
	string tmpstr;
	char tmpbuf[MAX_INPUT_LENGTH];

	list < MakeRecept * >::iterator p;

	ofstream bofs(LIB_MISC "makeitems.lst");

	if (!bofs)
	{
		// cout << "Unable input stream to create !!!" << endl;
		sprintf(tmpbuf, "MakeReceptList:: Unable create output file !!!");
		mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);

		return (FALSE);
	}

	sort();

	p = recepts.begin();

	while (p != recepts.end())
	{
		if ((*p)->save_to_str(tmpstr))
			bofs << tmpstr << endl;
		p++;
	}
	bofs.close();

	return 0;
}

void MakeReceptList::add(MakeRecept * recept)
{
	recepts.push_back(recept);
	return;
}

void MakeReceptList::del(MakeRecept * recept)
{
	recepts.remove(recept);
	return;
}

bool by_skill(MakeRecept * const &lhs, MakeRecept * const &rhs)
{
	return ((lhs->skill) > (rhs->skill));
}

void MakeReceptList::sort()
{
	// ������� ���������� �� �������.
	recepts.sort(by_skill);
	return;
}

int MakeReceptList::size()
{
	return recepts.size();
}

void MakeReceptList::clear()
{
	// ������� ������
	list < MakeRecept * >::iterator p;

	p = recepts.begin();

	while (p != recepts.end())
	{
		delete(*p);
		p++;
	}
	recepts.erase(recepts.begin(), recepts.end());

	return;
}

MakeRecept *MakeReceptList::operator[](int i)
{
	list < MakeRecept * >::iterator p;
	int j = 0;

	p = recepts.begin();

	while (p != recepts.end())
	{
		if (i == j)
			return (*p);
		j++;
		p++;
	}
	return (NULL);
}

MakeRecept *MakeReceptList::get_by_name(string & rname)
{
	// ���� �� ������ ������ � ����� ������.
	// ���� �� ������ ������� ������� ��� ����� ����������.
	list < MakeRecept * >::iterator p;

	string tmpstr;
	p = recepts.begin();


	int i, j, k;

	i = rname.find(".");

	if (i < 0)
	{
		// ������ �� �������.
		k = 1;
		i = -1;

	}
	else if (i == 0)
		k = 1;
	else
		k = atoi(rname.substr(0, i).c_str());

	if (k <= 0)
		return (NULL);	// ���� ����� -3.xxx

	j = 0;

	// cout <<  rname.substr(i+1) << endl;

	rname = rname.substr(i + 1);

	while (p != recepts.end())
	{
		const OBJ_DATA *tobj = read_object_mirror((*p)->obj_proto);
		if (!tobj)
			return 0;
		tmpstr = string(tobj->name);

//    cout << tmpstr << endl;

		tmpstr += " ";

		while (int (tmpstr.find(" ")) > 0)
		{
			if ((tmpstr.substr(0, tmpstr.find(" "))).find(rname) == 0)
			{
				if ((k - 1) == j)
					return (*p);
				j++;
				break;
			}
			tmpstr = tmpstr.substr(tmpstr.find(" ") + 1);
//      cout << tmpstr << endl;
		}


		p++;
	}

	return (NULL);
}

MakeReceptList *MakeReceptList::can_make(CHAR_DATA * ch, MakeReceptList * canlist, int use_skill)
{
	// ���� �� ������ ������� ������� ��� ����� ����������.
	list < MakeRecept * >::iterator p;
	MakeRecept *trec;

	p = recepts.begin();

	while (p != recepts.end())
	{

		if (((*p)->skill == use_skill) && ((*p)->can_make(ch)))
		{

			trec = new MakeRecept;
			trec = *p;
			canlist->add(trec);
		}
		p++;
	}

	return (canlist);
}

MakeRecept::MakeRecept()
{
	locked = true;		// �� ��������� ������ �������.
	skill = 0;
	obj_proto = 0;
	for (int i = 0; i < MAX_PARTS; i++)
	{
		parts[i].proto = 0;
		parts[i].min_weight = 0;
		parts[i].min_power = 0;
	}
}

MakeRecept::~MakeRecept()
{

}



int MakeRecept::can_make(CHAR_DATA * ch)
{
	int i, spellnum;
	OBJ_DATA *ingrobj = NULL;
	// char tmpbuf[MAX_INPUT_LENGTH];
	// ������� �������� �� ���� locked

	if (!ch)
		return (FALSE);

	if (locked)
		return (FALSE);

	// ������� �������� ������� ������ � ������.
	if (IS_NPC(ch) || !ch->get_skill(skill))
	{
		return (FALSE);
	}
	// ������ �������� ����� �� ��� ������� ����� ������ ����
	if ((skill == SKILL_MAKE_STAFF))
	{
		const OBJ_DATA *tobj = read_object_mirror(obj_proto);
		if (!tobj)
			return 0;
		spellnum = GET_OBJ_VAL(tobj, 3);

//   if (!((GET_OBJ_TYPE(tobj) == ITEM_WAND )||(GET_OBJ_TYPE(tobj) == ITEM_WAND )))

		// ����� ������ ����� ��������� ���� �� ���������� ���� � ������.
		if (!IS_SET(GET_SPELL_TYPE(ch, spellnum), SPELL_TEMP | SPELL_KNOW) && !IS_IMMORTAL(ch))
		{
			if (GET_LEVEL(ch) < SpINFO.min_level[(int) GET_CLASS(ch)][(int) GET_KIN(ch)] ||
					slot_for_char(ch, SpINFO.slot_forc[(int) GET_CLASS(ch)][(int) GET_KIN(ch)]) <= 0)
			{
				//send_to_char("���� ��� ��� ��������� ������ ������� !\r\n", ch);
				return (FALSE);
			}
			else
			{
				// send_to_char("���� �� ������� �������, ��� ������, ��� ����������...\r\n", ch);
				return (FALSE);
			}
		}
	}

	for (i = 0; i < MAX_PARTS; i++)
	{
		if (parts[i].proto == 0)
		{
			break;
		}

		if (real_object(parts[i].proto) < 0)
			return (FALSE);
//      send_to_char("������� ��� ������������ ������.\r\n",ch); //����� ����� ���� ��� ���� ������

		if (!(ingrobj = get_obj_in_list_vnum(parts[i].proto, ch->carrying)))
		{
//       sprintf(tmpbuf,"��� '%d' � ��� ��� '%d'.\r\n",obj_proto,parts[i].proto);
//       send_to_char(tmpbuf,ch);
			return (FALSE);
		}

		int ingr_lev = get_ingr_lev(ingrobj);
		// ���� ��� ���� ������ ����������� �� �� �� ����� ������ ������� � ���
		// ��������.
		if (ingr_lev > (GET_LEVEL(ch) + 2 * GET_REMORT(ch)))
		{
			return (FALSE);
		}
	}
	return (TRUE);
}


int MakeRecept::get_ingr_lev(OBJ_DATA * ingrobj)
{
	// �������� ������� ����������� ...
	if (GET_OBJ_TYPE(ingrobj) == ITEM_INGRADIENT)
	{
		// �������� ������� ���������� �� 128
		return (GET_OBJ_VAL(ingrobj, 0) >> 8);

	}
	else if (GET_OBJ_TYPE(ingrobj) == ITEM_MING || GET_OBJ_TYPE(ingrobj) == ITEM_MATERIAL)
	{
		// � ������ ���� 26 ��������� ������� � ����.
		return GET_OBJ_VAL(ingrobj, IM_POWER_SLOT);

	}
	else
		return (-1);
}


int MakeRecept::get_ingr_pow(OBJ_DATA * ingrobj)
{
	// �������� ���� ����������� ...
	if (GET_OBJ_TYPE(ingrobj) == ITEM_INGRADIENT || GET_OBJ_TYPE(ingrobj) == ITEM_MATERIAL)
	{

		return GET_OBJ_VAL(ingrobj, 2);

	}
	else if (GET_OBJ_TYPE(ingrobj) == ITEM_MING)
	{

		return GET_OBJ_VAL(ingrobj, IM_POWER_SLOT);

	}
	else
		return (-1);
}


// ������� ������� �� �������
int MakeRecept::make(CHAR_DATA * ch)
{
	char tmpbuf[MAX_STRING_LENGTH];	//, tmpbuf2[MAX_STRING_LENGTH];
	OBJ_DATA *ingrs[MAX_PARTS];
	string tmpstr, charwork, roomwork, charfail, roomfail, charsucc, roomsucc, chardam, roomdam, tagging, itemtag;
	int dam = 0;
	bool make_fail;

	// 1. ��������� ���� �� ����� � ����
	if (IS_NPC(ch) || !ch->get_skill(skill))
	{
		send_to_char("������� ��� ��� ������ ������ � ������ c������ ���.\r\n", ch);
		return (FALSE);
	}
	// 2. ��������� ���� �� ����� � ����
	if (!can_make(ch))
	{
		send_to_char("� ��� ��� ������������ ��� �����.\r\n", ch);
		return (FALSE);
	}

	if (GET_MOVE(ch) < MIN_MAKE_MOVE)
	{
		send_to_char("�� ������� ������ � ��� ������ �� ������� ������.\r\n", ch);
		return (FALSE);
	}

	const OBJ_DATA *tobj = read_object_mirror(obj_proto);
	if (!tobj)
		return 0;

	// ��������� ��������� �� ����� � ���� �� �����
	if (!IS_IMMORTAL(ch) && (skill == SKILL_MAKE_STAFF) && (GET_SPELL_MEM(ch, GET_OBJ_VAL(tobj, 3)) == 0))
	{
		act("�� �� ������ � ���� ����� ������� $o3.", FALSE, ch, tobj, 0, TO_CHAR);
		return (FALSE);
	}
	// ���������� � ������ �������� �����
	// 3. ��������� ������ ������ � ����
	int ingr_cnt = 0, ingr_lev, i, craft_weight, ingr_pow;

	for (i = 0; i < MAX_PARTS; i++)
	{
		if (parts[i].proto == 0)
			break;

		ingrs[i] = get_obj_in_list_vnum(parts[i].proto, ch->carrying);

		ingr_lev = get_ingr_lev(ingrs[i]);

		if (ingr_lev > (GET_LEVEL(ch) + 2 * GET_REMORT(ch)))
		{
			tmpstr = "�� ��������� ��������� " + string(ingrs[i]->PNames[3]) +
					 "\r\n � ���������� ������ ��� " + string(tobj->PNames[4]) + ".\r\n";
			send_to_char(tmpstr.c_str(), ch);
			return (FALSE);
		};

		ingr_pow = get_ingr_pow(ingrs[i]);

		if (ingr_pow < parts[i].min_power)
		{

			tmpstr = "$o �� �������� ��� ������������ " + string(tobj->PNames[1]) + ".";
			act(tmpstr.c_str(), FALSE, ch, ingrs[i], 0, TO_CHAR);
			return (FALSE);
		}

		ingr_cnt++;
	}

	int stat_bonus;

	// ������ ������ ��� �������� ��� ��������� ������.
	switch (skill)
	{
	case SKILL_MAKE_WEAPON:
	case SKILL_MAKE_ARMOR:
		// ��������� ���� �� ��� ���������� ��� ������� �����.
		if ((!ROOM_FLAGGED(IN_ROOM(ch), ROOM_SMITH)) && (!IS_IMMORTAL(ch)))
		{
			send_to_char("��� ����� ������� � ������� ��� �����.\r\n", ch);
			return (FALSE);
		}

		charwork = "�� ��������� ��������� �� ���������� � ������ ������ $o3.";
		roomwork = "$n ��������$g ��������� �� ���������� � �����$g ������.";
		charfail = "��������� ��� ������ ������� ��������� ����� ������ � �����������.";
		roomfail = "��� ������� ������ $n1 ��������� �����������.";
		charsucc = "�� �������� $o3.";
		roomsucc = "$n �������$G $o3.";
		chardam = "��������� ��������� �� ��� ������ � ������ ������� ���.";
		roomdam = "��������� ��������� �� ��� ������ $n1, ������ $s ������.";
		tagging = "�� ��������� ���� ������ �� $o5.";
		itemtag = "�� $o5 ����� ������ '�������$g $n'.";

		dam = 70;
		// ����� ����
		stat_bonus = number(0, GET_REAL_STR(ch));

		break;

	case SKILL_MAKE_BOW:
		charwork = "�� ������ ��������� $o3.";
		roomwork = "$n ����� ��������� ���-�� ����� ������������ $o3.";
		charfail = "� ������� ������� $o ������$U � ����� �������� �����.";
		roomfail = "� ������� ������� $o ������$U � �������� ����� $n1.";
		charsucc = "�� c��������� $o3.";
		roomsucc = "$n ���������$g $o3.";
		chardam = "$o3 � ������� ������� ������$U �������� ��� ����.";
		roomdam = "$o3 � ������� ������� ������$U �������� ���� $n2.";
		tagging = "�� �������� ���� ��� �� $o5.";
		itemtag = "�� $o5 ����� ����� '���������$g $n'.";

		// ����� ��������
		stat_bonus = number(0, GET_REAL_DEX(ch));

		dam = 40;

		break;

	case SKILL_MAKE_WEAR:
		charwork = "�� ����� � ���� ������ � ������ ���� $o3.";
		roomwork = "$n ���� � ���� ������ � ����� ��������� ����.";
		charfail = "� ��� ������ �� ���������� �����.";
		roomfail = "$n ������� ���-�� �����, �� � ���� ������ �� �����.";
		charsucc = "�� ����� $o3.";
		roomsucc = "$n ����$g $o3.";
		chardam = "���� ������� ����� � ���� ����. ���������� ���� ����.";
		roomdam = "$n ������� �������$g ���� � ���� � ����. \r\n� � ���� ������ ���������� �������.";
		tagging = "�� ������� � $o2 ����� �� ����� ������.";
		itemtag = "�� $o5 �� �������� ����� '����$g $n'.";

		// ����� ���� , �� ����������� ������ :))
		stat_bonus = number(0, GET_REAL_CON(ch));

		dam = 30;

		break;

	case SKILL_MAKE_JEWEL:
		charwork = "�� ������ ��������� $o3.";
		roomwork = "$n ����� ��������� �����-�� ���������.";
		charfail = "���� ��������� �������� ��������� $o5.";
		roomfail = "��������� �������� $n, ������� $s ������ �������������.";
		charsucc = "�� ���������� $o3.";
		roomsucc = "$n ���������$g $o3.";
		chardam = "������ ������� ��������� �������� � ����� ��� � ����.\r\n��� ���� ������!";
		roomdam = "������ ������� ��������� �������� � ����� � ���� $n2.";
		tagging = "�� ��������� � $o2 �������� �� ����� ������.";
		itemtag = "� ������ ������� $o1 ��������� �������� 'C������ $n4'.";

		// ����� ����

		stat_bonus = number(0, GET_REAL_CHA(ch));

		dam = 30;

		break;

	case SKILL_MAKE_STAFF:
		charwork = "�� ������ ��������� $o3.";
		roomwork = "$n ����� ��������� ���-�� ���������� ��� ���� �������� �����.";
		charfail = "$o3 ������� ������� ���������� ������ � ������.";
		roomfail = "������� � ����� $n1 ��������, ������ ������� ���������� ������ � ������.";
		charsucc =
			"������ ����� ���������� �� $o3 ��������� � �������.\r\n��, $E ������ �������� ������ �������.";
		roomsucc = "$n ���������$g $o3. �� ������������ ������� ���� ���������� � ���� ��������.";
		chardam = "$o ��������� � ����� �����. ��� ������ �������.";
		roomdam = "$o ��������� � ����� $n1, ������ ���.\r\n������ ������� ������� �������� �����.";
		tagging = "�� ��������� �� $o2 ���� ���.";
		itemtag = "����� ������ ������ ����� ������� '������� $n4'.";

		// ����� ��.

		stat_bonus = number(0, GET_REAL_INT(ch));

		dam = 70;

		break;

	case SKILL_MAKE_POTION:

		charwork = "�� ������� ��������� �������� � ������� ��� ��� �����, ����� ������ $o3.";
		roomwork = "$n ������ �������� � �������� ��� �� �����.";
		charfail = "�� �� �������� ��� ����� �������� � ��������� � �����.";
		roomfail =
			"����� ������� �����$g $n �������� � ��������� � �����,\r\n ������������� �� ������� ������� ����.";
		charsucc = "����� ������� ��� �� �����.";
		roomsucc = "$n ������$g $o3. �������� ������ ����� �� ��������, ��� � ����� ���.";
		chardam = "�� ���������� �������� � ������ �� ����, ������ �����������.";
		roomdam = "�������� � $o4 ����������� �� $n1, ������� $s.";
		tagging = "�� �� ���������� � $o2 ����� �� ����� ������.";
		itemtag = "�� $o1 �� �������� ����� '������� $n4'";

		// ����� �����
		stat_bonus = number(0, GET_REAL_WIS(ch));

		dam = 40;

		break;
	}

	act(charwork.c_str(), FALSE, ch, tobj, 0, TO_CHAR);
	act(roomwork.c_str(), FALSE, ch, tobj, 0, TO_ROOM);

	// ������� ����������� ��������� ��������� ����������
	// ���� ������� ���� = ������ ����� �� ���� 50%
	// ���� ������� ���� > ������ ����� �� 15 �� ���� 0%
	// ������� ���� * 2 - random(30) < 15 - ���� �� ��������� ���� ��������
	// �������� �� ��������� (...)
	// � �������.
	make_fail = false;

	// ��� ������� ������� ������������� ��������.
	// ���������� ��� ������� ���� ���������� ��������� � ����.
	int created_lev = 0;
	int used_non_ingrs = 0;

	for (i = 0; i < ingr_cnt; i++)
	{
		ingr_lev = get_ingr_lev(ingrs[i]);

		if (ingr_lev < 0)
			used_non_ingrs++;
		else
			created_lev += ingr_lev;
		// ���� ��������� �� ���������� ������� ����.
		if (number(0, 30) < (5 + ingr_lev - GET_LEVEL(ch) - 2 * GET_REMORT(ch)))
		{
			tmpstr = "�� ��������� " + string(ingrs[i]->PNames[3]) + ".\r\n";

			send_to_char(tmpstr.c_str(), ch);

			extract_obj(ingrs[i]);

			make_fail = true;
		}
	};
	created_lev = created_lev / MAX(1, (ingr_cnt - used_non_ingrs));
	int j;


	int craft_move = MIN_MAKE_MOVE + (created_lev / 2) - 1;

	// ������� ���� �� ������
	if (GET_MOVE(ch) < craft_move)
	{
		GET_MOVE(ch) = 0;
		// ��� �� ������� ��� ��������.
		tmpstr = "��� �� ������� ��� �������� " + string(tobj->PNames[3]) + ".\r\n";

		send_to_char(tmpstr.c_str(), ch);

		make_fail = true;

	}
	else
		GET_MOVE(ch) -= craft_move;

	// ������ ��� �������� ������.

	// �������� ������ �������� �� �������� ������ ��������� � ������.
	// ��� ������� �� ������� ������� �� 0 ������� �������.
	// ��� ������� ������� ��� 1 ������� ���������� � 2 ����.
	// ��� ������� ������� ��� � 2 ������� ���������� � 3 ����.
	if (skill == SKILL_MAKE_STAFF)
	{
		if (number(0, GET_LEVEL(ch) - created_lev) < GET_SPELL_MEM(ch, GET_OBJ_VAL(tobj, 3)))
			train_skill(ch, skill, skill_info[skill].max_percent, 0);
	}
	train_skill(ch, skill, skill_info[skill].max_percent, 0);

	// 4. ������� ������� ��������� �����.
	if (!make_fail)
		for (i = 0; i < ingr_cnt; i++)
		{
			//
			// ������ �������� = ���.�������� +
			// random(100) - skill
			// ���� ��� < 20 �� ���.��� + rand(���.���/3)
			// ���� ��� < 50 �� ���.���*rand(1,2) + rand(���.���/3)
			// ���� ��� > 50    ���.���*rand(2,5) + rand(���.���/3)
			if (get_ingr_lev(ingrs[i]) == -1)
				continue;	// ��������� �� ����. ����������.

			craft_weight = parts[i].min_weight + number(0, (parts[i].min_weight / 3) + 1);

			j = number(0, 100) - calculate_skill(ch, skill, skill_info[skill].max_percent, 0);

			if ((j >= 20) && (j < 50))
				craft_weight += parts[i].min_weight * number(1, 2);
			else if (j > 50)
				craft_weight += parts[i].min_weight * number(2, 5);

			// 5. ������ �������� ���� �� ������� ���������.
			// ���� �� ������� �� ������� ��������� � ������.
			if (craft_weight > GET_OBJ_WEIGHT(ingrs[i]))
			{
				tmpstr = "��� �� ������� " + string(ingrs[i]->PNames[1]) + ".\r\n";
				//  ������� ������ � �������.
				send_to_char(tmpstr.c_str(), ch);
				IS_CARRYING_W(ch) -= GET_OBJ_WEIGHT(ingrs[i]);
				GET_OBJ_WEIGHT(ingrs[i]) = 0;
				make_fail = true;
				// ��� make_fail = true ���� ���� ������������� ������ ��-�� �������� ����
				continue;
			}

			GET_OBJ_WEIGHT(ingrs[i]) -= craft_weight;
			IS_CARRYING_W(ch) -= craft_weight;

			send_to_char(ch, "�� ��������� %s(%d)\r\n", ingrs[i]->PNames[3], craft_weight);

			if (GET_OBJ_WEIGHT(ingrs[i]) == 0)
			{
				// ������ ������� ������� �� ��� ���������.
				tmpstr = "�� ��������� ������������ " + string(ingrs[i]->PNames[0]) + ".\r\n";
				// ���������� ... ������� � ����.
				// extract_obj(ingrs[i]);
			}
		}

	if (make_fail)
	{
		// ������� ���� ���� ��� ���.
		int crit_fail = number(0, 100);

		if (crit_fail > 2)
		{
			act(charfail.c_str(), FALSE, ch, tobj, 0, TO_CHAR);
			act(roomfail.c_str(), FALSE, ch, tobj, 0, TO_ROOM);
		}
		else
		{
			act(chardam.c_str(), FALSE, ch, tobj, 0, TO_CHAR);
			act(roomdam.c_str(), FALSE, ch, tobj, 0, TO_ROOM);
			dam = number(0, dam);
			// ������� �����.
			if (GET_LEVEL(ch) >= LVL_IMMORT && dam > 0)
			{
				send_to_char("������ �����������, �� �������� �����������...", ch);
				return (FALSE);
			}
			GET_HIT(ch) -= dam;
			update_pos(ch);
			char_dam_message(dam, ch, ch, TYPE_UNDEFINED, 0);
			if (GET_POS(ch) == POS_DEAD)
			{
				// ������ �������.
				if (!IS_NPC(ch))
				{
					sprintf(tmpbuf, "%s killed by a crafting at %s",
							GET_NAME(ch),
							IN_ROOM(ch) == NOWHERE ? "NOWHERE" : world[IN_ROOM(ch)]->name);
					mudlog(tmpbuf, BRF, LVL_BUILDER, SYSLOG, TRUE);
				}
				die(ch, NULL);
			}
		}
		for (i = 0; i < ingr_cnt; i++)
		{
			if (GET_OBJ_WEIGHT(ingrs[i]) <= 0)
			{
				extract_obj(ingrs[i]);
			}
		}

		return (FALSE);
	}
	// ������ ������� ������

	OBJ_DATA *obj = read_object(obj_proto, VIRTUAL);

	act(charsucc.c_str(), FALSE, ch, obj, 0, TO_CHAR);
	act(roomsucc.c_str(), FALSE, ch, obj, 0, TO_ROOM);
	// 6. ������� ������� ������ �������� � ������
	//  ������� ��� ������� ������ ���������

	// ��� �������� �-�:  �-��+(skill - random(100))/20;
	// ��� ������ ???: random(200) - skill > 0 �� ���� ������������.

	// �.�. ������� �� ����� ����������� ����� �������.

	// ������������ ��� �������� � ��� ������.
	// ��� ��� ��������� ���� � ������� ����������.

	i = GET_OBJ_WEIGHT(obj);
	int sign = -1;

	if ((GET_OBJ_TYPE(obj) == ITEM_WEAPON) || (GET_OBJ_TYPE(obj) == ITEM_INGRADIENT))
		sign = 1;

	GET_OBJ_WEIGHT(obj) = stat_modify(ch, GET_OBJ_WEIGHT(obj), 20 * sign);

//  sprintf(tmpbuf,"���: %d %d\r\n", i, GET_OBJ_WEIGHT(obj));
//  send_to_char(tmpbuf,ch);

	i = obj->get_timer();
	obj->set_timer(stat_modify(ch, obj->get_timer(), 1));

	// ������������ ���������� ��� ��������� �-��.

	// ������������ ����������� ���������� �������� (!20!)
	// ��� �������� 20 � ������ 100 � ���� ������� ����� +5 � ���������

	// ����� ��������� ������ �� ������� �����.
	// ������� ������� ���� ������ .
	switch (GET_OBJ_TYPE(obj))
	{
	case ITEM_LIGHT:
		// ������� ������������ ��������.
		if (GET_OBJ_VAL(obj, 2) != -1)
			GET_OBJ_VAL(obj, 2) = stat_modify(ch, GET_OBJ_VAL(obj, 2), 1);

		break;
	case ITEM_WAND:
	case ITEM_STAFF:
		// ��������� �����

		// ������� ������� �����
		GET_OBJ_VAL(obj, 0) = GET_LEVEL(ch);

		// ������� ������ � ������� ... ������ ����� ������� ������
		// ����� ���������� ������ ...
		/*      if (!IS_IMMORTAL(ch))
		      {
		        GET_OBJ_VAL(obj,1) = GET_SPELL_MEM(ch,GET_OBJ_VAL(obj,3));

		      // ������� �������� ��� ����� ���.
		        GET_OBJ_VAL(obj,2) = GET_SPELL_MEM(ch,GET_OBJ_VAL(obj,3));
		      } else
		      {
		   GET_OBJ_VAL(obj,1) = 10;
		   GET_OBJ_VAL(obj,2) = 10;
		      }
		*/
//      affect_join(ch, &af, TRUE, FALSE, FALSE, FALSE);

		break;
	case ITEM_WEAPON:
		// ������� ����� xdy
		// ������������ XdY
		if (GET_OBJ_VAL(obj, 1) > GET_OBJ_VAL(obj, 2))
			GET_OBJ_VAL(obj, 1) = stat_modify(ch, GET_OBJ_VAL(obj, 1), 1);
		else
			GET_OBJ_VAL(obj, 2) = stat_modify(ch, GET_OBJ_VAL(obj, 2) - 1, 1) + 1;

		break;
	case ITEM_ARMOR:
	case ITEM_ARMOR_LIGHT:
	case ITEM_ARMOR_MEDIAN:
	case ITEM_ARMOR_HEAVY:
		// ������� + ��
		GET_OBJ_VAL(obj, 0) = stat_modify(ch, GET_OBJ_VAL(obj, 0), 1);

		// ������� ����������.
		GET_OBJ_VAL(obj, 1) = stat_modify(ch, GET_OBJ_VAL(obj, 1), 1);

		break;
	case ITEM_POTION:
		// ������� ������� �������� �������
		GET_OBJ_VAL(obj, 0) = stat_modify(ch, GET_OBJ_VAL(obj, 0), 1);

		break;
	case ITEM_CONTAINER:
		// ������� ����� ����������.
		GET_OBJ_VAL(obj, 0) = stat_modify(ch, GET_OBJ_VAL(obj, 0), 1);
		break;

	case ITEM_DRINKCON:
		// ������� ����� ����������.
		GET_OBJ_VAL(obj, 0) = stat_modify(ch, GET_OBJ_VAL(obj, 0), 1);
		break;

	case ITEM_INGRADIENT:
		// ��� ������ ������ �� ������� ... ��� ������. :)
		break;
	}
//  sprintf(tmpbuf,"VAL0 = %d ; VAL1 = %d ; VAL2 = %d ; VAL3 = %d \r\n",
//     GET_OBJ_VAL(obj,0),GET_OBJ_VAL(obj,1),GET_OBJ_VAL(obj,2),GET_OBJ_VAL(obj,3));

//  send_to_char(tmpbuf,ch);
	// 7. ������� ���. ������ ��������.
	// �-�� ��������� +
	// ���� (random(100) - ���� ����� ) < 1 �� ������������ ���� ��������.
	// ���� �� 1 �� 25 �� ������������ 1/2
	// ���� �� 25 �� 50 �� ������������ 1/3
	// ������ ������������ 0

	// ��������� ��� ������� ...+����� +����� � �.�.

	for (j = 0; j < ingr_cnt; j++)
	{
		ingr_pow = get_ingr_pow(ingrs[j]);

		if (ingr_pow < 0)
			ingr_pow = 20;

		// ��������� ������� ... c ������ �� ��������.
		add_flags(ch, &obj->obj_flags.affects, &ingrs[j]->obj_flags.affects, ingr_pow);

		// �������� ������� ... � ������ �� ��������.
		add_flags(ch, &obj->obj_flags.extra_flags, &ingrs[j]->obj_flags.extra_flags, ingr_pow);

		add_affects(ch, obj->affected, ingrs[j]->affected, ingr_pow);
	};

	/*  send_to_char("������������� ������� : ", ch);
	  sprintbits(obj->obj_flags.affects, weapon_affects, tmpbuf, ",");
	  strcat(tmpbuf, "\r\n");
	  send_to_char(tmpbuf, ch);

	  send_to_char("�������������� �����  : ", ch);
	  sprintbits(obj->obj_flags.extra_flags, extra_bits, tmpbuf, ",");
	  strcat(tmpbuf, "\r\n");
	  send_to_char(tmpbuf, ch);

	  send_to_char("�������:", ch);
	  for (i = 0; i < MAX_OBJ_AFFECT; i++)
	      if (obj->affected[i].modifier)
	         {sprinttype(obj->affected[i].location, apply_types, tmpbuf2);
	          sprintf(tmpbuf, "%s %+d to %s",";",
	         obj->affected[i].modifier, tmpbuf2);
	          send_to_char(tmpbuf, ch);
	         }
	  send_to_char("\r\n",ch);
	*/
	// ����� ����������� �����.
	for (i = 0; i < ingr_cnt; i++)
	{
		if ((GET_OBJ_TYPE(ingrs[i]) != ITEM_INGRADIENT) && (GET_OBJ_TYPE(ingrs[i]) != ITEM_MING))
		{
			// ���� ����������� ��� 0 ��  ������� ������� ���� �� �������.
			if (parts[i].min_weight == 0)
				extract_obj(ingrs[i]);
			continue;
		}

		if (GET_OBJ_WEIGHT(ingrs[i]) <= 0)
		{
			extract_obj(ingrs[i]);
		}
	}

	// 8. ��������� ���. �������.
	// ������� �� ������� (31 - ��. ������� ��������) * 5 -
	// ���� ����� � ���� �� 30 ���� �� ������ 5 ����
	// �.�. ��. ������� ������ ����� ����������
	// ����� ������ � ���� �� ������ �� ���� ����� ���������
	// ������ �� ����� (� ����� ��� �� ��� �������).


	if ((obj_index[GET_OBJ_RNUM(obj)].number + obj_index[GET_OBJ_RNUM(obj)].stored) >= (31 - created_lev) * 5)
	{
		tmpstr = "$o �������� ����� �������� � �����.\r\n";
		act(tmpstr.c_str(), FALSE, ch, obj, 0, TO_CHAR);
		tmpstr = "$o � ����� $n1 �������� ����� �������� � �����.";
		act(tmpstr.c_str(), FALSE, ch, obj, 0, TO_ROOM);
		extract_obj(obj);
		return (FALSE);
	};

	// ������ ����� ���� ��� ������.
	if (((GET_OBJ_TYPE(obj) != ITEM_INGRADIENT) &&
			(GET_OBJ_TYPE(obj) != ITEM_MING)) &&
			(number(1, 100) - calculate_skill(ch, skill, skill_info[skill].max_percent, 0) < 0))
	{
		act(tagging.c_str(), FALSE, ch, obj, 0, TO_CHAR);
		// ���������� � ������ �������� �������.
		EXTRA_DESCR_DATA *new_desc;
		EXTRA_DESCR_DATA *desc = obj->ex_description;

		char *tagchar = format_act(itemtag.c_str(), ch, obj, 0);

		if (desc == NULL)
		{
			CREATE(desc, EXTRA_DESCR_DATA, 1);
			obj->ex_description = desc;
			desc->keyword = str_dup(obj->name);
			desc->description = str_dup(tagchar);
			desc->next = NULL;	// �� ������ ������ :)
		}
		else
		{
			CREATE(new_desc, EXTRA_DESCR_DATA, 1);
			obj->ex_description = new_desc;
			new_desc->keyword = str_dup(desc->keyword);
			new_desc->description = str_dup(desc->description);
			new_desc->next = NULL;	// �� ������ ������ :)

			new_desc->description = str_add(new_desc->description, tagchar);
			// �� ��� ��� ���� �� ������� ������ ��������� ���� ��� �� � ���������

			obj->ex_description = new_desc;
		}

		free(tagchar);
	};
	// ����� ������������� � ����.
	GET_OBJ_MAKER(obj) = GET_UNIQUE(ch);

	// 9. ��������� ������� 2

	if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
	{
		send_to_char("�� �� ������� ������ ������� ���������.\r\n", ch);
		obj_to_room(obj, IN_ROOM(ch));
	}
	else if (IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj) > CAN_CARRY_W(ch))
	{
		send_to_char("�� �� ������� ������ ����� ���.\r\n", ch);
		obj_to_room(obj, IN_ROOM(ch));
	}
	else
		obj_to_char(obj, ch);

	return (TRUE);
}

// �������� ������ �� ������.
int MakeRecept::load_from_str(string & rstr)
{
	// ��������� ������.
	char tmpbuf[MAX_INPUT_LENGTH];

	// ��������� ������ �� ���������� .
	if (rstr.substr(0, 1) == string("*"))
	{
		rstr = rstr.substr(1);
		locked = true;
	}
	else
		locked = false;

	skill = atoi(rstr.substr(0, rstr.find(" ")).c_str());
	rstr = rstr.substr(rstr.find(" ") + 1);

	obj_proto = atoi((rstr.substr(0, rstr.find(" "))).c_str());
	rstr = rstr.substr(rstr.find(" ") + 1);

	if (real_object(obj_proto) < 0)
	{
		// ��������� �������������� �������� �������.
		sprintf(tmpbuf, "MakeRecept::Unfound object proto %d", obj_proto);
		mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);

		// ��������� ������� ��� ������.
		locked = true;
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{
		// ������� ����� ��������� ����������
		parts[i].proto = atoi((rstr.substr(0, rstr.find(" "))).c_str());
		rstr = rstr.substr(rstr.find(" ") + 1);

		// ��������� �� ����� �����������.
		if (parts[i].proto == 0)
			break;

		if (real_object(parts[i].proto) < 0)
		{
			// ��������� �������������� �������� ����������.
			sprintf(tmpbuf, "MakeRecept::Unfound item part %d for %d", obj_proto, parts[i].proto);
			mudlog(tmpbuf, LGH, LVL_IMMORT, SYSLOG, TRUE);

			// ��������� ������� ��� ������.
			locked = true;
		}
		parts[i].min_weight = atoi(rstr.substr(0, rstr.find(" ")).c_str());
		rstr = rstr.substr(rstr.find(" ") + 1);

		parts[i].min_power = atoi(rstr.substr(0, rstr.find(" ")).c_str());
		rstr = rstr.substr(rstr.find(" ") + 1);

	}
	return (TRUE);
}

// ��������� ������ � ������.
int MakeRecept::save_to_str(string & rstr)
{
	char tmpstr[MAX_INPUT_LENGTH];

	if (obj_proto == 0)
		return (FALSE);

	if (locked)
		rstr = "*";
	else
		rstr = "";

	sprintf(tmpstr, "%d %d", skill, obj_proto);

	rstr += string(tmpstr);

	for (int i = 0; i < MAX_PARTS; i++)
	{
		sprintf(tmpstr, " %d %d %d", parts[i].proto, parts[i].min_weight, parts[i].min_power);
		rstr += string(tmpstr);
	}

	return (TRUE);
}


MakeRecept & MakeRecept::operator=(MakeRecept & x)
{
	obj_proto = x.obj_proto;
	skill = x.skill;
	for (int i = 0; i < MAX_PARTS; i++)
	{
		parts[i].proto = x.parts[i].proto;
		parts[i].min_weight = x.parts[i].min_weight;
		parts[i].min_power = x.parts[i].min_power;
	}
	return *this;
}

// ����������� ������� ��������.
int MakeRecept::stat_modify(CHAR_DATA * ch, int value, float devider)
{
	// ��� �������� �-�:  �-��+(skill - random(100))/20;
	// ��� ������ ???: random(200) - skill > 0 �� ���� ������������.
	int res = value;
	float delta = 0;
	int skill_prc = 0;
	if (devider == 0)
		return res;

	skill_prc = calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
	delta = (int)((float)(skill_prc - number(0, skill_info[skill].max_percent)));

	if (delta > 0)
		delta = (value / 2) * delta / skill_info[skill].max_percent / devider;
	else
		delta = (value / 4) * delta / skill_info[skill].max_percent / devider;

	res += (int) delta;

	// ���� �������� �������� �� ���������� 1;
	if (res < 0)
		return 1;

	return res;
}


int MakeRecept::add_flags(CHAR_DATA * ch, FLAG_DATA * base_flag, FLAG_DATA * add_flag, int delta)
{
// ��� ��������� ������ :(
	int tmpprob;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			tmpprob = number(0, 200) - calculate_skill(ch, skill, skill_info[skill].max_percent, 0);
			if ((add_flag->flags[i] & (1 << j)) && (tmpprob < 0))
			{
//        cout << "Prob : " << tmpprob << endl;
				base_flag->flags[i] |= (1 << j);
//        cout << "Base now : " << base_flag->flags[i] << endl;
			}
		}
	}
	return (TRUE);

}


int MakeRecept::add_affects(CHAR_DATA * ch, boost::array<obj_affected_type, MAX_OBJ_AFFECT>& base, const boost::array<obj_affected_type, MAX_OBJ_AFFECT>& add, int delta)
{
	bool found = false;
	int i, j;

	for (i = 0; i < MAX_OBJ_AFFECT; i++)
	{
		found = false;
		if (add[i].location == APPLY_NONE)
			continue;

		for (j = 0; j < MAX_OBJ_AFFECT; j++)
		{
			if (base[j].location == APPLY_NONE)
				continue;

			if (add[i].location == base[j].location)
			{
				// ������� �������.
				found = true;

				if (number(0, 100) > delta)
					break;

				base[j].modifier += stat_modify(ch, add[i].modifier, 1);

				break;
			}
		}

		if (!found)
		{
			// ���� ������ ��������� ������ � ������� ���� �����.
			for (int j = 0; j < MAX_OBJ_AFFECT; j++)
			{
				if (base[j].location == APPLY_NONE)
				{
					if (number(0, 100) > delta)
						break;

					base[j].location = add[i].location;
					base[j].modifier += stat_modify(ch, add[i].modifier, 1);

//    cout << "add affect " << int(base[j].location) <<" - " << int(base[j].modifier) << endl;

					break;
				}
			}
		}
	}

	return (TRUE);
}
