/*************************************************************************
*   File: liquid.cpp                                   Part of Bylins    *
*   ��� �� ���������                                                     *
*                                                                        *
*  $Author$                                                      *
*  $Date$                                          *
*  $Revision$                                                     *
************************************************************************ */

#include "liquid.hpp"
#include "char.hpp"
#include "db.h"
#include "comm.h"
#include "handler.h"
#include "spells.h"
#include "skills.h"
#include "features.hpp"
#include "room.hpp"

extern void weight_change_object(OBJ_DATA * obj, int weight);

// ���� ����������, ���������� � ����������
const int LIQ_WATER = 0;
const int LIQ_BEER = 1;
const int LIQ_WINE = 2;
const int LIQ_ALE = 3;
const int LIQ_QUAS = 4;
const int LIQ_BRANDY = 5;
const int LIQ_MORSE = 6;
const int LIQ_VODKA = 7;
const int LIQ_BRAGA = 8;
const int LIQ_MED = 9;
const int LIQ_MILK = 10;
const int LIQ_TEA = 11;
const int LIQ_COFFE = 12;
const int LIQ_BLOOD = 13;
const int LIQ_SALTWATER = 14;
const int LIQ_CLEARWATER = 15;
const int LIQ_POTION = 16;
const int LIQ_POTION_RED = 17;
const int LIQ_POTION_BLUE = 18;
const int LIQ_POTION_WHITE = 19;
const int LIQ_POTION_GOLD = 20;
const int LIQ_POTION_BLACK = 21;
const int LIQ_POTION_GREY = 22;
const int LIQ_POTION_FUCHSIA = 23;
const int LIQ_POISON_ACONITUM = 24;
const int LIQ_POISON_SCOPOLIA = 25;
const int LIQ_POISON_BELENA = 26;
const int LIQ_POISON_DATURA = 27;
// ����������
const int NUM_LIQ_TYPES = 28;

/* LIQ_x */
const char *drinks[] = { "����",
						 "����",
						 "����",
						 "��������",
						 "�����",
						 "��������",
						 "�����",
						 "�����",
						 "�����",
						 "����",
						 "������",
						 "���",
						 "�����",
						 "�����",
						 "������� ����",
						 "���������� ����",
						 "����������� �����",
						 "�������� ����������� �����",
						 "������ ����������� �����",
						 "������ ����������� �����",
						 "����������� ����������� �����",
						 "������� ����������� �����",
						 "������ ����������� �����",
						 "����������� ����������� �����",
						 "�������� �������",
						 "�������� ��������",
						 "�������� ������",
						 "�������� �������",
						 "\n"
					   };

/* one-word alias for each drink */
const char *drinknames[] = { "�����",
							 "�����",
							 "�����",
							 "���������",
							 "������",
							 "���������",
							 "������",
							 "������",
							 "������",
							 "�����",
							 "�������",
							 "����",
							 "����",
							 "������",
							 "������� �����",
							 "���������� �����",
							 "���������� ������",
							 "������� ���������� ������",
							 "����� ���������� ������",
							 "����� ���������� ������",
							 "���������� ���������� ������",
							 "������ ���������� ������",
							 "����� ���������� ������",
							 "���������� ���������� ������",
							 "��������� �������",
							 "��������� ��������",
							 "��������� ������",
							 "��������� �������",
							 "\n"
						   };

/* effect of drinks on DRUNK, FULL, THIRST -- see values.doc */
const int drink_aff[][3] = { {0, 1, 10},	// ����
	{2, 2, 3},			// ����
	{5, 2, 2},			// ����
	{3, 2, 3},			// ��������
	{1, 2, 5},			// ����
	{8, 1, 1},			// �������
	{0, 1, 8},			// ����
	{10, 0, 1},			// �����
	{3, 3, 3},			// �����
	{0, 4, -8},			// ���
	{0, 3, 6},			// ������
	{0, 1, 6},			// ���
	{0, 1, 6},			// ����
	{0, 2, -1},			// �����
	{0, 1, -2},			// ������� ����
	{0, 0, 13},			// ���������� ����
	{0, 1, -1},			// ���������� �����
	{0, 1, -1},			// ������� ���������� �����
	{0, 1, -1},			// ����� ���������� �����
	{0, 1, -1},			// ����� ���������� �����
	{0, 1, -1},			// �������������������� �����
	{0, 1, -1},			// ���������������� �����
	{0, 1, -1},			// ����� ���������� �����
	{0, 1, -1},			// ���������� ���������� �����
	{0, 0, 0},			// �������� �������
	{0, 0, 0},			// �������� ��������
	{0, 0, 0},			// �������� ������
	{0, 0, 0}			// �������� �������
};

