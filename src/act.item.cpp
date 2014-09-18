/* ************************************************************************
*   File: act.item.cpp                                  Part of Bylins    *
*  Usage: object handling routines -- get/drop and container handling     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "skills.h"
#include "constants.h"
#include "dg_scripts.h"
#include "house.h"
#include "magic.h"
#include "fight.h"
#include "features.hpp"
#include "depot.hpp"
#include "char.hpp"
#include "liquid.hpp"
#include "poison.hpp"
#include "pk.h"
#include "room.hpp"
#include "named_stuff.hpp"

/* extern variables */
extern vector < OBJ_DATA * >obj_proto;
extern CHAR_DATA *mob_proto;
extern struct house_control_rec house_control[];

/* from act.informative.cpp */
char *find_exdesc(char *word, EXTRA_DESCR_DATA * list);

/* local functions */
int can_take_obj(CHAR_DATA * ch, OBJ_DATA * obj);
void get_check_money(CHAR_DATA * ch, OBJ_DATA * obj);
int perform_get_from_room(CHAR_DATA * ch, OBJ_DATA * obj);
void get_from_room(CHAR_DATA * ch, char *arg, int amount);
void perform_give_gold(CHAR_DATA * ch, CHAR_DATA * vict, int amount);
void perform_give(CHAR_DATA * ch, CHAR_DATA * vict, OBJ_DATA * obj);
int perform_drop(CHAR_DATA * ch, OBJ_DATA * obj, byte mode, const int sname, room_rnum RDR);
void perform_drop_gold(CHAR_DATA * ch, int amount, byte mode, room_rnum RDR);
CHAR_DATA *give_find_vict(CHAR_DATA * ch, char *arg);
void weight_change_object(OBJ_DATA * obj, int weight);
int perform_put(CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * cont);
void get_from_container(CHAR_DATA * ch, OBJ_DATA * cont, char *arg, int mode, int amount, bool autoloot);
void perform_wear(CHAR_DATA * ch, OBJ_DATA * obj, int where);
int find_eq_pos(CHAR_DATA * ch, OBJ_DATA * obj, char *arg);
bool perform_get_from_container(CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * cont, int mode);
void perform_remove(CHAR_DATA * ch, int pos);
int invalid_anti_class(CHAR_DATA * ch, const OBJ_DATA * obj);
void feed_charmice(CHAR_DATA * ch, char *arg);
int get_player_charms(CHAR_DATA * ch, int spellnum);
OBJ_DATA *create_skin(CHAR_DATA * mob);
int invalid_unique(CHAR_DATA * ch, const OBJ_DATA * obj);

ACMD(do_split);
ACMD(do_remove);
ACMD(do_put);
ACMD(do_get);
ACMD(do_drop);
ACMD(do_give);
ACMD(do_drink);
ACMD(do_eat);
ACMD(do_drunkoff);
ACMD(do_pour);
ACMD(do_wear);
ACMD(do_wield);
ACMD(do_grab);
ACMD(do_upgrade);

// ����� ������� ������������� �������� � ����-������,
// ����� ��� ��� ��� ��� ����� ���� �� ������ �������, ��� ����
// 0 - ��� ��, 1 - ������ �������� � ������ �� ������������ (��� ������), 2 - ������ �������� � ���� ������
int perform_put(CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * cont)
{
	if (!bloody::handle_transfer(ch, NULL, obj, cont))
		return 2;
	if (!drop_otrigger(obj, ch))
	{
		return 2;
	}
	// ���� ������ � �������� ������
	if (Clan::is_clan_chest(cont))
	{
		if (!Clan::PutChest(ch, obj, cont))
		{
			return 1;
		}
		return 0;
	}
	// ����-��������� ��� �����
	if (ClanSystem::is_ingr_chest(cont))
	{
		if (!Clan::put_ingr_chest(ch, obj, cont))
		{
			return 1;
		}
		return 0;
	}
	// ������������ ������
	if (Depot::is_depot(cont))
	{
		if (!Depot::put_depot(ch, obj))
		{
			return 1;
		}
		return 0;
	}

	if (GET_OBJ_WEIGHT(cont) + GET_OBJ_WEIGHT(obj) > GET_OBJ_VAL(cont, 0))
		act("$O : $o �� ���������� ����.", FALSE, ch, obj, cont, TO_CHAR);
	else if (GET_OBJ_TYPE(obj) == ITEM_CONTAINER)
		act("���������� �������� ��������� � ���������.", FALSE, ch, 0, 0, TO_CHAR);
	else if (IS_OBJ_STAT(obj, ITEM_NODROP))
		act("��������� ���� �������� �������� $o3 � $O3.", FALSE, ch, obj, cont, TO_CHAR);
	else if (OBJ_FLAGGED(obj, ITEM_ZONEDECAY)
			 || GET_OBJ_TYPE(obj) == ITEM_KEY)
		act("��������� ���� �������� �������� $o3 � $O3.", FALSE, ch, obj, cont, TO_CHAR);
	else
	{
		obj_from_char(obj);
		// ����� ��� �� 1 ���� ��� �� ����, ���� ��� ������������ �� ����, � �� � ��������� ������
		if (GET_OBJ_TYPE(obj) == ITEM_MONEY)
		{
			OBJ_DATA *temp, *obj_next;
			for (temp = cont->contains; temp; temp = obj_next)
			{
				obj_next = temp->next_content;
				if (GET_OBJ_TYPE(temp) == ITEM_MONEY)
				{
					// ��� ����� ������ � ���� ���������, �� ��� �������� ��� ��� ������ �� ���-��
					int money = GET_OBJ_VAL(temp, 0);
					money += GET_OBJ_VAL(obj, 0);
					obj_from_obj(temp);
					extract_obj(temp);
					obj_from_obj(obj);
					extract_obj(obj);
					obj = create_money(money);
					if (!obj)
						return 0;
					break;
				}
			}
		}
		obj_to_obj(obj, cont);

		act("$n �������$g $o3 � $O3.", TRUE, ch, obj, cont, TO_ROOM);

		/* Yes, I realize this is strange until we have auto-equip on rent. -gg */
		if (IS_OBJ_STAT(obj, ITEM_NODROP) && !IS_OBJ_STAT(cont, ITEM_NODROP))
		{
			SET_BIT(GET_OBJ_EXTRA(cont, ITEM_NODROP), ITEM_NODROP);
			act("�� ������������� ���-�� ��������, ����� �������� $o3 � $O3.",
				FALSE, ch, obj, cont, TO_CHAR);
		}
		else
			act("�� �������� $o3 � $O3.", FALSE, ch, obj, cont, TO_CHAR);
		return 0;
	}
	return 2;
}

const int effects[][2] = { {APPLY_MOVEREG, 100},
	{APPLY_HITROLL, 2},
	{APPLY_DAMROLL, 2},
	{APPLY_MANAREG, 30},
	{APPLY_MOVE, 100},
	{APPLY_C1, 4},
	{APPLY_C2, 4},
	{APPLY_CHAR_WEIGHT, 10},
	{APPLY_HITREG, 76},
	{APPLY_SAVING_CRITICAL, -20},
	{APPLY_SAVING_STABILITY, -20},
	{APPLY_RESIST_FIRE, 20},
	{APPLY_RESIST_AIR, 20},
	{APPLY_C4, 3},
	{APPLY_SAVING_WILL, -20},
	{APPLY_HIT, 80},
	{APPLY_C3, 3},
	{APPLY_HITROLL, 4},
	{APPLY_DAMROLL, 4},
	{APPLY_SIZE, 15},
	{APPLY_STR, 3},
	{APPLY_DEX, 3},
	{APPLY_INT, 3},
	{APPLY_WIS, 3},
	{APPLY_CON, 3},
	{APPLY_CHA, 3},
	{APPLY_SAVING_REFLEX, -20},
	{APPLY_INITIATIVE, 12},
	{APPLY_ABSORBE, 25},
	{APPLY_RESIST_WATER, 20},
	{APPLY_RESIST_EARTH, 20},
	{APPLY_RESIST_VITALITY, 15},
	{APPLY_RESIST_IMMUNITY, 25},
	{APPLY_C5, 2},
	{APPLY_MORALE, 12},
	{APPLY_C6, 2},
	{APPLY_CAST_SUCCESS, 12},
	{APPLY_RESIST_MIND, 15},
	{APPLY_C8, 1},
	{APPLY_C9, 1}
};

OBJ_DATA *create_skin(CHAR_DATA *mob, CHAR_DATA *ch)
{
	OBJ_DATA *skin;
	int definitor, vnum, eff, limit, i, n, k = 0, num, effect, max_eff;
	bool concidence;
	const int vnum_skin_prototype = 1660;


	vnum = vnum_skin_prototype + MIN((int)(GET_LEVEL(mob) / 5), 9);
	skin = read_object(vnum, VIRTUAL);
	if (skin == NULL)
	{
		mudlog("������� ����� ����� ��������� ��� ����������� � act.item.cpp::create_skin!",
			   NRM, LVL_GRGOD, ERRLOG, TRUE);
		return NULL;
	}

	definitor = (int)((mob->get_str() + mob->get_dex() + mob->get_con() + mob->get_wis() + mob->get_int()) / 5);
	GET_OBJ_PARENT(skin) = GET_MOB_VNUM(mob);
	trans_obj_name(skin, mob);
	if (0 <= definitor && definitor <= 15)
	{
		limit = 5;
		eff = number(0, 1);
		max_eff = 9;
		//aff = 0;
		GET_OBJ_VAL(skin, 0) = number(1, 3);
		GET_OBJ_VAL(skin, 1) = number(0, 2);
	}
	else if (16 <= definitor && definitor <= 25)
	{
		limit = 7;
		eff = number(0, 2);
		max_eff = 12;
		//aff = number(0, 1);
		GET_OBJ_VAL(skin, 0) = number(2, 5);
		GET_OBJ_VAL(skin, 1) = number(2, 5);
	}
	else if (26 <= definitor && definitor <= 35)
	{
		limit = 8;
		eff = number(0, 3);
		max_eff = 25;
		//aff = number(0, 1);
		GET_OBJ_VAL(skin, 0) = number(4, 7);
		GET_OBJ_VAL(skin, 1) = number(4, 6);
	}
	else if (36 <= definitor && definitor <= 45)
	{
		limit = 10;
		eff = number(0, 4);
		max_eff = 30;
		//aff = number(0, 2);
		GET_OBJ_VAL(skin, 0) = number(6, 9);
		GET_OBJ_VAL(skin, 1) = number(5, 7);
	}
	else
	{
		limit = 10;
		eff = number(0, 6);
		max_eff = 39;
		//aff = number(0, 3);
		GET_OBJ_VAL(skin, 0) = number(6, 10);
		GET_OBJ_VAL(skin, 1) = number(5, 8);
	}

	for (i = 1; i <= eff; i++)
	{
		if (number(0, 1000) <= 200)
			continue;
		concidence = TRUE;
		while (concidence)
		{
			num = number(0, max_eff);
			concidence = FALSE;
			for (n = 0; n <= k && i > 1; n++)
			{
				if (effects[num][0] == (skin)->affected[n].location)
					concidence = TRUE;
			}
		}
		(skin)->affected[k].location = effects[num][0];
		effect = number(1, (int)(effects[num][1] * limit / 10));
		if (number(0, 1000) <= 150)
			effect *= -1;
		(skin)->affected[k].modifier = effect;
		k++;
	}
	GET_OBJ_COST(skin) = GET_LEVEL(mob) * number(2, MAX(3, 3 * k));
	GET_OBJ_VAL(skin, 2) = (int)(1 + (GET_WEIGHT(mob) + GET_SIZE(mob)) / 20);

	return skin;
}

/* The following put modes are supported by the code below:

   1) put <object> <container>
   2) put all.<object> <container>
   3) put all <container>

   <container> must be in inventory or on ground.
   all objects to be put into container must be in inventory.
*/