/* color of the various drinks */
const char *color_liquid[] = { "����������",
							   "����������",
							   "��������",
							   "����������",
							   "�����",
							   "����������",
							   "���������",
							   "�������",
							   "������",
							   "������ �������",
							   "�����",
							   "����������",
							   "�����-����������",
							   "�������",
							   "����������",
							   "���������� ������",
							   "����������",
							   "��������",
							   "��������",
							   "�����������",
							   "����������",
							   "������ ������",
							   "����������",
							   "����������",
							   "��������",
							   "��������",
							   "��������",
							   "��������",
							   "\n"
							 };

/**
* �����, ��������� � ����������, ����� ���� �� ����� ���.
* �� ������, ����� �������� ��������� ��� �������, �������
* ��� ����� ���� �� ������ �� �������.
*/
bool is_potion(OBJ_DATA *obj)
{
	switch(GET_OBJ_VAL(obj, 2))
	{
	case LIQ_POTION:
	case LIQ_POTION_RED:
	case LIQ_POTION_BLUE:
	case LIQ_POTION_WHITE:
	case LIQ_POTION_GOLD:
	case LIQ_POTION_BLACK:
	case LIQ_POTION_GREY:
	case LIQ_POTION_FUCHSIA:
		return true;
		break;
	}
	return false;
}

ACMD(do_drink)
{
	OBJ_DATA *temp;
	AFFECT_DATA af;
	OBJ_DATA *obj_potion;
	int amount, weight, duration, i, level;
	int on_ground = 0;

	one_argument(argument, arg);

	if (IS_NPC(ch))		/* Cannot use GET_COND() on mobs. */
		return;

	if (!*arg)
	{
		send_to_char("���� �� ���� ?\r\n", ch);
		return;
	}

	if (!(temp = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		if (!(temp = get_obj_in_list_vis(ch, arg, world[ch->in_room]->contents)))
		{
			send_to_char("�� �� ������ ��� �����!\r\n", ch);
			return;
		}
		else
			on_ground = 1;
	}
	if (IS_SET(PRF_FLAGS(ch, PRF_IRON_WIND), PRF_IRON_WIND))
	{
		send_to_char("�� ����� ����������� � ���!\r\n", ch);
		return;
	}
	if ((GET_OBJ_TYPE(temp) != ITEM_DRINKCON) && (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN))
	{
		send_to_char("�� �����. ������ � ��� ����� !\r\n", ch);
		return;
	}
	if (on_ground && (GET_OBJ_TYPE(temp) == ITEM_DRINKCON))
	{
		send_to_char("������ ��� ����� �������.\r\n", ch);
		return;
	}

	if (!GET_OBJ_VAL(temp, 1))
	{
		send_to_char("�����.\r\n", ch);
		return;
	}
	// Added by Adept - ������ ���� � ������� ��� ������� �����
	if (is_potion(temp))
	{
		act("$n �����$g ����� �� $o1.", TRUE, ch, temp, 0, TO_ROOM);
		sprintf(buf, "�� ������ ����� �� %s.\r\n", OBJN(temp, ch, 1));
		send_to_char(buf, ch);
		obj_potion = read_object(GET_OBJ_SKILL(temp), VIRTUAL);
		if (obj_potion == NULL)
		{
			sprintf(buf,
					"ERROR: ������� ����������� ���������� �� ��������������� ����� � �������� (VNUM:%d).",
					GET_OBJ_VNUM(temp));
			mudlog(buf, NRM, LVL_BUILDER, ERRLOG, TRUE);
			return;
		}
		if (GET_OBJ_TYPE(obj_potion) != ITEM_POTION)
		{
			sprintf(buf, "ERROR: �������� vnum ����� � ������� (VNUM:%d).", GET_OBJ_VNUM(temp));
			mudlog(buf, NRM, LVL_BUILDER, ERRLOG, TRUE);
		}
		else
		{
			//���� ���� ������ � ����� ����� ���� - ������ �� ���� ������ � ����������
			WAIT_STATE(ch, PULSE_VIOLENCE);
			level = GET_OBJ_VAL(obj_potion, 0);
			for (i = 1; i <= 3; i++)
				if (call_magic(ch, ch, NULL, world[IN_ROOM(ch)], GET_OBJ_VAL(obj_potion, i), level, CAST_POTION) <= 0)
					break;
			//���� ��� ����� ������ - �������� ���� �����-���������
			if (--GET_OBJ_VAL(temp, 1) <= 0 && GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN)
			{
				name_from_drinkcon(temp);
				GET_OBJ_SKILL(temp) = 0;
			}
			GET_OBJ_WEIGHT(temp)--;
		}
		extract_obj(obj_potion);
		return;
	}
	else if (ch->get_fighting())
	{
		send_to_char("�� ����� ����������� � ���.\r\n", ch);
		return;
	}
	//����� ��������� Adept'��

//	if ((GET_COND(ch, THIRST) > 0) && AFF_FLAGGED(ch, AFF_DRUNKED))  	// The pig is drunk
//	{
//		send_to_char("�� �� ������ ������� � ������.\r\n", ch);
//		act("$n �������$u ������ ���, �� �� ����$q ������� � ������.", TRUE, ch, 0, 0, TO_ROOM);
//		return;
//	}

	if (subcmd == SCMD_DRINK)
	{
		if (drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] > 0)
			if (GET_DRUNK_STATE(ch)<MAX_COND_VALUE && !AFF_FLAGGED(ch, AFF_ABSTINENT))
				amount = 24 / drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK];
			else
				amount = 0;
		else
			amount = number(3, 10);
	}
	else
	{
		amount = 1;
	}

	amount = MIN(amount, GET_OBJ_VAL(temp, 1));
	amount = MIN(amount, 24 - GET_COND(ch, THIRST));

	if (amount <= 0)
	{
		send_to_char("� ��� ������ �� �����.\r\n", ch);
		return;
	}
	else if (subcmd == SCMD_DRINK)
	{
		sprintf(buf, "$n �����$g %s �� $o1.", drinks[GET_OBJ_VAL(temp, 2)]);
		act(buf, TRUE, ch, temp, 0, TO_ROOM);
		sprintf(buf, "�� ������ %s �� %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)], OBJN(temp, ch, 1));
		send_to_char(buf, ch);
	}
	else
	{
		act("$n ���������$g �� $o1.", TRUE, ch, temp, 0, TO_ROOM);
		sprintf(buf, "�� ������ ���� %s.\r\n", drinks[GET_OBJ_VAL(temp, 2)]);
		send_to_char(buf, ch);
	}

	/* You can't subtract more than the object weighs */
	weight = MIN(amount, GET_OBJ_WEIGHT(temp));

	if (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN)
		weight_change_object(temp, -weight);	/* Subtract amount */

	
	if ((GET_DRUNK_STATE(ch) < MAX_COND_VALUE && GET_DRUNK_STATE(ch) == GET_COND(ch, DRUNK)) 
		|| (GET_COND(ch, DRUNK) < CHAR_DRUNKED && !AFF_FLAGGED(ch, AFF_ABSTINENT)))
	{
		gain_condition(ch, DRUNK, (int)((int) drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] * amount) / 4);
		GET_DRUNK_STATE(ch) = MAX(GET_DRUNK_STATE(ch), GET_COND(ch, DRUNK));
	}

	gain_condition(ch, FULL, (int)((int) drink_aff[GET_OBJ_VAL(temp, 2)][FULL] * amount) / 4);

	gain_condition(ch, THIRST, (int)((int) drink_aff[GET_OBJ_VAL(temp, 2)][THIRST] * amount) / 4);

	if (GET_COND(ch, THIRST) > 20)
		send_to_char("�� �� ���������� �����.\r\n", ch);

	if (GET_COND(ch, FULL) > 20)
		send_to_char("�� ���������� �������� ������� � �������.\r\n", ch);

	if (GET_COND(ch, DRUNK) >= CHAR_DRUNKED)
	{
		if (GET_DRUNK_STATE(ch) == MAX_COND_VALUE || GET_COND(ch, DRUNK) < GET_DRUNK_STATE(ch))
		{
			send_to_char("�� ������� ��� ����������, ������ ��� �������...\r\n", ch);
		}else
			if (GET_COND(ch, DRUNK) >= CHAR_MORTALLY_DRUNKED)
			{
				send_to_char("�������� �� �����, �� ����� ��� �� ����....\r\n", ch);
			}
			else
			{
				send_to_char("�������� ����� ��������� �� ������ ����.\r\n", ch);
			}
		duration = 2 + MAX(0, GET_COND(ch, DRUNK) - CHAR_DRUNKED);
		if (can_use_feat(ch, DRUNKARD_FEAT))
			duration += duration/2;
		if (!AFF_FLAGGED(ch, AFF_ABSTINENT) 
				&& GET_DRUNK_STATE(ch) < MAX_COND_VALUE 
					&& GET_DRUNK_STATE(ch) == GET_COND(ch, DRUNK))
		{
			send_to_char("������ ���� ������� ��� � ������.\r\n", ch);
			/***** Decrease AC ******/
			af.type = SPELL_DRUNKED;
			af.duration = pc_duration(ch, duration, 0, 0, 0, 0);
			af.modifier = -20;
			af.location = APPLY_AC;
			af.bitvector = AFF_DRUNKED;
			af.battleflag = 0;
			affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
			/***** Decrease HR ******/
			af.type = SPELL_DRUNKED;
			af.duration = pc_duration(ch, duration, 0, 0, 0, 0);
			af.modifier = -2;
			af.location = APPLY_HITROLL;
			af.bitvector = AFF_DRUNKED;
			af.battleflag = 0;
			affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
			/***** Increase DR ******/
			af.type = SPELL_DRUNKED;
			af.duration = pc_duration(ch, duration, 0, 0, 0, 0);
			af.modifier = (GET_LEVEL(ch) + 4) / 5;
			af.location = APPLY_DAMROLL;
			af.bitvector = AFF_DRUNKED;
			af.battleflag = 0;
			affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
		}
	}

	if (GET_OBJ_VAL(temp, 3) && !IS_GOD(ch))  	/* The shit was poisoned ! */
	{
		send_to_char("���-�� ���� �����-�� �������� !\r\n", ch);
		act("$n ���������$u � ��������$g.", TRUE, ch, 0, 0, TO_ROOM);

		af.type = SPELL_POISON;
		af.duration = pc_duration(ch, amount == 1 ? amount : amount * 3, 0, 0, 0, 0);
		af.modifier = -2;
		af.location = APPLY_STR;
		af.bitvector = AFF_POISON;
		af.battleflag = AF_SAME_TIME;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
		af.type = SPELL_POISON;
		af.modifier = amount * 3;
		af.location = APPLY_POISON;
		af.bitvector = AFF_POISON;
		af.battleflag = AF_SAME_TIME;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
		ch->Poisoner = 0;
	}

	/* empty the container, and no longer poison. 999 - whole fountain */
	if (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN || GET_OBJ_VAL(temp, 1) != 999)
		GET_OBJ_VAL(temp, 1) -= amount;
	if (!GET_OBJ_VAL(temp, 1))  	/* The last bit */
	{
		GET_OBJ_VAL(temp, 2) = 0;
		GET_OBJ_VAL(temp, 3) = 0;
		name_from_drinkcon(temp);
	}

	return;
}