ACMD(do_put)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	char arg4[MAX_INPUT_LENGTH];
	OBJ_DATA *obj, *next_obj, *cont;
	CHAR_DATA *tmp_char;
	int obj_dotmode, cont_dotmode, found = 0, howmany = 1, money_mode = FALSE;
	char *theobj, *thecont, *theplace;
	int where_bits = FIND_OBJ_INV | FIND_OBJ_EQUIP | FIND_OBJ_ROOM;


	argument = two_arguments(argument, arg1, arg2);
	argument = two_arguments(argument, arg3, arg4);

	if (is_number(arg1))
	{
		howmany = atoi(arg1);
		theobj = arg2;
		thecont = arg3;
		theplace = arg4;
	}
	else
	{
		theobj = arg1;
		thecont = arg2;
		theplace = arg3;
	}

	if (isname(theplace, "����� ������� room ground"))
		where_bits = FIND_OBJ_ROOM;
	else if (isname(theplace, "��������� inventory"))
		where_bits = FIND_OBJ_INV;
	else if (isname(theplace, "���������� equipment"))
		where_bits = FIND_OBJ_EQUIP;


	if (theobj && (!strn_cmp("coin", theobj, 4) || !strn_cmp("���", theobj, 3)))
	{
		money_mode = TRUE;
		if (howmany <= 0)
		{
			send_to_char("������� ������� ����� ���������� �����.\r\n", ch);
			return;
		}
		if (ch->get_gold() < howmany)
		{
			send_to_char("��� � ��� ����� �����.\r\n", ch);
			return;
		}
		obj_dotmode = FIND_INDIV;
	}
	else
		obj_dotmode = find_all_dots(theobj);

	cont_dotmode = find_all_dots(thecont);

	if (!*theobj)
		send_to_char("�������� ��� � ���� ?\r\n", ch);
	else if (cont_dotmode != FIND_INDIV)
		send_to_char("�� ������ �������� ���� ������ � ���� ���������.\r\n", ch);
	else if (!*thecont)
	{
		sprintf(buf, "���� �� ������ �������� '%s' ?\r\n", theobj);
		send_to_char(buf, ch);
	}
	else
	{
		generic_find(thecont, where_bits, ch, &tmp_char, &cont);
		if (!cont)
		{
			sprintf(buf, "�� �� ������ ����� '%s'.\r\n", thecont);
			send_to_char(buf, ch);
		}
		else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER)
			act("� $o3 ������ ������ ��������.", FALSE, ch, cont, 0, TO_CHAR);
		else if (OBJVAL_FLAGGED(cont, CONT_CLOSED))
			act("$o3 ������$A !", FALSE, ch, cont, 0, TO_CHAR);
		else
		{
			if (obj_dotmode == FIND_INDIV)  	/* put <obj> <container> */
			{
				if (money_mode)
				{
					if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_NOITEM))
					{
						act("��������� ���� �������� ��� ������� ���!!", FALSE,
							ch, 0, 0, TO_CHAR);
						return;
					}
					obj = create_money(howmany);
					if (!obj)
						return;
					obj_to_char(obj, ch);
					ch->remove_gold(howmany);
					// ���� �������� �� ������� - ���������� ��� ����
					if (perform_put(ch, obj, cont))
					{
						obj_from_char(obj);
						extract_obj(obj);
						ch->add_gold(howmany);
						return;
					}
				}
				else if (!(obj = get_obj_in_list_vis(ch, theobj, ch->carrying)))
				{
					sprintf(buf, "� ��� ��� '%s'.\r\n", theobj);
					send_to_char(buf, ch);
				}
				else if (obj == cont)
					send_to_char("��� ����� ������ ��������� ���� ���� � ����.\r\n", ch);
				else
				{
					OBJ_DATA *next_obj;
					while (obj && howmany--)
					{
						next_obj = obj->next_content;
						if (perform_put(ch, obj, cont) == 1)
							return;
						obj = get_obj_in_list_vis(ch, theobj, next_obj);
					}
				}
			}
			else
			{
				for (obj = ch->carrying; obj; obj = next_obj)
				{
					next_obj = obj->next_content;
					if (obj != cont && CAN_SEE_OBJ(ch, obj) &&
							(obj_dotmode == FIND_ALL || isname(theobj, obj->name)))
					{
						found = 1;
						if (perform_put(ch, obj, cont) == 1)
							return;
					}
				}
				if (!found)
				{
					if (obj_dotmode == FIND_ALL)
						send_to_char
						("����� �������� ���-�� �������� ����� ������ ���-�� ��������.\r\n",
						 ch);
					else
					{
						sprintf(buf, "�� �� ������ ������ �������� �� '%s'.\r\n", theobj);
						send_to_char(buf, ch);
					}
				}
			}
		}
	}
}



int can_take_obj(CHAR_DATA * ch, OBJ_DATA * obj)
{
	if (!IS_NPC(ch) && CLAN(ch))
		sprintf(buf, "clan%d!", CLAN(ch)->GetRent());

	if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
	{
		act("$p: �� �� ������ ����� ������� �����.", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}
	else if ((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch))
	{
		act("$p: �� �� � ��������� ����� ��� � $S.", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}
	else if (!(CAN_WEAR(obj, ITEM_WEAR_TAKE)))
	{
		act("$p: �� �� ������ ����� $S.", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}
	else if (invalid_anti_class(ch, obj))
	{
		if(!OBJ_FLAGGED(obj, ITEM_NAMED) || !NamedStuff::wear_msg(ch, obj))
			act("$p: ��� ���� �� ������������� ��� ��� !", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}
	else if (invalid_unique(ch, obj) || (strstr(obj->name, "clan") && (IS_NPC(ch) || !CLAN(ch) || !strstr(obj->name, buf))))
	{
		act("��� ������� ��� ������� ����� $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n �������$u ����� $o3 - � ����� �� ������$g.", FALSE, ch, obj, 0, TO_ROOM);
		return (0);
	}
	return (1);
}


void get_check_money(CHAR_DATA * ch, OBJ_DATA * obj)
{
	char local_buf[256];
	int value = GET_OBJ_VAL(obj, 0);

	if (GET_OBJ_TYPE(obj) != ITEM_MONEY || value <= 0)
		return;

	obj_from_char(obj);
	extract_obj(obj);

	ch->add_gold(value);

	sprintf(buf, "��� ��������� %d %s.\r\n", value, desc_count(value, WHAT_MONEYu));
	send_to_char(buf, ch);
	if (IS_AFFECTED(ch, AFF_GROUP) && PRF_FLAGGED(ch, PRF_AUTOSPLIT))
	{
		sprintf(local_buf, "%ld", (long) value);
		do_split(ch, local_buf, 0, 0);
	}
}


// return 0 - ����� ������� ������������� ����� �� ����-�������,
// ����� ��� �� ��� ��� ����� ���� �� ������ �������, ��� ����
bool perform_get_from_container(CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * cont, int mode)
{
	if (!bloody::handle_transfer(NULL, ch, obj))
		return false;
	if ((mode == FIND_OBJ_INV || mode == FIND_OBJ_ROOM || mode == FIND_OBJ_EQUIP) && can_take_obj(ch, obj) && get_otrigger(obj, ch))
	{
		// ���� ����� �� ����-�������
		if (Clan::is_clan_chest(cont))
		{
			if (!Clan::TakeChest(ch, obj, cont))
			{
				return 0;
			}
			return 1;
		}
		// ����-��������� ������
		if (ClanSystem::is_ingr_chest(cont))
		{
			if (!Clan::take_ingr_chest(ch, obj, cont))
			{
				return 0;
			}
			return 1;
		}
		obj_from_obj(obj);
		obj_to_char(obj, ch);
		if (obj->carried_by == ch)
		{
			if (bloody::is_bloody(obj))
			{
				act("�� ����� $o3 �� $O1, �������� ���� ���� ������!", FALSE, ch, obj, cont, TO_CHAR);
				act("$n ����$g $o3 �� $O1, �������� ���� ������.", TRUE, ch, obj, cont, TO_ROOM);
			} else
			{
				act("�� ����� $o3 �� $O1.", FALSE, ch, obj, cont, TO_CHAR);
				act("$n ����$g $o3 �� $O1.", TRUE, ch, obj, cont, TO_ROOM);
			}
			get_check_money(ch, obj);
		}
	}
	return 1;
}

/**
* \param autoloot - true ������ ��� ������ ������ �� ����� � ������ �����������
*/
void get_from_container(CHAR_DATA * ch, OBJ_DATA * cont, char *arg, int mode, int howmany, bool autoloot)
{
	if (Depot::is_depot(cont))
	{
		Depot::take_depot(ch, arg, howmany);
		return;
	}

	OBJ_DATA *obj, *next_obj;
	int obj_dotmode, found = 0;

	obj_dotmode = find_all_dots(arg);
	if (OBJVAL_FLAGGED(cont, CONT_CLOSED))
		act("$o ������$A.", FALSE, ch, cont, 0, TO_CHAR);
	else if (obj_dotmode == FIND_INDIV)
	{
		if (!(obj = get_obj_in_list_vis(ch, arg, cont->contains)))
		{
			sprintf(buf, "�� �� ������ '%s' � $o5.", arg);
			act(buf, FALSE, ch, cont, 0, TO_CHAR);
		}
		else
		{
			OBJ_DATA *obj_next;
			while (obj && howmany--)
			{
				obj_next = obj->next_content;
				if (!perform_get_from_container(ch, obj, cont, mode))
					return;
				obj = get_obj_in_list_vis(ch, arg, obj_next);
			}
		}
	}
	else
	{
		if (obj_dotmode == FIND_ALLDOT && !*arg)
		{
			send_to_char("����� ��� \"���\" ?\r\n", ch);
			return;
		}
		for (obj = cont->contains; obj; obj = next_obj)
		{
			next_obj = obj->next_content;
			if (CAN_SEE_OBJ(ch, obj) && (obj_dotmode == FIND_ALL || isname(arg, obj->name)))
			{
				if (autoloot && (GET_OBJ_TYPE(obj) == ITEM_INGRADIENT || GET_OBJ_TYPE(obj) == ITEM_MING) && PRF_FLAGGED(ch, PRF_NOINGR_LOOT))
				{
					continue;
				}
				found = 1;
				if (!perform_get_from_container(ch, obj, cont, mode))
					return;
			}
		}
		if (!found)
		{
			if (obj_dotmode == FIND_ALL)
				act("$o ����$A.", FALSE, ch, cont, 0, TO_CHAR);
			else
			{
				sprintf(buf, "�� �� ������ ������ �������� �� '%s' � $o5.", arg);
				act(buf, FALSE, ch, cont, 0, TO_CHAR);
			}
		}
	}
}


int perform_get_from_room(CHAR_DATA * ch, OBJ_DATA * obj)
{
	if (can_take_obj(ch, obj) && get_otrigger(obj, ch) && bloody::handle_transfer(NULL, ch, obj))
	{
		obj_from_room(obj);
		obj_to_char(obj, ch);
		if (obj->carried_by == ch)
		{
			if (bloody::is_bloody(obj))
			{
				act("�� ������� $o3, ��������  ���� ���� ������!", FALSE, ch, obj, 0, TO_CHAR);
				act("$n ������$g $o3, �������� ���� ������.", TRUE, ch, obj, 0, TO_ROOM);
			} else {
				act("�� ������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
				act("$n ������$g $o3.", TRUE, ch, obj, 0, TO_ROOM);
			}
			get_check_money(ch, obj);
			return (1);
		}
	}
	return (0);
}


void get_from_room(CHAR_DATA * ch, char *arg, int howmany)
{
	OBJ_DATA *obj, *next_obj;
	int dotmode, found = 0;

	/* Are they trying to take something in a room extra description? */
	if (find_exdesc(arg, world[IN_ROOM(ch)]->ex_description) != NULL)
	{
		send_to_char("�� �� ������ ��� �����.\r\n", ch);
		return;
	}

	dotmode = find_all_dots(arg);

	if (dotmode == FIND_INDIV)
	{
		if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room]->contents)))
		{
			sprintf(buf, "�� �� ������ ����� '%s'.\r\n", arg);
			send_to_char(buf, ch);
		}
		else
		{
			OBJ_DATA *obj_next;
			while (obj && howmany--)
			{
				obj_next = obj->next_content;
				perform_get_from_room(ch, obj);
				obj = get_obj_in_list_vis(ch, arg, obj_next);
			}
		}
	}
	else
	{
		if (dotmode == FIND_ALLDOT && !*arg)
		{
			send_to_char("����� ��� \"���\" ?\r\n", ch);
			return;
		}
		for (obj = world[ch->in_room]->contents; obj; obj = next_obj)
		{
			next_obj = obj->next_content;
			if (CAN_SEE_OBJ(ch, obj) && (dotmode == FIND_ALL || isname(arg, obj->name)))
			{
				found = 1;
				perform_get_from_room(ch, obj);
			}
		}
		if (!found)
		{
			if (dotmode == FIND_ALL)
				send_to_char("������, ����� ������ ���.\r\n", ch);
			else
			{
				sprintf(buf, "�� �� ����� ����� '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
		}
	}
}

ACMD(do_mark)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];

	int cont_dotmode, found = 0, mode;
	OBJ_DATA *cont;
	CHAR_DATA *tmp_char;

	argument = two_arguments(argument, arg1, arg2);

	if (!*arg1)
		send_to_char("��� �� ������ ����������� ?\r\n", ch);
	else if (!*arg2 || !is_number(arg2))
		send_to_char("�� ������ ��� �������� ������.\r\n", ch);
	else
	{
		cont_dotmode = find_all_dots(arg1);
		if (cont_dotmode == FIND_INDIV)
		{
			mode = generic_find(arg1, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP, ch, &tmp_char, &cont);
			if (!cont)
			{
				sprintf(buf, "� ��� ��� '%s'.\r\n", arg1);
				send_to_char(buf, ch);
				return;
			}
			cont->obj_flags.Obj_owner = atoi(arg2);
			act("�� �������� $o3.", FALSE, ch, cont, 0, TO_CHAR);
		}
		else
		{
			if (cont_dotmode == FIND_ALLDOT && !*arg1)
			{
				send_to_char("�������� ��� \"���\" ?\r\n", ch);
				return;
			}
			for (cont = ch->carrying; cont; cont = cont->next_content)
				if (CAN_SEE_OBJ(ch, cont) && (cont_dotmode == FIND_ALL || isname(arg1, cont->name)))
				{
					cont->obj_flags.Obj_owner = atoi(arg2);
					act("�� �������� $o3.", FALSE, ch, cont, 0, TO_CHAR);
					found = TRUE;
				}
			for (cont = world[ch->in_room]->contents; cont; cont = cont->next_content)
				if (CAN_SEE_OBJ(ch, cont)
						&& (cont_dotmode == FIND_ALL || isname(arg2, cont->name)))
				{
					cont->obj_flags.Obj_owner = atoi(arg2);
					act("�� �������� $o3.", FALSE, ch, cont, 0, TO_CHAR);
					found = TRUE;
				}
			if (!found)
			{
				if (cont_dotmode == FIND_ALL)
					send_to_char("�� �� ������ ����� ������ ��� ����������.\r\n", ch);
				else
				{
					sprintf(buf, "�� ���-�� �� ������ ����� '%s'.\r\n", arg1);
					send_to_char(buf, ch);
				}
			}
		}
	}
}


ACMD(do_get)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	char arg4[MAX_INPUT_LENGTH];
	char *theobj, *thecont, *theplace;
	int where_bits = FIND_OBJ_INV | FIND_OBJ_EQUIP | FIND_OBJ_ROOM;

	int cont_dotmode, found = 0, mode, amount = 1;
	OBJ_DATA *cont;
	CHAR_DATA *tmp_char;

	argument = two_arguments(argument, arg1, arg2);
	argument = two_arguments(argument, arg3, arg4);

	if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
		send_to_char("� ��� ������ ���� !\r\n", ch);
	else if (!*arg1)
		send_to_char("��� �� ������ ����� ?\r\n", ch);
	else if (!*arg2 || isname(arg2, "����� ������� ground room"))
		get_from_room(ch, arg1, 1);
	else if (is_number(arg1) && (!*arg3 || isname(arg3, "����� ������� ground room")))
		get_from_room(ch, arg2, atoi(arg1));
	else if ((!*arg3 && isname(arg2, "��������� ���������� inventory equipment")) ||
			 (is_number(arg1) && !*arg4 && isname(arg3, "��������� ���������� inventory equipment")))
		send_to_char("�� ��� ��������� ���� ������� !\r\n", ch);
	else
	{
		if (is_number(arg1))
		{
			amount = atoi(arg1);
			theobj = arg2;
			thecont = arg3;
			theplace = arg4;
		}
		else
		{
			theobj = arg1;
			thecont = arg2;
			theplace = arg3;
		}


		if (isname(theplace, "����� ������� room ground"))
			where_bits = FIND_OBJ_ROOM;
		else if (isname(theplace, "��������� inventory"))
			where_bits = FIND_OBJ_INV;
		else if (isname(theplace, "���������� equipment"))
			where_bits = FIND_OBJ_EQUIP;

		cont_dotmode = find_all_dots(thecont);
		if (cont_dotmode == FIND_INDIV)
		{
			mode = generic_find(thecont, where_bits, ch, &tmp_char, &cont);
			if (!cont)
			{
				sprintf(buf, "�� �� ������ '%s'.\r\n", arg2);
				send_to_char(buf, ch);
			}
			else if (GET_OBJ_TYPE(cont) != ITEM_CONTAINER)
				act("$o - �� ���������.", FALSE, ch, cont, 0, TO_CHAR);
			else
				get_from_container(ch, cont, theobj, mode, amount, false);
		}
		else
		{
			if (cont_dotmode == FIND_ALLDOT && !*thecont)
			{
				send_to_char("����� �� ���� \"�����\" ?\r\n", ch);
				return;
			}
			for (cont = ch->carrying; cont && IS_SET(where_bits, FIND_OBJ_INV); cont = cont->next_content)
				if (CAN_SEE_OBJ(ch, cont) && (cont_dotmode == FIND_ALL || isname(thecont, cont->name)))
				{
					if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER)
					{
						found = 1;
						get_from_container(ch, cont, theobj, FIND_OBJ_INV, amount, false);
					}
					else if (cont_dotmode == FIND_ALLDOT)
					{
						found = 1;
						act("$o - �� ���������.", FALSE, ch, cont, 0, TO_CHAR);
					}
				}
			for (cont = world[ch->in_room]->contents;
					cont && IS_SET(where_bits, FIND_OBJ_ROOM); cont = cont->next_content)
				if (CAN_SEE_OBJ(ch, cont) && (cont_dotmode == FIND_ALL || isname(thecont, cont->name)))
				{
					if (GET_OBJ_TYPE(cont) == ITEM_CONTAINER)
					{
						get_from_container(ch, cont, theobj, FIND_OBJ_ROOM, amount, false);
						found = 1;
					}
					else if (cont_dotmode == FIND_ALLDOT)
					{
						act("$o - �� ���������.", FALSE, ch, cont, 0, TO_CHAR);
						found = 1;
					}
				}
			if (!found)
			{
				if (cont_dotmode == FIND_ALL)
					send_to_char("�� �� ������ ����� �� ������ ����������.\r\n", ch);
				else
				{
					sprintf(buf, "�� ���-�� �� ������ ����� '%s'.\r\n", thecont);
					send_to_char(buf, ch);
				}
			}
		}
	}
}