ACMD(do_drunkoff)
{
	OBJ_DATA *obj;
	AFFECT_DATA af[3];
	struct timed_type timed;
	int amount, weight, prob, percent, duration;
	int on_ground = 0;

	if (IS_NPC(ch))		/* Cannot use GET_COND() on mobs. */
		return;

	if (ch->get_fighting())
	{
		send_to_char("�� ����� ����������� � ���.\r\n", ch);
		return;
	}

	if (AFF_FLAGGED(ch, AFF_DRUNKED))
	{
		send_to_char("�� ������ ��������� ���� ���� ����� ?\r\n" "��� �� ���� �� ������ !\r\n", ch);
		return;
	}

	if (!AFF_FLAGGED(ch, AFF_ABSTINENT) && GET_COND(ch, DRUNK) < CHAR_DRUNKED)
	{
		send_to_char("�� ����� ������ ����� �� ������� ������.\r\n", ch);
		return;
	}

	if (timed_by_skill(ch, SKILL_DRUNKOFF))
	{
		send_to_char("�� �� � ��������� ��� ����� �����������.\r\n"
					 "��������� ����� ������������ ���.\r\n", ch);
		return;
	}

	one_argument(argument, arg);

	if (!*arg)
	{
		for (obj = ch->carrying; obj; obj = obj->next_content)
			if (GET_OBJ_TYPE(obj) == ITEM_DRINKCON)
				break;
		if (!obj)
		{
			send_to_char("� ��� ��� ����������� ������� ��� ��������.\r\n", ch);
			return;
		}
	}
	else if (!(obj = get_obj_in_list_vis(ch, arg, ch->carrying)))
	{
		if (!(obj = get_obj_in_list_vis(ch, arg, world[ch->in_room]->contents)))
		{
			send_to_char("�� �� ������ ��� �����!\r\n", ch);
			return;
		}
		else
			on_ground = 1;
	}

	if ((GET_OBJ_TYPE(obj) != ITEM_DRINKCON) && (GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN))
	{
		send_to_char("���� �� ����-�� ������� �����������.\r\n", ch);
		return;
	}

	if (on_ground && (GET_OBJ_TYPE(obj) == ITEM_DRINKCON))
	{
		send_to_char("������ ��� ����� �������.\r\n", ch);
		return;
	}

	if (!GET_OBJ_VAL(obj, 1))
	{
		send_to_char("�����.\r\n", ch);
		return;
	}

	switch (GET_OBJ_VAL(obj, 2))
	{
	case LIQ_BEER:
	case LIQ_WINE:
	case LIQ_ALE:
	case LIQ_QUAS:
	case LIQ_BRANDY:
	case LIQ_VODKA:
	case LIQ_BRAGA:
		break;
	default:
		send_to_char("��������� �������� �������� :\r\n" "\"���� �������� ������...\"\r\n", ch);
		return;
	}

	amount = MAX(1, GET_WEIGHT(ch) / 50);
	if (amount > GET_OBJ_VAL(obj, 1))
	{
		send_to_char("��� ����� �� ������ ����� ���������� ��� ��������...\r\n", ch);
		return;
	}

	timed.skill = SKILL_DRUNKOFF;
	timed.time = can_use_feat(ch, DRUNKARD_FEAT) ? feature_mod(DRUNKARD_FEAT, FEAT_TIMER) : 12;
	timed_to_char(ch, &timed);

	percent = number(1, skill_info[SKILL_DRUNKOFF].max_percent);
	prob = train_skill(ch, SKILL_DRUNKOFF, skill_info[SKILL_DRUNKOFF].max_percent, 0);
	amount = MIN(amount, GET_OBJ_VAL(obj, 1));
	weight = MIN(amount, GET_OBJ_WEIGHT(obj));
	weight_change_object(obj, -weight);	/* Subtract amount */
	GET_OBJ_VAL(obj, 1) -= amount;
	if (!GET_OBJ_VAL(obj, 1))  	/* The last bit */
	{
		GET_OBJ_VAL(obj, 2) = 0;
		GET_OBJ_VAL(obj, 3) = 0;
		name_from_drinkcon(obj);
	}

	if (percent > prob)
	{
		sprintf(buf,
				"�� ���������� %s �� $o1, �� ���� ������ ����� ��� �������...", drinks[GET_OBJ_VAL(obj, 2)]);
		act(buf, FALSE, ch, obj, 0, TO_CHAR);
		act("$n ����������$g �����������, �� ��� �� ����� $m �� ������.", FALSE, ch, 0, 0, TO_ROOM);
		duration = MAX(1, amount / 3);
		af[0].type = SPELL_ABSTINENT;
		af[0].duration = pc_duration(ch, duration, 0, 0, 0, 0);
		af[0].modifier = 0;
		af[0].location = APPLY_DAMROLL;
		af[0].bitvector = AFF_ABSTINENT;
		af[0].battleflag = 0;
		af[1].type = SPELL_ABSTINENT;
		af[1].duration = pc_duration(ch, duration, 0, 0, 0, 0);
		af[1].modifier = 0;
		af[1].location = APPLY_HITROLL;
		af[1].bitvector = AFF_ABSTINENT;
		af[1].battleflag = 0;
		af[2].type = SPELL_ABSTINENT;
		af[2].duration = pc_duration(ch, duration, 0, 0, 0, 0);
		af[2].modifier = 0;
		af[2].location = APPLY_AC;
		af[2].bitvector = AFF_ABSTINENT;
		af[2].battleflag = 0;
		switch (number(0, ch->get_skill(SKILL_DRUNKOFF) / 20))
		{
		case 0:
		case 1:
			af[0].modifier = -2;
		case 2:
		case 3:
			af[1].modifier = -2;
		default:
			af[2].modifier = 10;
		}
		for (prob = 0; prob < 3; prob++)
			affect_join(ch, &af[prob], TRUE, FALSE, TRUE, FALSE);
		gain_condition(ch, DRUNK, amount);
	}
	else
	{
		sprintf(buf,
				"�� ���������� %s �� $o1 � ������������� �������� �������� �� ���� ����...",
				drinks[GET_OBJ_VAL(obj, 2)]);
		act(buf, FALSE, ch, obj, 0, TO_CHAR);
		act("$n ��������$u � �������$g ���� �� ������.", FALSE, ch, 0, 0, TO_ROOM);
		affect_from_char(ch, SPELL_ABSTINENT);
	}

	return;
}

ACMD(do_pour)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	OBJ_DATA *from_obj = NULL, *to_obj = NULL;
	int amount;

	two_arguments(argument, arg1, arg2);

	if (subcmd == SCMD_POUR)
	{
		if (!*arg1)  	/* No arguments */
		{
			send_to_char("������ ���������� ?\r\n", ch);
			return;
		}
		if (!(from_obj = get_obj_in_list_vis(ch, arg1, ch->carrying)))
		{
			send_to_char("� ��� ��� ����� !\r\n", ch);
			return;
		}
		if (GET_OBJ_TYPE(from_obj) != ITEM_DRINKCON && GET_OBJ_TYPE(from_obj) != ITEM_POTION)
		{
			send_to_char("�� �� ������ �� ����� ���������� !\r\n", ch);
			return;
		}
	}
	if (subcmd == SCMD_FILL)
	{
		if (!*arg1)  	/* no arguments */
		{
			send_to_char("��� � �� ���� �� ������ �� ��������� ?\r\n", ch);
			return;
		}
		if (!(to_obj = get_obj_in_list_vis(ch, arg1, ch->carrying)))
		{
			send_to_char("� ��� ����� ��� !\r\n", ch);
			return;
		}
		if (GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON)
		{
			act("�� �� ������ ��������� $o3!", FALSE, ch, to_obj, 0, TO_CHAR);
			return;
		}
		if (!*arg2)  	/* no 2nd argument */
		{
			act("�� ���� �� ���������� ��������� $o3?", FALSE, ch, to_obj, 0, TO_CHAR);
			return;
		}
		if (!(from_obj = get_obj_in_list_vis(ch, arg2, world[ch->in_room]->contents)))
		{
			sprintf(buf, "�� �� ������ ����� '%s'.\r\n", arg2);
			send_to_char(buf, ch);
			return;
		}
		if (GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN)
		{
			act("�� �� ������� ������ ��������� �� $o1.", FALSE, ch, from_obj, 0, TO_CHAR);
			return;
		}
	}
	if (GET_OBJ_VAL(from_obj, 1) == 0)
	{
		act("�����.", FALSE, ch, from_obj, 0, TO_CHAR);
		return;
	}
	if (subcmd == SCMD_POUR)  	/* pour */
	{
		if (!*arg2)
		{
			send_to_char("���� �� ������ ���� ?  �� ����� ��� �� ���-�� ?\r\n", ch);
			return;
		}
		if (!str_cmp(arg2, "out") || !str_cmp(arg2, "�����"))
		{
			act("$n ���������$g $o3.", TRUE, ch, from_obj, 0, TO_ROOM);
			act("�� ���������� $o3.", FALSE, ch, from_obj, 0, TO_CHAR);

			weight_change_object(from_obj, -GET_OBJ_VAL(from_obj, 1));	/* Empty */

			GET_OBJ_VAL(from_obj, 1) = 0;
			GET_OBJ_VAL(from_obj, 2) = 0;
			GET_OBJ_VAL(from_obj, 3) = 0;
			GET_OBJ_SKILL(from_obj) = 0;
			name_from_drinkcon(from_obj);

			return;
		}
		if (!(to_obj = get_obj_in_list_vis(ch, arg2, ch->carrying)))
		{
			send_to_char("�� �� ������ ����� ����� !\r\n", ch);
			return;
		}
		if ((GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) && (GET_OBJ_TYPE(to_obj) != ITEM_FOUNTAIN))
		{
			send_to_char("�� �� ������� � ��� ������.\r\n", ch);
			return;
		}
	}
	if (to_obj == from_obj)
	{
		send_to_char("����� ������ ������� �� ���������, �������, �� �����.\r\n", ch);
		return;
	}

	if (GET_OBJ_VAL(to_obj, 1) != 0 &&
			GET_OBJ_TYPE(from_obj) != ITEM_POTION && GET_OBJ_VAL(to_obj, 2) != GET_OBJ_VAL(from_obj, 2))
	{
		send_to_char("�� ������� �������� �������, �� �� � ����� ����.\r\n", ch);
		return;
	}
	if (GET_OBJ_VAL(to_obj, 1) >= GET_OBJ_VAL(to_obj, 0))
	{
		send_to_char("��� ��� �����.\r\n", ch);
		return;
	}