void perform_drop_gold(CHAR_DATA * ch, int amount, byte mode, room_rnum RDR)
{
	OBJ_DATA *obj;
	if (amount <= 0)
		send_to_char("��, ������ �� ������� ���������� �������.\r\n", ch);
	else if (ch->get_gold() < amount)
		send_to_char("� ��� ��� ����� ����� !\r\n", ch);
	else
	{
		if (mode != SCMD_JUNK)
		{
			WAIT_STATE(ch, PULSE_VIOLENCE);	/* to prevent coin-bombing */
			if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_NOITEM))
			{
				act("��������� ���� �������� ��� ������� ���!!", FALSE, ch, 0, 0, TO_CHAR);
				return;
			}
			//������� ������� ����� � �������
			int additional_amount = 0;
			OBJ_DATA* next_obj;
			if (mode != SCMD_DONATE)
				for (OBJ_DATA* existing_obj=world[ch->in_room]->contents; existing_obj; existing_obj = next_obj)
				{
					next_obj  = existing_obj->next_content;
					if (GET_OBJ_TYPE(existing_obj) == ITEM_MONEY)
					{
						//���������� ��������� ������������ ����� � ������� ��
						additional_amount = GET_OBJ_VAL(existing_obj, 0);
						obj_from_room(existing_obj);
						extract_obj(existing_obj);
					}
				}
			obj = create_money(amount+additional_amount);
			if (mode == SCMD_DONATE)
			{
				sprintf(buf, "�� ��������� %d %s �� �����.\r\n", amount,
						desc_count(amount, WHAT_MONEYu));
				send_to_char(buf, ch);
				act("$n ��������$g ������... �� ����� :(", FALSE, ch, 0, 0, TO_ROOM);
				obj_to_room(obj, RDR);
				act("$o �����$Q � ������ ���� !", 0, 0, obj, 0, TO_ROOM);
			}
			else
			{
				if (!drop_wtrigger(obj, ch))
				{
					extract_obj(obj);
					return;
				}
				/* ���� ���� ��� ����� �� �������, �� �� �������� ���������
				   ����� ������ ������ ��������� � ��� � � ������ */
				if (!IS_NPC(ch) || !MOB_FLAGGED(ch, MOB_CORPSE))
				{
					sprintf(buf, "�� ������� %d %s �� �����.", amount, desc_count(amount, WHAT_MONEYu));
					send_to_char(buf, ch);
					sprintf(buf, "$n ������$g %s �� �����.", money_desc(amount, 3));
					act(buf, TRUE, ch, 0, 0, TO_ROOM);
				}
				obj_to_room(obj, ch->in_room);
			}
		}
		else
		{
			sprintf(buf, "$n �����������$g %s... � ������� ����� !", money_desc(amount, 3));
			act(buf, FALSE, ch, 0, 0, TO_ROOM);
			sprintf(buf, "�� ������������ ����� %d %s.\r\n", amount, desc_count(amount, WHAT_MONEYu));
			send_to_char(buf, ch);
		}
		ch->remove_gold(amount);
	}
}


#define VANISH(mode) ((mode == SCMD_DONATE || mode == SCMD_JUNK) ? \
            "  It vanishes in a puff of smoke!" : "")