//Added by Adept - ����������� ����� �� ������� ��� ������� � �������

	//���������� �� ������� � ������ � �������
	if (GET_OBJ_TYPE(from_obj) == ITEM_POTION)
	{
		if (GET_OBJ_VNUM(from_obj) == GET_OBJ_SKILL(to_obj) || GET_OBJ_VAL(to_obj, 1) == 0)
		{
			sprintf(buf, "�� �������� ������������ ����� � %s.\r\n", OBJN(to_obj, ch, 3));
			send_to_char(buf, ch);
			if (GET_OBJ_VAL(to_obj, 1) == 0)
			{
				/* ����������� �������� ����� �� ������������� ���������� */
				switch (GET_OBJ_VAL(from_obj, 1))
				{
					/* �������������� (�������) */
				case SPELL_REFRESH:
				case SPELL_GROUP_REFRESH:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_RED;
					name_to_drinkcon(to_obj, LIQ_POTION_RED);
					break;
					/* ��������� (�����) */
				case SPELL_FULL:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_BLUE;
					name_to_drinkcon(to_obj, LIQ_POTION_BLUE);
					break;
					/* ������� (�����) */
				case SPELL_DETECT_INVIS:
				case SPELL_DETECT_MAGIC:
				case SPELL_DETECT_POISON:
				case SPELL_DETECT_ALIGN:
				case SPELL_SENSE_LIFE:
				case SPELL_INFRAVISION:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_WHITE;
					name_to_drinkcon(to_obj, LIQ_POTION_WHITE);
					break;
					/* �������� (����������) */
				case SPELL_ARMOR:
				case SPELL_GROUP_ARMOR:
				case SPELL_CLOUDLY:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_GOLD;
					name_to_drinkcon(to_obj, LIQ_POTION_GOLD);
					break;
					/* ����������������� �������� (������) */
				case SPELL_CURE_CRITIC:
				case SPELL_CURE_LIGHT:
				case SPELL_HEAL:
				case SPELL_GROUP_HEAL:
				case SPELL_CURE_SERIOUS:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_BLACK;
					name_to_drinkcon(to_obj, LIQ_POTION_BLACK);
					break;
					/* ��������� ������� ������� (�����) */
				case SPELL_CURE_BLIND:
				case SPELL_REMOVE_CURSE:
				case SPELL_REMOVE_HOLD:
				case SPELL_REMOVE_SIELENCE:
				case SPELL_CURE_PLAQUE:
				case SPELL_REMOVE_DEAFNESS:
				case SPELL_REMOVE_POISON:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_GREY;
					name_to_drinkcon(to_obj, LIQ_POTION_GREY);
					break;
					/* ������ ���������� (����������) */
				case SPELL_INVISIBLE:
				case SPELL_GROUP_INVISIBLE:
				case SPELL_STRENGTH:
				case SPELL_GROUP_STRENGTH:
				case SPELL_FLY:
				case SPELL_GROUP_FLY:
				case SPELL_BLESS:
				case SPELL_GROUP_BLESS:
				case SPELL_HASTE:
				case SPELL_GROUP_HASTE:
				case SPELL_STONESKIN:
				case SPELL_BLINK:
				case SPELL_EXTRA_HITS:
				case SPELL_WATERBREATH:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION_FUCHSIA;
					name_to_drinkcon(to_obj, LIQ_POTION_FUCHSIA);
					break;
				default:
					GET_OBJ_VAL(to_obj, 2) = LIQ_POTION;
					name_to_drinkcon(to_obj, LIQ_POTION);	/* ��������� ����� ������� */
				}
			}
			GET_OBJ_SKILL(to_obj) = GET_OBJ_VNUM(from_obj);	/* �������� vnum ����� � ���� skill */
			weight_change_object(to_obj, 1);
			GET_OBJ_VAL(to_obj, 1)++;
			extract_obj(from_obj);
			return;
		}
		else
		{
			send_to_char("��������� ������ �����?! �� ��, ��������, ������!\r\n", ch);
			return;
		}
	}
	//���������� �� ������� ��� ������� � ������ ����-��
	if ((GET_OBJ_TYPE(from_obj) == ITEM_DRINKCON ||
			GET_OBJ_TYPE(from_obj) == ITEM_FOUNTAIN) &&
			GET_OBJ_VAL(from_obj, 2) >= LIQ_POTION && GET_OBJ_VAL(from_obj, 2) <= LIQ_POTION_FUCHSIA)
	{
		if ((GET_OBJ_SKILL(from_obj) == GET_OBJ_SKILL(to_obj)) || GET_OBJ_VAL(to_obj, 1) == 0)
			GET_OBJ_SKILL(to_obj) = GET_OBJ_SKILL(from_obj);
		else
		{
			send_to_char("��������� ������ �����?! �� ��, ��������, ������!\r\n", ch);
			return;
		}
	}