const char *drop_op[3][3] =
{
	{"���������", "���������", "��������"},
	{"������������", "������������", "�����������"},
	{"�������", "�������", "������"}
};
int perform_drop(CHAR_DATA * ch, OBJ_DATA * obj, byte mode, const int sname, room_rnum RDR)
{
	int value;
	if (!drop_otrigger(obj, ch))
		return 0;
	if (!bloody::handle_transfer(ch, NULL, obj))
		return 0;
	if ((mode == SCMD_DROP) && !drop_wtrigger(obj, ch))
		return 0;
	if (IS_OBJ_STAT(obj, ITEM_NODROP))
	{
		sprintf(buf, "�� �� ������ %s $o3!", drop_op[sname][0]);
		act(buf, FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}
	sprintf(buf, "�� %s $o3.%s", drop_op[sname][1], VANISH(mode));
	act(buf, FALSE, ch, obj, 0, TO_CHAR);
	sprintf(buf, "$n %s$g $o3.%s", drop_op[sname][2], VANISH(mode));
	act(buf, TRUE, ch, obj, 0, TO_ROOM);
	obj_from_char(obj);

	if ((mode == SCMD_DONATE) && IS_OBJ_STAT(obj, ITEM_NODONATE))
		mode = SCMD_JUNK;

	switch (mode)
	{
	case SCMD_DROP:
		obj_to_room(obj, ch->in_room);
		obj_decay(obj);
		return (0);
	case SCMD_DONATE:
		obj_to_room(obj, RDR);
		obj_decay(obj);
		act("$o ���������$U � ������ ���� !", FALSE, 0, obj, 0, TO_ROOM);
		return (0);
	case SCMD_JUNK:
		value = MAX(1, MIN(200, GET_OBJ_COST(obj) / 16));
		extract_obj(obj);
		return (value);
	default:
		log("SYSERR: Incorrect argument %d passed to perform_drop.", mode);
		break;
	}

	return (0);
}



ACMD(do_drop)
{
	OBJ_DATA *obj, *next_obj;
	room_rnum RDR = 0;
	byte mode = SCMD_DROP;
	int dotmode, amount = 0, multi;
	int sname;

	switch (subcmd)
	{
	case SCMD_JUNK:
		sname = 0;
		mode = SCMD_JUNK;
		break;
	case SCMD_DONATE:
		sname = 1;
		mode = SCMD_DONATE;
		switch (number(0, 2))
		{
		case 0:
			mode = SCMD_JUNK;
			break;
		case 1:
		case 2:
			// ��� ���� donation_room_1
			break;
		}
		if (RDR == NOWHERE)
		{
			send_to_char("�� �� ������ ����� ����� �������.\r\n", ch);
			return;
		}
		break;
	default:
		sname = 2;
		break;
	}

	argument = one_argument(argument, arg);

	if (!*arg)
	{
		sprintf(buf, "��� �� ������ %s?\r\n", drop_op[sname][0]);
		send_to_char(buf, ch);
		return;
	}
	else if (is_number(arg))
	{
		multi = atoi(arg);
		one_argument(argument, arg);
		if (!str_cmp("coins", arg) || !str_cmp("coin", arg) || !str_cmp("���", arg) || !str_cmp("�����", arg))
			perform_drop_gold(ch, multi, mode, RDR);
		else if (multi <= 0)
			send_to_char("�� ����� ������.\r\n", ch);
		else if (!*arg)
		{
			sprintf(buf, "%s %d ���� ?\r\n", drop_op[sname][0], multi);
			send_to_char(buf, ch);
		}
		else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
		{
			sprintf(buf, "� ��� ��� ������ �������� �� %s.\r\n", arg);
			send_to_char(buf, ch);
		}
		else
		{
			do
			{
				next_obj = get_obj_in_list_vis(ch, arg, obj->next_content);
				amount += perform_drop(ch, obj, mode, sname, RDR);
				obj = next_obj;
			}
			while (obj && --multi);
		}
	}
	else
	{
		dotmode = find_all_dots(arg);
		/* Can't junk or donate all */
		if ((dotmode == FIND_ALL) && (subcmd == SCMD_JUNK || subcmd == SCMD_DONATE))
		{
			if (subcmd == SCMD_JUNK)
				send_to_char("��� � ����������� ����. � ��������� :)\r\n", ch);
			else
				send_to_char("����� ������ � ������� �� ���� !\r\n", ch);
			return;
		}
		if (dotmode == FIND_ALL)
		{
			if (!ch->carrying)
				send_to_char("� � ��� ������ � ���.\r\n", ch);
			else
				for (obj = ch->carrying; obj; obj = next_obj)
				{
					next_obj = obj->next_content;
					amount += perform_drop(ch, obj, mode, sname, RDR);
				}
		}
		else if (dotmode == FIND_ALLDOT)
		{
			if (!*arg)
			{
				sprintf(buf, "%s \"���\" ������ ���� ��������� ?\r\n", drop_op[sname][0]);
				send_to_char(buf, ch);
				return;
			}
			if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
			{
				sprintf(buf, "� ��� ��� ������ �������� �� '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
			while (obj)
			{
				next_obj = get_obj_in_list_vis(ch, arg, obj->next_content);
				amount += perform_drop(ch, obj, mode, sname, RDR);
				obj = next_obj;
			}
		}
		else
		{
			if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
			{
				sprintf(buf, "� ��� ��� '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
			else
				amount += perform_drop(ch, obj, mode, sname, RDR);
		}
	}

	if (amount && (subcmd == SCMD_JUNK))
	{
		send_to_char("���� �� �������� �������� �� ���� ����.\r\n", ch);
		act("$n ������$q ������. �� ���� ���� ����� � �$m !", TRUE, ch, 0, 0, TO_ROOM);
	}

}


void perform_give(CHAR_DATA * ch, CHAR_DATA * vict, OBJ_DATA * obj)
{
	if (!bloody::handle_transfer(ch, vict, obj))
		return;
	if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_NOITEM) && !IS_GOD(ch))
	{
		act("��������� ���� �������� ��� ������� ���!!", FALSE, ch, 0, 0, TO_CHAR);
		return;
	}
	if (IS_OBJ_STAT(obj, ITEM_NODROP))
	{
		act("�� �� ������ �������� $o3!!", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}
	if (IS_CARRYING_N(vict) >= CAN_CARRY_N(vict))
	{
		act("� $N1 ������ ����.", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}
	if (GET_OBJ_WEIGHT(obj) + IS_CARRYING_W(vict) > CAN_CARRY_W(vict))
	{
		act("$E �� ����� ����� ����� ���.", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}
	if (!give_otrigger(obj, ch, vict) || !receive_mtrigger(vict, ch, obj))
		return;

	act("�� ���� $o3 $N2.", FALSE, ch, obj, vict, TO_CHAR);
	act("$n ���$g ��� $o3.", FALSE, ch, obj, vict, TO_VICT);
	act("$n ���$g $o3 $N2.", TRUE, ch, obj, vict, TO_NOTVICT);
	obj_from_char(obj);
	obj_to_char(obj, vict);
}

/* utility function for give */
CHAR_DATA *give_find_vict(CHAR_DATA * ch, char *arg)
{
	CHAR_DATA *vict;

	if (!*arg)
	{
		send_to_char("���� ?\r\n", ch);
		return (NULL);
	}
	else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
	{
		send_to_char(NOPERSON, ch);
		return (NULL);
	}
	else if (vict == ch)
	{
		send_to_char("�� ���������� ��� �� ������ ������� � ������.\r\n", ch);
		return (NULL);
	}
	else
		return (vict);
}


void perform_give_gold(CHAR_DATA * ch, CHAR_DATA * vict, int amount)
{
	if (amount <= 0)
	{
		send_to_char("��-��-�� (3 ����)...\r\n", ch);
		return;
	}
	if (ch->get_gold() < amount && (IS_NPC(ch) || !IS_IMPL(ch)))
	{
		send_to_char("� ������ �� �� ����� ����������� ?\r\n", ch);
		return;
	}
	if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_NOITEM) && !IS_GOD(ch))
	{
		act("��������� ���� �������� ��� ������� ���!!", FALSE, ch, 0, 0, TO_CHAR);
		return;
	}
	send_to_char(OK, ch);
	sprintf(buf, "$n ���$g ��� %d %s.", amount, desc_count(amount, WHAT_MONEYu));
	act(buf, FALSE, ch, 0, vict, TO_VICT);
	sprintf(buf, "$n ���$g %s $N2.", money_desc(amount, 3));
	act(buf, TRUE, ch, 0, vict, TO_NOTVICT);
	if (IS_NPC(ch) || !IS_IMPL(ch))
	{
		ch->remove_gold(amount);
	}
	vict->add_gold(amount);
	bribe_mtrigger(vict, ch, amount);
}


ACMD(do_give)
{
	int amount, dotmode;
	CHAR_DATA *vict;
	OBJ_DATA *obj, *next_obj;

	argument = one_argument(argument, arg);

	if (!*arg)
		send_to_char("���� ��� � ���� ?\r\n", ch);
	else if (is_number(arg))
	{
		amount = atoi(arg);
		argument = one_argument(argument, arg);
		if (!strn_cmp("coin", arg, 4) || !strn_cmp("���", arg, 5) || !str_cmp("�����", arg))
		{
			one_argument(argument, arg);
			if ((vict = give_find_vict(ch, arg)) != NULL)
				perform_give_gold(ch, vict, amount);
			return;
		}
		else if (!*arg)  	/* Give multiple code. */
		{
			sprintf(buf, "���� %d �� ������ ���� ?\r\n", amount);
			send_to_char(buf, ch);
		}
		else if (!(vict = give_find_vict(ch, argument)))
		{
			return;
		}
		else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
		{
			sprintf(buf, "� ��� ��� '%s'.\r\n", arg);
			send_to_char(buf, ch);
		}
		else
		{
			while (obj && amount--)
			{
				next_obj = get_obj_in_list_vis(ch, arg, obj->next_content);
				perform_give(ch, vict, obj);
				obj = next_obj;
			}
		}
	}
	else
	{
		one_argument(argument, buf1);
		if (!(vict = give_find_vict(ch, buf1)))
			return;
		dotmode = find_all_dots(arg);
		if (dotmode == FIND_INDIV)
		{
			if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
			{
				sprintf(buf, "� ��� ��� '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
			else
				perform_give(ch, vict, obj);
		}
		else
		{
			if (dotmode == FIND_ALLDOT && !*arg)
			{
				send_to_char("���� \"���\" ������ ���� ��������� ?\r\n", ch);
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
					if (CAN_SEE_OBJ(ch, obj) && ((dotmode == FIND_ALL || isname(arg, obj->name))))
					{
						perform_give(ch, vict, obj);
						has_items = true;
					}
				}
				if (!has_items)
					send_to_char(ch, "� ��� ��� '%s'.\r\n", arg);
			}
		}
	}
}



void weight_change_object(OBJ_DATA * obj, int weight)
{
	OBJ_DATA *tmp_obj;
	CHAR_DATA *tmp_ch;

	if (obj->in_room != NOWHERE)
	{
		GET_OBJ_WEIGHT(obj) = MAX(1, GET_OBJ_WEIGHT(obj) + weight);
	}
	else if ((tmp_ch = obj->carried_by))
	{
		obj_from_char(obj);
		GET_OBJ_WEIGHT(obj) = MAX(1, GET_OBJ_WEIGHT(obj) + weight);
		obj_to_char(obj, tmp_ch);
	}
	else if ((tmp_obj = obj->in_obj))
	{
		obj_from_obj(obj);
		GET_OBJ_WEIGHT(obj) = MAX(1, GET_OBJ_WEIGHT(obj) + weight);
		obj_to_obj(obj, tmp_obj);
	}
	else
	{
		log("SYSERR: Unknown attempt to subtract weight from an object.");
	}
}



ACMD(do_eat)
{
	OBJ_DATA *food;
	AFFECT_DATA af;
	int amount;

	one_argument(argument, arg);

	if (subcmd == SCMD_DEVOUR)
	{
		// changed by Pereplut
		if (MOB_FLAGGED(ch, MOB_RESURRECTED) && GET_CLASS(ch->master) == CLASS_NECROMANCER)
		{
			feed_charmice(ch, arg);
			return;
		}
	}
	if (!IS_NPC(ch) && subcmd == SCMD_DEVOUR)
	{
		send_to_char("�� �� �� ����� �����, �������� �����!\r\n", ch);
		return;
	}

	if (IS_NPC(ch))		/* Cannot use GET_COND() on mobs. */
		return;

	if (!*arg)
	{
		send_to_char("��� �� ��������� �������� ?\r\n", ch);
		return;
	}
	if (ch->get_fighting())
	{
		send_to_char("�� ����� ����������� � ���.\r\n", ch);
		return;
	}

	if (!(food = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� '%s'.\r\n", arg);
		send_to_char(buf, ch);
		return;
	}
	if (subcmd == SCMD_TASTE && ((GET_OBJ_TYPE(food) == ITEM_DRINKCON) || (GET_OBJ_TYPE(food) == ITEM_FOUNTAIN)))
	{
		do_drink(ch, argument, 0, SCMD_SIP);
		return;
	}
	if (GET_OBJ_TYPE(food) != ITEM_FOOD && GET_OBJ_TYPE(food) != ITEM_NOTE && !IS_GOD(ch))
	{
		send_to_char("��� ���������� !\r\n", ch);
		return;
	}
	if (GET_COND(ch, FULL) > 20 && GET_OBJ_TYPE(food) != ITEM_NOTE)  	/* Stomach full */
	{
		send_to_char("�� ������� ���� ��� ����� !\r\n", ch);
		return;
	}
	if (subcmd == SCMD_EAT || (subcmd == SCMD_TASTE && GET_OBJ_TYPE(food) == ITEM_NOTE))
	{
		act("�� ����� $o3.", FALSE, ch, food, 0, TO_CHAR);
		act("$n ����$g $o3.", TRUE, ch, food, 0, TO_ROOM);
	}
	else
	{
		act("�� �������� ��������� ������� �� $o1.", FALSE, ch, food, 0, TO_CHAR);
		act("$n ����������$g $o3 �� ����.", TRUE, ch, food, 0, TO_ROOM);
	}

	amount = ((subcmd == SCMD_EAT && GET_OBJ_TYPE(food) != ITEM_NOTE)
			  ? GET_OBJ_VAL(food, 0) : 1);

	gain_condition(ch, FULL, amount);

	if (GET_COND(ch, FULL) > 20)
		send_to_char("�� �������.\r\n", ch);

	if (GET_OBJ_VAL(food, 3) && !IS_IMMORTAL(ch))  	/* The shit was poisoned ! */
	{
		send_to_char("������, ����� �������� ���� !\r\n", ch);
		act("$n ��������$u � �����$g �������������.", FALSE, ch, 0, 0, TO_ROOM);

		af.type = SPELL_POISON;
		af.duration = pc_duration(ch, amount == 1 ? amount : amount * 2, 0, 0, 0, 0);
		af.modifier = 0;
		af.location = APPLY_STR;
		af.bitvector = AFF_POISON;
		af.battleflag = AF_SAME_TIME;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
		af.type = SPELL_POISON;
		af.duration = pc_duration(ch, amount == 1 ? amount : amount * 2, 0, 0, 0, 0);
		af.modifier = amount * 3;
		af.location = APPLY_POISON;
		af.bitvector = AFF_POISON;
		af.battleflag = AF_SAME_TIME;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
		ch->Poisoner = 0;
	}
	if (subcmd == SCMD_EAT || (subcmd == SCMD_TASTE && GET_OBJ_TYPE(food) == ITEM_NOTE))
		extract_obj(food);
	else
	{
		if (!(--GET_OBJ_VAL(food, 0)))
		{
			send_to_char("�� ����� ��� !\r\n", ch);
			extract_obj(food);
		}
	}
}

void perform_wear(CHAR_DATA * ch, OBJ_DATA * obj, int where)
{
	/*
	 * ITEM_WEAR_TAKE is used for objects that do not require special bits
	 * to be put into that position (e.g. you can hold any object, not just
	 * an object with a HOLD bit.)
	 */

	int wear_bitvectors[] = { ITEM_WEAR_TAKE, ITEM_WEAR_FINGER, ITEM_WEAR_FINGER, ITEM_WEAR_NECK,
							  ITEM_WEAR_NECK, ITEM_WEAR_BODY, ITEM_WEAR_HEAD, ITEM_WEAR_LEGS,
							  ITEM_WEAR_FEET, ITEM_WEAR_HANDS, ITEM_WEAR_ARMS, ITEM_WEAR_SHIELD,
							  ITEM_WEAR_ABOUT, ITEM_WEAR_WAIST, ITEM_WEAR_WRIST, ITEM_WEAR_WRIST,
							  ITEM_WEAR_WIELD, ITEM_WEAR_TAKE, ITEM_WEAR_BOTHS
							};

	const char *already_wearing[] = { "�� ��� ����������� ����.\r\n",
									  "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
									  "� ��� ��� ���-�� ������ �� �������.\r\n",
									  "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
									  "� ��� ��� ���-�� ������ �� ���.\r\n",
									  "� ��� ��� ���-�� ������ �� ��������.\r\n",
									  "� ��� ��� ���-�� ������ �� ������.\r\n",
									  "� ��� ��� ���-�� ������ �� ����.\r\n",
									  "� ��� ��� ���-�� ������ �� ������.\r\n",
									  "� ��� ��� ���-�� ������ �� �����.\r\n",
									  "� ��� ��� ���-�� ������ �� ����.\r\n",
									  "�� ��� ����������� ���.\r\n",
									  "�� ��� �������� �� ���-��.\r\n",
									  "� ��� ��� ���-�� ������ �� ����.\r\n",
									  "YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.\r\n",
									  "� ��� ��� ���-�� ������ �� ��������.\r\n",
									  "�� ��� ���-�� ������� � ������ ����.\r\n",
									  "�� ��� ���-�� ������� � ����� ����.\r\n",
									  "�� ��� ������� ������ � ����� �����.\r\n"
									};

	/* first, make sure that the wear position is valid. */
	if (!CAN_WEAR(obj, wear_bitvectors[where]))
	{
		act("�� �� ������ ������ $o3 �� ��� ����� ����.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}
	/* for neck, finger, and wrist, try pos 2 if pos 1 is already full */
	if (   /* �� ����� ������� ���� ���� ���� ��� ��������� */
		(where == WEAR_HOLD && (GET_EQ(ch, WEAR_BOTHS) || GET_EQ(ch, WEAR_LIGHT)
								|| GET_EQ(ch, WEAR_SHIELD))) ||
		/* �� ����� ����������� ���� ���� ��������� */
		(where == WEAR_WIELD && GET_EQ(ch, WEAR_BOTHS)) ||
		/* �� ����� ������� ��� ���� ���-�� ������ ��� ��������� */
		(where == WEAR_SHIELD && (GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_BOTHS))) ||
		/* �� ����� ��������� ���� ���� ���, ����, �������� ��� ������ */
		(where == WEAR_BOTHS && (GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_LIGHT)
								 || GET_EQ(ch, WEAR_SHIELD) || GET_EQ(ch, WEAR_WIELD))) ||
		/* �� ����� ������� ���� ���� ��������� ��� ������ */
		(where == WEAR_LIGHT && (GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_BOTHS))))
	{
		send_to_char("� ��� ������ ����.\r\n", ch);
		return;
	}
	/* ������ ������ ���, ���� ������������ ���� */
	if (!IS_IMMORTAL(ch) && (where == WEAR_SHIELD) && !OK_SHIELD(ch, obj))
	{
		act("��� ������� ������ ����� $o3 �� ����� ����.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}

	if ((where == WEAR_FINGER_R) || (where == WEAR_NECK_1) || (where == WEAR_WRIST_R))
		if (GET_EQ(ch, where))
			where++;

	if (GET_EQ(ch, where))
	{
		send_to_char(already_wearing[where], ch);
		return;
	}
	if (!wear_otrigger(obj, ch, where))
		return;

	//obj_from_char(obj);
	equip_char(ch, obj, where | 0x100);
}



int find_eq_pos(CHAR_DATA * ch, OBJ_DATA * obj, char *arg)
{
	int where = -1;

	/* \r to prevent explicit wearing. Don't use \n, it's end-of-array marker. */
	const char *keywords[] =
	{
		"\r!RESERVED!",
		"�����������",
		"����������",
		"���",
		"�����",
		"����",
		"������",
		"����",
		"������",
		"�����",
		"����",
		"���",
		"�����",
		"����",
		"��������",
		"\r!RESERVED!",
		"\r!RESERVED!",
		"\r!RESERVED!",
		"\n"
	};

	if (!arg || !*arg)
	{
		int tmp_where = -1;
		if (CAN_WEAR(obj, ITEM_WEAR_FINGER))
		{
			if (!GET_EQ(ch, WEAR_FINGER_R))
				where = WEAR_FINGER_R;
			else if (!GET_EQ(ch, WEAR_FINGER_L))
				where = WEAR_FINGER_L;
			else
				tmp_where = WEAR_FINGER_R;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_NECK))
		{
			if (!GET_EQ(ch, WEAR_NECK_1))
				where = WEAR_NECK_1;
			else if (!GET_EQ(ch, WEAR_NECK_2))
				where = WEAR_NECK_2;
			else
				tmp_where = WEAR_NECK_1;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_BODY))
		{
			if (!GET_EQ(ch, WEAR_BODY))
				where = WEAR_BODY;
			else
				tmp_where = WEAR_BODY;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_HEAD))
		{
			if (!GET_EQ(ch, WEAR_HEAD))
				where = WEAR_HEAD;
			else
				tmp_where = WEAR_HEAD;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_LEGS))
		{
			if (!GET_EQ(ch, WEAR_LEGS))
				where = WEAR_LEGS;
			else
				tmp_where = WEAR_LEGS;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_FEET))
		{
			if (!GET_EQ(ch, WEAR_FEET))
				where = WEAR_FEET;
			else
				tmp_where = WEAR_FEET;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_HANDS))
		{
			if (!GET_EQ(ch, WEAR_HANDS))
				where = WEAR_HANDS;
			else
				tmp_where = WEAR_HANDS;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_ARMS))
		{
			if (!GET_EQ(ch, WEAR_ARMS))
				where = WEAR_ARMS;
			else
				tmp_where = WEAR_ARMS;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_SHIELD))
		{
			if (!GET_EQ(ch, WEAR_SHIELD))
				where = WEAR_SHIELD;
			else
				tmp_where = WEAR_SHIELD;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_ABOUT))
		{
			if (!GET_EQ(ch, WEAR_ABOUT))
				where = WEAR_ABOUT;
			else
				tmp_where = WEAR_ABOUT;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_WAIST))
		{
			if (!GET_EQ(ch, WEAR_WAIST))
				where = WEAR_WAIST;
			else
				tmp_where = WEAR_WAIST;
		}
		if (CAN_WEAR(obj, ITEM_WEAR_WRIST))
		{
			if (!GET_EQ(ch, WEAR_WRIST_R))
				where = WEAR_WRIST_R;
			else if (!GET_EQ(ch, WEAR_WRIST_L))
				where = WEAR_WRIST_L;
			else
				tmp_where = WEAR_WRIST_R;
		}

		if (where == -1)
			where = tmp_where;
	}
	else
	{
		if (((where = search_block(arg, keywords, FALSE)) < 0) || (*arg == '!'))
		{
			sprintf(buf, "'%s'?  �������� �������� � ���� ������� !\r\n", arg);
			send_to_char(buf, ch);
			return -1;
		}
	}

	return (where);
}



ACMD(do_wear)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	OBJ_DATA *obj, *next_obj;
	int where, dotmode, items_worn = 0;

	two_arguments(argument, arg1, arg2);

	if (IS_NPC(ch) && (AFF_FLAGGED(ch, AFF_CHARM) && (!NPC_FLAGGED(ch, NPC_ARMORING) || MOB_FLAGGED(ch, MOB_RESURRECTED))))
		return;

	if (!*arg1)
	{
		send_to_char("��� �� ��������� ������ ?\r\n", ch);
		return;
	}
	dotmode = find_all_dots(arg1);

	if (*arg2 && (dotmode != FIND_INDIV))
	{
		send_to_char("� �� ����� ����� ���� �� ������� ��� ������ ?!\r\n", ch);
		return;
	}
	if (dotmode == FIND_ALL)
	{
		for (obj = ch->carrying; obj && !GET_MOB_HOLD(ch) && GET_POS(ch) > POS_SLEEPING; obj = next_obj)
		{
			next_obj = obj->next_content;
			if (CAN_SEE_OBJ(ch, obj)
					&& (where = find_eq_pos(ch, obj, 0)) >= 0)
			{
				items_worn++;
				perform_wear(ch, obj, where);
			}
		}
		if (!items_worn)
			send_to_char("���, �� ������ ��� ������.\r\n", ch);
	}
	else if (dotmode == FIND_ALLDOT)
	{
		if (!*arg1)
		{
			send_to_char("������ \"���\" ���� ?\r\n", ch);
			return;
		}
		if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying)))
		{
			sprintf(buf, "� ��� ��� ������ �������� �� '%s'.\r\n", arg1);
			send_to_char(buf, ch);
		}
		else
			while (obj && !GET_MOB_HOLD(ch) && GET_POS(ch) > POS_SLEEPING)
			{
				next_obj = get_obj_in_list_vis(ch, arg1, obj->next_content);
				if ((where = find_eq_pos(ch, obj, 0)) >= 0)
					perform_wear(ch, obj, where);
				else
					act("�� �� ������ ������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
				obj = next_obj;
			}
	}
	else
	{
		if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying)))
		{
			sprintf(buf, "� ��� ��� ������ �������� �� '%s'.\r\n", arg1);
			send_to_char(buf, ch);
		}
		else
		{
			if ((where = find_eq_pos(ch, obj, arg2)) >= 0)
				perform_wear(ch, obj, where);
			else if (!*arg2)
				act("�� �� ������ ������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
		}
	}
}