//����� ��������� Adept'��

	if (subcmd == SCMD_POUR)
	{
		sprintf(buf, "�� �������� ������������ %s � %s.",
				drinks[GET_OBJ_VAL(from_obj, 2)], OBJN(to_obj, ch, 3));
		send_to_char(buf, ch);
	}
	if (subcmd == SCMD_FILL)
	{
		act("�� ��������� $o3 �� $O1.", FALSE, ch, to_obj, from_obj, TO_CHAR);
		act("$n ��������$g $o3 �� $O1.", TRUE, ch, to_obj, from_obj, TO_ROOM);
	}

	/* �������� ��� �������� */
	GET_OBJ_VAL(to_obj, 2) = GET_OBJ_VAL(from_obj, 2);

	/* New alias */
	if (GET_OBJ_VAL(to_obj, 1) == 0)
		name_to_drinkcon(to_obj, GET_OBJ_VAL(from_obj, 2));

	/* Then how much to pour */
	if (GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN || GET_OBJ_VAL(from_obj, 1) != 999)
		GET_OBJ_VAL(from_obj, 1) -= (amount = (GET_OBJ_VAL(to_obj, 0) - GET_OBJ_VAL(to_obj, 1)));
	else
		amount = GET_OBJ_VAL(to_obj, 0) - GET_OBJ_VAL(to_obj, 1);

	GET_OBJ_VAL(to_obj, 1) = GET_OBJ_VAL(to_obj, 0);

	/* Then the poison boogie */
	GET_OBJ_VAL(to_obj, 3) = (GET_OBJ_VAL(to_obj, 3) || GET_OBJ_VAL(from_obj, 3));

	if (GET_OBJ_VAL(from_obj, 1) <= 0)  	/* There was too little */
	{
		GET_OBJ_VAL(to_obj, 1) += GET_OBJ_VAL(from_obj, 1);
		amount += GET_OBJ_VAL(from_obj, 1);
		GET_OBJ_VAL(from_obj, 1) = 0;
		GET_OBJ_VAL(from_obj, 2) = 0;
		GET_OBJ_VAL(from_obj, 3) = 0;
		name_from_drinkcon(from_obj);
	}

	/* And the weight boogie */
	if (GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN)
		weight_change_object(from_obj, -amount);
	weight_change_object(to_obj, amount);	/* Add weight */
}