ACMD(do_wield)
{
	OBJ_DATA *obj;
	int wear;

	if (IS_NPC(ch) && (AFF_FLAGGED(ch, AFF_CHARM)&&(!NPC_FLAGGED(ch, NPC_WIELDING) || MOB_FLAGGED(ch, MOB_RESURRECTED))))
		return;

	if (ch->is_morphed())
	{
		send_to_char("������ �������� ������� ������.\r\n", ch);
		return;
	}
	argument = one_argument(argument, arg);

	if (!*arg)
		send_to_char("����������� ��� ?\r\n", ch);
	else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "�� �� ������ ������ �������� �� \'%s\'.\r\n", arg);
		send_to_char(buf, ch);
	}
	else
	{
		if (!CAN_WEAR(obj, ITEM_WEAR_WIELD)
				&& !CAN_WEAR(obj, ITEM_WEAR_BOTHS))
			send_to_char("�� �� ������ ����������� ����.\r\n", ch);
		else if (GET_OBJ_TYPE(obj) != ITEM_WEAPON)
			send_to_char("��� �� ������.\r\n", ch);
		else if (IS_NPC(ch) && AFF_FLAGGED(ch, AFF_CHARM) && MOB_FLAGGED(ch, MOB_CORPSE))
			send_to_char("������� ����� �� ����� �����������.\r\n", ch);
		else
		{
			one_argument(argument, arg);
			if (!str_cmp(arg, "���") && CAN_WEAR(obj, ITEM_WEAR_BOTHS))
			{
				// ������ ������ ����
				if (!IS_IMMORTAL(ch) && !OK_BOTH(ch, obj))
				{
					act("��� ������� ������ ������� $o3 ����� ������.", FALSE, ch, obj, 0, TO_CHAR);
					return;
				};
				perform_wear(ch, obj, WEAR_BOTHS);
				return;
			}
			if (CAN_WEAR(obj, ITEM_WEAR_WIELD))
				wear = WEAR_WIELD;
			else
				wear = WEAR_BOTHS;
			if (wear == WEAR_WIELD && !IS_IMMORTAL(ch) && !OK_WIELD(ch, obj))
			{
				act("��� ������� ������ ������� $o3 � ������ ����.", FALSE, ch, obj, 0, TO_CHAR);
				message_str_need(ch, obj, STR_WIELD_W);

				if (CAN_WEAR(obj, ITEM_WEAR_BOTHS))
					wear = WEAR_BOTHS;
				else
					return;
			}
			if (wear == WEAR_BOTHS && !IS_IMMORTAL(ch) && !OK_BOTH(ch, obj))
			{
				act("��� ������� ������ ������� $o3 ����� ������.", FALSE, ch, obj, 0, TO_CHAR);
				return;
			};
			perform_wear(ch, obj, wear);
		}
	}
}

ACMD(do_grab)
{
	int where = WEAR_HOLD;
	OBJ_DATA *obj;
	one_argument(argument, arg);

	if (IS_NPC(ch) && !NPC_FLAGGED(ch, NPC_WIELDING))
		return;

	if (ch->is_morphed())
	{
		send_to_char("������ �������� ��� �������.\r\n", ch);
		return;
	}

	if (!*arg)
		send_to_char("�� ������� : '����� ��� !!! ������ ��� !!!'\r\n", ch);
	else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� ������ �������� �� '%s'.\r\n", arg);
		send_to_char(buf, ch);
	}
	else
	{
		if (GET_OBJ_TYPE(obj) == ITEM_LIGHT)
			perform_wear(ch, obj, WEAR_LIGHT);
		else
		{
			if (!CAN_WEAR(obj, ITEM_WEAR_HOLD) &&
					GET_OBJ_TYPE(obj) != ITEM_WAND &&
					GET_OBJ_TYPE(obj) != ITEM_STAFF &&
					GET_OBJ_TYPE(obj) != ITEM_SCROLL && GET_OBJ_TYPE(obj) != ITEM_POTION)
			{
				send_to_char("�� �� ������ ��� �������.\r\n", ch);
				return;
			}
			if (GET_OBJ_TYPE(obj) == ITEM_WEAPON)
			{
				if (GET_OBJ_SKILL(obj) == SKILL_BOTHHANDS || GET_OBJ_SKILL(obj) == SKILL_BOWS)
				{
					send_to_char("������ ��� ������ ������� ����������.", ch);
					return;
				}
			}
			if (IS_NPC(ch) && AFF_FLAGGED(ch, AFF_CHARM) && MOB_FLAGGED(ch, MOB_CORPSE))
			{
				send_to_char("������� ����� �� ����� �����������.\r\n", ch);
				return;
			}
			if (!IS_IMMORTAL(ch) && !OK_HELD(ch, obj))
			{
				act("��� ������� ������ ������� $o3 � ����� ����.", FALSE, ch, obj, 0, TO_CHAR);
				message_str_need(ch, obj, STR_HOLD_W);

				if (CAN_WEAR(obj, ITEM_WEAR_BOTHS))
				{
					if (!OK_BOTH(ch, obj))
					{
						act("��� ������� ������ ������� $o3 ����� ������.",
							FALSE, ch, obj, 0, TO_CHAR);
						return;
					}
					else
						where = WEAR_BOTHS;
				}
				else
					return;
			}
			perform_wear(ch, obj, where);
		}
	}
}



void perform_remove(CHAR_DATA * ch, int pos)
{
	OBJ_DATA *obj;

	if (!(obj = GET_EQ(ch, pos)))
		log("SYSERR: perform_remove: bad pos %d passed.", pos);
	else
		/*
		   if (IS_OBJ_STAT(obj, ITEM_NODROP))
		   act("�� �� ������ ����� $o3!", FALSE, ch, obj, 0, TO_CHAR);
		   else
		 */
		if (IS_CARRYING_N(ch) >= CAN_CARRY_N(ch))
			act("$p: �� �� ������ ����� ������� �����!", FALSE, ch, obj, 0, TO_CHAR);
		else
		{
			if (!remove_otrigger(obj, ch))
				return;
			act("�� ���������� ������������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
			act("$n ���������$g ������������ $o3.", TRUE, ch, obj, 0, TO_ROOM);
			obj_to_char(unequip_char(ch, pos | 0x40), ch);
		}
}



ACMD(do_remove)
{
	int i, dotmode, found;

	one_argument(argument, arg);

	if (!*arg)
	{
		send_to_char("����� ��� ?\r\n", ch);
		return;
	}
	dotmode = find_all_dots(arg);

	if (dotmode == FIND_ALL)
	{
		found = 0;
		for (i = 0; i < NUM_WEARS; i++)
			if (GET_EQ(ch, i))
			{
				perform_remove(ch, i);
				found = 1;
			}
		if (!found)
			send_to_char("�� ��� �� ������ ��������� ����� ����.\r\n", ch);
	}
	else if (dotmode == FIND_ALLDOT)
	{
		if (!*arg)
			send_to_char("����� ��� ���� ������ ���� ?\r\n", ch);
		else
		{
			found = 0;
			for (i = 0; i < NUM_WEARS; i++)
				if (GET_EQ(ch, i) && CAN_SEE_OBJ(ch, GET_EQ(ch, i)) && isname(arg, GET_EQ(ch, i)->name))
				{
					perform_remove(ch, i);
					found = 1;
				}
			if (!found)
			{
				sprintf(buf, "�� �� ����������� �� ������ '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
		}
	}
	else  		/* Returns object pointer but we don't need it, just true/false. */
	{
		if (!get_object_in_equip_vis(ch, arg, ch->equipment, &i))
		{
			// ���� ������� �� ������, �� �������� ����� ���� "�����" ��� "������"
			if (!str_cmp("������", arg))
			{
				if (!GET_EQ(ch, WEAR_WIELD))
					send_to_char("� ������ ���� ������ ���.\r\n", ch);
				else
					perform_remove(ch, WEAR_WIELD);
			}
			else if (!str_cmp("�����", arg))
			{
				if (!GET_EQ(ch, WEAR_HOLD))
					send_to_char("� ����� ���� ������ ���.\r\n", ch);
				else
					perform_remove(ch, WEAR_HOLD);
			}
			else
			{
				sprintf(buf, "�� �� ����������� '%s'.\r\n", arg);
				send_to_char(buf, ch);
			}
		}
		else
			perform_remove(ch, i);
	}
}


ACMD(do_upgrade)
{
	OBJ_DATA *obj;
	int weight, add_hr, add_dr, prob, percent, i;

	if (!ch->get_skill(SKILL_UPGRADE))
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	}

	one_argument(argument, arg);

	if (!*arg)
		send_to_char("��� �� ������ �������� ?\r\n", ch);

	if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� \'%s\'.\r\n", arg);
		send_to_char(buf, ch);
		return;
	};

	if (GET_OBJ_TYPE(obj) != ITEM_WEAPON)
	{
		send_to_char("�� ������ �������� ������ ������.\r\n", ch);
		return;
	}

	if (GET_OBJ_SKILL(obj) == SKILL_BOWS)
	{
		send_to_char("���������� �������� ���� ��� ������.\r\n", ch);
		return;
	}

	if (OBJ_FLAGGED(obj, ITEM_MAGIC) || OBJ_FLAGGED(obj, ITEM_SHARPEN))
	{
		send_to_char("�� �� ������ �������� ���� �������.\r\n", ch);
		return;
	}

	/* Make sure no other affections. */
	for (i = 0; i < MAX_OBJ_AFFECT; i++)
		if (obj->affected[i].location != APPLY_NONE)
		{
			send_to_char("���� ������� �� ����� ���� �������.\r\n", ch);
			return;
		}


	switch (obj->obj_flags.Obj_mater)
	{
	case MAT_BRONZE:
	case MAT_BULAT:
	case MAT_IRON:
	case MAT_STEEL:
	case MAT_SWORDSSTEEL:
	case MAT_COLOR:
	case MAT_BONE:
		act("�� ������� ������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ����$u ������ $o3.", FALSE, ch, obj, 0, TO_ROOM);
		weight = -1;
		break;
	case MAT_WOOD:
	case MAT_SUPERWOOD:
		act("�� ������� �������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ����$u �������� $o3.", FALSE, ch, obj, 0, TO_ROOM);
		weight = -1;
		break;
	case MAT_SKIN:
		act("�� ������� ������������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ����$u ������������ $o3.", FALSE, ch, obj, 0, TO_ROOM);
		weight = + 1;
		break;
	default:
		sprintf(buf, "� ���������, %s ������ �� ������������� ���������.\r\n", OBJN(obj, ch, 0));
		send_to_char(buf, ch);
		return;
	}

	SET_BIT(GET_OBJ_EXTRA(obj, ITEM_SHARPEN), ITEM_SHARPEN);
	percent = number(1, skill_info[SKILL_UPGRADE].max_percent);
	prob = train_skill(ch, SKILL_UPGRADE, skill_info[SKILL_UPGRADE].max_percent, 0);

	add_hr = IS_IMMORTAL(ch) ? 10 : number(1, (GET_LEVEL(ch) + 5) / 6);
	add_dr = IS_IMMORTAL(ch) ? 5 : number(1, (GET_LEVEL(ch) + 5) / 6);
	if (percent > prob || GET_GOD_FLAG(ch, GF_GODSCURSE))
	{
		act("�� ������ �������� $S.", FALSE, ch, obj, 0, TO_CHAR);
		add_hr = -add_hr;
		add_dr = -add_dr;
	}

	obj->affected[0].location = APPLY_HITROLL;
	obj->affected[0].modifier = add_hr;

	obj->affected[1].location = APPLY_DAMROLL;
	obj->affected[1].modifier = add_dr;
	obj->obj_flags.weight += weight;
	IS_CARRYING_W(ch) += weight;
//obj->obj_flags.Obj_owner  = GET_UNIQUE(ch);
}


ACMD(do_armored)
{
	OBJ_DATA *obj;
	int add_ac, add_armor, prob, percent, i, k_mul = 1, k_div = 1;

	if (!ch->get_skill(SKILL_ARMORED))
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	}

	one_argument(argument, arg);

	if (!*arg)
		send_to_char("��� �� ������ �������� ?\r\n", ch);

	if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� \'%s\'.\r\n", arg);
		send_to_char(buf, ch);
		return;
	};

	if (!ObjSystem::is_armor_type(obj))
	{
		send_to_char("�� ������ �������� ������ ������.\r\n", ch);
		return;
	}

	if (OBJ_FLAGGED(obj, ITEM_MAGIC) || OBJ_FLAGGED(obj, ITEM_ARMORED))
	{
		send_to_char("�� �� ������ �������� ���� �������.\r\n", ch);
		return;
	}

	/* Make sure no other affections. */
	for (i = 0; i < MAX_OBJ_AFFECT; i++)
		if (obj->affected[i].location != APPLY_NONE)
		{
			send_to_char("���� ������� �� ����� ���� ��������.\r\n", ch);
			return;
		}

	if (OBJWEAR_FLAGGED(obj, (ITEM_WEAR_BODY | ITEM_WEAR_ABOUT)))
	{
		k_mul = 1;
		k_div = 1;
	}
	else if (OBJWEAR_FLAGGED(obj, (ITEM_WEAR_SHIELD | ITEM_WEAR_HEAD | ITEM_WEAR_ARMS | ITEM_WEAR_LEGS)))
	{
		k_mul = 2;
		k_div = 3;
	}
	else if (OBJWEAR_FLAGGED(obj, (ITEM_WEAR_HANDS | ITEM_WEAR_FEET)))
	{
		k_mul = 1;
		k_div = 2;
	}
	else
	{
		act("$o3 ���������� ��������.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}

	switch (obj->obj_flags.Obj_mater)
	{
	case MAT_IRON:
	case MAT_STEEL:
		act("�� ��������� �������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ������$u �������� $o3.", FALSE, ch, obj, 0, TO_ROOM);
		break;
	case MAT_WOOD:
	case MAT_SUPERWOOD:
		act("�� ��������� �������� $o3 �������.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ������$u �������� $o3 �������.", FALSE, ch, obj, 0, TO_ROOM);
		break;
	case MAT_SKIN:
		act("�� ��������� ������������ $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n ������$u ������������ $o3.", FALSE, ch, obj, 0, TO_ROOM);
		break;
	default:
		sprintf(buf, "� ���������, %s ������ �� ������������� ���������.\r\n", OBJN(obj, ch, 0));
		send_to_char(buf, ch);
		return;
	}

	SET_BIT(GET_OBJ_EXTRA(obj, ITEM_ARMORED), ITEM_ARMORED);
	percent = number(1, skill_info[SKILL_ARMORED].max_percent);
	prob = train_skill(ch, SKILL_ARMORED, skill_info[SKILL_ARMORED].max_percent, 0);

	add_ac = IS_IMMORTAL(ch) ? -20 : -number(1, (GET_LEVEL(ch) + 4) / 5);
	add_armor = IS_IMMORTAL(ch) ? 5 : number(1, (GET_LEVEL(ch) + 4) / 5);

	if (percent > prob || GET_GOD_FLAG(ch, GF_GODSCURSE))
	{
		act("�� ������ ��������� $S.", FALSE, ch, obj, 0, TO_CHAR);
		add_ac = -add_ac;
		add_armor = -add_armor;
	}
	else
	{
		add_ac = MIN(-1, add_ac * k_mul / k_div);
		add_armor = MAX(1, add_armor * k_mul / k_div);
	};

	obj->affected[0].location = APPLY_AC;
	obj->affected[0].modifier = add_ac;

	obj->affected[1].location = APPLY_ARMOUR;
	obj->affected[1].modifier = add_armor;

//obj->obj_flags.Obj_owner = GET_UNIQUE(ch);
}

ACMD(do_fire)
{
	int percent, prob;
	if (!ch->get_skill(SKILL_FIRE))
	{
		send_to_char("�� �� �� ������ ���.\r\n", ch);
		return;
	}

	if (on_horse(ch))
	{
		send_to_char("������ ��� ����� ��������������.\r\n", ch);
		return;
	}

	if (AFF_FLAGGED(ch, AFF_BLIND))
	{
		send_to_char("�� ������ �� ������ !\r\n", ch);
		return;
	}


	if (world[IN_ROOM(ch)]->fires)
	{
		send_to_char("����� ��� ����� �����.\r\n", ch);
		return;
	}

	if (SECT(IN_ROOM(ch)) == SECT_INSIDE ||
			SECT(IN_ROOM(ch)) == SECT_CITY ||
			SECT(IN_ROOM(ch)) == SECT_WATER_SWIM ||
			SECT(IN_ROOM(ch)) == SECT_WATER_NOSWIM ||
			SECT(IN_ROOM(ch)) == SECT_FLYING ||
			SECT(IN_ROOM(ch)) == SECT_UNDERWATER || SECT(IN_ROOM(ch)) == SECT_SECRET)
	{
		send_to_char("� ���� ������� ������ ������� ������.\r\n", ch);
		return;
	}

	if (!check_moves(ch, FIRE_MOVES))
		return;

	percent = number(1, skill_info[SKILL_FIRE].max_percent);
	prob = calculate_skill(ch, SKILL_FIRE, skill_info[SKILL_FIRE].max_percent, 0);
	if (percent > prob)
	{
		send_to_char("�� ���������� ������� ������, �� � ��� ������ �� �����.\r\n", ch);
		return;
	}
	else
	{
		world[IN_ROOM(ch)]->fires = MAX(0, (prob - percent) / 5) + 1;
		send_to_char("�� ������� �������� � �������� �����.\n\r", ch);
		act("$n ������$g �����.", FALSE, ch, 0, 0, TO_ROOM);
		improove_skill(ch, SKILL_FIRE, TRUE, 0);
	}
}

ACMD(do_extinguish)
{
    AFFECT_DATA *aff;
    CHAR_DATA *caster;
    int tp, lag = 0;
    const char *targets[] = { "������",
                                                "�����",
                                                "�����",
                                                "fire",
                                                "�����",
                                                "�������",
                                                "����",
                                                "label",
                                                "\n"
                                                };

    if (IS_NPC(ch))
        return;

	one_argument(argument, arg);

	if ((!*arg) || ((tp = search_block(arg, targets, FALSE)) == -1))
	{
		send_to_char("��� �� ������ ���������?\r\n", ch);
		return;
	}
    tp >>= 2;

    switch (tp)
    {
    case 0:
        if (world[IN_ROOM(ch)]->fires)
        {
            world[IN_ROOM(ch)]->fires = 0;
            send_to_char("�� ��������� ������.\r\n", ch);
            act("$n ��������$g ������.", FALSE, ch, 0, 0, TO_ROOM);
            lag = 2;
        }
        else
        {
            send_to_char("� ��� ������� � ������ :)\r\n", ch);
        }
        break;
    case 1:
        if ((aff = room_affected_by_spell(world[IN_ROOM(ch)], SPELL_RUNE_LABEL))
            && (AFF_FLAGGED(ch, AFF_DETECT_MAGIC) || IS_IMMORTAL(ch) || PRF_FLAGGED(ch, PRF_CODERINFO)))
        {
            affect_room_remove(world[IN_ROOM(ch)], aff);
            send_to_char("������� ��������� ��� �� �����, �� ������ ���������� �������.\r\n", ch);
            act("$n �������$g ��������� ��� �� ���������� �����, ��������� �� ���������.", FALSE, ch, 0, 0, TO_ROOM);
            if (GET_ID(ch) != aff->caster_id) //��� ������� �� ���� ����� - ���, ��������
            {
                //���� ������� �� ����
                caster = find_char(aff->caster_id);
                //���� ������ ������ - ������ ������� �� ��� �� ���������
                if (caster)
                {
                    pk_thiefs_action(ch, caster);
                    sprintf(buf, "���������� ������� ���� ��������� ������, � ����� ���� ���������� ���������� ����� %s.\r\n", GET_PAD(ch,1));
                    send_to_char(buf, caster);
                }
            }
            lag = 3;
        }
        else
        {
            send_to_char("� ��� ������� � ������ :)\r\n", ch);
        }
        break;
    }
    //�������-�� ��� �� ��� ����.
    if (!WAITLESS(ch))
            WAIT_STATE(ch, lag * PULSE_VIOLENCE);
}

#define MAX_REMOVE  12
const int RemoveSpell[MAX_REMOVE] = { SPELL_SLEEP, SPELL_POISON, SPELL_WEAKNESS, SPELL_CURSE, SPELL_PLAQUE,
									  SPELL_SIELENCE, SPELL_BLINDNESS, SPELL_HAEMORRAGIA, SPELL_HOLD, SPELL_PEACEFUL, SPELL_CONE_OF_COLD,
									  SPELL_DEAFNESS
									};

ACMD(do_firstaid)
{
	int percent, prob, success = FALSE, need = FALSE, count, spellnum = 0;
	struct timed_type timed;
	CHAR_DATA *vict;

	if (!ch->get_skill(SKILL_AID))
	{
		send_to_char("��� ������� ����� ���������.\r\n", ch);
		return;
	}
	if (!IS_GOD(ch) && timed_by_skill(ch, SKILL_AID))
	{
		send_to_char("��� ����� ������ ������ - ������� �� ���������.\r\n", ch);
		return;
	}

	one_argument(argument, arg);

	if (!*arg)
		vict = ch;
	else if (!(vict = get_char_vis(ch, arg, FIND_CHAR_ROOM)))
	{
		send_to_char("���� �� ������ ��������� ?\r\n", ch);
		return;
	};

	if (vict->get_fighting())
	{
		act("$N ���������, $M �� �� ����� �������� ���������.", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}

	percent = number(1, skill_info[SKILL_AID].max_percent);
	prob = calculate_skill(ch, SKILL_AID, skill_info[SKILL_AID].max_percent, vict);

	if (IS_IMMORTAL(ch) || GET_GOD_FLAG(ch, GF_GODSLIKE) || GET_GOD_FLAG(vict, GF_GODSLIKE))
		percent = prob;
	if (GET_GOD_FLAG(ch, GF_GODSCURSE) || GET_GOD_FLAG(vict, GF_GODSCURSE))
		prob = 0;
	success = (prob >= percent);
	need = FALSE;

	if ((GET_REAL_MAX_HIT(vict) > 0 && (GET_HIT(vict) * 100 / GET_REAL_MAX_HIT(vict)) < 31)
			|| (GET_REAL_MAX_HIT(vict) <= 0 && GET_HIT(vict) < GET_REAL_MAX_HIT(vict))
			|| (GET_HIT(vict) < GET_REAL_MAX_HIT(vict) && can_use_feat(ch, HEALER_FEAT)))
	{
		need = TRUE;
		if (success)
		{
			int dif = GET_REAL_MAX_HIT(vict) - GET_HIT(vict);
			int add = MIN(dif, (dif * (prob - percent) / 100) + 1);
			GET_HIT(vict) += add;
			update_pos(vict);
		}
	}
	count = MIN(MAX_REMOVE, MAX_REMOVE * prob / 100);

	for (percent = 0, prob = need; !need && percent < MAX_REMOVE && RemoveSpell[percent]; percent++)
		if (affected_by_spell(vict, RemoveSpell[percent]))
		{
			need = TRUE;
			if (percent < count)
			{
				spellnum = RemoveSpell[percent];
				prob = TRUE;
			}
		}


	if (!need)
		act("$N � ������� �� ���������.", FALSE, ch, 0, vict, TO_CHAR);
	else if (!prob)
		act("� ��� �� ������ ������ �������� $N3.", FALSE, ch, 0, vict, TO_CHAR);
	else  			//improove_skill(ch, SKILL_AID, TRUE, 0);
	{
		timed.skill = SKILL_AID;
		timed.time = IS_IMMORTAL(ch) ? 2 : IS_PALADINE(ch) ? 4 : IS_CLERIC(ch) ? 2 : 6;
		timed_to_char(ch, &timed);
		if (vict != ch)
		{
			improove_skill(ch, SKILL_AID, success, 0);
			if (success)
			{
				act("�� ������� ������ ������ $N2.", FALSE, ch, 0, vict, TO_CHAR);
				act("$N ������$G ��� ������ ������.", FALSE, vict, 0, ch, TO_CHAR);
				act("$n ������$g ������ ������ $N2.", TRUE, ch, 0, vict, TO_NOTVICT);
				if (spellnum)
					affect_from_char(vict, spellnum);
			}
			else
			{
				act("�� �������������� ���������� ������� ������ ������ $N2.",
					FALSE, ch, 0, vict, TO_CHAR);
				act("$N �������������� �������$U ������� ��� ������ ������.",
					FALSE, vict, 0, ch, TO_CHAR);
				act("$n �������������� �������$u ������� ������ ������ $N2.",
					TRUE, ch, 0, vict, TO_NOTVICT);
			}
		}
		else
		{
			if (success)
			{
				act("�� ������� ���� ������ ������.", FALSE, ch, 0, 0, TO_CHAR);
				act("$n ������$g ���� ������ ������.", FALSE, ch, 0, 0, TO_ROOM);
				if (spellnum)
					affect_from_char(vict, spellnum);
			}
			else
			{
				act("�� �������������� ���������� ������� ���� ������ ������.",
					FALSE, ch, 0, vict, TO_CHAR);
				act("$n �������������� �������$u ������� ���� ������ ������.",
					FALSE, ch, 0, vict, TO_ROOM);
			}
		}
	}
}

ACMD(do_poisoned)
{
	if (!ch->get_skill(SKILL_POISONED))
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	}

	argument = one_argument(argument, arg);
	skip_spaces(&argument);

	if (!*arg)
	{
		send_to_char("��� �� ������ ��������?\r\n", ch);
		return;
	}
	else if (!*argument)
	{
		send_to_char("�� ���� �� ��������� ����� ��?\r\n", ch);
		return;
	}

	OBJ_DATA *weapon = 0;
	CHAR_DATA *dummy = 0;
	int result = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_EQUIP, ch, &dummy, &weapon);

	if (!weapon || !result)
	{
		send_to_char(ch, "� ��� ��� \'%s\'.\r\n", arg);
		return;
	}
	else if (GET_OBJ_TYPE(weapon) != ITEM_WEAPON)
	{
		send_to_char("�� ������ ������� �� ������ �� ������.\r\n", ch);
		return;
	}

	OBJ_DATA *cont = get_obj_in_list_vis(ch, argument, ch->carrying);
	if (!cont)
	{
		send_to_char(ch, "� ��� ��� \'%s\'.\r\n", argument);
		return;
	}
	else if (GET_OBJ_TYPE(cont) != ITEM_DRINKCON)
	{
		send_to_char(ch, "%s �� �������� ��������.\r\n", GET_OBJ_PNAME(cont, 0));
		return;
	}
	else if (GET_OBJ_VAL(cont, 1) <= 0)
	{
		send_to_char(ch, "� %s ��� ������� ��������.\r\n", GET_OBJ_PNAME(cont, 5));
		return;
	}
	else if (!poison_in_vessel(GET_OBJ_VAL(cont, 2)))
	{
		send_to_char(ch, "� %s ��� ����������� ���.\r\n", GET_OBJ_PNAME(cont, 5));
		return;
	}

	int cost = MIN(GET_OBJ_VAL(cont, 1), GET_LEVEL(ch) <= 10 ? 1 : GET_LEVEL(ch) <= 20 ? 2 : 3);
	GET_OBJ_VAL(cont, 1) -= cost;
	weight_change_object(cont, -cost);
	if (!GET_OBJ_VAL(cont, 1))
		name_from_drinkcon(cont);

	set_weap_poison(weapon, GET_OBJ_VAL(cont, 2));

	snprintf(buf, sizeof(buf), "�� ��������� ������� ������� %s �� $o3.", drinks[GET_OBJ_VAL(cont, 2)]);
	act(buf, FALSE, ch, weapon, 0, TO_CHAR);
	act("$n ��������� �����$q �� �� $o3.", FALSE, ch, weapon, 0, TO_ROOM);
}