void name_from_drinkcon(OBJ_DATA * obj)
{
	int i, c, j = 0;
	char new_name[MAX_STRING_LENGTH];

	for (i = 0; *(obj->name + i) && a_isspace(*(obj->name + i)); i++);
	for (j = 0; *(obj->name + i) && !(a_isspace(*(obj->name + i))); new_name[j] = *(obj->name + i), i++, j++);
	new_name[j] = '\0';
	if (*new_name)
	{
		if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)]->name)
			free(obj->name);
		obj->name = str_dup(new_name);
	}

	for (i = 0; *(obj->short_description + i)
			&& a_isspace(*(obj->short_description + i)); i++);
	for (j = 0; *(obj->short_description + i)
			&& !(a_isspace(*(obj->short_description + i))); new_name[j] = *(obj->short_description + i), i++, j++);
	new_name[j] = '\0';
	if (*new_name)
	{
		if (GET_OBJ_RNUM(obj) < 0 || obj->short_description != obj_proto[GET_OBJ_RNUM(obj)]->short_description)
			free(obj->short_description);
		obj->short_description = str_dup(new_name);
	}


	for (c = 0; c < NUM_PADS; c++)
	{
		for (i = 0; a_isspace(*(obj->PNames[c] + i)); i++);
		for (j = 0; !a_isspace(*(obj->PNames[c] + i)); new_name[j] = *(obj->PNames[c] + i), i++, j++);
		new_name[j] = '\0';
		if (*new_name)
		{
			if (GET_OBJ_RNUM(obj) < 0 || obj->PNames[c] != obj_proto[GET_OBJ_RNUM(obj)]->PNames[c])
				free(obj->PNames[c]);
			obj->PNames[c] = str_dup(new_name);
		}
	}
}

void name_to_drinkcon(OBJ_DATA * obj, int type)
{
	int c;
	char new_name[MAX_INPUT_LENGTH];

	sprintf(new_name, "%s %s", obj->name, drinknames[type]);
	if (GET_OBJ_RNUM(obj) < 0 || obj->name != obj_proto[GET_OBJ_RNUM(obj)]->name)
		free(obj->name);
	obj->name = str_dup(new_name);

	sprintf(new_name, "%s c %s", obj->short_description, drinknames[type]);
	if (GET_OBJ_RNUM(obj) < 0 || obj->short_description != obj_proto[GET_OBJ_RNUM(obj)]->short_description)
		free(obj->short_description);
	obj->short_description = str_dup(new_name);


	for (c = 0; c < NUM_PADS; c++)
	{
		sprintf(new_name, "%s � %s", obj->PNames[c], drinknames[type]);
		if (GET_OBJ_RNUM(obj) < 0 || obj->PNames[c] != obj_proto[GET_OBJ_RNUM(obj)]->PNames[c])
			free(obj->PNames[c]);
		obj->PNames[c] = str_dup(new_name);
	}
}

void set_abstinent(CHAR_DATA *ch)
{
	AFFECT_DATA af;
	
	int duration = pc_duration(ch, 2, MAX(0, GET_DRUNK_STATE(ch) - CHAR_DRUNKED), 4, 2, 5);
	
	if (can_use_feat(ch, DRUNKARD_FEAT))
		duration /= 2;

	af.type = SPELL_ABSTINENT;
	af.bitvector = AFF_ABSTINENT;
	af.duration = duration;

	af.location = APPLY_AC;
	af.modifier = 20;
	affect_join(ch, &af, 0,0,0,0);

	af.location = APPLY_HITROLL;
	af.modifier = -2;
	affect_join(ch, &af, 0,0,0,0);

	af.location = APPLY_DAMROLL;
	af.modifier = -2;
	affect_join(ch, &af, 0,0,0,0);

}