ACMD(do_repair)
{
	OBJ_DATA *obj;
	int prob, percent = 0, decay;

	if (!ch->get_skill(SKILL_REPAIR))
	{
		send_to_char("�� �� ������ �����.\r\n", ch);
		return;
	}

	one_argument(argument, arg);

	if (ch->get_fighting())
	{
		send_to_char("�� �� ������ ������� ��� � ��� !\r\n", ch);
		return;
	}

	if (!*arg)
	{
		send_to_char("��� �� ������ ������������� ?\r\n", ch);
		return;
	}

	if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� \'%s\'.\r\n", arg);
		send_to_char(buf, ch);
		return;
	};

	if (GET_OBJ_MAX(obj) <= GET_OBJ_CUR(obj))
	{
		act("$o � ������� �� ���������.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}
	if ((GET_OBJ_TYPE(obj) != ITEM_WEAPON) && !ObjSystem::is_armor_type(obj))
	{
		send_to_char("�� ������ ��������������� ������ ������ ��� �����.\r\n", ch);
		return;
	}

	prob = number(1, skill_info[SKILL_REPAIR].max_percent);
	percent = train_skill(ch, SKILL_REPAIR, skill_info[SKILL_REPAIR].max_percent, 0);
	if (prob > percent)
	{
//Polos.repair_bug
//������ ��� 0 ���������� ������ ��������� ���� ��� ����� 100+ �
//��������� ������ <����� ������>
		if (!percent) percent = ch->get_skill(SKILL_REPAIR) / 10;
//-Polos.repair_bug
		GET_OBJ_CUR(obj) = MAX(0, GET_OBJ_CUR(obj) * percent / prob);
		if (obj->obj_flags.Obj_cur)
		{
			act("�� ���������� �������� $o3, �� ������� $S ��� ������.", FALSE, ch, obj, 0, TO_CHAR);
			act("$n �������$u �������� $o3, �� ������$g $S ��� ������.", FALSE, ch, obj, 0, TO_ROOM);
			decay = (GET_OBJ_MAX(obj) - GET_OBJ_CUR(obj)) / 10;
			decay = MAX(1, MIN(decay, GET_OBJ_MAX(obj) / 20));
			if (GET_OBJ_MAX(obj) > decay)
				GET_OBJ_MAX(obj) -= decay;
			else
				GET_OBJ_MAX(obj) = 1;
		}
		else
		{
			act("�� ������������ �������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
			act("$n ������������ �������$g $o3.", FALSE, ch, obj, 0, TO_ROOM);
			extract_obj(obj);
		}
	}
	else
	{
		// �������. � ����� ����������� ��� ���������
		if (!IS_IMMORTAL(ch) && !ROOM_FLAGGED(IN_ROOM(ch), ROOM_SMITH))
		{
			GET_OBJ_MAX(obj) -= MAX(1, (GET_OBJ_MAX(obj) - GET_OBJ_CUR(obj)) / 40);
		}
		GET_OBJ_CUR(obj) = MIN(GET_OBJ_MAX(obj), GET_OBJ_CUR(obj) * percent / prob + 1);
		send_to_char(ch, "������ %s ������%s �����.\r\n", GET_OBJ_PNAME(obj, 0), GET_OBJ_POLY_1(ch, obj));
		act("$n ����� �������$g $o3.", FALSE, ch, obj, 0, TO_ROOM);
	}
}

const int meet_vnum[] = { 320, 321, 322, 323 };

ACMD(do_makefood)
{
	OBJ_DATA *obj, *tobj, *skin;
	CHAR_DATA *mob;
	int prob, percent = 0, mobn, wgt = 0;

//	send_to_char("�������� �� ��������.\r\n",ch);
//	return;

	if (!ch->get_skill(SKILL_MAKEFOOD))
	{
		send_to_char("�� �� ������ �����.\r\n", ch);
		return;
	}

	one_argument(argument, arg);
	if (!*arg)
	{
		send_to_char("��� �� ������ ���������� ?\r\n", ch);
		return;
	}

	if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)) &&
			!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room]->contents)))
	{
		sprintf(buf, "�� �� ������ ����� '%s'.\r\n", arg);
		send_to_char(buf, ch);
		return;
	}

	if (!IS_CORPSE(obj) || (mobn = GET_OBJ_VAL(obj, 2)) < 0)
	{
		act("�� �� ������� ���������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}
	mob = (mob_proto + real_mobile(mobn));
	mob->set_normal_morph();

	if (!IS_IMMORTAL(ch) && (GET_RACE(mob) != NPC_RACE_ANIMAL || (wgt = GET_WEIGHT(mob)) < 11))
	{
		send_to_char("���� ���� ���������� ����������.\r\n", ch);
		return;
	}
	prob = number(1, skill_info[SKILL_MAKEFOOD].max_percent);
	percent =
		train_skill(ch, SKILL_MAKEFOOD, skill_info[SKILL_MAKEFOOD].max_percent,
					mob) + number(1, GET_REAL_DEX(ch)) + number(1, GET_REAL_STR(ch));
	if (prob > percent || !(tobj = read_object(meet_vnum[number(0, 3)], VIRTUAL)))
	{
		act("�� �� ������ ���������� $o3.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n �������$u ���������� $o3, �� ��������.", FALSE, ch, obj, 0, TO_ROOM);
	}
	else
	{
		//sprintf (buf, "$n ����� �������$g %s �� $o1.", tobj->PNames[3]);
		act("$n ����� ���������$g $o3.", FALSE, ch, obj, 0, TO_ROOM);
		//sprintf (buf, "�� ����� �������� %s �� $o1.", tobj->PNames[3]);
		act("�� ����� ���������� $o3.", FALSE, ch, obj, 0, TO_CHAR);

		dl_load_obj(obj, mob, ch, DL_SKIN);
		if (number(1, ch->get_skill(SKILL_MAKEFOOD)) + number(1, GET_REAL_DEX(ch)) >= prob)
		{
			skin = create_skin(mob, ch);
			if (skin != NULL)
			{
				if (obj->carried_by == ch)
					can_carry_obj(ch, skin);
				else
					obj_to_room(skin, IN_ROOM(ch));
			}
		}
		if (obj->carried_by == ch)
			can_carry_obj(ch, tobj);
		else
			obj_to_room(tobj, IN_ROOM(ch));

//  obj_decay(tobj);
	}
//�����-�� ���� ����������� � ������� ����� ������������.
	if (obj->carried_by)
	{
		obj_from_char(obj);
		obj_to_room(obj, IN_ROOM(ch));
	}
	extract_obj(obj);
}

void feed_charmice(CHAR_DATA * ch, char *arg)
{
	OBJ_DATA *obj;
	AFFECT_DATA af;
	int max_charm_duration = 1;
	int chance_to_eat = 0;
	struct follow_type *k;
	int reformed_hp_summ = 0;

	obj = get_obj_in_list_vis(ch, arg, world[ch->in_room]->contents);

	if (!obj || !IS_CORPSE(obj) || !ch->master)
	{
		return;
	}

	for (k = ch->master->followers; k; k = k->next)
	{
		if (AFF_FLAGGED(k->follower, AFF_CHARM)
				&& k->follower->master == ch->master)
		{
			reformed_hp_summ += get_reformed_charmice_hp(ch->master, k->follower, SPELL_ANIMATE_DEAD);
		}
	}

	if (reformed_hp_summ >= get_player_charms(ch->master, SPELL_ANIMATE_DEAD))
	{
		send_to_char("�� �� ������ ��������� ��������� ���������������.\r\n", ch->master);
		extract_char(ch, FALSE);
		return;
	}

	int mob_level = 1;
	// ���� �� ������
	if (GET_OBJ_VAL(obj, 2) != -1)
	{
		mob_level = GET_LEVEL(mob_proto + real_mobile(GET_OBJ_VAL(obj, 2)));
	}
	const int max_heal_hp = 3 * mob_level;
	chance_to_eat = (100 - 2 * mob_level) / 2;
	//Added by Ann
	if (affected_by_spell(ch->master, SPELL_FASCINATION))
	{
		chance_to_eat -= 30;
	}
	//end Ann
	if (number(1, 100) < chance_to_eat)
	{
		act("$N �������$U � �����$G ������ �������.", TRUE, ch, NULL, ch, TO_ROOM);
		GET_HIT(ch) -= 3 * mob_level;
		update_pos(ch);
		// ��������� ��������.
		if (GET_POS(ch) == POS_DEAD)
		{
			die(ch, NULL);
		}
		extract_obj(obj);
		return;
	}
	if (weather_info.moon_day < 14)
	{
		max_charm_duration =
			pc_duration(ch, GET_REAL_WIS(ch->master) - 6 + number(0, weather_info.moon_day % 14), 0, 0, 0, 0);
	}
	else
	{
		max_charm_duration =
			pc_duration(ch,
						GET_REAL_WIS(ch->master) - 6 + number(0, 14 - weather_info.moon_day % 14), 0, 0, 0, 0);
	}

	af.type = SPELL_CHARM;
	af.duration = MIN(max_charm_duration, (int)(mob_level * max_charm_duration / 30));
	af.modifier = 0;
	af.location = 0;
	af.bitvector = AFF_CHARM;
	af.battleflag = 0;

	affect_join_fspell(ch, &af);

	act("������ ������, $N ������$G ����.", TRUE, ch, obj, ch, TO_ROOM);
	act("������, ��������� �������� �� �����.", TRUE, ch, NULL, ch->master, TO_VICT);
	act("�� �������������� ������� ��� ���� �� ���������.", TRUE, ch, NULL, ch->master, TO_NOTVICT);

	if (GET_HIT(ch) < GET_MAX_HIT(ch))
	{
		GET_HIT(ch) = MIN(GET_HIT(ch) + MIN(max_heal_hp, GET_MAX_HIT(ch)), GET_MAX_HIT(ch));
	}

	if (GET_HIT(ch) >= GET_MAX_HIT(ch))
	{
		act("$n ���� ������$g � ���������� ���������$g �� ���.", TRUE, ch, NULL, ch->master, TO_VICT);
		act("$n ���� ������$g � ���������� ���������$g �� $N3.", TRUE, ch, NULL, ch->master, TO_NOTVICT);
	}

	extract_obj(obj);
}
