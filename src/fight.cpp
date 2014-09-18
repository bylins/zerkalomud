/* ************************************************************************
*   File: fight.cpp                                     Part of Bylins    *
*  Usage: Combat system                                                   *
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
#include "handler.h"
#include "interpreter.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "constants.h"
#include "dg_scripts.h"
#include "pk.h"
#include "im.h"
#include "fight.h"
#include "skills.h"
#include "features.hpp"
#include "house.h"
#include "stuff.hpp"
#include "random.hpp"
#include "char.hpp"
#include "char_player.hpp"
#include "top.h"
#include "magic.h"
#include "poison.hpp"
#include "corpse.hpp"
#include "room.hpp"
#include "AffectHandler.hpp"
#include "genchar.h"

extern CHAR_DATA *mob_proto;

/* Structures */
CHAR_DATA *combat_list = NULL;	/* head of l-list of fighting chars */
CHAR_DATA *next_combat_list = NULL;

extern struct message_list fight_messages[MAX_MESSAGES];
extern CHAR_DATA *character_list;
extern vector < OBJ_DATA * >obj_proto;
extern int max_exp_gain_npc;	/* see config.cpp */
extern int max_npc_corpse_time, max_pc_corpse_time;
extern int material_value[];

extern int r_helled_start_room;
extern MobRaceListType mobraces_list;

/* External procedures */
CHAR_DATA *try_protect(CHAR_DATA * victim, CHAR_DATA * ch);
char *fread_action(FILE * fl, int nr);
ACMD(do_flee);
ACMD(do_assist);
ACMD(do_get);
void get_from_container(CHAR_DATA * ch, OBJ_DATA * cont, char *arg, int mode, int amount, bool autoloot);
int thaco(int ch_class, int level);
int ok_damage_shopkeeper(CHAR_DATA * ch, CHAR_DATA * victim);
void battle_affect_update(CHAR_DATA * ch);
void go_throw(CHAR_DATA * ch, CHAR_DATA * vict);
void go_bash(CHAR_DATA * ch, CHAR_DATA * vict);
void go_kick(CHAR_DATA * ch, CHAR_DATA * vict);
void go_rescue(CHAR_DATA * ch, CHAR_DATA * vict, CHAR_DATA * tmp_ch);
void go_parry(CHAR_DATA * ch);
void go_multyparry(CHAR_DATA * ch);
void go_block(CHAR_DATA * ch);
void go_touch(CHAR_DATA * ch, CHAR_DATA * vict);
void go_protect(CHAR_DATA * ch, CHAR_DATA * vict);
void go_chopoff(CHAR_DATA * ch, CHAR_DATA * vict);
void go_disarm(CHAR_DATA * ch, CHAR_DATA * vict);
const char *skill_name(int num);
void npc_groupbattle(CHAR_DATA * ch);
int npc_battle_scavenge(CHAR_DATA * ch);
void npc_wield(CHAR_DATA * ch);
void npc_armor(CHAR_DATA * ch);
int max_exp_gain_pc(CHAR_DATA * ch);
int max_exp_loss_pc(CHAR_DATA * ch);
int level_exp(CHAR_DATA * ch, int chlevel);
int extra_aco(int class_num, int level);
int perform_mob_switch(CHAR_DATA * ch);

/* local functions */
//void perform_group_gain(CHAR_DATA * ch, CHAR_DATA * victim, int members, int koef);
void dam_message(int dam, CHAR_DATA * ch, CHAR_DATA * victim, int w_type);
void appear(CHAR_DATA * ch);
void load_messages(void);
void change_alignment(CHAR_DATA * ch, CHAR_DATA * victim);
void death_cry(CHAR_DATA * ch);
void raw_kill(CHAR_DATA * ch, CHAR_DATA * killer);
int can_loot(CHAR_DATA * ch);
void die(CHAR_DATA * ch, CHAR_DATA * killer);
void group_gain(CHAR_DATA * ch, CHAR_DATA * victim);
//void solo_gain(CHAR_DATA * ch, CHAR_DATA * victim);
char *replace_string(const char *str, const char *weapon_singular, const char *weapon_plural);
void perform_violence(void);
int armor_class_limit(CHAR_DATA * ch);
int compute_armor_class(CHAR_DATA * ch);
int check_agro_follower(CHAR_DATA * ch, CHAR_DATA * victim);
void apply_weapon_bonus(int ch_class, int skill, int *damroll, int *hitroll);
void perform_drop_gold(CHAR_DATA * ch, int amount, byte mode, room_rnum RDR);


/* Weapon attack texts */
struct attack_hit_type attack_hit_text[] =
{
	{"������", "�������"},	/* 0 */
	{"�������", "��������"},
	{"��������", "���������"},
	{"�������", "��������"},
	{"������", "�������"},
	{"�����", "������"},	/* 5 */
	{"��������", "���������"},
	{"�������", "��������"},
	{"��������", "���������"},
	{"����������", "�����������"},
	{"������", "�������"},	/* 10 */
	{"������", "�������"},
	{"�����", "������"},
	{"������", "�������"},
	{"������", "�������"},
	{"������", "�������"},
	{"������", "�������"},
	{"*", "*"},
	{"*", "*"},
	{"*", "*"}
};
//Polud �������, ���������� ����������� ��������, ���� ��� ����
template <class S> void handle_affects( S& params ) //��� params ������������ ��� ������ �������
{
	AFFECT_DATA* aff;
	for (aff=params.ch->affected; aff; aff = aff->next)
	{
		if (aff->handler)
			aff->handler->Handle(params); //� ����������� �� ���� params ��������� ������ Handler
	}
}

void go_autoassist(CHAR_DATA * ch)
{
	struct follow_type *k;
	CHAR_DATA *ch_lider = 0;
	if (ch->master)
	{
		ch_lider = ch->master;
	}
	else
		ch_lider = ch;	// ������� ������ �� ������

	buf2[0] = '\0';
	for (k = ch_lider->followers; k; k = k->next)
	{
		if (PRF_FLAGGED(k->follower, PRF_AUTOASSIST) &&
				(IN_ROOM(k->follower) == IN_ROOM(ch)) && !k->follower->get_fighting() &&
				(GET_POS(k->follower) == POS_STANDING) && !CHECK_WAIT(k->follower))
			do_assist(k->follower, buf2, 0, 0);
	}
	if (PRF_FLAGGED(ch_lider, PRF_AUTOASSIST) &&
			(IN_ROOM(ch_lider) == IN_ROOM(ch)) && !ch_lider->get_fighting() &&
			(GET_POS(ch_lider) == POS_STANDING) && !CHECK_WAIT(ch_lider))
		do_assist(ch_lider, buf2, 0, 0);
}

int calc_leadership(CHAR_DATA * ch)
{
	int prob, percent;
	CHAR_DATA *leader = 0;

	if (IS_NPC(ch) || !AFF_FLAGGED(ch, AFF_GROUP) || (!ch->master && !ch->followers))
		return (FALSE);

	if (ch->master)
	{
		if (IN_ROOM(ch) != IN_ROOM(ch->master))
			return (FALSE);
		leader = ch->master;
	}
	else
		leader = ch;

	if (!leader->get_skill(SKILL_LEADERSHIP))
		return (FALSE);

	percent = number(1, 101);
	prob = calculate_skill(leader, SKILL_LEADERSHIP, 121, 0);
	if (percent > prob)
		return (FALSE);
	else
		return (TRUE);
}

/* The Fight related routines */

void appear(CHAR_DATA * ch)
{
	int appear_msg = AFF_FLAGGED(ch, AFF_INVISIBLE) || AFF_FLAGGED(ch, AFF_CAMOUFLAGE) || AFF_FLAGGED(ch, AFF_HIDE);

	if (affected_by_spell(ch, SPELL_INVISIBLE))
		affect_from_char(ch, SPELL_INVISIBLE);
	if (affected_by_spell(ch, SPELL_HIDE))
		affect_from_char(ch, SPELL_HIDE);
	if (affected_by_spell(ch, SPELL_SNEAK))
		affect_from_char(ch, SPELL_SNEAK);
	if (affected_by_spell(ch, SPELL_CAMOUFLAGE))
		affect_from_char(ch, SPELL_CAMOUFLAGE);

	REMOVE_BIT(AFF_FLAGS(ch, AFF_INVISIBLE), AFF_INVISIBLE);
	REMOVE_BIT(AFF_FLAGS(ch, AFF_HIDE), AFF_HIDE);
	REMOVE_BIT(AFF_FLAGS(ch, AFF_SNEAK), AFF_SNEAK);
	REMOVE_BIT(AFF_FLAGS(ch, AFF_CAMOUFLAGE), AFF_CAMOUFLAGE);

	if (appear_msg)
	{
		if (IS_NPC(ch) || GET_LEVEL(ch) < LVL_IMMORT)
			act("$n �������� ������$u �� �������.", FALSE, ch, 0, 0, TO_ROOM);
		else
			act("�� ������������� �������� ����������� $n1.", FALSE, ch, 0, 0, TO_ROOM);
	}
}

int armor_class_limit(CHAR_DATA * ch)
{
	if (IS_CHARMICE(ch))
	{
		return -200;
	};
	if (IS_NPC(ch))
	{
		return -300;
	};
	switch (GET_CLASS(ch))
	{
	case CLASS_ASSASINE:
	case CLASS_THIEF:
	case CLASS_GUARD:
		return -250;
		break;
	case CLASS_MERCHANT:
	case CLASS_WARRIOR:
	case CLASS_PALADINE:
	case CLASS_RANGER:
	case CLASS_SMITH:
		return -200;
		break;
	case CLASS_CLERIC:
	case CLASS_DRUID:
		return -150;
		break;
	case CLASS_BATTLEMAGE:
	case CLASS_DEFENDERMAGE:
	case CLASS_CHARMMAGE:
	case CLASS_NECROMANCER:
		return -100;
		break;
	default:
		return -300;
	}
}

int compute_armor_class(CHAR_DATA * ch)
{
	int armorclass = GET_REAL_AC(ch);

	if (AWAKE(ch))
	{
		armorclass -= dex_ac_bonus(GET_REAL_DEX(ch)) * 10;
		armorclass += extra_aco((int) GET_CLASS(ch), (int) GET_LEVEL(ch));
	};

	if (AFF_FLAGGED(ch, AFF_BERSERK))
	{
		armorclass -= (240 * ((GET_REAL_MAX_HIT(ch) / 2) - GET_HIT(ch)) / GET_REAL_MAX_HIT(ch));
	}

	/* Gorrah: ����� � �� �� ���������� ������ "�������� �����" */
	if (IS_SET(PRF_FLAGS(ch, PRF_IRON_WIND), PRF_IRON_WIND))
		armorclass += ch->get_skill(SKILL_IRON_WIND) / 2;

	armorclass += (size_app[GET_POS_SIZE(ch)].ac * 10);

	if (GET_AF_BATTLE(ch, EAF_PUNCTUAL))
	{
		if (GET_EQ(ch, WEAR_WIELD))
		{
			if (GET_EQ(ch, WEAR_HOLD))
				armorclass +=
					10 * MAX(-1,
							 (GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_WIELD)) +
							  GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_HOLD))) / 5 - 6);
			else
				armorclass += 10 * MAX(-1, GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_WIELD)) / 5 - 6);
		}
		if (GET_EQ(ch, WEAR_BOTHS))
			armorclass += 10 * MAX(-1, GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_BOTHS)) / 5 - 6);
	}

	armorclass = MIN(100, armorclass);
	return (MAX(armor_class_limit(ch), armorclass));
}


void load_messages(void)
{
	FILE *fl;
	int i, type;
	struct message_type *messages;
	char chk[128];

	if (!(fl = fopen(MESS_FILE, "r")))
	{
		log("SYSERR: Error reading combat message file %s: %s", MESS_FILE, strerror(errno));
		exit(1);
	}
	for (i = 0; i < MAX_MESSAGES; i++)
	{
		fight_messages[i].a_type = 0;
		fight_messages[i].number_of_attacks = 0;
		fight_messages[i].msg = 0;
	}


	fgets(chk, 128, fl);
	while (!feof(fl) && (*chk == '\n' || *chk == '*'))
		fgets(chk, 128, fl);

	while (*chk == 'M')
	{
		fgets(chk, 128, fl);
		sscanf(chk, " %d\n", &type);
		for (i = 0; (i < MAX_MESSAGES) &&
				(fight_messages[i].a_type != type) && (fight_messages[i].a_type); i++);
		if (i >= MAX_MESSAGES)
		{
			log("SYSERR: Too many combat messages.  Increase MAX_MESSAGES and recompile.");
			exit(1);
		}
		log("BATTLE MESSAGE %d(%d)", i, type);
		CREATE(messages, struct message_type, 1);
		fight_messages[i].number_of_attacks++;
		fight_messages[i].a_type = type;
		messages->next = fight_messages[i].msg;
		fight_messages[i].msg = messages;

		messages->die_msg.attacker_msg = fread_action(fl, i);
		messages->die_msg.victim_msg = fread_action(fl, i);
		messages->die_msg.room_msg = fread_action(fl, i);
		messages->miss_msg.attacker_msg = fread_action(fl, i);
		messages->miss_msg.victim_msg = fread_action(fl, i);
		messages->miss_msg.room_msg = fread_action(fl, i);
		messages->hit_msg.attacker_msg = fread_action(fl, i);
		messages->hit_msg.victim_msg = fread_action(fl, i);
		messages->hit_msg.room_msg = fread_action(fl, i);
		messages->god_msg.attacker_msg = fread_action(fl, i);
		messages->god_msg.victim_msg = fread_action(fl, i);
		messages->god_msg.room_msg = fread_action(fl, i);
		fgets(chk, 128, fl);
		while (!feof(fl) && (*chk == '\n' || *chk == '*'))
			fgets(chk, 128, fl);
	}

	fclose(fl);
}


void update_pos(CHAR_DATA * victim)
{
	if ((GET_HIT(victim) > 0) && (GET_POS(victim) > POS_STUNNED))
		GET_POS(victim) = GET_POS(victim);
	//������� �������� �� ���, ����� ��������� �������� �������� ��������� � ���� �������, ��� ������� ������ ������ ��� ���_�������.
	else if (GET_HIT(victim) > 0 && GET_WAIT(victim) <= 0 && !GET_MOB_HOLD(victim))
		GET_POS(victim) = POS_STANDING;
	else if (GET_HIT(victim) <= -11)
		GET_POS(victim) = POS_DEAD;
	else if (GET_HIT(victim) <= -6)
		GET_POS(victim) = POS_MORTALLYW;
	else if (GET_HIT(victim) <= -3)
		GET_POS(victim) = POS_INCAP;
	else
		GET_POS(victim) = POS_STUNNED;

	if (AFF_FLAGGED(victim, AFF_SLEEP) && GET_POS(victim) != POS_SLEEPING)
		affect_from_char(victim, SPELL_SLEEP);

	if (on_horse(victim) && GET_POS(victim) < POS_FIGHTING)
		horse_drop(get_horse(victim));

	if (IS_HORSE(victim) && GET_POS(victim) < POS_FIGHTING && on_horse(victim->master))
		horse_drop(victim);
}

void set_battle_pos(CHAR_DATA * ch)
{
	switch (GET_POS(ch))
	{
	case POS_STANDING:
		GET_POS(ch) = POS_FIGHTING;
		break;
	case POS_RESTING:
	case POS_SITTING:
	case POS_SLEEPING:
		if (GET_WAIT(ch) <= 0 &&
				!GET_MOB_HOLD(ch) && !AFF_FLAGGED(ch, AFF_SLEEP) && !AFF_FLAGGED(ch, AFF_CHARM))
		{
			if (IS_NPC(ch))
			{
				act("$n �����$g �� ����.", FALSE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_FIGHTING;
			}
			else if (GET_POS(ch) == POS_SLEEPING)
			{
				act("�� ���������� � ����.", FALSE, ch, 0, 0, TO_CHAR);
				act("$n �������$u � ���$g.", FALSE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_SITTING;
			}
			else if (GET_POS(ch) == POS_RESTING)
			{
				act("�� ���������� ����� � ����.", FALSE, ch, 0, 0, TO_CHAR);
				act("$n ���������$g ����� � ���$g.", FALSE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_SITTING;
			}
		}
		break;
	}
}

void restore_battle_pos(CHAR_DATA * ch)
{
	switch (GET_POS(ch))
	{
	case POS_FIGHTING:
		GET_POS(ch) = POS_STANDING;
		break;
	case POS_RESTING:
	case POS_SITTING:
	case POS_SLEEPING:
		if (IS_NPC(ch) &&
				GET_WAIT(ch) <= 0 &&
				!GET_MOB_HOLD(ch) && !AFF_FLAGGED(ch, AFF_SLEEP) && !AFF_FLAGGED(ch, AFF_CHARM))
		{
			act("$n �����$g �� ����.", FALSE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_STANDING;
		}
		break;
	}
	if (AFF_FLAGGED(ch, AFF_SLEEP))
		GET_POS(ch) = POS_SLEEPING;
}

/* start one char fighting another (yes, it is horrible, I know... )  */
void set_fighting(CHAR_DATA * ch, CHAR_DATA * vict)
{
	if (ch == vict)
		return;

	if (ch->get_fighting())
	{
		log("SYSERR: set_fighting(%s->%s) when already fighting(%s)...",
			GET_NAME(ch), GET_NAME(vict), GET_NAME(ch->get_fighting()));
		// core_dump();
		return;
	}

	if ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_NOFIGHT)) || (IS_NPC(vict) && MOB_FLAGGED(vict, MOB_NOFIGHT)))
		return;

	// if (AFF_FLAGGED(ch,AFF_STOPFIGHT))
	//    return;

	if (AFF_FLAGGED(ch, AFF_BANDAGE))
	{
		send_to_char("��������� ���� ��������!\r\n", ch);
		affect_from_char(ch, SPELL_BANDAGE);
	}

	ch->next_fighting = combat_list;
	combat_list = ch;

	if (AFF_FLAGGED(ch, AFF_SLEEP))
		affect_from_char(ch, SPELL_SLEEP);
	ch->set_fighting(vict);
	NUL_AF_BATTLE(ch);
//Polud ���������� � ����� �� ������ ����������
	if (ch->get_protecting())
		SET_AF_BATTLE(ch, EAF_PROTECT);
	ch->set_touching(0);
	INITIATIVE(ch) = 0;
	BATTLECNTR(ch) = 0;
	ROUND_COUNTER(ch) = 0;
	ch->set_extra_attack(0, 0);
	set_battle_pos(ch);
	/* Set combat style */
	if (!AFF_FLAGGED(ch, AFF_COURAGE) && !AFF_FLAGGED(ch, AFF_DRUNKED) && !AFF_FLAGGED(ch, AFF_ABSTINENT))
	{
		if (PRF_FLAGGED(ch, PRF_PUNCTUAL))
			SET_AF_BATTLE(ch, EAF_PUNCTUAL);
		else if (PRF_FLAGGED(ch, PRF_AWAKE))
			SET_AF_BATTLE(ch, EAF_AWAKE);
	}

	if (GET_CLASS(ch) == CLASS_GUARD && ch->get_skill(SKILL_BLOCK))
	{
		SET_AF_BATTLE(ch, EAF_AUTOBLOCK);
	}

	start_fight_mtrigger(ch, vict);
//  check_killer(ch, vict);
}

/* remove a char from the list of fighting chars */
void stop_fighting(CHAR_DATA * ch, int switch_others)
{
	CHAR_DATA *temp, *found;

	if (ch == next_combat_list)
		next_combat_list = ch->next_fighting;

	REMOVE_FROM_LIST(ch, combat_list, next_fighting);
	ch->next_fighting = NULL;
	if (ch->last_comm != NULL)
		free(ch->last_comm);
	ch->last_comm = NULL;
	ch->set_touching(0);
	ch->set_fighting(0);
	INITIATIVE(ch) = 0;
	BATTLECNTR(ch) = 0;
	ch->set_extra_attack(0, 0);
	ch->set_cast(0, 0, 0, 0, 0);
	restore_battle_pos(ch);
	NUL_AF_BATTLE(ch);
	DpsSystem::check_round(ch);
	StopFightParameters params(ch); //������� ��������� ������� ���� � �������� ��������� �������
	handle_affects(params);
	// sprintf(buf,"[Stop fighting] %s - %s\r\n",GET_NAME(ch),switch_others ? "switching" : "no switching");
	// send_to_gods(buf);
	/**** switch others *****/

	for (temp = combat_list; temp; temp = temp->next_fighting)
	{
		if (temp->get_touching() == ch)
		{
			temp->set_touching(0);
			CLR_AF_BATTLE(temp, EAF_TOUCH);
		}
		if (temp->get_extra_victim() == ch)
			temp->set_extra_attack(0, 0);
		if (temp->get_cast_char() == ch)
			temp->set_cast(0, 0, 0, 0, 0);
		if (temp->get_fighting() == ch && switch_others)
		{
			log("[Stop fighting] %s : Change victim for fighting", GET_NAME(temp));
			for (found = combat_list; found; found = found->next_fighting)
				if (found != ch && found->get_fighting() == temp)
				{
					act("�� ����������� ���� �������� �� $N3.", FALSE, temp, 0, found, TO_CHAR);
					temp->set_fighting(found);
					break;
				}
			if (!found)
				stop_fighting(temp, FALSE);
		}
	}

	update_pos(ch);

	/* �������� ������ "�������� �����" - ������� ���� �� ��������� ��� */
	if ((ch->get_fighting() == NULL) && IS_SET(PRF_FLAGS(ch, PRF_IRON_WIND), PRF_IRON_WIND))
	{
		REMOVE_BIT(PRF_FLAGS(ch, PRF_IRON_WIND), PRF_IRON_WIND);
		if (GET_POS(ch) > POS_INCAP)
		{
			send_to_char("������� ��� ��������� ���, � ���� ���������� ���������...\r\n", ch);
			act("$n ����� ��������$g � ���������$u, �������� ��� ����� ���.", FALSE, ch, 0, 0, TO_ROOM);
		};
	};
}

/* When ch kills victim */
void change_alignment(CHAR_DATA * ch, CHAR_DATA * victim)
{
	/*
	 * new alignment change algorithm: if you kill a monster with alignment A,
	 * you move 1/16th of the way to having alignment -A.  Simple and fast.
	 */
	GET_ALIGNMENT(ch) += (-GET_ALIGNMENT(victim) - GET_ALIGNMENT(ch)) / 16;
}



void death_cry(CHAR_DATA * ch)
{
	int door;
	act("�������� ������ � ����� �� ������������� ����� $n1.", FALSE, ch, 0, 0, TO_ROOM | CHECK_DEAF);

	for (door = 0; door < NUM_OF_DIRS; door++)
	{
		if (CAN_GO(ch, door))
		{
			CHAR_DATA *people = world[world[IN_ROOM(ch)]->dir_option[door]->to_room]->people;
			if (people)
			{
				act("�������� ������ � ����� �� �����-�� ������������� �����.", FALSE, people, 0, 0, TO_CHAR | CHECK_DEAF);
				act("�������� ������ � ����� �� �����-�� ������������� �����.", FALSE, people, 0, 0, TO_ROOM | CHECK_DEAF);
			}
		}
	}
}

/**
* ������ �������� � ���� ��� ������/����� � ��.
*/
void reset_affects(CHAR_DATA *ch)
{
	AFFECT_DATA *af, *naf;

	for (af = ch->affected; af; af = naf)
	{
		naf = af->next;
		if (!IS_SET(af->battleflag, AF_DEADKEEP))
			affect_remove(ch, af);
	}

	affect_total(ch);
}

void forget_all_spells(CHAR_DATA *ch)
{
	MemQ_flush(ch);
	for (int i = 0; i < MAX_SPELLS + 1; i++)
	{
		ch->real_abils.SplMem[i] = 0;
	}
}

void raw_kill(CHAR_DATA * ch, CHAR_DATA * killer)
{
	CHAR_DATA *hitter;
	OBJ_DATA *corpse = NULL;
	int to_room;
	long local_gold = 0;
	char obj[256];


	if(IS_NPC(ch) && killer && killer != ch && MOB_FLAGGED(ch, MOB_CLONE) && ch->master && affected_by_spell(ch, SPELL_CAPABLE))
	{
		act("����, ���������� �� $n3, ������ ����������� � ����� ������������ � ����� �������.", FALSE, ch, 0, killer, TO_ROOM);
		int pos = GET_POS(ch);
		GET_POS(ch) = POS_STANDING;
		call_magic(ch, killer, NULL, world[IN_ROOM(ch)], ch->mob_specials.capable_spell, GET_LEVEL(ch), CAST_SPELL);
		GET_POS(ch) = pos;
	}
	if (ch->get_fighting())
		stop_fighting(ch, TRUE);

	for (hitter = combat_list; hitter; hitter = hitter->next_fighting)
		if (hitter->get_fighting() == ch)
			WAIT_STATE(hitter, 0);

	reset_affects(ch);

	if (!killer || death_mtrigger(ch, killer))
		if (IN_ROOM(ch) != NOWHERE)
			death_cry(ch);

	if (IN_ROOM(ch) != NOWHERE)
	{
		if (!IS_NPC(ch) && ((!RENTABLE(ch) && ROOM_FLAGGED(IN_ROOM(ch), ROOM_ARENA))
				|| (killer && PRF_FLAGGED(killer, PRF_EXECUTOR))))//���� ����� �� ����� ��� �����
		{
			make_arena_corpse(ch, killer);
			if(killer && PRF_FLAGGED(killer, PRF_EXECUTOR))//���� ���� ����� �� ��� ������ ������������ � ����
			{
				killer->set_gold(ch->get_gold() + killer->get_gold());
				ch->set_gold(0);
			}
			change_fighting(ch, TRUE);
			GET_HIT(ch) = 1;
			GET_POS(ch) = POS_SITTING;
			char_from_room(ch);
			to_room = real_room(GET_LOADROOM(ch));
			// ��� �������� ������� ������ ���� �� ������, ���� ��� � ����� �� ����
			if (!Clan::MayEnter(ch, to_room, HCE_PORTAL))
				to_room = Clan::CloseRent(to_room);
			if (to_room == NOWHERE)
			{
				SET_BIT(PLR_FLAGS(ch, PLR_HELLED), PLR_HELLED);
				HELL_DURATION(ch) = time(0) + 6;
				to_room = r_helled_start_room;
			}
			char_to_room(ch, to_room);
			look_at_room(ch, to_room);
			act("$n �� ������� ����$g � �����...", FALSE, ch, 0, 0, TO_ROOM);
		}
		else
		{
			if (IS_NPC(ch) && killer && (!IS_NPC(killer) || IS_CHARMICE(killer)))
			{
				log("Killed: %d %d %ld", GET_LEVEL(ch), GET_MAX_HIT(ch), GET_EXP(ch));
				CHAR_DATA *master = killer;
				if (IS_CHARMICE(killer))
				{
					master = killer->master ? killer->master : killer;
				}
				kill_log("%s (%d): %d", GET_NAME(master), GET_LEVEL(master), GET_LEVEL(ch));
			}
			local_gold = ch->get_gold();
			corpse = make_corpse(ch, killer);
			bloody::handle_corpse(corpse, ch, killer);
			//������� ����� pk_revenge_action �� die, ����� �� ������ �������� ����� ����� �� ������ ���� ��� ����
			if (IS_NPC(ch) || !ROOM_FLAGGED(IN_ROOM(ch), ROOM_ARENA)
					|| RENTABLE(ch))
				pk_revenge_action(killer, ch);
			if (MOB_FLAGGED(ch, MOB_CORPSE))
			{
				perform_drop_gold(ch, local_gold, SCMD_DROP, 0);
				ch->set_gold(0);
			}
			if (killer && (!IS_NPC(killer) || IS_CHARMICE(killer)))
			{
				obj_load_on_death(corpse, ch);
			}

			if (!IS_NPC(ch))
			{
				forget_all_spells(ch);
				for (hitter = character_list; hitter; hitter = hitter->next)
					if (IS_NPC(hitter) && MEMORY(hitter))
						forget(hitter, ch);
				/*
				   for (hitter = character_list; hitter && IS_NPC(hitter) && MEMORY(hitter); hitter = hitter->next)
				   forget(hitter, ch);
				 */
			}
			else
			{
				dl_load_obj(corpse, ch, NULL, DL_ORDINARY);
				dl_load_obj(corpse, ch, NULL, DL_PROGRESSION);
			}
//send_to_char (buf,killer);
			/* ������ ���������.
			   (�) ������� ��� dzMUDiST */

// ������ ���������� ������� "����������" � "����� ����" ���������� �� � damage,
// � �����, ����� �������� ���������������� �����. ����� ����,
// ���� ���� ������ � ������ � �������, �� ������� ���������� �������
			if ((ch != NULL) && (killer != NULL))
			{
				if (IS_NPC(ch) && !IS_NPC(killer) && PRF_FLAGGED(killer, PRF_AUTOLOOT)
						&& (corpse != NULL) && can_loot(killer))
				{
					sprintf(obj, "all");
					get_from_container(killer, corpse, obj, FIND_OBJ_INV, 1, true);
				}
				else if (IS_NPC(ch) && !IS_NPC(killer) && local_gold
						 && PRF_FLAGGED(killer, PRF_AUTOMONEY) && (corpse != NULL)
						 && can_loot(killer))
				{
					sprintf(obj, "all.coin");
					get_from_container(killer, corpse, obj, FIND_OBJ_INV, 1, false);
				}
				else if (IS_NPC(ch) && IS_NPC(killer) && (AFF_FLAGGED(killer, AFF_CHARM) || MOB_FLAGGED(killer, MOB_ANGEL))
						 && (corpse != NULL) && killer->master
						 && killer->in_room == killer->master->in_room
						 && PRF_FLAGGED(killer->master, PRF_AUTOLOOT) && can_loot(killer->master))
				{
					sprintf(obj, "all");
					get_from_container(killer->master, corpse, obj, FIND_OBJ_INV, 1, true);
				}
				else if (IS_NPC(ch) && IS_NPC(killer) && local_gold && (AFF_FLAGGED(killer, AFF_CHARM) || MOB_FLAGGED(killer, MOB_ANGEL))
						 && (corpse != NULL) && killer->master
						 && killer->in_room == killer->master->in_room
						 && PRF_FLAGGED(killer->master, PRF_AUTOMONEY) && can_loot(killer->master))
				{
					sprintf(obj, "all.coin");
					get_from_container(killer->master, corpse, obj, FIND_OBJ_INV, 1, false);
				}
			}

			/* ����� ���������.
			   (�) ������� ��� dzMUDiST */

			/* ���� ���� � ��� - �� ����� ����� �� ���� */
			if (!IS_NPC(ch))
			{
				RENTABLE(ch) = 0;
				AGRESSOR(ch) = 0;
				AGRO(ch) = 0;
			}
			extract_char(ch, TRUE);
		}
	}
}

/* ������� ������������ ��� "�����������" � "��������",
   ����� �� ���� ���� ��� ������ ��� � �����             */
int can_loot(CHAR_DATA * ch)
{
	if (ch != NULL)
	{
		if (!IS_NPC(ch) && GET_MOB_HOLD(ch) == 0 &&	// ���� ��� ������
				!AFF_FLAGGED(ch, AFF_STOPFIGHT) &&	// ����������� ������
				!AFF_FLAGGED(ch, AFF_BLIND))	// ����
			return TRUE;
	}
	return FALSE;
}

void die(CHAR_DATA * ch, CHAR_DATA * killer)
{
	CHAR_DATA *master = NULL;
	struct follow_type *f;
	int dec_exp = 0, e = GET_EXP(ch);

	if (!IS_NPC(ch) && (IN_ROOM(ch) == NOWHERE))
	{
		log("SYSERR: %s is dying in room NOWHERE.", GET_NAME(ch));
		return;
	}

	if (IS_NPC(ch) || !ROOM_FLAGGED(IN_ROOM(ch), ROOM_ARENA)
			|| RENTABLE(ch))
	{
		if (!(IS_NPC(ch) || IS_IMMORTAL(ch) || GET_GOD_FLAG(ch, GF_GODSLIKE) || (killer && PRF_FLAGGED(killer, PRF_EXECUTOR))))//���� ���� �� �����
		{
			dec_exp = (level_exp(ch, GET_LEVEL(ch) + 1) - level_exp(ch, GET_LEVEL(ch))) / (3 + MIN(3, GET_REMORT(ch) / 5));
			gain_exp(ch, -dec_exp);
			dec_exp = e - GET_EXP(ch);
			sprintf(buf, "�� �������� %d %s �����.\r\n", dec_exp, desc_count(dec_exp, WHAT_POINT));
			send_to_char(buf, ch);
		}

		/* ��������� ������ �� ����� */
		/* ����� �������� ����������, ����� ������ ����������,         */
		/* ����� ����, �������� ������ � ������������ ������ � ������� */
		if (IS_NPC(ch) && killer)
		{
			if (IS_NPC(killer) &&
					(AFF_FLAGGED(killer, AFF_CHARM) || MOB_FLAGGED(killer, MOB_ANGEL)) && killer->master)
				master = killer->master;
			else if (!IS_NPC(killer))
				master = killer;

			// �� ���� ������ master - PC

			if (master)
			{
				if (AFF_FLAGGED(master, AFF_GROUP))
				{
					int cnt = 0;

					// master - ���� ������, ��������� �� ������ ������
					if (master->master)
						master = master->master;
					if (IN_ROOM(master) == IN_ROOM(killer))
					{
						// ����� ������ � ����� �������, ��� � ������
						cnt = 1;
					}

					for (f = master->followers; f; f = f->next)
					{
						if (AFF_FLAGGED(f->follower, AFF_GROUP) &&
								IN_ROOM(f->follower) == IN_ROOM(killer))
						{
							if (!number(0, cnt))
								master = f->follower;
							++cnt;
						}
					}
				}
				master->mobmax_add(master, GET_MOB_VNUM(ch), 1, GET_LEVEL(ch));
			}
		}

		/* train LEADERSHIP */

		if (IS_NPC(ch) && killer)
			if (!IS_NPC(killer) &&
					AFF_FLAGGED(killer, AFF_GROUP) &&
					killer->master &&
					killer->master->get_skill(SKILL_LEADERSHIP) > 0 &&
					IN_ROOM(killer) == IN_ROOM(killer->master))
				improove_skill(killer->master, SKILL_LEADERSHIP, number(0, 1), ch);

		if (!IS_NPC(ch) && killer)  	/* decrease LEADERSHIP */
		{
			if (IS_NPC(killer) &&
					AFF_FLAGGED(ch, AFF_GROUP) && ch->master && IN_ROOM(ch) == IN_ROOM(ch->master))
			{
				if (ch->master->get_inborn_skill(SKILL_LEADERSHIP) > 1)
					ch->master->set_skill(SKILL_LEADERSHIP, ch->master->get_trained_skill(SKILL_LEADERSHIP) - 1);
			}
		}
	}

	/*29.11.09 ����������� �������� ����� (�) �������� */
	//edited by WorM
	CHAR_DATA *rkiller = killer;//��������� ������ ������ �������/����/������
	if (rkiller && IS_NPC(rkiller) && (IS_CHARMICE(rkiller) || IS_HORSE(rkiller) || MOB_FLAGGED(killer, MOB_ANGEL)))
	{
		if(rkiller->master)
			rkiller = rkiller->master;
		else
		{
			snprintf(buf, MAX_STRING_LENGTH,
				"die: %s killed by %s(without master)",
				GET_PAD(ch,0), GET_PAD(rkiller,0));
			mudlog(buf, LGH, LVL_IMMORT, SYSLOG, TRUE);
			rkiller = NULL;
		}
	}
	if (!IS_NPC(ch)) {
		if (rkiller && rkiller != ch)
		{
			if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_ARENA)) //��� �� �����
			{
				GET_RIP_ARENA(ch)= GET_RIP_ARENA(ch)+1;
				GET_WIN_ARENA(killer)= GET_WIN_ARENA(killer)+1;
				if (dec_exp)
				{
					GET_EXP_ARENA(ch)=GET_EXP_ARENA(ch)+dec_exp; //���� ��� � ��
				}
			}
			else if (IS_NPC(rkiller))
			{
				//��� �� ����
				GET_RIP_MOB(ch)= GET_RIP_MOB(ch)+1;
				GET_RIP_MOBTHIS(ch)= GET_RIP_MOBTHIS(ch)+1;
				if (dec_exp)
				{
					GET_EXP_MOB(ch)= GET_EXP_MOB(ch)+dec_exp;
					GET_EXP_MOBTHIS(ch)= GET_EXP_MOBTHIS(ch)+dec_exp;
				}
			}
			else if (!IS_NPC(rkiller))
			{
				//��� � ��
				GET_RIP_PK(ch)= GET_RIP_PK(ch)+1;
				GET_RIP_PKTHIS(ch)= GET_RIP_PKTHIS(ch)+1;
				if (dec_exp)
				{
					GET_EXP_PK(ch)= GET_EXP_PK(ch)+dec_exp;
					GET_EXP_PKTHIS(ch)= GET_EXP_PKTHIS(ch)+dec_exp;
				}
			}
		}
		else if ((!rkiller || (rkiller && rkiller == ch)) &&
			(ROOM_FLAGGED(IN_ROOM(ch), ROOM_DEATH) ||
			ROOM_FLAGGED(IN_ROOM(ch), ROOM_SLOWDEATH) ||
			ROOM_FLAGGED(IN_ROOM(ch), ROOM_ICEDEATH)))
		{
			//��� � ��
			GET_RIP_DT(ch)= GET_RIP_DT(ch)+1;
			GET_RIP_DTTHIS(ch)= GET_RIP_DTTHIS(ch)+1;
			if (dec_exp)
			{
				GET_EXP_DT(ch)= GET_EXP_DT(ch)+dec_exp;
				GET_EXP_DTTHIS(ch)= GET_EXP_DTTHIS(ch)+dec_exp;
			}
		}
		else// if (!rkiller || (rkiller && rkiller == ch))
		{
			//��� �� �������� �������������
			GET_RIP_OTHER(ch)= GET_RIP_OTHER(ch)+1;
			GET_RIP_OTHERTHIS(ch)= GET_RIP_OTHERTHIS(ch)+1;
			if (dec_exp)
			{
				GET_EXP_OTHER(ch)= GET_EXP_OTHER(ch)+dec_exp;
				GET_EXP_OTHERTHIS(ch)= GET_EXP_OTHERTHIS(ch)+dec_exp;
			}
		}
	}
	//end by WorM
	/*����� ������ (�) �������� */
	raw_kill(ch, killer);
	// if (killer)
	//   log("Killer lag is %d", GET_WAIT(killer));
}

int get_extend_exp(int exp, CHAR_DATA * ch, CHAR_DATA * victim)
{
	int base, diff;
	int koef;

	if (!IS_NPC(victim) || IS_NPC(ch))
		return (exp);

	for (koef = 100, base = 0, diff = ch->mobmax_get(GET_MOB_VNUM(victim));
			base < diff && koef > 5; base++, koef = koef * 95 / 100);

	exp = exp * MAX(5, koef) / 100;
	exp /= MAX(1, GET_REMORT(ch) - MAX_EXP_COEFFICIENTS_USED - 1);

	return (exp);
}

/*++
   ������� ���������� �����
      ch - ���� ���� ���������
           ����� ���� ������� ��� NPC ������ �� �����, �� ��� �����
           �����-�� �������� ������ ����� �� ��������
--*/
void perform_group_gain(CHAR_DATA * ch, CHAR_DATA * victim, int members, int koef)
{
	if (!EXTRA_FLAGGED(victim, EXTRA_GRP_KILL_COUNT)
		&& !IS_NPC(ch)
		&& !IS_IMMORTAL(ch)
		&& IS_NPC(victim)
		&& !ROOM_FLAGGED(IN_ROOM(victim), ROOM_ARENA))
	{
		FullSetDrop::add_mob_stat(victim, members);
		SET_BIT(EXTRA_FLAGS(victim, EXTRA_GRP_KILL_COUNT), EXTRA_GRP_KILL_COUNT);
	}

// �������, �� ��� NPC ��� ������� ���� ������ ��������
//  if (IS_NPC(ch) || !OK_GAIN_EXP(ch,victim))
	if (!OK_GAIN_EXP(ch, victim))
	{
		send_to_char("���� ������ ����� �� ������.\r\n", ch);
		return;
	}

	// 1. ���� ������� ������� �� ����
	int exp = GET_EXP(victim) / MAX(members, 1);

	if(victim->get_zone_group() > 1 && members < victim->get_zone_group())
	{
		// � ������ ����-���� ������ ���� ������ �� ��� ���-�� ������� � ������
		exp = GET_EXP(victim) / victim->get_zone_group();
	}

	// 2. ����������� ����������� (���������, �������� �������)
	//    �� ��� ������ ��� ���������� ������������ ��� � �� � ����� ���������,
	//    ���� � ����������� ������� ��� ��� �����
	exp = exp * koef / 100;

	// 3. ���������� ����� ��� PC � NPC
	if (IS_NPC(ch))
	{
		exp = MIN(max_exp_gain_npc, exp);
		exp += MAX(0, (exp * MIN(4, (GET_LEVEL(victim) - GET_LEVEL(ch)))) / 8);
	}
	else
	{
		exp = MIN(max_exp_gain_pc(ch), get_extend_exp(exp, ch, victim));
	}

	// 4. ��������� ��������
	exp = MAX(1, exp);

	int clan_exp = 0;
	if (CLAN(ch))
	{
		clan_exp = exp - CLAN(ch)->SetClanExp(ch, exp);
		exp -= clan_exp;
		if (clan_exp > 0)
		{
			sprintf(buf1, "�� ������ %d %s ����� ������ �����.\r\n", clan_exp, desc_count(clan_exp, WHAT_POINT));
			send_to_char(buf1, ch);
		}
	}

	if (exp > 1)
	{
		sprintf(buf2, "��� ���� ��������� �� %d %s.\r\n", exp, desc_count(exp, WHAT_POINT));
		send_to_char(buf2, ch);
	}
	else if (exp == 1)
		send_to_char("��� ���� ��������� ����� ���� �� ��������� ��������.\r\n", ch);

	gain_exp(ch, exp, clan_exp);
	change_alignment(ch, victim);
	TopPlayer::Refresh(ch);
}


/*++
   ������� ����������� ������ ������ ��� ������ ��� ��������� �����,
 ����� ���� �������� ������� ��������� ����� ��� ���� ������ ������
 �.�. ������ ������ ����� ���� ������ PC, �� ��� ������� ������� ���� ������ PC

   ch - ����������� ���� ������, �� ���� �������:
            1. ��� �� NPC
            2. �� ��������� � ������ ������ (��� ��� �����)

   ������ ��� PC-�������������� ��� ������� �� ����������

--*/
void group_gain(CHAR_DATA * ch, CHAR_DATA * victim)
{
	int inroom_members, koef = 100, maxlevel, rmrt;
	CHAR_DATA *k;
	struct follow_type *f;
	int leader_inroom;

	maxlevel = GET_LEVEL(ch);

	if (!(k = ch->master))
		k = ch;

	// k - ���������� �� ������ ������
	leader_inroom = (AFF_FLAGGED(k, AFF_GROUP)
					 && (k->in_room == IN_ROOM(ch)));

	// ���������� ����������� � �������
	if (leader_inroom)
	{
		inroom_members = 1;
		maxlevel = GET_LEVEL(k);
	}
	else
		inroom_members = 0;

	// ��������� ������������ ������� � ������
	for (f = k->followers; f; f = f->next)
		if (AFF_FLAGGED(f->follower, AFF_GROUP) && f->follower->in_room == IN_ROOM(ch))
		{
			// �������� ������ ������ � ��� �� �������
			// ���� ������ => PC �������������
			++inroom_members;
			maxlevel = MAX(maxlevel, GET_LEVEL(f->follower));
		}

	// ���������, ���� �� ������ �����, ������� ������� ������, ���� �� �����
	rmrt = MIN(14, (int)GET_REMORT(k));
	if (maxlevel - GET_LEVEL(k) > grouping[(int)GET_CLASS(k)][rmrt] && leader_inroom)
	{
		koef -= 50 + (maxlevel - GET_LEVEL(k) - grouping[(int)GET_CLASS(k)][rmrt]) * 2;
	}
	else	// ���� � ������� ��� �� ���� �� �� ���, ������� �� ������
	{
		for (f = k->followers; f; f = f->next)
		{
			if (AFF_FLAGGED(f->follower, AFF_GROUP) && f->follower->in_room == IN_ROOM(ch))
			{
				rmrt = MIN(14, (int)GET_REMORT(f->follower));
				if (maxlevel - GET_LEVEL(f->follower) >
						grouping[(int)GET_CLASS(f->follower)][rmrt])
				{
					koef -= 50 + (maxlevel - GET_LEVEL(f->follower)
						- grouping[(int)GET_CLASS(f->follower)][rmrt]) * 2;
					break;
				}
			}
		}
	}

	koef = MAX(0, koef);

	// ��������� ������������, ���� � ������� ����� � ���� ��� ���� ���-��
	// �� ������ �� PC (������������� ���� ������ ��� �������� �� ���������)
	if (koef >= 100 && leader_inroom && (inroom_members > 1) && calc_leadership(k))
	{
		koef += 20;
	}

	// ������� �����

	if (leader_inroom)
		perform_group_gain(k, victim, inroom_members, koef);

	for (f = k->followers; f; f = f->next)
		if (AFF_FLAGGED(f->follower, AFF_GROUP) && f->follower->in_room == IN_ROOM(ch))
			perform_group_gain(f->follower, victim, inroom_members, koef);
}

/*
void solo_gain(CHAR_DATA * ch, CHAR_DATA * victim)
{
  int exp;

  if (IS_NPC(ch) || !OK_GAIN_EXP(ch, victim))
     {send_to_char("���� ������ ����� �� ������.\r\n",ch);
      return;
     }

  if (IS_NPC(ch))
     {exp  = MIN(max_exp_gain_npc, GET_EXP(victim));
      exp += MAX(0, (exp * MIN(4, (GET_LEVEL(victim) - GET_LEVEL(ch)))) / 8);
     }
  else
     {exp = get_extend_exp(GET_EXP(victim), ch, victim);
      exp = MIN(max_exp_gain_pc(ch), exp);
     };

  if (!IS_NPC(ch))
     exp = MIN(max_exp_gain_pc(ch),exp);
  exp = MAX(1,exp);

  if (exp > 1)
     {sprintf(buf2, "��� ���� ��������� �� %d %s.\r\n", exp, desc_count(exp, WHAT_POINT));
      send_to_char(buf2, ch);
     }
  else
    send_to_char("��� ���� ��������� ����� ���� �� ��������� ��������.\r\n", ch);

  gain_exp(ch, exp);
  change_alignment(ch, victim);
}
*/


char *replace_string(const char *str, const char *weapon_singular, const char *weapon_plural)
{
	static char buf[256];
	char *cp = buf;

	for (; *str; str++)
	{
		if (*str == '#')
		{
			switch (*(++str))
			{
			case 'W':
				for (; *weapon_plural; *(cp++) = *(weapon_plural++));
				break;
			case 'w':
				for (; *weapon_singular; *(cp++) = *(weapon_singular++));
				break;
			default:
				*(cp++) = '#';
				break;
			}
		}
		else
			*(cp++) = *str;

		*cp = 0;
	}			/* For */

	return (buf);
}

/* message for doing damage with a weapon */
void dam_message(int dam, CHAR_DATA * ch, CHAR_DATA * victim, int w_type)
{
	char *buf;
	int msgnum;

	static struct dam_weapon_type
	{
		const char *to_room;
		const char *to_char;
		const char *to_victim;
	} dam_weapons[] =
	{

		/* use #w for singular (i.e. "slash") and #W for plural (i.e. "slashes") */

		{
			"$n �������$u #W $N3, �� ���������$u.",	/* 0: 0      0 */
			"�� ���������� #W $N3, �� ������������.",
			"$n �������$u #W ���, �� ���������$u."
		}, {
			"$n �������� #w$g $N3.",	/*  1..5 1 */
			"�� �������� #w� $N3.",
			"$n �������� #w$g ���."
		}, {
			"$n ������ #w$g $N3.",	/*  6..11  2 */
			"�� ������ #w� $N3.",
			"$n ������ #w$g ���."
		}, {
			"$n #w$g $N3.",	/*  12..18   3 */
			"�� #w� $N3.",
			"$n #w$g ���."
		}, {
			"$n #w$g $N3.",	/* 19..26  4 */
			"�� #w� $N3.",
			"$n #w$g ���."
		}, {
			"$n ������ #w$g $N3.",	/* 27..35  5 */
			"�� ������ #w� $N3.",
			"$n ������ #w$g ���."
		}, {
			"$n ����� ������ #w$g $N3.",	/*  36..45 6  */
			"�� ����� ������ #w� $N3.",
			"$n ����� ������ #w$g ���."
		}, {
			"$n ����������� ������ #w$g $N3.",	/*  46..55  7 */
			"�� ����������� ������ #w� $N3.",
			"$n ����������� ������ #w$g ���."
		}, {
			"$n ������ #w$g $N3.",	/*  56..96   8 */
			"�� ������ #w� $N3.",
			"$n ������ #w$g ���."
		}, {
			"$n ����� ������ #w$g $N3.",	/*    97..136  9  */
			"�� ����� ������ #w� $N3.",
			"$n ����� ������ #w$g ���."
		}, {
			"$n ����������� ������ #w$g $N3.",	/*   137..176  10 */
			"�� ����������� ������ #w� $N3.",
			"$n ����������� ������ #w$g ���."
		}, {
			"$n ���������� ������ #w$g $N3.",	/*    177..216  11 */
			"�� ���������� ������ #w� $N3.",
			"$n ���������� ������ #w$g ���."
		}, {
			"$n ������ #w$g $N3.",	/*    217..256  13 */
			"�� ������ #w� $N3.",
			"$n ������ #w$g ���."
		}, {
			"$n ����������� #w$g $N3.",	/*    257..296  15 */
			"�� ����������� #w� $N3.",
			"$n ����������� #w$g ���."
		}, {
			"$n ���������� #w$g $N3.",	/* 297+  16 */
			"�� ���������� #w� $N3.",
			"$n ���������� #w$g ���."
		}
	};


	if (w_type >= TYPE_HIT && w_type < TYPE_MAGIC)
		w_type -= TYPE_HIT;	/* Change to base of table with text */
	else
		w_type = TYPE_HIT;

	if (dam == 0)
		msgnum = 0;
	else if (dam <= 5)
		msgnum = 1;
	else if (dam <= 11)
		msgnum = 2;
	else if (dam <= 18)
		msgnum = 3;
	else if (dam <= 26)
		msgnum = 4;
	else if (dam <= 35)
		msgnum = 5;
	else if (dam <= 45)
		msgnum = 6;
	else if (dam <= 56)
		msgnum = 7;
	else if (dam <= 96)
		msgnum = 8;
	else if (dam <= 136)
		msgnum = 9;
	else if (dam <= 176)
		msgnum = 10;
	else if (dam <= 216)
		msgnum = 11;
	else if (dam <= 256)
		msgnum = 12;
	else if (dam <= 296)
		msgnum = 13;
	else
		msgnum = 14;

	/* damage message to onlookers */

	buf = replace_string(dam_weapons[msgnum].to_room,
						 attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_NOTVICT);

	/* damage message to damager */
	if (dam)
		send_to_char("&Y&q", ch);
	else
		send_to_char("&y&q", ch);
	buf = replace_string(dam_weapons[msgnum].to_char,
						 attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_CHAR);
	send_to_char("&Q&n", ch);

	/* damage message to damagee */
	/*if (dam)
		send_to_char(CCIRED(victim, C_CMP), victim);
	else
		send_to_char(CCIRED(victim, C_CMP), victim);*/
	send_to_char("&R&q", victim);
	buf = replace_string(dam_weapons[msgnum].to_victim,
						 attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_VICT | TO_SLEEP);
	send_to_char("&Q&n", victim);
//  sprintf(buf,"���������� ����������� - %d\n",dam);
//  send_to_char(buf,ch);
}

/* Alterate equipment
 *
 */
void alterate_object(OBJ_DATA * obj, int dam, int chance)
{
	if (!obj)
		return;
	dam = number(0, dam * (material_value[GET_OBJ_MATER(obj)] + 30) /
				 MAX(1, GET_OBJ_MAX(obj) *
					 (IS_OBJ_STAT(obj, ITEM_NODROP) ? 5 :
					  IS_OBJ_STAT(obj, ITEM_BLESS) ? 15 : 10) * (GET_OBJ_SKILL(obj) == SKILL_BOWS ? 3 : 1)));

	if (dam > 0 && chance >= number(1, 100))
	{
		if (dam > 1 && obj->worn_by && GET_EQ(obj->worn_by, WEAR_SHIELD) == obj)
		{
			dam /= 2;
		}
		if ((GET_OBJ_CUR(obj) -= dam) <= 0)
		{
			if (obj->worn_by)
				act("$o ��������$U, �� �������� �����������.", FALSE, obj->worn_by, obj, 0, TO_CHAR);
			else if (obj->carried_by)
				act("$o ��������$U, �� �������� �����������.", FALSE, obj->carried_by, obj, 0, TO_CHAR);
			extract_obj(obj);
		}
	}
}

void alt_equip(CHAR_DATA * ch, int pos, int dam, int chance)
{
	// calculate chance if
	if (pos == NOWHERE)
	{
		pos = number(0, 100);
		if (pos < 3)
			pos = WEAR_FINGER_R + number(0, 1);
		else if (pos < 6)
			pos = WEAR_NECK_1 + number(0, 1);
		else if (pos < 20)
			pos = WEAR_BODY;
		else if (pos < 30)
			pos = WEAR_HEAD;
		else if (pos < 45)
			pos = WEAR_LEGS;
		else if (pos < 50)
			pos = WEAR_FEET;
		else if (pos < 58)
			pos = WEAR_HANDS;
		else if (pos < 66)
			pos = WEAR_ARMS;
		else if (pos < 76)
			pos = WEAR_SHIELD;
		else if (pos < 86)
			pos = WEAR_ABOUT;
		else if (pos < 90)
			pos = WEAR_WAIST;
		else if (pos < 94)
			pos = WEAR_WRIST_R + number(0, 1);
		else
			pos = WEAR_HOLD;
	}

	if (pos <= 0 || pos > WEAR_BOTHS || !GET_EQ(ch, pos) || dam < 0)
		return;
	alterate_object(GET_EQ(ch, pos), dam, chance);
}

/*  Global variables for critical damage */
int was_critic = FALSE;
int dam_critic = 0;

void haemorragia(CHAR_DATA * ch, int percent)
{
	AFFECT_DATA af[3];
	int i;

	af[0].type = SPELL_HAEMORRAGIA;
	af[0].location = APPLY_HITREG;
	af[0].modifier = -percent;
	af[0].duration = pc_duration(ch, number(1, 31 - GET_REAL_CON(ch)), 0, 0, 0, 0);
	af[0].bitvector = 0;
	af[0].battleflag = 0;
	af[1].type = SPELL_HAEMORRAGIA;
	af[1].location = APPLY_MOVEREG;
	af[1].modifier = -percent;
	af[1].duration = af[0].duration;
	af[1].bitvector = 0;
	af[1].battleflag = 0;
	af[2].type = SPELL_HAEMORRAGIA;
	af[2].location = APPLY_MANAREG;
	af[2].modifier = -percent;
	af[2].duration = af[0].duration;
	af[2].bitvector = 0;
	af[2].battleflag = 0;

	for (i = 0; i < 3; i++)
		affect_join(ch, &af[i], TRUE, FALSE, TRUE, FALSE);
}


int compute_critical(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
	const char *to_char = NULL, *to_vict = NULL;
	AFFECT_DATA af[4];
	OBJ_DATA *obj;
	int i, unequip_pos = 0;

	if (!dam_critic)
		return(dam);

	for (i = 0; i < 4; i++)
	{
		af[i].type = 0;
		af[i].location = APPLY_NONE;
		af[i].bitvector = 0;
		af[i].modifier = 0;
		af[i].battleflag = 0;
		af[i].duration = pc_duration(victim, 2, 0, 0, 0, 0);
	}

	was_critic = FALSE;
	switch (number(1, 10))
	{
	case 1:
	case 2:
	case 3:
	case 4:		// FEETS
		switch (dam_critic)
		{
		case 1:
		case 2:
		case 3:
			// Nothing
			return dam;
		case 5:	// Hit genus, victim bashed, speed/2
			SET_AF_BATTLE(victim, EAF_SLOW);
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 10);
		case 4:	// victim bashed
			if (GET_POS(victim) > POS_SITTING)
				GET_POS(victim) = POS_SITTING;
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			to_char = "�������� $N3 �� �����";
			to_vict = "��������� ��� ������";
			break;
		case 6:	// foot damaged, speed/2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 9);
			to_char = "��������� �������� $N1";
			to_vict = "������� ��� �������";
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 7:
		case 9:	// armor damaged else foot damaged, speed/4
			if (GET_EQ(victim, WEAR_LEGS))
				alt_equip(victim, WEAR_LEGS, 100, 100);
			else
			{
				dam *= (ch->get_skill(SKILL_PUNCTUAL) / 8);
				to_char = "��������� �������� $N1";
				to_vict = "������� ��� ����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_NOFLEE;
				SET_AF_BATTLE(victim, EAF_SLOW);
			}
			break;
		case 8:	// femor damaged, no speed
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "������ ��������� �������� $N1";
			to_vict = "������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 10:	// genus damaged, no speed, -2HR
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "������ ��������� �������� $N1";
			to_vict = "���������� ��� ������";
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			af[0].bitvector = AFF_NOFLEE;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 11:	// femor damaged, no speed, no attack
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "������ $N3 �� �����";
			to_vict = "���������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		default:	// femor damaged, no speed, no attack
			if (dam_critic > 12)
				dam *= (ch->get_skill(SKILL_PUNCTUAL) / 5);
			else
				dam *= (ch->get_skill(SKILL_PUNCTUAL) / 6);
			to_char = "������ $N3 �� �����";
			to_vict = "����������� ��� ����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 50);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		}
		break;
	case 5:		//  ABDOMINAL
		switch (dam_critic)
		{
		case 1:
		case 2:
		case 3:
			// nothing
			return dam;
		case 4:	// waits 1d6
			WAIT_STATE(victim, number(2, 6) * PULSE_VIOLENCE);
			to_char = "����� $N2 �������";
			to_vict = "����� ��� �������";
			break;

		case 5:	// abdomin damaged, waits 1, speed/2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 8);
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			to_char = "������ $N3 � �����";
			to_vict = "������ ��� � �����";
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 6:	// armor damaged else dam*3, waits 1d6
			WAIT_STATE(victim, number(2, 6) * PULSE_VIOLENCE);
			if (GET_EQ(victim, WEAR_WAIST))
				alt_equip(victim, WEAR_WAIST, 100, 100);
			else
				dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "��������� $N2 �����";
			to_vict = "��������� ��� �����";
			break;
		case 7:
		case 8:	// abdomin damage, speed/2, HR-2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 6);
			to_char = "������ $N3 � �����";
			to_vict = "������ ��� � �����";
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			af[0].bitvector = AFF_NOFLEE;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 9:	// armor damaged, abdomin damaged, speed/2, HR-2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 5);
			alt_equip(victim, WEAR_BODY, 100, 100);
			to_char = "������ $N3 � �����";
			to_vict = "������ ��� � �����";
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			af[0].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 10:	// abdomin damaged, no speed, no attack
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 4);
			to_char = "��������� $N2 �����";
			to_vict = "��������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 11:	// abdomin damaged, no speed, no attack
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 3);
			to_char = "��������� $N2 �����";
			to_vict = "��������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 40);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		default:	// abdomin damaged, hits = 0
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			to_char = "���������� $N2 �����";
			to_vict = "���������� ��� �����";
			haemorragia(victim, 60);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		}
		break;
	case 6:
	case 7:		// CHEST
		switch (dam_critic)
		{
		case 1:
		case 2:
		case 3:
			// nothing
			return dam;
		case 4:	// waits 1d4, bashed
			WAIT_STATE(victim, number(2, 5) * PULSE_VIOLENCE);
			if (GET_POS(victim) > POS_SITTING)
				GET_POS(victim) = POS_SITTING;
			to_char = "��������� $N2 �����";
			to_vict = "��������� ��� �����";
			break;
		case 5:	// chest damaged, waits 1, speed/2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 6);
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			to_char = "��������� $N2 ��������";
			to_vict = "��������� ��� ��������";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_NOFLEE;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 6:	// shield damaged, chest damaged, speed/2
			alt_equip(victim, WEAR_SHIELD, 100, 100);
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 6);
			to_char = "��������� $N2 ��������";
			to_vict = "��������� ��� ��������";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_NOFLEE;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 7:	// srmor damaged, chest damaged, speed/2, HR-2
			alt_equip(victim, WEAR_BODY, 100, 100);
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 5);
			to_char = "��������� $N2 ��������";
			to_vict = "��������� ��� ��������";
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			af[0].bitvector = AFF_NOFLEE;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 8:	// chest damaged, no speed, no attack
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 5);
			to_char = "������ $N3 �� �����";
			to_vict = "��������� ��� ��������";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 9:	// chest damaged, speed/2, HR-2
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 4);
			to_char = "��������� $N3 �������� ������";
			to_vict = "������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 20);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 10:	// chest damaged, no speed, no attack
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 4);
			to_char = "������ $N3 �� �����";
			to_vict = "������� ��� �����";
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 40);
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		case 11:	// chest crushed, hits 0
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			haemorragia(victim, 50);
			to_char = "������ $N3 �� �����";
			to_vict = "��������� ��� �����";
			break;
		default:	// chest crushed, killing
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			haemorragia(victim, 60);
			to_char = "������ $N3 �� �����";
			to_vict = "���������� ��� �����";
			break;
		}
		break;
	case 8:
	case 9:		// HANDS
		switch (dam_critic)
		{
		case 1:
		case 2:
		case 3:
			return dam;
		case 4:	// hands damaged, weapon/shield putdown
			to_char = "�������� ������ $N1";
			to_vict = "������ ��� ����";
			if (GET_EQ(victim, WEAR_BOTHS))
				unequip_pos = WEAR_BOTHS;
			else if (GET_EQ(victim, WEAR_WIELD))
				unequip_pos = WEAR_WIELD;
			else if (GET_EQ(victim, WEAR_HOLD))
				unequip_pos = WEAR_HOLD;
			else if (GET_EQ(victim, WEAR_SHIELD))
				unequip_pos = WEAR_SHIELD;
			break;
		case 5:	// hands damaged, shield damaged/weapon putdown
			to_char = "�������� ������ $N1";
			to_vict = "������ ��� � ����";
			if (GET_EQ(victim, WEAR_SHIELD))
				alt_equip(victim, WEAR_SHIELD, 100, 100);
			else if (GET_EQ(victim, WEAR_BOTHS))
				unequip_pos = WEAR_BOTHS;
			else if (GET_EQ(victim, WEAR_WIELD))
				unequip_pos = WEAR_WIELD;
			else if (GET_EQ(victim, WEAR_HOLD))
				unequip_pos = WEAR_HOLD;
			break;

		case 6:	// hands damaged, HR-2, shield putdown
			to_char = "�������� ������ $N1";
			to_vict = "������� ��� ����";
			if (GET_EQ(victim, WEAR_SHIELD))
				unequip_pos = WEAR_SHIELD;
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			break;
		case 7:	// armor damaged, hand damaged if no armour
			if (GET_EQ(victim, WEAR_ARMS))
				alt_equip(victim, WEAR_ARMS, 100, 100);
			else
				alt_equip(victim, WEAR_HANDS, 100, 100);
			if (!GET_EQ(victim, WEAR_ARMS) && !GET_EQ(victim, WEAR_HANDS))
				dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "�������� ����� $N1";
			to_vict = "��������� ��� ����";
			break;
		case 8:	// shield damaged, hands damaged, waits 1
			alt_equip(victim, WEAR_SHIELD, 100, 100);
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 7);
			to_char = "���������� $N3";
			to_vict = "��������� ��� ����";
			break;
		case 9:	// weapon putdown, hands damaged, waits 1d4
			WAIT_STATE(victim, number(2, 4) * PULSE_VIOLENCE);
			if (GET_EQ(victim, WEAR_BOTHS))
				unequip_pos = WEAR_BOTHS;
			else if (GET_EQ(victim, WEAR_WIELD))
				unequip_pos = WEAR_WIELD;
			else if (GET_EQ(victim, WEAR_HOLD))
				unequip_pos = WEAR_HOLD;
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 6);
			to_char = "���������� $N3";
			to_vict = "��������� ��� ����";
			break;
		case 10:	// hand damaged, no attack this
			if (!AFF_FLAGGED(victim, AFF_STOPRIGHT))
			{
				to_char = "�������� ����� $N1";
				to_vict = "����������� ��� ������ ����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPRIGHT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			else if (!AFF_FLAGGED(victim, AFF_STOPLEFT))
			{
				to_char = "�������� ����� $N1";
				to_vict = "����������� ��� ����� ����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPLEFT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			else
			{
				to_char = "������ $N3 �� �����";
				to_vict = "������ ��� �� �����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPFIGHT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			haemorragia(victim, 20);
			break;
		default:	// no hand attack, no speed, dam*2 if >= 13
			if (!AFF_FLAGGED(victim, AFF_STOPRIGHT))
			{
				to_char = "�������� ������ $N1";
				to_vict = "����������� ��� ������ ����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPRIGHT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			else if (!AFF_FLAGGED(victim, AFF_STOPLEFT))
			{
				to_char = "�������� ������ $N1";
				to_vict = "����������� ��� ����� ����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPLEFT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			else
			{
				to_char = "������ $N3 �� �����";
				to_vict = "������ ��� �� �����";
				af[0].type = SPELL_BATTLE;
				af[0].bitvector = AFF_STOPFIGHT;
				af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			}
			af[1].type = SPELL_BATTLE;
			af[1].bitvector = AFF_NOFLEE;
			haemorragia(victim, 30);
			if (dam_critic >= 13)
				dam *= ch->get_skill(SKILL_PUNCTUAL) / 5;
			SET_AF_BATTLE(victim, EAF_SLOW);
			break;
		}
		break;
	default:		// HEAD
		switch (dam_critic)
		{
		case 1:
		case 2:
		case 3:
			// nothing
			return dam;
		case 4:	// waits 1d6
			WAIT_STATE(victim, number(2, 6) * PULSE_VIOLENCE);
			to_char = "�������� $N2 ��������";
			to_vict = "�������� ���� ��������";
			break;

		case 5:	// head damaged, cap putdown, waits 1, HR-2 if no cap
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			if (GET_EQ(victim, WEAR_HEAD))
				unequip_pos = WEAR_HEAD;
			else
			{
				af[0].type = SPELL_BATTLE;
				af[0].location = APPLY_HITROLL;
				af[0].modifier = -2;
			}
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 4);
			to_char = "��������� $N2 ������";
			to_vict = "��������� ��� ������";
			break;
		case 6:	// head damaged
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -2;
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 4);
			to_char = "��������� $N2 ������";
			to_vict = "��������� ��� ������";
			break;
		case 7:	// cap damaged, waits 1d6, speed/2, HR-4
			WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
			alt_equip(victim, WEAR_HEAD, 100, 100);
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_HITROLL;
			af[0].modifier = -4;
			af[0].bitvector = AFF_NOFLEE;
			to_char = "������ $N3 � ������";
			to_vict = "������ ��� � ������";
			break;
		case 8:	// cap damaged, hits 0
			WAIT_STATE(victim, 4 * PULSE_VIOLENCE);
			alt_equip(victim, WEAR_HEAD, 100, 100);
			//dam = GET_HIT(victim);
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			to_char = "������ � $N1 ��������";
			to_vict = "������ � ��� ��������";
			haemorragia(victim, 20);
			break;
		case 9:	// head damaged, no speed, no attack
			af[0].type = SPELL_BATTLE;
			af[0].bitvector = AFF_STOPFIGHT;
			af[0].duration = pc_duration(victim, 1, 0, 0, 0, 0);
			haemorragia(victim, 30);
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 3);
			to_char = "�������� $N3 � ����������";
			to_vict = "�������� ��� � ����������";
			break;
		case 10:	// head damaged, -1 INT/WIS/CHA
			dam *= (ch->get_skill(SKILL_PUNCTUAL) / 2);
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_INT;
			af[0].modifier = -1;
			af[0].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[0].battleflag = AF_DEADKEEP;
			af[1].type = SPELL_BATTLE;
			af[1].location = APPLY_WIS;
			af[1].modifier = -1;
			af[1].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[1].battleflag = AF_DEADKEEP;
			af[2].type = SPELL_BATTLE;
			af[2].location = APPLY_CHA;
			af[2].modifier = -1;
			af[2].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[2].battleflag = AF_DEADKEEP;
			af[3].type = SPELL_BATTLE;
			af[3].bitvector = AFF_STOPFIGHT;
			af[3].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			haemorragia(victim, 50);
			to_char = "������� � $N1 �����";
			to_vict = "������� � ��� �����";
			break;
		case 11:	// hits 0, WIS/2, INT/2, CHA/2
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_INT;
			af[0].modifier = -victim->get_int() / 2;
			af[0].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[0].battleflag = AF_DEADKEEP;
			af[1].type = SPELL_BATTLE;
			af[1].location = APPLY_WIS;
			af[1].modifier = -victim->get_wis() / 2;
			af[1].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[1].battleflag = AF_DEADKEEP;
			af[2].type = SPELL_BATTLE;
			af[2].location = APPLY_CHA;
			af[2].modifier = -victim->get_cha() / 2;
			af[2].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[2].battleflag = AF_DEADKEEP;
			haemorragia(victim, 60);
			to_char = "������� � $N1 �����";
			to_vict = "������� � ��� �����";
			break;
		default:	// killed
			af[0].type = SPELL_BATTLE;
			af[0].location = APPLY_INT;
			af[0].modifier = -victim->get_int() / 2;
			af[0].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[0].battleflag = AF_DEADKEEP;
			af[1].type = SPELL_BATTLE;
			af[1].location = APPLY_WIS;
			af[1].modifier = -victim->get_wis() / 2;
			af[1].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[1].battleflag = AF_DEADKEEP;
			af[2].type = SPELL_BATTLE;
			af[2].location = APPLY_CHA;
			af[2].modifier = -victim->get_cha() / 2;
			af[2].duration = pc_duration(victim, number(1, 6) * 24, 0, 0, 0, 0);
			af[2].battleflag = AF_DEADKEEP;
			dam *= ch->get_skill(SKILL_PUNCTUAL) / 2;
			to_char = "���������� $N2 ������";
			to_vict = "���������� ��� ������";
			haemorragia(victim, 90);
			break;
		}
		break;
	}

	for (i = 0; i < 4; i++)
		if (af[i].type)
			affect_join(victim, af + i, TRUE, FALSE, TRUE, FALSE);
	if (to_char)
	{
		sprintf(buf, "&G&q���� ������ ��������� %s.&Q&n", to_char);
		act(buf, FALSE, ch, 0, victim, TO_CHAR);
		sprintf(buf, "������ ��������� $n1 %s.", to_char);
		act(buf, TRUE, ch, 0, victim, TO_NOTVICT);
	}
	if (to_vict)
	{
		sprintf(buf, "&R&q������ ��������� $n1 %s.&Q&n", to_vict);
		act(buf, FALSE, ch, 0, victim, TO_VICT);
	}
	if (unequip_pos && GET_EQ(victim, unequip_pos))
	{
		obj = unequip_char(victim, unequip_pos);
		if (!IS_NPC(victim) && ROOM_FLAGGED(IN_ROOM(victim), ROOM_ARENA))
			obj_to_char(obj, victim);
		else
			obj_to_room(obj, IN_ROOM(victim));
		obj_decay(obj);
	}
	if (!IS_NPC(victim))
	{
		dam /= 5;
	}
	return calculate_resistance_coeff(victim, VITALITY_RESISTANCE, dam);
}

/**
* ������ ��������� ������ ����� � ������������� ����.
* �������: 1 + ((����-25)*0.4 + �����*0.2)/10 * ����/5,
* � ������ ��������� �������� �� 1 �� 2.6 � �����������
* �������������� 62.5% �� ���� � 37.5% �� ������ + ������ �� 5 �����.
* ����������� �� ��������� ��� �������� ����� � ���������.
*/
int calculate_strconc_damage(CHAR_DATA * ch, OBJ_DATA * wielded, int damage)
{
	if (IS_NPC(ch)
		|| GET_REAL_STR(ch) <= 25
		|| !can_use_feat(ch, STRENGTH_CONCETRATION_FEAT)
		|| GET_AF_BATTLE(ch, EAF_IRON_WIND)
		|| GET_AF_BATTLE(ch, EAF_STUPOR))
	{
		return damage;
	}
	float str_mod = (GET_REAL_STR(ch) - 25) * 0.4;
	float lvl_mod = GET_LEVEL(ch) * 0.2;
	float rmt_mod = MIN(5, GET_REMORT(ch)) / 5.0;
	float res_mod = 1 + (str_mod + lvl_mod) / 10.0 * rmt_mod;

	return static_cast<int>(damage * res_mod);
}

/**
* ������ �������� ������ �� �������� �����.
* (�����/5 + �������*3) * (�������/(10 + �������/2)) * (�����/30)
*/
int calculate_noparryhit_dmg(CHAR_DATA * ch, OBJ_DATA * wielded)
{
	if (!ch->get_skill(SKILL_NOPARRYHIT)) return 0;

	float weap_dmg = (((GET_OBJ_VAL(wielded, 2) + 1) / 2.0) * GET_OBJ_VAL(wielded, 1));
	float weap_mod = weap_dmg / (10 + weap_dmg / 2);
	float level_mod = static_cast<float>(GET_LEVEL(ch)) / 30;
	float skill_mod = static_cast<float>(ch->get_skill(SKILL_NOPARRYHIT)) / 5;

	return static_cast<int>((skill_mod + GET_REMORT(ch) * 3) * weap_mod * level_mod);
}

void might_hit_bash(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (MOB_FLAGGED(victim, MOB_NOBASH) || !GET_MOB_HOLD(victim))
	{
		return;
	}

	act("$n ��������� �������$u �� �����.", TRUE, victim, 0, 0, TO_ROOM);
	WAIT_STATE(victim, 3 * PULSE_VIOLENCE);

	if (GET_POS(victim) > POS_SITTING)
	{
		GET_POS(victim) = POS_SITTING;
		send_to_char(victim, "&R&q����������� ���� %s ���� ��� � ���.&Q&n\r\n", PERS(ch, victim, 1));
	}
}

int extdamage(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int attacktype, OBJ_DATA * wielded, int mayflee)
{
	if (!ch || ch->purged() || !victim || victim->purged())
	{
		log("SYSERROR: ch = %s, victim = %s (%s:%d)",
				ch ? (ch->purged() ? "purged" : "true") : "false",
				victim ? (victim->purged() ? "purged" : "true") : "false",
				__FILE__, __LINE__);
		return 0;
	}

	int prob, percent = 0, lag = 0, k, mem_dam = dam;
	AFFECT_DATA af;

	if (dam < 0)
		dam = 0;

	// MIGHT_HIT
	if (attacktype == TYPE_HIT && GET_AF_BATTLE(ch, EAF_MIGHTHIT) && GET_WAIT(ch) <= 0)
	{
		CLR_AF_BATTLE(ch, EAF_MIGHTHIT);
		if (IS_NPC(ch) ||
				IS_IMMORTAL(ch) ||
				!(GET_EQ(ch, WEAR_BOTHS) || GET_EQ(ch, WEAR_WIELD) ||
				  GET_EQ(ch, WEAR_HOLD) || GET_EQ(ch, WEAR_LIGHT) ||
				  GET_EQ(ch, WEAR_SHIELD) || GET_AF_BATTLE(ch, EAF_TOUCH)))
		{
			percent = number(1, skill_info[SKILL_MIGHTHIT].max_percent);
			prob = train_skill(ch, SKILL_MIGHTHIT, skill_info[SKILL_MIGHTHIT].max_percent, victim);
			if (GET_MOB_HOLD(victim))
			{
				prob = MAX(prob, percent);
			}
			if (IS_IMMORTAL(victim))
				prob = 0;
			if (prob * 100 / percent < 100 || dam == 0)
			{
				sprintf(buf, "&c&q��� ����������� ���� ������ �������.&Q&n\r\n");
				send_to_char(buf, ch);
				lag = 3;
				dam = 0;
			}
			else if (prob * 100 / percent < 150)
			{
				sprintf(buf, "&b&q��� ����������� ���� ����� %s.&Q&n\r\n",
						PERS(victim, ch, 3));
				send_to_char(buf, ch);
				lag = 1;
				WAIT_STATE(victim, PULSE_VIOLENCE);
				af.type = SPELL_BATTLE;
				af.bitvector = AFF_STOPFIGHT;
				af.location = 0;
				af.modifier = 0;
				af.duration = pc_duration(victim, 1, 0, 0, 0, 0);
				af.battleflag = AF_BATTLEDEC | AF_PULSEDEC;
				affect_join(victim, &af, TRUE, FALSE, TRUE, FALSE);
				sprintf(buf,
						"&R&q���� �������� ������������ ����� ����� %s.&Q&n\r\n",
						PERS(ch, victim, 1));
				send_to_char(buf, victim);
				act("$N ���������$U �� ������������ ����� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
				if (!number(0, 2))
				{
					might_hit_bash(ch, victim);
				}
			}
			else if (prob * 100 / percent < 400)
			{
				sprintf(buf, "&g&q��� ����������� ���� �������� %s.&Q&n\r\n",
						PERS(victim, ch, 3));
				send_to_char(buf, ch);
				lag = 2;
				dam += (dam / 1);
				WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
				af.type = SPELL_BATTLE;
				af.bitvector = AFF_STOPFIGHT;
				af.location = 0;
				af.modifier = 0;
				af.duration = pc_duration(victim, 2, 0, 0, 0, 0);
				af.battleflag = AF_BATTLEDEC | AF_PULSEDEC;
				affect_join(victim, &af, TRUE, FALSE, TRUE, FALSE);
				sprintf(buf,
						"&R&q���� �������� ���������� ����� ����� %s.&Q&n\r\n",
						PERS(ch, victim, 1));
				send_to_char(buf, victim);
				act("$N ��������$U �� ������������ ����� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
				if (!number(0, 1))
				{
					might_hit_bash(ch, victim);
				}
			}
			else
			{
				sprintf(buf, "&G&q��� ����������� ���� ������ %s.&Q&n\r\n",
						PERS(victim, ch, 3));
				send_to_char(buf, ch);
				lag = 2;
				dam *= 4;
				WAIT_STATE(victim, 3 * PULSE_VIOLENCE);
				af.type = SPELL_BATTLE;
				af.bitvector = AFF_STOPFIGHT;
				af.location = 0;
				af.modifier = 0;
				af.duration = pc_duration(victim, 3, 0, 0, 0, 0);
				af.battleflag = AF_BATTLEDEC | AF_PULSEDEC;
				affect_join(victim, &af, TRUE, FALSE, TRUE, FALSE);
				sprintf(buf, "&R&q���� �������� �������� ����� ����� %s.&Q&n\r\n",
						PERS(ch, victim, 1));
				send_to_char(buf, victim);
				act("$N �������$U �� ������������ ����� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
				might_hit_bash(ch, victim);
			}
			if (!WAITLESS(ch))
				WAIT_STATE(ch, lag * PULSE_VIOLENCE);
		}
	}
	// STUPOR
	else if (GET_AF_BATTLE(ch, EAF_STUPOR) && GET_WAIT(ch) <= 0)
	{
		CLR_AF_BATTLE(ch, EAF_STUPOR);
		if (IS_NPC(ch)
			|| IS_IMMORTAL(ch)
			|| (wielded && GET_OBJ_WEIGHT(wielded) > 18
				&& GET_OBJ_SKILL(wielded) != SKILL_BOWS
				&& !GET_AF_BATTLE(ch, EAF_PARRY)
				&& !GET_AF_BATTLE(ch, EAF_MULTYPARRY)))
		{
			percent = number(1, skill_info[SKILL_STUPOR].max_percent);
			prob = train_skill(ch, SKILL_STUPOR, skill_info[SKILL_STUPOR].max_percent, victim);
			if (GET_MOB_HOLD(victim))
			{
				prob = MAX(prob, percent * 150 / 100 + 1);
			}
			if (IS_IMMORTAL(victim))
			{
				prob = 0;
			}
			if (prob * 100 / percent < 117 || dam == 0 || MOB_FLAGGED(victim, MOB_NOSTUPOR))
			{
				sprintf(buf,
						"&c&q�� ���������� �������� %s, �� �� ������.&Q&n\r\n",
						PERS(victim, ch, 3));
				send_to_char(buf, ch);
				lag = 3;
				dam = 0;
			}
			else if (prob * 100 / percent < 300)
			{
				sprintf(buf, "&g&q���� ������ ����� �������� %s.&Q&n\r\n",
						PERS(victim, ch, 3));
				send_to_char(buf, ch);
				lag = 2;
				k = ch->get_skill(SKILL_STUPOR) / 30;
				if (!IS_NPC(victim))
				{
					k = MIN(2, k);
				}
				dam *= MAX(2, number(1, k));
				WAIT_STATE(victim, 3 * PULSE_VIOLENCE);
				sprintf(buf,
						"&R&q���� �������� ������ ���������� ����� ����� %s.&Q&n\r\n",
						PERS(ch, victim, 1));
				send_to_char(buf, victim);
				act("$n �������$a $N3.", TRUE, ch, 0, victim, TO_NOTVICT);
			}
			else
			{
				if (MOB_FLAGGED(victim, MOB_NOBASH))
					sprintf(buf, "&G&q��� ��������� ���� ������� %s.&Q&n\r\n",
							PERS(victim, ch, 3));
				else
					sprintf(buf, "&G&q��� ��������� ���� ���� %s � ���.&Q&n\r\n",
							PERS(victim, ch, 3));
				send_to_char(buf, ch);
				if (MOB_FLAGGED(victim, MOB_NOBASH))
					act("$n ������ ������ �������$a $N3.", TRUE, ch, 0, victim, TO_NOTVICT);
				else
					act("$n ����� ���������� ������ ����$a $N3 � ���.", TRUE, ch,
						0, victim, TO_NOTVICT);
				lag = 2;
				k = ch->get_skill(SKILL_STUPOR) / 20;
				if (!IS_NPC(victim))
				{
					k = MIN(4, k);
				}
				dam *= MAX(3, number(1, k));
				WAIT_STATE(victim, 3 * PULSE_VIOLENCE);
				if (GET_POS(victim) > POS_SITTING && !MOB_FLAGGED(victim, MOB_NOBASH))
				{
					GET_POS(victim) = POS_SITTING;
					sprintf(buf, "&R&q���������� ���� %s ���� ��� � ���.&Q&n\r\n",
							PERS(ch, victim, 1));
					send_to_char(buf, victim);
				}
				else
				{
					sprintf(buf,
							"&R&q���� �������� ������ ���������� ����� ����� %s.&Q&n\r\n",
							PERS(ch, victim, 1));
					send_to_char(buf, victim);
				}
			}
			if (!WAITLESS(ch))
				WAIT_STATE(ch, lag * PULSE_VIOLENCE);
		}
	}
	// ����������� �����
	else if (!MOB_FLAGGED(victim, MOB_PROTECT) && dam && wielded && wielded->timed_spell.is_spell_poisoned() && ch->get_skill(SKILL_POISONED))
	{
		try_weap_poison(ch, victim, wielded);
	}
	// Calculate mob-poisoner
	else if (dam &&
			 IS_NPC(ch) &&
			 NPC_FLAGGED(ch, NPC_POISON) &&
			 !AFF_FLAGGED(ch, AFF_CHARM) &&
			 GET_WAIT(ch) <= 0 &&
			 !AFF_FLAGGED(victim, AFF_POISON) && number(0, 100) < GET_LIKES(ch) + GET_LEVEL(ch) - GET_LEVEL(victim)
			 && !general_savingthrow(ch, victim, SAVING_CRITICAL, - GET_REAL_CON(victim)))
	{
		poison_victim(ch, victim, MAX(1, GET_LEVEL(ch) - GET_LEVEL(victim)) * 10);
	}

	// ���� ���� ���������, ���������� ��� ����� ��������� � �����.
	// ���������� damage � ������������� ������
	return damage(ch, victim, mem_dam >= 0 ? dam : -1, attacktype, mayflee);
}



/*
 * Alert: As of bpl14, this function returns the following codes:
 *	< 0	Victim  died.
 *	= 0	No damage.
 *	> 0	How much damage done.
 */

void char_dam_message(int dam, CHAR_DATA * ch, CHAR_DATA * victim, int attacktype, int mayflee)
{
	if (IN_ROOM(ch) == NOWHERE)
		return;
	switch (GET_POS(victim))
	{
	case POS_MORTALLYW:
		act("$n ���������� �����$a � �����, ���� $m �� �������.", TRUE, victim, 0, 0, TO_ROOM);
		send_to_char("�� ���������� ������ � ������, ���� ��� �� �������.\r\n", victim);
		break;
	case POS_INCAP:
		act("$n ��� �������� � �������� �������. �������� �� $m.", TRUE, victim, 0, 0, TO_ROOM);
		send_to_char("�� ��� �������� � �������� ��������, ��������� ��� ������.\r\n", victim);
		break;
	case POS_STUNNED:
		act("$n ��� ��������, �� �������� $e ��� ������� (������� :).", TRUE, victim, 0, 0, TO_ROOM);
		send_to_char("�������� �������� ���. � ����� �� ��� ���� ����� ����.\r\n", victim);
		break;
	case POS_DEAD:
		if (IS_NPC(victim) && (MOB_FLAGGED(victim, MOB_CORPSE)))
		{
			act("$n ��������$g � ��������$u � ����.", FALSE, victim, 0, 0, TO_ROOM);
			send_to_char("������ ��� ����� � ���� ���� �� �������� !\r\n", victim);
		}
		else
		{
			act("$n �����$a, $s ���� �������� ���������� � ������.", FALSE, victim, 0, 0, TO_ROOM);
			send_to_char("�� ������!  H�� ����� ����...\r\n", victim);
		}
		break;
	default:		/* >= POSITION SLEEPING */
		if (dam > (GET_REAL_MAX_HIT(victim) / 4))
			send_to_char("��� ������������� ������ !\r\n", victim);

		if (dam > 0 && GET_HIT(victim) < (GET_REAL_MAX_HIT(victim) / 4))
		{
			sprintf(buf2,
					"%s �� �������, ����� ���� ���� �� ����������� ��� ������ ! %s\r\n",
					CCRED(victim, C_SPR), CCNRM(victim, C_SPR));
			send_to_char(buf2, victim);
		}
		if (ch != victim &&
				IS_NPC(victim) &&
				GET_HIT(victim) < (GET_REAL_MAX_HIT(victim) / 4) &&
				MOB_FLAGGED(victim, MOB_WIMPY) && mayflee && GET_POS(victim) > POS_SITTING)
			do_flee(victim, NULL, 0, 0);

		if (ch != victim &&
				!IS_NPC(victim) &&
				HERE(victim) &&
				GET_WIMP_LEV(victim) &&
				GET_HIT(victim) < GET_WIMP_LEV(victim) && mayflee && GET_POS(victim) > POS_SITTING)
		{
			send_to_char("�� ������������ � ���������� ������� !\r\n", victim);
			do_flee(victim, NULL, 0, 0);
		}
		break;
	}
}

void gain_battle_exp(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
	if (ch != victim
		&& OK_GAIN_EXP(ch, victim)
		&& GET_EXP(victim) > 0
		&& !AFF_FLAGGED(victim, AFF_CHARM)
		&& !MOB_FLAGGED(victim, MOB_ANGEL)
		&& !IS_NPC(ch))
	{
		int max_exp = MIN(max_exp_gain_pc(ch), (GET_LEVEL(victim) * GET_MAX_HIT(victim) + 4) /
			(5 * MAX(1, GET_REMORT(ch) - MAX_EXP_COEFFICIENTS_USED - 1)));
		double coeff = MIN(dam, GET_HIT(victim)) / static_cast<double>(GET_MAX_HIT(victim));
		int battle_exp = MAX(1, static_cast<int>(max_exp * coeff));
//		int battle_exp = MAX(1, (GET_LEVEL(victim) * MIN(dam, GET_HIT(victim)) + 4) /
//						 (5 * MAX(1, GET_REMORT(ch) - MAX_EXP_COEFFICIENTS_USED - 1)));
		gain_exp(ch, battle_exp);
		ch->dps_add_exp(battle_exp, true);
	}
}

/**
* ��� ������ ���� � 1.5 � �� ���� 1% ����, ��� ���� ����� ������ ����� ������.
*/
void try_remove_extrahits(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if (((!IS_NPC(ch) && ch != victim) || (ch->master && !IS_NPC(ch->master) && ch->master != victim))
		&& !IS_NPC(victim)
		&& GET_POS(victim) != POS_DEAD
		&& GET_HIT(victim) > GET_REAL_MAX_HIT(victim) * 1.5
		&& number(1, 100) == 1)
	{
		GET_HIT(victim) = GET_REAL_MAX_HIT(victim);
		send_to_char(victim, "%s'����%s ���%s ��� ������' - ��������� ����� ����� � ����� ������.%s\r\n",
				CCWHT(victim, C_NRM), GET_CH_POLY_1(victim), GET_CH_EXSUF_1(victim), CCNRM(victim, C_NRM));
		act("�� �������� ���������� ����, �������� $N3 ������.", FALSE, ch, 0, victim, TO_CHAR);
		act("$n �������$g ���������� ����, �������� $N3 ������.", FALSE, ch, 0, victim, TO_NOTVICT);
	}
}

// ��������� �����, ��, ����������, ��������� ��� ���. ���� �� �����
// ���������� ��������� �����
int damage(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int attacktype, int mayflee)
{
	if (!ch || ch->purged() || !victim || victim->purged())
	{
		log("SYSERROR: ch = %s, victim = %s (%s:%d)",
				ch ? (ch->purged() ? "purged" : "true") : "false",
				victim ? (victim->purged() ? "purged" : "true") : "false",
				__FILE__, __LINE__);
		return 0;
	}

	int FS_damage = 0;
	ACMD(do_get);
//  long local_gold = 0;
//  char local_corpse[256];


	if (IN_ROOM(victim) == NOWHERE || IN_ROOM(ch) == NOWHERE || IN_ROOM(ch) != IN_ROOM(victim))
	{
		log("SYSERR: Attempt to damage '%s' in room NOWHERE by '%s'.", GET_NAME(victim), GET_NAME(ch));
		return 0;
	}

	if (GET_POS(victim) <= POS_DEAD)
	{
		log("SYSERR: Attempt to damage corpse '%s' in room #%d by '%s'.",
			GET_NAME(victim), GET_ROOM_VNUM(IN_ROOM(victim)), GET_NAME(ch));
		die(victim, NULL);
		return 0;	/* -je, 7/7/92 */
	}

	// ������ ����� �������� ���� � hit ����� �������� �����
	if (dam >= 0 && damage_mtrigger(ch, victim))
		return 0;

	// Shopkeeper protection
	if (!ok_damage_shopkeeper(ch, victim))
		return 0;

	// No fight mobiles
	if ((IS_NPC(ch) && MOB_FLAGGED(ch, MOB_NOFIGHT)) || (IS_NPC(victim) && MOB_FLAGGED(victim, MOB_NOFIGHT)))
	{
		return 0;
	}

	if (dam > 0)
	{
		// You can't damage an immortal!
		if (IS_GOD(victim))
			dam = 0;
		else if (IS_IMMORTAL(victim) || GET_GOD_FLAG(victim, GF_GODSLIKE))
			dam /= 4;
		else if (GET_GOD_FLAG(victim, GF_GODSCURSE))
			dam *= 2;
	}

// �����������.
// ����-�������� ��� ������ ������, ���� �� ��� ���� ���������������.
// ���� ���� ����� (�������� �� MOB_CLONE), ���� ������ ������.
// ���� ���� ����� ��� ������ (��� ��������� ��� ������), �� ������ ����
// ��� ����� ������ �� �������.

// ������ -- ���� ����, �� ���������� ��������
	if (IS_NPC(victim) && MOB_FLAGGED(victim, MOB_MEMORY))
	{
		if (!IS_NPC(ch))
			remember(victim, ch);
		else if (AFF_FLAGGED(ch, AFF_CHARM) && ch->master && !IS_NPC(ch->master))
		{
			if (MOB_FLAGGED(ch, MOB_CLONE))
				remember(victim, ch->master);
			else if (IN_ROOM(ch->master) == IN_ROOM(victim) && CAN_SEE(victim, ch->master))
				remember(victim, ch->master);
		}
	}

// ������ -- ���� ��� ��� � ����������, ���� ����� �������� :)
	if (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_MEMORY))
	{
		if (!IS_NPC(victim))
			remember(ch, victim);
		else if (AFF_FLAGGED(victim, AFF_CHARM) && victim->master && !IS_NPC(victim->master))
		{
			if (MOB_FLAGGED(victim, MOB_CLONE))
				remember(ch, victim->master);
			else if (IN_ROOM(victim->master) == IN_ROOM(ch) && CAN_SEE(ch, victim->master))
				remember(ch, victim->master);
		}
	}

	//*************** If the attacker is invisible, he becomes visible
	appear(ch);

	// shapirus
	//*************** ���� ������ ��������, � ��� ���� ����� ������� ���
	//*************** �������������� ����������� ������ � ������ �������
	appear(victim);

	//**************** If you attack a pet, it hates your guts

	if (!same_group(ch, victim))
		check_agro_follower(ch, victim);


	if (victim != ch)  	//**************** Start the attacker fighting the victim
	{
		if (GET_POS(ch) > POS_STUNNED && (ch->get_fighting() == NULL))
		{
			pk_agro_action(ch, victim);
			set_fighting(ch, victim);
			npc_groupbattle(ch);
		}
		//***************** Start the victim fighting the attacker
		if (GET_POS(victim) > POS_STUNNED && (victim->get_fighting() == NULL))
		{
			set_fighting(victim, ch);
			npc_groupbattle(victim);
		}
	}

	//*************** If negative damage - return
	if (dam < 0 || IN_ROOM(ch) == NOWHERE || IN_ROOM(victim) == NOWHERE || IN_ROOM(ch) != IN_ROOM(victim))
		return (0);

	// ���� ��� ������������� ����� �� �������, � ��������� ����� �� �����

	// added by WorM(�������) ���������� ���.����� � %
	if(GET_PR(victim) && IS_NPC(victim) && (attacktype <= 0 || attacktype > TOP_SPELL_DEFINE))
	{
		dam = dam - (dam * GET_PR(victim) / 100);
	}

	if (victim != ch)
	{
		if (dam && AFF_FLAGGED(victim, AFF_SHIELD))
		{
			if (attacktype == SKILL_BASH + TYPE_HIT)
				skill_message(dam, ch, victim, attacktype);
			act("���������� ����� ��������� �������� ���� $N1.", FALSE, victim, 0, ch, TO_CHAR);
			act("���������� ����� ������ $N1 ��������� �������� ��� ����.", FALSE, ch, 0, victim, TO_CHAR);
			act("���������� ����� ������ $N1 ��������� �������� ���� $n1.",
				TRUE, ch, 0, victim, TO_NOTVICT);
			return (0);
		}

		if ((dam > 0 && !was_critic && AFF_FLAGGED(victim, AFF_FIRESHIELD))
				&& (attacktype != (TYPE_HIT + SKILL_BACKSTAB))
				&& (attacktype != (TYPE_HIT + SKILL_THROW))
				&& (attacktype <= 0 || attacktype >= LAST_USED_SPELL || !IS_SET(spell_info[attacktype].routines, MAG_WARCRY)))
		{
			FS_damage = dam * 20 / 100;
			dam -= (dam * number(10, 30) / 100);
		}

		if (dam > 0 && !was_critic && AFF_FLAGGED(victim, AFF_ICESHIELD))
		{
			act("������� ��� ������ ����� ����� �� ����.", FALSE, ch, 0, victim, TO_VICT);
			act("������� ��� ������ $N1 ������� ��� ����.", FALSE, ch, 0, victim, TO_CHAR);
			act("������� ��� ������ $N1 ������� ���� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
			dam -= (dam * number(30, 50) / 100);
		}

		if (dam > 0 && !was_critic && AFF_FLAGGED(victim, AFF_AIRSHIELD))
		{
			act("��������� ��� ������� ���� $n1.", FALSE, ch, 0, victim, TO_VICT);
			act("��������� ��� ������ $N1 ������� ��� ����.", FALSE, ch, 0, victim, TO_CHAR);
			act("��������� ��� ������ $N1 ������� ���� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
			dam -= (dam * number(30, 50) / 100);
		}

		if (dam && (IS_WEAPON(attacktype) || attacktype == (SKILL_KICK + TYPE_HIT)))
		{
			alt_equip(victim, NOWHERE, dam, 50);
			if (!was_critic)
			{
				if (GET_CLASS(victim) == CLASS_GUARD && IS_SET(PRF_FLAGS(victim, PRF_AWAKE), PRF_AWAKE))
				{
					// � ����� � ��������� ���������� ������ �� 99
					int decrease = MIN(50, (GET_ABSORBE(victim) + 1) / 2) + GET_ARMOUR(victim);
					// ���� ��������� ��������� ���� 4%
					if (decrease >= number(dam, dam * 25))
					{
						act("���� ������� ��������� ��������� ���� $n1.", FALSE, ch, 0, victim, TO_VICT);
						act("������� $N1 ��������� ��������� ��� ����.", FALSE, ch, 0, victim, TO_CHAR);
						act("������� $N1 ��������� ��������� ���� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
						return 0;
					}
					// ����� + ���������� ������ �� 75% �������� ������
					dam -= (dam * MAX(0, MIN(75, decrease)) / 100);
				}
				else
				{
					// � ���� ��������� ���������� ������ �� 49
					int decrease = MIN(25, (GET_ABSORBE(victim) + 1) / 2) + GET_ARMOUR(victim);
					// ���� ��������� ��������� ���� 2%
					if (decrease >= number(dam, dam * 50))
					{
						act("���� ������� ��������� ��������� ���� $n1.", FALSE, ch, 0, victim, TO_VICT);
						act("������� $N1 ��������� ��������� ��� ����.", FALSE, ch, 0, victim, TO_CHAR);
						act("������� $N1 ��������� ��������� ���� $n1.", TRUE, ch, 0, victim, TO_NOTVICT);
						return 0;
					}
					// ����� + ���������� ������ �� 50% �������� ������
					dam -= (dam * MAX(0, MIN(50, decrease)) / 100);
				}
				/* �������� ����� ��� ���� �����, ���� ����� ��� � ����� ������ �� ����
				   �� ������ �� ��������, ����� �� ������� ������ � 1 ����� */
			}
			else if ((GET_LEVEL(victim) >= 5 || !IS_NPC(ch))
					 && !AFF_FLAGGED(victim, AFF_PRISMATICAURA) && !AFF_FLAGGED(victim, AFF_FIRESHIELD)
					 && !AFF_FLAGGED(victim, AFF_ICESHIELD) && !AFF_FLAGGED(victim, AFF_AIRSHIELD))
				dam = MAX(dam, MIN(GET_REAL_MAX_HIT(victim) / 8, dam * 2));

		}
	}
	else if (MOB_FLAGGED(victim, MOB_PROTECT))
	{
		return (0);
	}
	//*************** Set the maximum damage per round and subtract the hit points
	if (MOB_FLAGGED(victim, MOB_PROTECT))
	{
		act("$n ��������� ��� ������� �����.", FALSE, victim, 0, 0, TO_ROOM);
		return (0);
	}
	// log("[DAMAGE] Compute critic...");
	dam = MAX(dam, 0);
	if (dam && was_critic)
	{
		FS_damage = 0;
		dam = compute_critical(ch, victim, dam);
		if (!dam_critic && attacktype != SPELL_POISON)
		{
			sprintf(buf, "&B&q���� ������ ��������� ������ ������ %s.&Q&n\r\n",
					PERS(victim, ch, 3));
			send_to_char(buf, ch);
			sprintf(buf, "&r&q������ ��������� %s ������ ������ ���.&Q&n\r\n",
					PERS(ch, victim, 1));
			send_to_char(buf, victim);
			/* ����������� ����� �� �������, ������� ����� � ���� ������ */
			//act("������ ��������� $N1 ��������� $n3 �����������.", TRUE, victim, 0, ch, TO_NOTVICT);
		}
	}

	// �� ���� ���� attacktype ��� ������ ��� ������, �� ���� ������
	// �������� � �����, � ������ ������� ������� ���������
	if (attacktype != SKILL_BACKSTAB+TYPE_HIT)
	{
		// + ������� ������ � ��� ��������
		if (AFF_FLAGGED(victim, AFF_SCOPOLIA_POISON))
		{
			int mod = dam * GET_POISON(victim) / 100;
			dam += mod;
		}
	}
	/* �������� ���� ����� */
	dam = MIN(dam, MAX_HITS);

	//*************** Gain exp for the hit
	//Battle exp gain for mobs is DISABLED
	gain_battle_exp(ch, victim, dam);

	int real_dam = dam;
	int over_dam = 0;

	if (attacktype == SPELL_FIRE_SHIELD)
	{
		if ((GET_HIT(victim) -= dam) < 1)
		{
			GET_HIT(victim) = 1;
			real_dam = GET_HIT(victim) - 1;
			over_dam = dam - real_dam;
		}
	}
	else
	{
		if (dam > GET_HIT(victim) + 11)
		{
			real_dam = GET_HIT(victim) + 11;
			over_dam = dam - real_dam;
		}
		GET_HIT(victim) -= dam;
	}

	if (!IS_NPC(ch))
	{
		ch->dps_add_dmg(DpsSystem::PERS_DPS, real_dam, over_dam);
		if (AFF_FLAGGED(ch, AFF_GROUP))
		{
			CHAR_DATA *leader = ch->master ? ch->master : ch;
			leader->dps_add_dmg(DpsSystem::GROUP_DPS, real_dam, over_dam, ch);
		}
	}
	else if (IS_CHARMICE(ch) && ch->master)
	{
		ch->master->dps_add_dmg(DpsSystem::PERS_CHARM_DPS, real_dam, over_dam, ch);
		if (AFF_FLAGGED(ch->master, AFF_GROUP))
		{
			CHAR_DATA *leader = ch->master->master ? ch->master->master : ch->master;
			leader->dps_add_dmg(DpsSystem::GROUP_CHARM_DPS, real_dam, over_dam, ch);
		}
	}

	// ���� ������ � ������ � ���-�� � ������� - ������ ������ ������� ������� �����
	if (GET_HIT(victim) <= 0 && !IS_NPC(victim) && AFF_FLAGGED(victim, AFF_GROUP))
	{
		for (CHAR_DATA *keeper = world[IN_ROOM(victim)]->people; keeper; keeper = keeper->next_in_room)
		{
			if (IS_NPC(keeper) && MOB_FLAGGED(keeper, MOB_ANGEL)
				&& keeper->master && AFF_FLAGGED(keeper->master, AFF_GROUP))
			{
				CHAR_DATA *keeper_leader = keeper->master->master ? keeper->master->master : keeper->master;
				CHAR_DATA *victim_leader = victim->master ? victim->master : victim;

				if ((keeper_leader == victim_leader) && (may_kill_here(keeper->master, ch)))
				{
					pk_agro_action(keeper->master, ch);
					send_to_char(victim, "%s �����������%s ����� ������, ���������� ��� � ���� �����!\r\n",
							GET_PAD(keeper, 0), GET_CH_SUF_1(keeper));
					snprintf(buf, MAX_STRING_LENGTH, "%s �����������%s ����� ������, ���������� %s � ���� �����!",
							GET_PAD(keeper, 0), GET_CH_SUF_1(keeper), GET_PAD(victim, 3));
					act(buf, FALSE, victim, 0, 0, TO_ROOM);

					extract_char(keeper, 0);
					GET_HIT(victim) = MIN(300, GET_MAX_HIT(victim) / 2);
				}
			}
		}
	}

	update_pos(victim);

	if (attacktype != SPELL_POISON && dam > 0)
		try_remove_extrahits(ch, victim);

	// * skill_message sends a message from the messages file in lib/misc.
	//  * dam_message just sends a generic "You hit $n extremely hard.".
	// * skill_message is preferable to dam_message because it is more
	// * descriptive.
	// *
	// * If we are _not_ attacking with a weapon (i.e. a spell), always use
	// * skill_message. If we are attacking with a weapon: If this is a miss or a
	// * death blow, send a skill_message if one exists; if not, default to a
	// * dam_message. Otherwise, always send a dam_message.
	// log("[DAMAGE] Attack message...");

	if (!IS_WEAPON(attacktype))
		skill_message(dam, ch, victim, attacktype);
	else
	{
		if (GET_POS(victim) == POS_DEAD || dam == 0)
		{
			if (!skill_message(dam, ch, victim, attacktype))
				dam_message(dam, ch, victim, attacktype);
		}
		else
		{
			dam_message(dam, ch, victim, attacktype);
		}
	}

	// log("[DAMAGE] Victim message...");
	//******** Use send_to_char -- act() doesn't send message if you are DEAD.
	char_dam_message(dam, ch, victim, attacktype, mayflee);
	// log("[DAMAGE] Flee etc...");

	// ���������, ��� ������ ��� ��� ���. ����� ��� ������� �� ��������.
	// �����, ������� �������� ����������.
	// ����������, ���� ������ � FIRESHIELD,
	// �� ��������� ����������� �� ���������� �� �����
	if (IN_ROOM(ch) != IN_ROOM(victim))
		return dam;

	// *********** Stop someone from fighting if they're stunned or worse
	if ((GET_POS(victim) <= POS_STUNNED) && (victim->get_fighting() != NULL))
	{
		stop_fighting(victim, GET_POS(victim) <= POS_DEAD);
	}
	// *********** Uh oh.  Victim died.
	if (GET_POS(victim) == POS_DEAD)
	{
		CHAR_DATA *killer = NULL;

		if (IS_NPC(victim) || victim->desc)
		{
			if (victim == ch && IN_ROOM(victim) != NOWHERE)
			{
				if (attacktype == SPELL_POISON)
				{
					CHAR_DATA *poisoner;
					for (poisoner = world[IN_ROOM(victim)]->people; poisoner;
							poisoner = poisoner->next_in_room)
						if (poisoner != victim && GET_ID(poisoner) == victim->Poisoner)
							killer = poisoner;
				}
				else if (attacktype == TYPE_SUFFERING)
				{
					CHAR_DATA *attacker;
					for (attacker = world[IN_ROOM(victim)]->people; attacker;
							attacker = attacker->next_in_room)
						if (attacker->get_fighting() == victim)
							killer = attacker;
				}
			}
			if (ch != victim)
				killer = ch;
		}

		if (killer)
		{
			if (AFF_FLAGGED(killer, AFF_GROUP))
// �.�. ������� ������ AFF_GROUP - ����� PC
				group_gain(killer, victim);
			else if ((AFF_FLAGGED(killer, AFF_CHARM) || MOB_FLAGGED(killer, MOB_ANGEL)) && killer->master)
// killer - ������������ NPC � ��������
			{
				if (!IS_NPC(killer->master)
						&& AFF_FLAGGED(killer->master, AFF_GROUP))
// ������ - PC � ������ => ���� ������
					group_gain(killer->master, victim);
				else if (IN_ROOM(killer) == IN_ROOM(killer->master))
					// ������ � ������ � ����� �������
					// ���� �������
				{
					perform_group_gain(killer->master, victim, 1, 100);
					//solo_gain(killer->master, victim);
					//solo_gain(killer,victim);
				}
				// else
				// � ������� �� ����� �� ���������, ��� ������� - ������
// ����� �������� ����  perform_group_gain( killer, victim, 1, 100 );
			}
			else
				// ������ NPC ��� PC ��� �� ����
				perform_group_gain(killer, victim, 1, 100);
		}
		// � ������ ����� ���� ������ ������ � �� (��� ����), � ���� ������� ��� ������ �����
		if (!IS_NPC(victim) && !(killer && PRF_FLAGGED(killer, PRF_EXECUTOR)))//���� ��� ���� ������� �� ���� �� ������
		{
			ClanPkLog::check(ch, victim);
			sprintf(buf2, "%s killed by %s at %s", GET_NAME(victim),
					GET_NAME(ch), IN_ROOM(victim) != NOWHERE ? world[IN_ROOM(victim)]->name : "NOWHERE");
			log(buf2);

			if ((!IS_NPC(ch) || (ch->master && !IS_NPC(ch->master)))
					&& (RENTABLE(victim) && !ROOM_FLAGGED(IN_ROOM(victim), ROOM_ARENA)))
			{
				mudlog(buf2, BRF, LVL_IMPL, SYSLOG, 0);
				if (IS_NPC(ch)
						&& (AFF_FLAGGED(ch, AFF_CHARM) || IS_HORSE(ch))
						&& ch->master && !IS_NPC(ch->master))
				{
					sprintf(buf2, "%s ����������� %s.", GET_NAME(ch), GET_PAD(ch->master, 2));
					mudlog(buf2, BRF, LVL_IMPL, SYSLOG, TRUE);
				}
			}

			if (MOB_FLAGGED(ch, MOB_MEMORY))
				forget(ch, victim);
		}

		/* ���� �� � �������� ����� ����...? */
//      if (IS_NPC (victim))
//      local_gold = victim->get_gold();
//Polos.poison_mob_max_fix
		if (killer) ch = killer;
//-Polos.poison_mob_max_fix
		die(victim, ch);
		/* ���������� */
//      sprintf (local_corpse, "����.%s", GET_PAD (victim, 1));
//      if (IS_NPC (victim) && !IS_NPC (ch) && PRF_FLAGGED (ch, PRF_AUTOLOOT)
//        && get_obj_in_list_vis (ch, local_corpse,
//                                world[ch->in_room]->contents))
//      {
//        sprintf (local_corpse, "��� ����.%s �����", GET_PAD (victim, 1));
//        do_get (ch, local_corpse, 0, 0);
//      }
//      else
//      {
//        /* ����� ���� */
//        if (IS_NPC (victim) && !IS_NPC (ch)
//            && PRF_FLAGGED (ch, PRF_AUTOMONEY) && (local_gold > 0)
//            && get_obj_in_list_vis (ch, local_corpse,
//                                    world[ch->in_room]->contents))
//          {
//            sprintf (local_corpse, "���.��� ����.%s �����",
//                     GET_PAD (victim, 1));
//            do_get (ch, local_corpse, 0, 0);
//          }
//      }
		return (-1);
	}
	if (FS_damage && victim->get_fighting() && GET_POS(victim) > POS_STUNNED
			&& IN_ROOM(victim) != NOWHERE && attacktype != SKILL_TURN_UNDEAD + TYPE_HIT)
		damage(victim, ch, FS_damage, SPELL_FIRE_SHIELD, FALSE);

	return (dam);
}

/**
* ������� �������������� ���������.
* �������� ��� 100 ������ � 21 �����: 100  60 30 10
* �������� ��� 200 ������ � 50 �����: 100 100 70 30
* �� ������� 70% �� �������� ���������.
* � �������� 100% � 60% ��� ��������� �����.
*/
void addshot_damage(CHAR_DATA * ch, int type, int weapon)
{
	int prob = train_skill(ch, SKILL_ADDSHOT, skill_info[SKILL_ADDSHOT].max_percent, ch->get_fighting());

	// ����� ������ ������ ���� 21 (��������� �������� �����) � �� 50
	float dex_mod = static_cast<float>(MAX(GET_REAL_DEX(ch) - 21, 0)) / 29;
	// ����� �� 4� � 5� �������� ��� ����� ���� 5 ������, �� 20% �� ����
	float remort_mod = static_cast<float>(GET_REMORT(ch)) / 5;
	if (remort_mod > 1) remort_mod = 1;
	// �� ���� ����������� ��������� �� 70% �� �������� ��������� ������� ����
	float sit_mod = (GET_POS(ch) >= POS_FIGHTING) ? 1 : 0.7;

	// � �������� ������� ������ �� 100+ ������ � �������� 2 ��� �����
	if (IS_CHARMICE(ch))
	{
		prob = MIN(100, prob);
		dex_mod = 0;
		remort_mod = 0;
	}

	// ���� 100% ���� ������ �������
	float add_prob = MAX(prob - 100, 0);
	// ����� ���� 100 ��������� ������ ����� ������� � ���������
	float skill_mod = add_prob / 100;
	// � ��� ������ �� ����� ��� �������� ��� ����
	prob = MIN(100, prob);

	int percent = number(1, skill_info[SKILL_ADDSHOT].max_percent);
	// 1� ��� - �� ����� 100% ��� ������ 100+
	if (prob * sit_mod >= percent / 2)
		hit(ch, ch->get_fighting(), type, weapon);

	percent = number(1, skill_info[SKILL_ADDSHOT].max_percent);
	// 2� ��� - 60% ��� ������ 100, �� 100% ��� ��������� ������ � �����
	if ((prob * 3 + skill_mod * 100 + dex_mod * 100) * sit_mod > percent * 5 / 2 && ch->get_fighting())
		hit(ch, ch->get_fighting(), type, weapon);

	percent = number(1, skill_info[SKILL_ADDSHOT].max_percent);
	// 3� ��� - 30% ��� ������ 100, �� 70% ��� ��������� ������ � ����� (��� 5+ ������)
	if ((prob * 3 + skill_mod * 200 + dex_mod * 200) * remort_mod * sit_mod > percent * 5 && ch->get_fighting())
		hit(ch, ch->get_fighting(), type, weapon);

	percent = number(1, skill_info[SKILL_ADDSHOT].max_percent);
	// 4� ��� - 10% ��� ������ 100, �� 30% ��� ��������� ������ � ����� (��� 5+ ������)
	if ((prob + skill_mod * 100 + dex_mod * 100) * remort_mod * sit_mod > percent * 5 && ch->get_fighting())
		hit(ch, ch->get_fighting(), type, weapon);
}

/**** This function realize second shot for bows *******/
void exthit(CHAR_DATA * ch, int type, int weapon)
{
	if (!ch || ch->purged())
	{
		log("SYSERROR: ch = %s (%s:%d)",
				ch ? (ch->purged() ? "purged" : "true") : "false",
				__FILE__, __LINE__);
		return;
	}

	OBJ_DATA *wielded = NULL;
	int percent = 0, prob = 0, div = 0, moves = 0;
	CHAR_DATA *tch;


	if (IS_NPC(ch))
	{
		if (MOB_FLAGGED(ch, MOB_EADECREASE) && weapon > 1)
		{
			if (ch->mob_specials.ExtraAttack * GET_HIT(ch) * 2 < weapon * GET_REAL_MAX_HIT(ch))
				return;
		}
		if (MOB_FLAGGED(ch, (MOB_FIREBREATH | MOB_GASBREATH | MOB_FROSTBREATH |
							 MOB_ACIDBREATH | MOB_LIGHTBREATH)))
		{
			for (prob = percent = 0; prob <= 4; prob++)
				if (MOB_FLAGGED(ch, (INT_TWO | (1 << prob))))
					percent++;
			percent = weapon % percent;
			for (prob = 0; prob <= 4; prob++)
				if (MOB_FLAGGED(ch, (INT_TWO | (1 << prob))))
				{
					if (percent)
						percent--;
					else
						break;
				}
			if (MOB_FLAGGED(ch, MOB_AREA_ATTACK))
			{
				for (tch = world[IN_ROOM(ch)]->people; tch; tch = tch->next_in_room)
				{
					if (IS_IMMORTAL(tch))	/* immortal    */
						continue;
					if (IN_ROOM(ch) == NOWHERE ||	/* Something killed in process ... */
							IN_ROOM(tch) == NOWHERE)
						continue;
					if (tch != ch && !same_group(ch, tch))
						mag_damage(GET_LEVEL(ch), ch, tch,
								   SPELL_FIRE_BREATH + MIN(prob, 4), SAVING_CRITICAL);
				}
			}
			else
				mag_damage(GET_LEVEL(ch), ch, ch->get_fighting(),
						   SPELL_FIRE_BREATH + MIN(prob, 4), SAVING_CRITICAL);
			return;
		}
	}

	if (weapon == RIGHT_WEAPON)
	{
		if (!(wielded = GET_EQ(ch, WEAR_WIELD)))
			wielded = GET_EQ(ch, WEAR_BOTHS);
	}
	else if (weapon == LEFT_WEAPON)
		wielded = GET_EQ(ch, WEAR_HOLD);

	if (wielded
			&& !GET_EQ(ch, WEAR_SHIELD)
			&& GET_OBJ_SKILL(wielded) == SKILL_BOWS
			&& GET_EQ(ch, WEAR_BOTHS))
	{
		/* ��� � ����� ����� - ����� ���. ��� ������� ������� */
		if (can_use_feat(ch, DOUBLESHOT_FEAT) && !ch->get_skill(SKILL_ADDSHOT)
				&& MIN(850, 200 + ch->get_skill(SKILL_BOWS) * 4 + GET_REAL_DEX(ch) * 5) >= number(1, 1000))
		{
			hit(ch, ch->get_fighting(), type, weapon);
			prob = 0;
		}
		else if (ch->get_skill(SKILL_ADDSHOT) > 0)
		{
			addshot_damage(ch, type, weapon);
		}
	}

	/*
	������ ������ ������ "�������� �����"
	������ �������������� ����� ������ ��������� 100%
	������ �������������� ����� ������ �������� ���������� � 80%+ ������, �� �� ����� ��� � 80% ������������
	������ �������������� ����� ����� �������� ���������� �����, �� �� ����� ��� � 80% ������������
	������ �������������� ����� ����� �������� ���������� � 170%+ ������, �� �� ����� ��� � 30% �����������
	*/
	if (IS_SET(PRF_FLAGS(ch, PRF_IRON_WIND), PRF_IRON_WIND))
	{
		percent = ch->get_skill(SKILL_IRON_WIND);
		moves = GET_MAX_MOVE(ch) / (6 + MAX(10, percent) / 10);
		prob = GET_AF_BATTLE(ch, EAF_IRON_WIND);
		if (prob && !check_moves(ch, moves))
		{
			CLR_AF_BATTLE(ch, EAF_IRON_WIND);
		}
		else if (!prob && (GET_MOVE(ch) > moves))
		{
			SET_AF_BATTLE(ch, EAF_IRON_WIND);
		};
	};
	if (GET_AF_BATTLE(ch, EAF_IRON_WIND))
	{
		(void) train_skill(ch, SKILL_IRON_WIND, skill_info[SKILL_IRON_WIND].max_percent, ch->get_fighting());
		if (weapon == RIGHT_WEAPON)
		{
			div = 100 + MIN(80, MAX(1, percent - 80));
			prob = 100;
		}
		else
		{
			div = MIN(80, percent + 10);
			prob = 80 - MIN(30, MAX(0, percent - 170));
		};
		while (div > 0)
		{
			if (number(1, 100) < div)
				hit(ch, ch->get_fighting(), type, weapon);
			div -= prob;
		};
	};

	hit(ch, ch->get_fighting(), type, weapon);
}

// ������/������ ������� �� ������ ������������ ����� ������
void apply_weapon_bonus(int ch_class, int skill, int *damroll, int *hitroll)
{
	int dam = *damroll;
	int calc_thaco = *hitroll;

	switch (ch_class)
	{
	case CLASS_CLERIC:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 2;
			dam -= 1;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco += 1;
			dam -= 2;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_BATTLEMAGE:
	case CLASS_DEFENDERMAGE:
	case CLASS_CHARMMAGE:
	case CLASS_NECROMANCER:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam -= 3;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_WARRIOR:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco += 2;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco += 2;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_RANGER:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_GUARD:
	case CLASS_THIEF:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_ASSASINE:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
		/*	case CLASS_PALADINE:
			case CLASS_SMITH:
				switch (skill) {
					case SKILL_CLUBS:	calc_thaco -= 0; dam += 0; break;
					case SKILL_AXES:	calc_thaco -= 0; dam += 0; break;
					case SKILL_LONGS:	calc_thaco -= 0; dam += 0; break;
					case SKILL_SHORTS:	calc_thaco -= 0; dam += 0; break;
					case SKILL_NONSTANDART:	calc_thaco -= 0; dam += 0; break;
					case SKILL_BOTHHANDS:	calc_thaco -= 0; dam += 0; break;
					case SKILL_PICK:	calc_thaco -= 0; dam += 0; break;
					case SKILL_SPADES:	calc_thaco -= 0; dam += 0; break;
					case SKILL_BOWS:	calc_thaco -= 0; dam += 0; break;
				}
				break; */
	case CLASS_MERCHANT:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco -= 0;
			dam += 0;
			break;
		}
		break;
	case CLASS_DRUID:
		switch (skill)
		{
		case SKILL_CLUBS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_AXES:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_LONGS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_SHORTS:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_NONSTANDART:
			calc_thaco -= 0;
			dam += 0;
			break;
		case SKILL_BOTHHANDS:
			calc_thaco += 1;
			dam += 0;
			break;
		case SKILL_PICK:
			calc_thaco += 0;
			dam += 0;
			break;
		case SKILL_SPADES:
			calc_thaco += 0;
			dam += 0;
			break;
		case SKILL_BOWS:
			calc_thaco += 1;
			dam += 0;
			break;
		}
		break;
	}

	*damroll = dam;
	*hitroll = calc_thaco;
}

inline int do_punctual(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wielded)
{
	int dam_critic = 0, skill, wapp;

	if (wielded)
		wapp = (int)(GET_OBJ_SKILL(wielded) == SKILL_BOWS) ?
			   GET_OBJ_WEIGHT(wielded) * 1 / 3 : GET_OBJ_WEIGHT(wielded);
	else
		wapp = 0;

	if (wapp < 10)
		dam_critic = dice(1, 6);
	else
		if (wapp < 19)
			dam_critic = dice(2, 5);
		else
			if (wapp < 27)
				dam_critic = dice(3, 4);
			else
				if (wapp < 36)
					dam_critic = dice(3, 5);
				else
					if (wapp < 44)
						dam_critic = dice(3, 6);
					else
						dam_critic = dice(4, 5);
	skill = 1 + ch->get_skill(SKILL_PUNCTUAL) / 6;
	dam_critic = MIN(number(4, skill), dam_critic);

	return dam_critic;
}

/**
* ��������� ������ ��� �����.
*/
int backstab_mult(int level)
{
	if (level <= 0)
		return 1;	/* level 0 */
	else if (level <= 5)
		return 2;	/* level 1 - 5 */
	else if (level <= 10)
		return 3;	/* level 6 - 10 */
	else if (level <= 15)
		return 4;	/* level 11 - 15 */
	else if (level <= 20)
		return 5;	/* level 16 - 20 */
	else if (level <= 25)
		return 6;	/* level 21 - 25 */
	else if (level <= 30)
		return 7;	/* level 26 - 30 */
	else
		return 10;
}

/**
* ������� ����������� ����.����� = �����/11 + (�����-20)/(�����/30)
* �������� �� 50% �� ������ � �����, �������� 36,18%.
*/
int calculate_crit_backstab_percent(CHAR_DATA *ch)
{
	return static_cast<int>(ch->get_skill(SKILL_BACKSTAB)/11.0 + (GET_REAL_DEX(ch) - 20) / (GET_REAL_DEX(ch) / 30.0));
}

/**
* ������ ��������� ����.����� (�� ������� ������ ��� �����).
*/
double crit_backstab_multiplier(CHAR_DATA *ch, CHAR_DATA *victim)
{
	double bs_coeff = 1;
	if (IS_NPC(victim))
	{
		if (ch->get_skill(SKILL_BACKSTAB) <= 100)
		{
			bs_coeff = ch->get_skill(SKILL_BACKSTAB) / 20;
			if (bs_coeff < 2)
				bs_coeff = 2;
		}
		else
		{
			bs_coeff = 5 + (ch->get_skill(SKILL_BACKSTAB) - 100) / 40;
		}
		send_to_char("&G����� � ������!&n\r\n", ch);
	}
	else if (GET_CLASS(ch) == CLASS_THIEF)
	{
		// �� ����� ����. �� 1.25 ��� 200 �����
		bs_coeff *= 1 + (ch->get_skill(SKILL_BACKSTAB) * 0.00125);
		// ����� ��� ����� ��� �� �������
		if (AFF_FLAGGED(victim, AFF_SANCTUARY))
			bs_coeff *= 2;
		send_to_char("&G����� � ������!&n\r\n", ch);
	}
	return bs_coeff;
}

/**
* ����� �� �������� ����������� ����� ��������� (������ � ������ ������, ��� ����� �����).
*/
bool can_auto_block(CHAR_DATA *ch)
{
	if (GET_EQ(ch, WEAR_SHIELD) && GET_AF_BATTLE(ch, EAF_AWAKE) && GET_AF_BATTLE(ch, EAF_AUTOBLOCK))
		return true;
	else
		return false;
}

/**
* �������� �� ��� "������� ������".
*/
void check_weap_feats(CHAR_DATA *ch, int skill, int *dam, int *calc_thaco)
{
	switch (skill)
	{
	case SKILL_PUNCH:
		if (HAVE_FEAT(ch, PUNCH_FOCUS_FEAT))
		{
			*calc_thaco -= 2;
			*dam += 2;
		}
		break;
	case SKILL_CLUBS:
		if (HAVE_FEAT(ch, CLUB_FOCUS_FEAT))
		{
			*calc_thaco -= 2;
			*dam += 2;
		}
		break;
	case SKILL_AXES:
		if (HAVE_FEAT(ch, AXES_FOCUS_FEAT))
		{
			*calc_thaco -= 1;
			*dam += 2;
		}
		break;
	case SKILL_LONGS:
		if (HAVE_FEAT(ch, LONGS_FOCUS_FEAT))
		{
			*calc_thaco -= 1;
			*dam += 2;
		}
		break;
	case SKILL_SHORTS:
		if (HAVE_FEAT(ch, SHORTS_FOCUS_FEAT))
		{
			*calc_thaco -= 2;
			*dam += 3;
		}
		break;
	case SKILL_NONSTANDART:
		if (HAVE_FEAT(ch, NONSTANDART_FOCUS_FEAT))
		{
			*calc_thaco -= 1;
			*dam += 3;
		}
		break;
	case SKILL_BOTHHANDS:
		if (HAVE_FEAT(ch, BOTHHANDS_FOCUS_FEAT))
		{
			*calc_thaco -= 1;
			*dam += 3;
		}
		break;
	case SKILL_PICK:
		if (HAVE_FEAT(ch, PICK_FOCUS_FEAT))
		{
			*calc_thaco -= 2;
			*dam += 3;
		}
		break;
	case SKILL_SPADES:
		if (HAVE_FEAT(ch, SPADES_FOCUS_FEAT))
		{
			*calc_thaco -= 1;
			*dam += 2;
		}
		break;
	case SKILL_BOWS:
		if (HAVE_FEAT(ch, BOWS_FOCUS_FEAT))
		{
			*calc_thaco -= 2;
			*dam += 2;
		}
		break;
	}
}

/**
* ������.
*/
void hit_touching(CHAR_DATA *ch, CHAR_DATA *vict, int *dam)
{
	if (vict->get_touching() == ch
		&& !AFF_FLAGGED(vict, AFF_STOPFIGHT)
		&& !AFF_FLAGGED(vict, AFF_MAGICSTOPFIGHT)
		&& !AFF_FLAGGED(vict, AFF_STOPRIGHT)
		&& GET_WAIT(vict) <= 0
		&& !GET_MOB_HOLD(vict)
		&& (IS_IMMORTAL(vict) || IS_NPC(vict)
			|| !(GET_EQ(vict, WEAR_WIELD) || GET_EQ(vict, WEAR_BOTHS)))
		&& GET_POS(vict) > POS_SLEEPING)
	{
		int percent = number(1, skill_info[SKILL_TOUCH].max_percent);
		int prob = train_skill(vict, SKILL_TOUCH, skill_info[SKILL_TOUCH].max_percent, ch);
		if (IS_IMMORTAL(vict) || GET_GOD_FLAG(vict, GF_GODSLIKE))
		{
			percent = prob;
		}
		if (GET_GOD_FLAG(vict, GF_GODSCURSE))
		{
			percent = 0;
		}
		CLR_AF_BATTLE(vict, EAF_TOUCH);
		SET_AF_BATTLE(vict, EAF_USEDRIGHT);
		vict->set_touching(0);
		if (prob < percent)
		{
			act("�� �� ������ ����������� ����� $N1.", FALSE, vict, 0, ch, TO_CHAR);
			act("$N �� ����$Q ����������� ���� �����.", FALSE, ch, 0, vict, TO_CHAR);
			act("$n �� ����$q ����������� ����� $N1.", TRUE, vict, 0, ch, TO_NOTVICT);
			prob = 2;
		}
		else
		{
			act("�� ����������� ����� $N1.", FALSE, vict, 0, ch, TO_CHAR);
			act("$N ����������$G ���� �����.", FALSE, ch, 0, vict, TO_CHAR);
			act("$n ����������$g ����� $N1.", TRUE, vict, 0, ch, TO_NOTVICT);
			*dam = -1;
			prob = 1;
		}
		if (!WAITLESS(vict))
		{
			WAIT_STATE(vict, prob * PULSE_VIOLENCE);
		}
	}
}

void hit_deviate(CHAR_DATA *ch, CHAR_DATA *victim, int *dam)
{
	int range = number(1, skill_info[SKILL_DEVIATE].max_percent);
	int prob = train_skill(victim, SKILL_DEVIATE, skill_info[SKILL_DEVIATE].max_percent, ch);
	if (GET_GOD_FLAG(victim, GF_GODSCURSE))
	{
		prob = 0;
	}
	prob = prob * 100 / range;
	if (prob < 60)
	{
		act("�� �� ������ ���������� �� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
		act("$N �� �����$G ���������� �� ����� �����", FALSE, ch, 0, victim, TO_CHAR);
		act("$n �� �����$g ���������� �� ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
	}
	else if (prob < 100)
	{
		act("�� ������� ���������� �� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
		act("$N ������� �������$U �� ����� �����", FALSE, ch, 0, victim, TO_CHAR);
		act("$n ������� �������$u �� ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
		*dam = *dam * 10 / 15;
	}
	else if (prob < 200)
	{
		act("�� �������� ���������� �� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
		act("$N �������� �������$U �� ����� �����", FALSE, ch, 0, victim, TO_CHAR);
		act("$n �������� �������$u �� ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
		*dam = *dam / 2;
	}
	else
	{
		act("�� ���������� �� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
		act("$N �������$U �� ����� �����", FALSE, ch, 0, victim, TO_CHAR);
		act("$n �������$u �� ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
		*dam = -1;
	}
	BATTLECNTR(victim)++;
}

void hit_parry(CHAR_DATA *ch, CHAR_DATA *victim, int skill, int w_type, int *dam)
{
	if (!((GET_EQ(victim, WEAR_WIELD)
			&& GET_OBJ_TYPE(GET_EQ(victim, WEAR_WIELD)) == ITEM_WEAPON
			&& GET_EQ(victim, WEAR_HOLD)
			&& GET_OBJ_TYPE(GET_EQ(victim, WEAR_HOLD)) == ITEM_WEAPON)
		|| IS_NPC(victim)
		|| IS_IMMORTAL(victim)))
	{
		send_to_char("� ��� ����� ��������� ����� ����������\r\n", victim);
		CLR_AF_BATTLE(victim, EAF_PARRY);
	}
	else
	{
		int range = number(1, skill_info[SKILL_PARRY].max_percent);
		int prob = train_skill(victim, SKILL_PARRY,
				skill_info[SKILL_PARRY].max_percent, ch);
		prob = prob * 100 / range;

		if (prob < 70
			|| ((skill == SKILL_BOWS || w_type == TYPE_MAUL)
				&& !IS_IMMORTAL(victim)
				&& (!can_use_feat(victim, PARRY_ARROW_FEAT)
				|| number(1, 1000) >= 20 * MIN(GET_REAL_DEX(victim), 35))))
		{
			act("�� �� ������ ������ ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �� �����$G ������ ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �� �����$g ������ ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			prob = 2;
			SET_AF_BATTLE(victim, EAF_USEDLEFT);
		}
		else if (prob < 100)
		{
			act("�� ������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 10);
			prob = 1;
			*dam = *dam * 10 / 15;
			SET_AF_BATTLE(victim, EAF_USEDLEFT);
		}
		else if (prob < 170)
		{
			act("�� �������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 15);
			prob = 0;
			*dam = *dam / 2;
			SET_AF_BATTLE(victim, EAF_USEDLEFT);
		}
		else
		{
			act("�� ��������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ��������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ��������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 25);
			prob = 0;
			*dam = -1;
		}
		if (!WAITLESS(ch) && prob)
		{
			WAIT_STATE(victim, PULSE_VIOLENCE * prob);
		}
		CLR_AF_BATTLE(victim, EAF_PARRY);
	}
}

void hit_multyparry(CHAR_DATA *ch, CHAR_DATA *victim, int skill, int w_type, int *dam)
{
	if (!((GET_EQ(victim, WEAR_WIELD)
			&& GET_OBJ_TYPE(GET_EQ(victim, WEAR_WIELD)) == ITEM_WEAPON
			&& GET_EQ(victim, WEAR_HOLD)
			&& GET_OBJ_TYPE(GET_EQ(victim, WEAR_HOLD)) == ITEM_WEAPON)
		|| IS_NPC(victim)
		|| IS_IMMORTAL(victim)))
	{
		send_to_char("� ��� ����� ��������� ����� �����������\r\n", victim);
	}
	else
	{
		int range = number(1,
				skill_info[SKILL_MULTYPARRY].max_percent) + 15 * BATTLECNTR(victim);
		int prob = train_skill(victim, SKILL_MULTYPARRY,
				skill_info[SKILL_MULTYPARRY].max_percent + BATTLECNTR(ch) * 15, ch);
		prob = prob * 100 / range;

		if ((skill == SKILL_BOWS || w_type == TYPE_MAUL)
			&& !IS_IMMORTAL(victim)
			&& (!can_use_feat(victim, PARRY_ARROW_FEAT)
				|| number(1, 1000) >= 20 * MIN(GET_REAL_DEX(victim), 35)))
		{
			prob = 0;
		}
		else
		{
			BATTLECNTR(victim)++;
		}

		if (prob < 50)
		{
			act("�� �� ������ ������ ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �� �����$G ������ ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �� �����$g ������ ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
		}
		else if (prob < 90)
		{
			act("�� ������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 10);
			*dam = *dam * 10 / 15;
		}
		else if (prob < 180)
		{
			act("�� �������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 15);
			*dam = *dam / 2;
		}
		else
		{
			act("�� ��������� ��������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ��������� ��������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ��������� ��������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, number(0, 2) ? WEAR_WIELD : WEAR_HOLD, *dam, 25);
			*dam = -1;
		}
	}
}

void hit_block(CHAR_DATA *ch, CHAR_DATA *victim, int *dam)
{
	if (!(GET_EQ(victim, WEAR_SHIELD)
		|| IS_NPC(victim)
		|| IS_IMMORTAL(victim)))
	{
		send_to_char("� ��� ����� �������� ����� ����������\r\n", victim);
	}
	else
	{
		int range = number(1, skill_info[SKILL_BLOCK].max_percent);
		int prob =
			train_skill(victim, SKILL_BLOCK, skill_info[SKILL_BLOCK].max_percent, ch);
		prob = prob * 100 / range;
		BATTLECNTR(victim)++;
		if (prob < 100)
		{
			act("�� �� ������ �������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �� �����$G �������� ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �� �����$g �������� ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
		}
		else if (prob < 150)
		{
			act("�� ������� �������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ������� �������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ������� �������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, WEAR_SHIELD, *dam, 10);
			*dam = *dam * 10 / 15;
		}
		else if (prob < 250)
		{
			act("�� �������� �������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N �������� �������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n �������� �������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, WEAR_SHIELD, *dam, 15);
			*dam = *dam / 2;
		}
		else
		{
			act("�� ��������� �������� ����� $N1", FALSE, victim, 0, ch, TO_CHAR);
			act("$N ��������� �������$G ���� �����", FALSE, ch, 0, victim, TO_CHAR);
			act("$n ��������� �������$g ����� $N1", TRUE, victim, 0, ch, TO_NOTVICT);
			alt_equip(victim, WEAR_SHIELD, *dam, 25);
			*dam = -1;
		}
	}
}

/*
int limit_added_dr(CHAR_DATA *ch, int damroll, int total_dr)
{
	int calc_dr = damroll;
	int rmrt = MIN(14, GET_REMORT(ch));

	if (ch->get_level() < 30 - grouping[GET_CLASS(ch)][rmrt])
	{
		int cap_dr = MIN(2 * ch->get_level(), total_dr);
		if (cap_dr < total_dr)
		{
			double coeff = total_dr / static_cast<double>(cap_dr);
			calc_dr = static_cast<int>(damroll/coeff);
		}
	}

	return MIN(calc_dr, damroll);
}

int add_pc_damroll(CHAR_DATA *ch, int dam, bool info = false)
{
	int dr_by_str = GET_REAL_STR(ch) - 14;
	int native_str_dr = ch->get_start_stat(G_STR) + ch->get_remort() - 14;
	int obj_str_dr = MAX(0, dr_by_str - native_str_dr);

	int added_dr_total = GET_REAL_DR(ch) + obj_str_dr;

	dam += limit_added_dr(ch, GET_REAL_DR(ch), added_dr_total);
	if (!info)
	{
		dam = dam > 0 ? number(1, (dam * 2)) : dam;
	}
	dam += limit_added_dr(ch, obj_str_dr, added_dr_total);
	dam += MAX(0, native_str_dr);

	return dam;
}

int limit_weap_dam(CHAR_DATA *ch, OBJ_DATA *weap, int dam)
{
	int rmrt = MIN(14, GET_REMORT(ch));
	if (GET_OBJ_TYPE(weap) != ITEM_WEAPON || ch->get_level() >= 30 - grouping[GET_CLASS(ch)][rmrt])
	{
		return dam;
	}

	double median_dam = 1.0, capped_dam = 1.0;
	switch (GET_OBJ_SKILL(weap))
	{
	case SKILL_BOWS:
		// 1..5 ��� = 4 ��
		// 6..25 ��� = 4,5..14 ��
		median_dam = MMAX(1.0, (GET_OBJ_VAL(weap, 2) + 1) * GET_OBJ_VAL(weap, 1) / 2.0);
		capped_dam = MMAX(5.0, 1.5 + ch->get_level() * 0.5);
		break;
	case SKILL_SHORTS:
	case SKILL_LONGS:
	case SKILL_AXES:
	case SKILL_CLUBS:
	case SKILL_NONSTANDART:
	case SKILL_BOTHHANDS:
	case SKILL_PICK:
	case SKILL_SPADES:
		// 1..5 ��� = 6 ��
		// 6..25 ��� = 6,7..20 ��
		median_dam = MMAX(1.0, (GET_OBJ_VAL(weap, 2) + 1) * GET_OBJ_VAL(weap, 1) / 2.0);
		capped_dam = MMAX(5.0, 2.5 + ch->get_level() * 0.7);
		break;
	}

	double over_coeff = median_dam / capped_dam;
	int limited_dam = static_cast<int>(dam / over_coeff);

	return MIN(limited_dam, dam);
}

*/

/**
* ��������� ������ �������, �����, ������, �����, ���.
* \param weapon = 1 - ����� ������ ��� ����� ������
*               = 2 - ����� ����� �����
*/
void hit(CHAR_DATA *ch, CHAR_DATA *victim, int type, int weapon)
{
	if (!victim)
	{
		return;
	}
	if (!ch || ch->purged() || victim->purged())
	{
		log("SYSERROR: ch = %s, victim = %s (%s:%d)",
				ch ? (ch->purged() ? "purged" : "true") : "false",
				victim->purged() ? "purged" : "true", __FILE__, __LINE__);
		return;
	}

	/* Do some sanity checking, in case someone flees, etc. */
	if (IN_ROOM(ch) != IN_ROOM(victim) || IN_ROOM(ch) == NOWHERE)
	{
		if (ch->get_fighting() && ch->get_fighting() == victim)
		{
			stop_fighting(ch, TRUE);
		}
		return;
	}

	/* Stand awarness mobs */
	if (CAN_SEE(victim, ch)
		&& !victim->get_fighting()
		&& ((IS_NPC(victim) && (GET_HIT(victim) < GET_MAX_HIT(victim)
			|| MOB_FLAGGED(victim, MOB_AWARE)))
			|| AFF_FLAGGED(victim, AFF_AWARNESS))
		&& !GET_MOB_HOLD(victim) && GET_WAIT(victim) <= 0)
	{
		set_battle_pos(victim);
	}

	OBJ_DATA *wielded = 0;
	int weapon_pos = WEAR_WIELD;

	/* Find weapon for attack number weapon */
	if (weapon == 1)
	{
		if (!(wielded = GET_EQ(ch, WEAR_WIELD)))
		{
			wielded = GET_EQ(ch, WEAR_BOTHS);
			weapon_pos = WEAR_BOTHS;
		}
	}
	else if (weapon == 2)
	{
		wielded = GET_EQ(ch, WEAR_HOLD);
		weapon_pos = WEAR_HOLD;
	}

	/* ��������� SKILL_NOPARRYHIT */
	if (type == TYPE_UNDEFINED && ch->get_skill(SKILL_NOPARRYHIT))
	{
		int tmp_skill = train_skill(ch, SKILL_NOPARRYHIT,
				skill_info[SKILL_NOPARRYHIT].max_percent, ch->get_fighting());
		// TODO: max_percent � ������ ������ 100 (������ �� �� ���� 200, � � % �����)
		if (tmp_skill >= number(1, skill_info[SKILL_NOPARRYHIT].max_percent))
		{
			type = TYPE_NOPARRY;
		}
	}

	int skill = 0, skill_is = 0;
	if (wielded && GET_OBJ_TYPE(wielded) == ITEM_WEAPON)
	{
		// ��� ���� ����� ���� ����� ������� �� �����, ���� ��� ����
		skill = GET_OBJ_SKILL(wielded);
		skill_is = train_skill(ch, skill, skill_info[skill].max_percent, victim);
	}
	else
	{
		// ���� ������ ������
		skill = SKILL_PUNCH;
		skill_is = train_skill(ch, skill, skill_info[skill].max_percent, victim);
	}

	int calc_thaco = 0, victim_ac = 0, dam = 0;
	int i, w_type = 0, prob, range;
	int percent, modi = 0;

	/* Find the weapon type (for display purposes only) */
	if (type == SKILL_THROW || type == SKILL_BACKSTAB)
	{
		w_type = type + TYPE_HIT;
	}
	else if (wielded && GET_OBJ_TYPE(wielded) == ITEM_WEAPON)
	{
		if (!IS_NPC(ch))
		{
			// Apply HR for light weapon
			percent = 0;
			switch (weapon_pos)
			{
			case WEAR_WIELD:
				percent = (str_bonus(GET_REAL_STR(ch), STR_WIELD_W) - GET_OBJ_WEIGHT(wielded) + 1) / 2;
				break;
			case WEAR_HOLD:
				percent = (str_bonus(GET_REAL_STR(ch), STR_HOLD_W) - GET_OBJ_WEIGHT(wielded) + 1) / 2;
				break;
			case WEAR_BOTHS:
				percent = (str_bonus(GET_REAL_STR(ch), STR_WIELD_W) +
						   str_bonus(GET_REAL_STR(ch), STR_HOLD_W) - GET_OBJ_WEIGHT(wielded) + 1) / 2;
				break;
			}
			calc_thaco -= MIN(3, MAX(percent, 0));

			// Penalty for unknown weapon type
// shapirus: ������ ����� ������ �� ��������, ��� �����, ��� unknown_weapon_fault
// ����� �� ������������. ������ ����� �� ���� ���� ����. ���� ������ ����� �� �������.
// ���� ����� ����, �� ����� �� ����, � ��������� ������/������ �� ������
			if (ch->get_skill(skill) == 0)
			{
				calc_thaco += (50 - MIN(50, GET_REAL_INT(ch))) / 3;
				dam -= (50 - MIN(50, GET_REAL_INT(ch))) / 6;
			}
			else
			{
				apply_weapon_bonus(GET_CLASS(ch), skill, &dam, &calc_thaco);
			}

			// Bonus for leadership
			if (calc_leadership(ch))
				calc_thaco -= 2;
		}
		w_type = GET_OBJ_VAL(wielded, 3) + TYPE_HIT;
	}
	else
	{
		weapon_pos = 0;

		if (!IS_NPC(ch))
		{
			// �������� � ��� ���������� ���� ������ ��������� :)
			if (GET_CLASS(ch) != CLASS_WARRIOR)
				calc_thaco += 4;
			else	// � ��������� ������� ����� �� ���������� ������
				calc_thaco -= 3;

			// ����� � ������� 1 �������� �� ������
			// ������������ 10% ������ "���� ����� �����"
			if (weapon == 2)
				calc_thaco += (skill_info[SKILL_SHIT].max_percent -
							   train_skill(ch, SKILL_SHIT,
										   skill_info[SKILL_SHIT].max_percent, victim)) / 10;
			// Bonus for leadership
			if (calc_leadership(ch))
				calc_thaco -= 2;
		}

		if (IS_NPC(ch) && (ch->mob_specials.attack_type != 0))
			w_type = ch->mob_specials.attack_type + TYPE_HIT;
		else
			w_type += TYPE_HIT;
	}

	/*  ��������� ���. ��� ������ */
	if (AFF_FLAGGED(ch, AFF_CLOUD_OF_ARROWS) && IS_WEAPON(w_type))
	{
		// ��� ��� ����� �������� ���������� �������
		if (mag_damage(1, ch, victim, SPELL_MAGIC_MISSILE, SAVING_REFLEX) == -1)
			return;
	}

	check_weap_feats(ch, skill, &dam, &calc_thaco);

	// courage
	if (affected_by_spell(ch, SPELL_COURAGE))
	{
		range = number(1, skill_info[SKILL_COURAGE].max_percent + GET_REAL_MAX_HIT(ch) - GET_HIT(ch));
		prob = train_skill(ch, SKILL_COURAGE, skill_info[SKILL_COURAGE].max_percent, victim);
		if (prob > range)
		{
			dam += ((ch->get_skill(SKILL_COURAGE) + 19) / 20);
			calc_thaco -= ((ch->get_skill(SKILL_COURAGE) + 9) / 20);
		}
	}

	if (GET_AF_BATTLE(ch, EAF_STUPOR) || GET_AF_BATTLE(ch, EAF_MIGHTHIT))
	{
		calc_thaco -= MAX(0, (ch->get_skill(skill) - 70) / 8);
	}

	//    AWAKE style - decrease hitroll
	if (GET_AF_BATTLE(ch, EAF_AWAKE)
		&& GET_CLASS(ch) != CLASS_ASSASINE
		&& type != SKILL_THROW
		&& type != SKILL_BACKSTAB)
	{
		if (can_auto_block(ch))
		{
			// ��������� �� ����� � ����� � ����� � ������ - ������ �� ������� (�� 0 �� 10)
			calc_thaco += ch->get_skill(SKILL_AWAKE) * 5 / 100;
		}
		else
		{
			// ����� ��� ���� ������ �� ����� ����� �������, �� �������������� ������
			// �� ���� ����� ��� ���, ��� ��� ������ �� ���� ����
			calc_thaco += ((ch->get_skill(SKILL_AWAKE) + 9) / 10) + 2;
		}
	}

	if (!IS_NPC(ch) && type != SKILL_THROW && type != SKILL_BACKSTAB)
	{
		// Casters use weather, int and wisdom
		if (IS_CASTER(ch))
		{
			/*	  calc_thaco +=
				    (10 -
				     complex_skill_modifier (ch, SKILL_THAC0, GAPPLY_SKILL_SUCCESS,
							     10));
			*/
			calc_thaco -= (int)((GET_REAL_INT(ch) - 13) / GET_LEVEL(ch));
			calc_thaco -= (int)((GET_REAL_WIS(ch) - 13) / GET_LEVEL(ch));
		}
		// Horse modifier for attacker
		if (on_horse(ch))
		{
			prob = train_skill(ch, SKILL_HORSE, skill_info[SKILL_HORSE].max_percent, victim);
			dam += ((prob + 19) / 10);
			range = number(1, skill_info[SKILL_HORSE].max_percent);
			if (range > prob)
				calc_thaco += ((range - prob) + 19 / 20);
			else
				calc_thaco -= ((prob - range) + 19 / 20);
		}
		// Skill level increase damage
		if (ch->get_skill(skill) >= 60)
			dam += ((ch->get_skill(skill) - 50) / 10);
	}
	// not can see (blind, dark, etc)
	if (!CAN_SEE(ch, victim))
		calc_thaco += (can_use_feat(ch, BLIND_FIGHT_FEAT) ? 2 : IS_NPC(ch) ? 6 : 10);
	if (!CAN_SEE(victim, ch))
		calc_thaco -= (can_use_feat(victim, BLIND_FIGHT_FEAT) ? 2 : 8);

	// bless
	if (AFF_FLAGGED(ch, AFF_BLESS))
	{
		calc_thaco -= 4;
	}
	// curse
	if (AFF_FLAGGED(ch, AFF_CURSE))
	{
		calc_thaco += 6;
		dam -= 5;
	}
	/* ���� ������ � ���������� ����� */
	if (PRF_FLAGGED(ch, PRF_POWERATTACK) && can_use_feat(ch, POWER_ATTACK_FEAT))
	{
		calc_thaco += 2;
		dam += 5;
	}
	else if (PRF_FLAGGED(ch, PRF_GREATPOWERATTACK) && can_use_feat(ch, GREAT_POWER_ATTACK_FEAT))
	{
		calc_thaco += 4;
		dam += 10;
	}
	else if (PRF_FLAGGED(ch, PRF_AIMINGATTACK) && can_use_feat(ch, AIMING_ATTACK_FEAT))
	{
		calc_thaco -= 2;
		dam -= 5;
	}
	else if (PRF_FLAGGED(ch, PRF_GREATAIMINGATTACK) && can_use_feat(ch, GREAT_AIMING_ATTACK_FEAT))
	{
		calc_thaco -= 4;
		dam -= 10;
	}

	/* Gorrah: ����� � ������������ �� ������ "�������� �����" */
	if (GET_AF_BATTLE(ch, EAF_IRON_WIND))
		dam += ch->get_skill(SKILL_IRON_WIND) / 10;

	// some protects
	if (AFF_FLAGGED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch))
		calc_thaco += 2;
	if (AFF_FLAGGED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch))
		calc_thaco += 2;

	// "Dirty" methods for battle
	if (type != SKILL_THROW && type != SKILL_BACKSTAB)
	{
		prob = (ch->get_skill(skill) + cha_app[GET_REAL_CHA(ch)].illusive) -
			   (victim->get_skill(skill) + int_app[GET_REAL_INT(victim)].observation);
		if (prob >= 30 && !GET_AF_BATTLE(victim, EAF_AWAKE)
				&& (IS_NPC(ch) || !GET_AF_BATTLE(ch, EAF_PUNCTUAL)))
		{
			calc_thaco -= (ch->get_skill(skill) - victim->get_skill(skill) > 60 ? 2 : 1);
			if (!IS_NPC(victim))
				dam += (prob >= 70 ? 3 : (prob >= 50 ? 2 : 1));
		}
	}
	// AWAKE style for victim
	if (GET_AF_BATTLE(victim, EAF_AWAKE) &&
			!AFF_FLAGGED(victim, AFF_STOPFIGHT) &&
			!AFF_FLAGGED(victim, AFF_MAGICSTOPFIGHT) &&
			!GET_MOB_HOLD(victim) &&
			train_skill(victim, SKILL_AWAKE, skill_info[SKILL_AWAKE].max_percent,
						ch) >= number(1, skill_info[SKILL_AWAKE].max_percent))
	{
		dam -= IS_NPC(ch) ? 5 : 5;
		calc_thaco += IS_NPC(ch) ? 4 : 2;
	}

	// Calculate the THAC0 of the attacker
	if (!IS_NPC(ch))
	{
		calc_thaco += thaco((int) GET_CLASS(ch), (int) GET_LEVEL(ch));
	}
	else
	{
		// ����� ����� �� ������������ ��������
		calc_thaco += (25 - GET_LEVEL(ch) / 3);
	}
	calc_thaco -= GET_REAL_HR(ch);

	/* ������������� �������� ������ ���� ��� ��������� */
	if (can_use_feat(ch, WEAPON_FINESSE_FEAT))
	{
		if (wielded && GET_OBJ_WEIGHT(wielded) > 20)
			calc_thaco -= str_bonus(GET_REAL_STR(ch), STR_TO_HIT);
		else
			calc_thaco -= str_bonus(GET_REAL_DEX(ch), STR_TO_HIT);
	}
	else
	{
		calc_thaco -= str_bonus(GET_REAL_STR(ch), STR_TO_HIT);
	}

	if ((type == SKILL_THROW || type == SKILL_BACKSTAB) && wielded && GET_OBJ_TYPE(wielded) == ITEM_WEAPON)
	{
		if (type == SKILL_BACKSTAB)
			calc_thaco -= MAX(0, (ch->get_skill(SKILL_SNEAK) + ch->get_skill(SKILL_HIDE) - 100) / 30);
	}
	else
// ������ ��������� �������� ��� :)
		calc_thaco += 4;

	if (skill_is <= 80)
		calc_thaco -= skill_is / 20;
	else if (skill_is <= 110)
		calc_thaco -= 4 + (skill_is - 80) / 10;
	else
		calc_thaco -= 4 + 3 + (skill_is - 110) / 5;

	// Calculate the raw armor including magic armor.  Lower AC is better.

	victim_ac += compute_armor_class(victim);
	victim_ac /= 10;

	if (GET_POS(victim) < POS_FIGHTING)
		victim_ac += 4;
	if (GET_POS(victim) < POS_RESTING)
		victim_ac += 3;
	if (AFF_FLAGGED(victim, AFF_HOLD))
		victim_ac += 4;
	if (AFF_FLAGGED(victim, AFF_CRYING))
		victim_ac += 4;

	// roll the dice and take your chances...
	const int diceroll = number(100, 2099) / 100;

	// decide whether this is a hit or a miss
	// ������ ���� 5% ����������� ������� ��� ���������,
	// ����� �� AC � ���������� �� ���
	if (((diceroll < 20) && AWAKE(victim)) && ((diceroll == 1) || (calc_thaco - diceroll > victim_ac)))
	{
		// the attacker missed the victim
		extdamage(ch, victim, 0, w_type, wielded, TRUE);
		hitprcnt_mtrigger(victim);
		return;
	}

	if ((AFF_FLAGGED(victim, AFF_BLINK)
			|| (GET_CLASS(victim) == CLASS_THIEF))
			&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
			&& !GET_AF_BATTLE(ch, EAF_STUPOR)
			&& (!(type == SKILL_BACKSTAB && GET_CLASS(ch) == CLASS_THIEF))
			&& number(1, 100) <= 20)
	{
		sprintf(buf,
				"%s�� ��������� �� ������� �� ���� ������ ����������.%s\r\n",
				CCINRM(victim, C_NRM), CCNRM(victim, C_NRM));
		send_to_char(buf, victim);
		extdamage(ch, victim, 0, w_type, wielded, TRUE);
		return;
	}
	// okay, we know the guy has been hit.  now calculate damage.

	// Start with the damage bonuses: the damroll and strength apply

	dam += GET_REAL_DR(ch);
	dam = dam > 0 ? number(1, (dam * 2)) : dam;
	dam += str_bonus(GET_REAL_STR(ch), STR_TO_DAM);

	if (GET_EQ(ch, WEAR_BOTHS) && skill != SKILL_BOWS)
		dam *= 2;

	if (IS_NPC(ch))
	{
		dam += dice(ch->mob_specials.damnodice, ch->mob_specials.damsizedice);
	}

	// ���� ��� ������ - ����� ������ ��������� �����, �� ��� ���� ������ �����/������
	// ������ ��� ������ ������� �� ��� ������� - �������� ������������������ �������������
	int noparryhit = 0;

	if (wielded && GET_OBJ_TYPE(wielded) == ITEM_WEAPON)  	// Add weapon-based damage if a weapon is being wielded
	{
		percent = dice(GET_OBJ_VAL(wielded, 1), GET_OBJ_VAL(wielded, 2));
		if (IS_NPC(ch) && !AFF_FLAGGED(ch, AFF_CHARM)
				&& !MOB_FLAGGED(ch, MOB_ANGEL))
		{
			percent *= MOB_DAMAGE_MULT;
		}
		else
		{
			percent = MIN(percent, percent * GET_OBJ_CUR(wielded) / MAX(1, GET_OBJ_MAX(wielded)));
		}
		percent = calculate_strconc_damage(ch, wielded, percent);
		dam += MAX(1, percent);
		noparryhit += calculate_noparryhit_dmg(ch, wielded);
		if (type == SKILL_BACKSTAB)
			noparryhit = noparryhit * 10 / 15;
	}
	else  	// If no weapon, add bare hand damage instead
	{
		if (AFF_FLAGGED(ch, AFF_STONEHAND))
		{
			dam += number(5, 10);
		}
		else
		{
			dam += number(1, 3);
		}

		if (GET_CLASS(ch) == CLASS_WARRIOR)
		{
			dam += GET_LEVEL(ch) / 5;
			dam += MAX(0, GET_REAL_STR(ch) - 25);
		}
		// �������������� ����������� ��� ������ � � ��������� (�������� ������������)
		// <��� ��������> <����������>
		// 0  50%
		// 5 100%
		// 10 150%
		// 15 200%
		// �� ����� �� ������
		if (!GET_AF_BATTLE(ch, EAF_MIGHTHIT))
		{
			modi = 10 * (5 + (GET_EQ(ch, WEAR_HANDS) ? GET_OBJ_WEIGHT(GET_EQ(ch, WEAR_HANDS)) : 0));
			if (IS_NPC(ch) || IS_WARRIOR(ch))
				modi = MAX(100, modi);
			dam = modi * dam / 100;
		}
	}

	//dzMUDiST ��������� !�����������! +Gorrah
	if (affected_by_spell(ch, SPELL_BERSERK))
	{
		if (AFF_FLAGGED(ch, AFF_BERSERK))
		{
			dam = (dam * MAX(150, 150 + GET_LEVEL(ch) + dice(0, GET_REMORT(ch)) * 2)) / 100;
			calc_thaco -= (12 * ((GET_REAL_MAX_HIT(ch) / 2) - GET_HIT(ch)) / GET_REAL_MAX_HIT(ch));
		}
	}

// ��������� ������ �� ������ ������� ���� ������ ����

	// Include a damage multiplier if victim isn't ready to fight:
	// Position sitting  1.5 x normal
	// Position resting  2.0 x normal
	// Position sleeping 2.5 x normal
	// Position stunned  3.0 x normal
	// Position incap    3.5 x normal
	// Position mortally 4.0 x normal
	//
	// Note, this is a hack because it depends on the particular
	// values of the POSITION_XXX constants.
	//
	if (GET_POS(ch) < POS_FIGHTING)
	{
		dam -= (dam * (POS_FIGHTING - GET_POS(ch)) / 4);
		noparryhit -= (noparryhit * (POS_FIGHTING - GET_POS(ch)) / 4);
	}

	if (GET_POS(victim) == POS_SITTING
			&& (AFF_FLAGGED(victim, AFF_AIRSHIELD)
				|| AFF_FLAGGED(victim, AFF_FIRESHIELD)
				|| AFF_FLAGGED(victim, AFF_ICESHIELD)))
	{
		// ������ ����� � ����, ����������� �� ��������
		// �� ������� � ��� �����
	}
	else if (GET_POS(victim) < POS_FIGHTING)
	{
		dam += (dam * (POS_FIGHTING - GET_POS(victim)) / 3);
		noparryhit += (noparryhit * (POS_FIGHTING - GET_POS(victim)) / 3);
	}

	if (GET_MOB_HOLD(victim))
	{
		if (IS_NPC(ch))
			dam = dam * 15 / 10;
		else
			dam = dam * 125 / 100;
	}

	// Cut damage in half if victim has sanct, to a minimum 1
	if (AFF_FLAGGED(victim, AFF_PRISMATICAURA))
		dam *= 2;
	if (AFF_FLAGGED(victim, AFF_SANCTUARY) && dam >= 2)
		dam /= 2;

	// ���������� ����� �� ��������, � ����� ����� � ������
	dam += noparryhit;

	if (!IS_NPC(victim) && IS_CHARMICE(ch))
		dam = dam * 8 / 10;

	if (AFF_FLAGGED(ch, AFF_BELENA_POISON))
		dam -= dam * GET_POISON(ch) / 100;

	// at least 1 hp damage min per hit
	dam = MAX(1, dam);

	// Shopkeeper protection
	if (damage_mtrigger(ch, victim) || !ok_damage_shopkeeper(ch, victim))
	{
		// ������� �� alt_equip, ����� �� ������� ����������� �����
		return;
	}
	if (weapon_pos)
	{
		alt_equip(ch, weapon_pos, dam, 10);
	}
	dam_critic = 0;
	was_critic = 0;
	/* ����, ������ � ��-���������� ������� �� ����� ������� */
	if ((!IS_NPC(ch) && !IS_MAGIC_USER(ch) && !IS_DRUID(ch))
			|| (IS_NPC(ch) && (!AFF_FLAGGED(ch, AFF_CHARM) && !AFF_FLAGGED(ch, AFF_HELPER))))
	{
		was_critic = MIN(ch->get_skill(skill), 70);
		/* Gorrah ���������� ���� �� ������ ��������� ���� ������������ ��������� */
		for (i = PUNCH_MASTER_FEAT; i <= BOWS_MASTER_FEAT; i++)
			if ((ubyte) feat_info[i].affected[0].location == skill && can_use_feat(ch, i))
			{
				was_critic += MAX(0, ch->get_skill(skill) -  70);
				break;
			}
		if (GET_CLASS(ch) == CLASS_THIEF)
			was_critic += ch->get_skill(SKILL_BACKSTAB);
		if (GET_CLASS(ch) == CLASS_PALADINE)
			was_critic += (int)(ch->get_skill(SKILL_PUNCTUAL) / 2);
		if (GET_CLASS(ch) == CLASS_ASSASINE)
			was_critic += (int)(ch->get_skill(SKILL_NOPARRYHIT) / 3);
		if (IS_NPC(ch) && !AFF_FLAGGED(ch, AFF_CHARM))
			was_critic += GET_LEVEL(ch);
	} else was_critic = FALSE; //Polud �� ������ - ��� ����� � �� �������
	//critical hit ignore magic_shields and armour
	if (number(0, 2000) < was_critic)
		was_critic = TRUE;
	else
		was_critic = FALSE;

	if (type == SKILL_BACKSTAB)
	{
		dam *= backstab_mult(GET_LEVEL(ch));
		if (number(1, 100) < calculate_crit_backstab_percent(ch)
			&& !general_savingthrow(ch, victim, SAVING_REFLEX, dex_bonus(GET_REAL_DEX(ch))))
		{
			dam = static_cast<int>(dam * crit_backstab_multiplier(ch, victim));
		}

		//Adept: ��������� ������� �� ����. �����������
		dam = calculate_resistance_coeff(victim, VITALITY_RESISTANCE, dam);
		extdamage(ch, victim, dam, w_type, 0, TRUE);
		return;
	}

	if (type == SKILL_THROW)
	{
		dam *= (calculate_skill(ch, SKILL_THROW, skill_info[SKILL_THROW].max_percent, victim) + 10) / 10;
		if (IS_NPC(ch))
			dam = MIN(300, dam);
		dam = calculate_resistance_coeff(victim, VITALITY_RESISTANCE, dam);
		extdamage(ch, victim, dam, w_type, 0, TRUE);
		return;
	}

	if (GET_AF_BATTLE(ch, EAF_PUNCTUAL)
		&& GET_PUNCTUAL_WAIT(ch) <= 0
		&& GET_WAIT(ch) <= 0
		&& (diceroll >= 18 - GET_MOB_HOLD(victim))
		&& !MOB_FLAGGED(victim, MOB_NOTKILLPUNCTUAL))
	{
		percent = train_skill(ch, SKILL_PUNCTUAL,
				skill_info[SKILL_PUNCTUAL].max_percent, victim);
		if (!PUNCTUAL_WAITLESS(ch))
		{
			PUNCTUAL_WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
		}
		if (percent >= number(1, skill_info[SKILL_PUNCTUAL].max_percent)
			&& (calc_thaco - diceroll < victim_ac - 5
				|| percent >= skill_info[SKILL_PUNCTUAL].max_percent))
		{
			was_critic = TRUE;
			dam_critic = do_punctual(ch, victim, wielded);

			if (!PUNCTUAL_WAITLESS(ch))
			{
				PUNCTUAL_WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
			}
		}
	}

	/* �������� �����, ���� � ����������� ���� ��� */
	if ((GET_AF_BATTLE(ch, EAF_STUPOR)
		|| GET_AF_BATTLE(ch, EAF_MIGHTHIT))
		&& GET_WAIT(ch) > 0)
	{
		CLR_AF_BATTLE(ch, EAF_STUPOR);
		CLR_AF_BATTLE(ch, EAF_MIGHTHIT);
	}

	if (type != TYPE_NOPARRY
		&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
		&& !GET_AF_BATTLE(ch, EAF_STUPOR))
	{
		/**** ���������� �������� ������ */
		for (CHAR_DATA *vict = world[IN_ROOM(ch)]->people; vict && dam >= 0;
			vict = vict->next_in_room)
		{
			hit_touching(ch, vict, &dam);
		}
	}

	if (dam > 0
		&& type != TYPE_NOPARRY
		&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
		&& !GET_AF_BATTLE(ch, EAF_STUPOR)
		&& GET_AF_BATTLE(victim, EAF_DEVIATE)
		&& GET_WAIT(victim) <= 0
		&& !AFF_FLAGGED(victim, AFF_STOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_MAGICSTOPFIGHT)
		&& GET_MOB_HOLD(victim) == 0
		&& BATTLECNTR(victim) < (GET_LEVEL(victim) + 7) / 8)
	{
		/**** ���������� �������   ���������� */
		hit_deviate(ch, victim, &dam);
	}
	else
	if (dam > 0
		&& type != TYPE_NOPARRY
		&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
		&& !GET_AF_BATTLE(ch, EAF_STUPOR)
		&& GET_AF_BATTLE(victim, EAF_PARRY)
		&& !AFF_FLAGGED(victim, AFF_STOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_MAGICSTOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_STOPRIGHT)
		&& !AFF_FLAGGED(victim, AFF_STOPLEFT)
		&& GET_WAIT(victim) <= 0
		&& GET_MOB_HOLD(victim) == 0)
	{
		/**** ���������� �������  ���������� */
		hit_parry(ch, victim, skill, w_type, &dam);
	}
	else
	if (dam > 0
		&& type != TYPE_NOPARRY
		&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
		&& !GET_AF_BATTLE(ch, EAF_STUPOR)
		&& GET_AF_BATTLE(victim, EAF_MULTYPARRY)
		&& !AFF_FLAGGED(victim, AFF_STOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_MAGICSTOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_STOPRIGHT)
		&& !AFF_FLAGGED(victim, AFF_STOPLEFT)
		&& BATTLECNTR(victim) < (GET_LEVEL(victim) + 4) / 5
		&& GET_WAIT(victim) <= 0
		&& GET_MOB_HOLD(victim) == 0)
	{
		/**** ���������� �������  ������� ������ */
		hit_multyparry(ch, victim, skill, w_type, &dam);
	}
	else
	if (dam > 0
		&& type != TYPE_NOPARRY
		&& !GET_AF_BATTLE(ch, EAF_MIGHTHIT)
		&& !GET_AF_BATTLE(ch, EAF_STUPOR)
		&& (GET_AF_BATTLE(victim, EAF_BLOCK) || can_auto_block(victim))
		&& !AFF_FLAGGED(victim, AFF_STOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_MAGICSTOPFIGHT)
		&& !AFF_FLAGGED(victim, AFF_STOPLEFT)
		&& GET_WAIT(victim) <= 0
		&& GET_MOB_HOLD(victim) == 0
		&& BATTLECNTR(victim) < (GET_LEVEL(victim) + 8) / 9)
	{
		/**** ���������� �������   ����������� */
		hit_block(ch, victim, &dam);
	}

	DamageActorParameters params(ch, victim, dam);
	handle_affects(params);
	dam = params.damage;
	DamageVictimParameters params1(ch, victim, dam);
	handle_affects(params1);
	dam = params1.damage;

	int made_dam = extdamage(ch, victim, dam, w_type, wielded, TRUE);
	was_critic = FALSE;
	dam_critic = 0;

	/* check if the victim has a hitprcnt trigger */
	if (made_dam != -1)
	{
		// victim is not dead after hit
		hitprcnt_mtrigger(victim);
	}
}

int GET_MAXDAMAGE(CHAR_DATA * ch)
{
	if (AFF_FLAGGED(ch, AFF_HOLD))
		return 0;
	else
		return GET_DAMAGE(ch);
}

int GET_MAXCASTER(CHAR_DATA * ch)
{
	if (AFF_FLAGGED(ch, AFF_HOLD) || AFF_FLAGGED(ch, AFF_SIELENCE)
			|| GET_WAIT(ch) > 0)
		return 0;
	else
		return IS_IMMORTAL(ch) ? 1 : GET_CASTER(ch);
}

#define GET_HP_PERC(ch) ((int)(GET_HIT(ch) * 100 / GET_MAX_HIT(ch)))
#define POOR_DAMAGE  15
#define POOR_CASTER  5
#define MAX_PROBES   0

int in_same_battle(CHAR_DATA * npc, CHAR_DATA * pc, int opponent)
{
	int ch_friend_npc, ch_friend_pc, vict_friend_npc, vict_friend_pc;
	CHAR_DATA *ch, *vict, *npc_master, *pc_master, *ch_master, *vict_master;

	if (npc == pc)
		return (!opponent);
	if (npc->get_fighting() == pc)	// NPC fight PC - opponent
		return (opponent);
	if (pc->get_fighting() == npc)	// PC fight NPC - opponent
		return (opponent);
	if (npc->get_fighting() && npc->get_fighting() == pc->get_fighting())
		return (!opponent);	// Fight same victim - friend
	if (AFF_FLAGGED(pc, AFF_HORSE) || AFF_FLAGGED(pc, AFF_CHARM))
		return (opponent);

	npc_master = npc->master ? npc->master : npc;
	pc_master = pc->master ? pc->master : pc;

	for (ch = world[IN_ROOM(npc)]->people; ch; ch = ch->next)
	{
		if (!ch->get_fighting())
			continue;
		ch_master = ch->master ? ch->master : ch;
		ch_friend_npc = (ch_master == npc_master) ||
						(IS_NPC(ch) && IS_NPC(npc) &&
						 !AFF_FLAGGED(ch, AFF_CHARM) && !AFF_FLAGGED(npc, AFF_CHARM) &&
						 !AFF_FLAGGED(ch, AFF_HORSE) && !AFF_FLAGGED(npc, AFF_HORSE));
		ch_friend_pc = (ch_master == pc_master) ||
					   (IS_NPC(ch) && IS_NPC(pc) &&
						!AFF_FLAGGED(ch, AFF_CHARM) && !AFF_FLAGGED(pc, AFF_CHARM) &&
						!AFF_FLAGGED(ch, AFF_HORSE) && !AFF_FLAGGED(pc, AFF_HORSE));
		if (ch->get_fighting() == pc && ch_friend_npc)	// Friend NPC fight PC - opponent
			return (opponent);
		if (pc->get_fighting() == ch && ch_friend_npc)	// PC fight friend NPC - opponent
			return (opponent);
		if (npc->get_fighting() == ch && ch_friend_pc)	// NPC fight friend PC - opponent
			return (opponent);
		if (ch->get_fighting() == npc && ch_friend_pc)	// Friend PC fight NPC - opponent
			return (opponent);
		vict = ch->get_fighting();
		vict_master = vict->master ? vict->master : vict;
		vict_friend_npc = (vict_master == npc_master) ||
						  (IS_NPC(vict) && IS_NPC(npc) &&
						   !AFF_FLAGGED(vict, AFF_CHARM) && !AFF_FLAGGED(npc, AFF_CHARM) &&
						   !AFF_FLAGGED(vict, AFF_HORSE) && !AFF_FLAGGED(npc, AFF_HORSE));
		vict_friend_pc = (vict_master == pc_master) ||
						 (IS_NPC(vict) && IS_NPC(pc) &&
						  !AFF_FLAGGED(vict, AFF_CHARM) && !AFF_FLAGGED(pc, AFF_CHARM) &&
						  !AFF_FLAGGED(vict, AFF_HORSE) && !AFF_FLAGGED(pc, AFF_HORSE));
		if (ch_friend_npc && vict_friend_pc)
			return (opponent);	// Friend NPC fight friend PC - opponent
		if (ch_friend_pc && vict_friend_npc)
			return (opponent);	// Friend PC fight friend NPC - opponent
	}

	return (!opponent);
}

CHAR_DATA *find_friend_cure(CHAR_DATA * caster, int spellnum)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0, AFF_USED = 0;
	switch (spellnum)
	{
	case SPELL_CURE_LIGHT:
		AFF_USED = 80;
		break;
	case SPELL_CURE_SERIOUS:
		AFF_USED = 70;
		break;
	case SPELL_EXTRA_HITS:
	case SPELL_CURE_CRITIC:
		AFF_USED = 50;
		break;
	case SPELL_HEAL:
	case SPELL_GROUP_HEAL:
		AFF_USED = 30;
		break;
	}

	if ((AFF_FLAGGED(caster, AFF_CHARM) || MOB_FLAGGED(caster, MOB_ANGEL))
			&& AFF_FLAGGED(caster, AFF_HELPER))
	{
		if (GET_HP_PERC(caster) < AFF_USED)
			return (caster);
		else if (caster->master &&
//         !IS_NPC(caster->master)                    &&
				 CAN_SEE(caster, caster->master) &&
				 IN_ROOM(caster->master) == IN_ROOM(caster) &&
				 caster->master->get_fighting() && GET_HP_PERC(caster->master) < AFF_USED)
			return (caster->master);
		return (NULL);
	}

	for (vict = world[IN_ROOM(caster)]->people; AFF_USED && vict; vict = vict->next_in_room)
	{
		if (!IS_NPC(vict) || AFF_FLAGGED(vict, AFF_CHARM) || (MOB_FLAGGED(vict, MOB_ANGEL)
				&& (vict->master && !IS_NPC(vict->master)))
				|| !CAN_SEE(caster, vict))
			continue;
		if (!vict->get_fighting() && !MOB_FLAGGED(vict, MOB_HELPER))
			continue;
		if (GET_HP_PERC(vict) < AFF_USED && (!victim || vict_val > GET_HP_PERC(vict)))
		{
			victim = vict;
			vict_val = GET_HP_PERC(vict);
			if (GET_REAL_INT(caster) < number(10, 20))
				break;
		}
	}
	return (victim);
}

CHAR_DATA *find_friend(CHAR_DATA * caster, int spellnum)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0, AFF_USED = 0, spellreal = -1;
	switch (spellnum)
	{
	case SPELL_CURE_BLIND:
		SET_BIT(AFF_USED, AFF_BLIND);
		break;
	case SPELL_REMOVE_POISON:
		SET_BIT(AFF_USED, ((AFF_POISON) | (AFF_SCOPOLIA_POISON) | (AFF_BELENA_POISON) | (AFF_DATURA_POISON)));
		break;
	case SPELL_REMOVE_HOLD:
		SET_BIT(AFF_USED, AFF_HOLD);
		break;
	case SPELL_REMOVE_CURSE:
		SET_BIT(AFF_USED, AFF_CURSE);
		break;
	case SPELL_REMOVE_SIELENCE:
		SET_BIT(AFF_USED, AFF_SIELENCE);
		break;
	case SPELL_CURE_PLAQUE:
		spellreal = SPELL_PLAQUE;
		break;

	}
	if ((AFF_FLAGGED(caster, AFF_CHARM) || MOB_FLAGGED(caster, MOB_ANGEL)) && AFF_FLAGGED(caster, AFF_HELPER))
	{
		if (AFF_FLAGGED(caster, AFF_USED) || affected_by_spell(caster, spellreal))
			return (caster);
		else if (caster->master &&
//         !IS_NPC(caster->master)                    &&
				 CAN_SEE(caster, caster->master) && IN_ROOM(caster->master) == IN_ROOM(caster) &&
				 (AFF_FLAGGED(caster->master, AFF_USED) || affected_by_spell(caster->master, spellreal)))
			return (caster->master);
		return (NULL);
	}

	for (vict = world[IN_ROOM(caster)]->people; AFF_USED && vict; vict = vict->next_in_room)
	{
		if (!IS_NPC(vict) || AFF_FLAGGED(vict, AFF_CHARM) || (MOB_FLAGGED(vict, MOB_ANGEL)
				&& (vict->master && !IS_NPC(vict->master)))
				|| !CAN_SEE(caster, vict))
			continue;
		if (!AFF_FLAGGED(vict, AFF_USED))
			continue;
		if (!vict->get_fighting() && !MOB_FLAGGED(vict, MOB_HELPER))
			continue;
		if (!victim || vict_val < GET_MAXDAMAGE(vict))
		{
			victim = vict;
			vict_val = GET_MAXDAMAGE(vict);
			if (GET_REAL_INT(caster) < number(10, 20))
				break;
		}
	}
	return (victim);
}

CHAR_DATA *find_caster(CHAR_DATA * caster, int spellnum)
{
	CHAR_DATA *vict = NULL, *victim = NULL;
	int vict_val = 0, AFF_USED, spellreal = -1;
	AFF_USED = 0;
	switch (spellnum)
	{
	case SPELL_CURE_BLIND:
		SET_BIT(AFF_USED, AFF_BLIND);
		break;
	case SPELL_REMOVE_POISON:
		SET_BIT(AFF_USED, ((AFF_POISON) | (AFF_SCOPOLIA_POISON) | (AFF_BELENA_POISON) | (AFF_DATURA_POISON)));
		break;
	case SPELL_REMOVE_HOLD:
		SET_BIT(AFF_USED, AFF_HOLD);
		break;
	case SPELL_REMOVE_CURSE:
		SET_BIT(AFF_USED, AFF_CURSE);
		break;
	case SPELL_REMOVE_SIELENCE:
		SET_BIT(AFF_USED, AFF_SIELENCE);
		break;
	case SPELL_CURE_PLAQUE:
		spellreal = SPELL_PLAQUE;
		break;
	}

	if ((AFF_FLAGGED(caster, AFF_CHARM) || MOB_FLAGGED(caster, MOB_ANGEL)) && AFF_FLAGGED(caster, AFF_HELPER))
	{
		if (AFF_FLAGGED(caster, AFF_USED) || affected_by_spell(caster, spellreal))
			return (caster);
		else if (caster->master &&
//         !IS_NPC(caster->master)                    &&
				 CAN_SEE(caster, caster->master) && IN_ROOM(caster->master) == IN_ROOM(caster) &&
				 (AFF_FLAGGED(caster->master, AFF_USED) || affected_by_spell(caster->master, spellreal)))
			return (caster->master);
		return (NULL);
	}

	for (vict = world[IN_ROOM(caster)]->people; AFF_USED && vict; vict = vict->next_in_room)
	{
		if (!IS_NPC(vict) || AFF_FLAGGED(vict, AFF_CHARM) || (MOB_FLAGGED(vict, MOB_ANGEL)
				&& (vict->master && !IS_NPC(vict->master)))
				|| !CAN_SEE(caster, vict))
			continue;
		if (!AFF_FLAGGED(vict, AFF_USED))
			continue;
		if (!vict->get_fighting() && !MOB_FLAGGED(vict, MOB_HELPER))
			continue;
		if (!victim || vict_val < GET_MAXCASTER(vict))
		{
			victim = vict;
			vict_val = GET_MAXCASTER(vict);
			if (GET_REAL_INT(caster) < number(10, 20))
				break;
		}
	}
	return (victim);
}


CHAR_DATA *find_affectee(CHAR_DATA * caster, int spellnum)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0, spellreal = spellnum;

	if (spellreal == SPELL_GROUP_ARMOR)
		spellreal = SPELL_ARMOR;
	else if (spellreal == SPELL_GROUP_STRENGTH)
		spellreal = SPELL_STRENGTH;
	else if (spellreal == SPELL_GROUP_BLESS)
		spellreal = SPELL_BLESS;
	else if (spellreal == SPELL_GROUP_HASTE)
		spellreal = SPELL_HASTE;
	else if (spellreal == SPELL_GROUP_SANCTUARY)
		spellreal = SPELL_SANCTUARY;
	else if (spellreal == SPELL_GROUP_PRISMATICAURA)
		spellreal = SPELL_PRISMATICAURA;

	if ((AFF_FLAGGED(caster, AFF_CHARM) || MOB_FLAGGED(caster, MOB_ANGEL)) && AFF_FLAGGED(caster, AFF_HELPER))
	{
		if (!affected_by_spell(caster, spellreal))
			return (caster);
		else if (caster->master &&
//         !IS_NPC(caster->master)                    &&
				 CAN_SEE(caster, caster->master) &&
				 IN_ROOM(caster->master) == IN_ROOM(caster) &&
				 caster->master->get_fighting() && !affected_by_spell(caster->master, spellreal))
			return (caster->master);
		return (NULL);
	}

	if (GET_REAL_INT(caster) > number(5, 15))
		for (vict = world[IN_ROOM(caster)]->people; vict; vict = vict->next_in_room)
		{
			if (!IS_NPC(vict) || AFF_FLAGGED(vict, AFF_CHARM) || (MOB_FLAGGED(vict, MOB_ANGEL)
					&& (vict->master
						&& !IS_NPC(vict->master)))
					|| !CAN_SEE(caster, vict))
				continue;
			if (!vict->get_fighting() || AFF_FLAGGED(vict, AFF_HOLD) || affected_by_spell(vict, spellreal))
				continue;
			if (!victim || vict_val < GET_MAXDAMAGE(vict))
			{
				victim = vict;
				vict_val = GET_MAXDAMAGE(vict);
			}
		}
	if (!victim && !affected_by_spell(caster, spellreal))
		victim = caster;

	return (victim);
}

CHAR_DATA *find_opp_affectee(CHAR_DATA * caster, int spellnum)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0, spellreal = spellnum;

	if (spellreal == SPELL_POWER_HOLD || spellreal == SPELL_MASS_HOLD)
		spellreal = SPELL_HOLD;
	else if (spellreal == SPELL_POWER_BLINDNESS || spellreal == SPELL_MASS_BLINDNESS)
		spellreal = SPELL_BLINDNESS;
	else if (spellreal == SPELL_POWER_SIELENCE || spellreal == SPELL_MASS_SIELENCE)
		spellreal = SPELL_SIELENCE;
	else if (spellreal == SPELL_MASS_CURSE)
		spellreal = SPELL_CURSE;
	else if (spellreal == SPELL_MASS_SLOW)
		spellreal = SPELL_SLOW;

	if (GET_REAL_INT(caster) > number(10, 20))
		for (vict = world[caster->in_room]->people; vict; vict = vict->next_in_room)
		{
			if ((IS_NPC(vict) && !((MOB_FLAGGED(vict, MOB_ANGEL)
									|| AFF_FLAGGED(vict, AFF_CHARM)) && (vict->master
																		 && !IS_NPC(vict->master))))
					|| !CAN_SEE(caster, vict))
				continue;
			if ((!vict->get_fighting()
					&& (GET_REAL_INT(caster) < number(20, 27)
						|| !in_same_battle(caster, vict, TRUE)))
					|| AFF_FLAGGED(vict, AFF_HOLD)
					|| affected_by_spell(vict, spellreal))
				continue;
			if (!victim || vict_val < GET_MAXDAMAGE(vict))
			{
				victim = vict;
				vict_val = GET_MAXDAMAGE(vict);
			}
		}

	if (!victim && caster->get_fighting()
			&& !affected_by_spell(caster->get_fighting(), spellreal))
		victim = caster->get_fighting();
	return (victim);
}

CHAR_DATA *find_opp_caster(CHAR_DATA * caster)
{
	CHAR_DATA *vict = NULL, *victim = NULL;
	int vict_val = 0;

	for (vict = world[IN_ROOM(caster)]->people; vict; vict = vict->next_in_room)
	{
		if (IS_NPC(vict) &&
//         !AFF_FLAGGED(vict,AFF_CHARM) &&
				!(MOB_FLAGGED(vict, MOB_ANGEL)
				  && (vict->master && !IS_NPC(vict->master))))
			continue;
		if ((!vict->get_fighting()
				&& (GET_REAL_INT(caster) < number(15, 25)
					|| !in_same_battle(caster, vict, TRUE)))
				|| AFF_FLAGGED(vict, AFF_HOLD) || AFF_FLAGGED(vict, AFF_SIELENCE)
				|| (!CAN_SEE(caster, vict) && caster->get_fighting() != vict))
			continue;
		if (vict_val < GET_MAXCASTER(vict))
		{
			victim = vict;
			vict_val = GET_MAXCASTER(vict);
		}
	}
	return (victim);
}

CHAR_DATA *find_damagee(CHAR_DATA * caster)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0;

	if (GET_REAL_INT(caster) > number(10, 20))
		for (vict = world[IN_ROOM(caster)]->people; vict; vict = vict->next_in_room)
		{
			if ((IS_NPC(vict) && !((MOB_FLAGGED(vict, MOB_ANGEL)
									|| AFF_FLAGGED(vict, AFF_CHARM)) && (vict->master
																		 && !IS_NPC(vict->master))))
					|| !CAN_SEE(caster, vict))
				continue;
			if ((!vict->get_fighting()
					&& (GET_REAL_INT(caster) < number(20, 27)
						|| !in_same_battle(caster, vict, TRUE)))
					|| AFF_FLAGGED(vict, AFF_HOLD))
				continue;
			if (GET_REAL_INT(caster) >= number(25, 30))
			{
				if (!victim || vict_val < GET_MAXCASTER(vict))
				{
					victim = vict;
					vict_val = GET_MAXCASTER(vict);
				}
			}
			else if (!victim || vict_val < GET_MAXDAMAGE(vict))
			{
				victim = vict;
				vict_val = GET_MAXDAMAGE(vict);
			}
		}
	if (!victim)
		victim = caster->get_fighting();

	return (victim);
}

CHAR_DATA *find_minhp(CHAR_DATA * caster)
{
	CHAR_DATA *vict, *victim = NULL;
	int vict_val = 0;

	if (GET_REAL_INT(caster) > number(10, 20))
		for (vict = world[IN_ROOM(caster)]->people; vict; vict = vict->next_in_room)
		{
			if ((IS_NPC(vict) && !((MOB_FLAGGED(vict, MOB_ANGEL)
									|| AFF_FLAGGED(vict, AFF_CHARM)) && (vict->master
																		 && !IS_NPC(vict->master))))
					|| !CAN_SEE(caster, vict))
				continue;
			if (!vict->get_fighting() && (GET_REAL_INT(caster) < number(20, 27)
									|| !in_same_battle(caster, vict, TRUE)))
				continue;
			if (!victim || vict_val > GET_HIT(vict))
			{
				victim = vict;
				vict_val = GET_HIT(vict);
			}
		}
	if (!victim)
		victim = caster->get_fighting();

	return (victim);
}

CHAR_DATA *find_cure(CHAR_DATA * caster, CHAR_DATA * patient, int *spellnum)
{
	if (GET_HP_PERC(patient) <= number(20, 33))
	{
		if (GET_SPELL_MEM(caster, SPELL_EXTRA_HITS))
			*spellnum = SPELL_EXTRA_HITS;
		else if (GET_SPELL_MEM(caster, SPELL_HEAL))
			*spellnum = SPELL_HEAL;
		else if (GET_SPELL_MEM(caster, SPELL_CURE_CRITIC))
			*spellnum = SPELL_CURE_CRITIC;
		else if (GET_SPELL_MEM(caster, SPELL_GROUP_HEAL))
			*spellnum = SPELL_GROUP_HEAL;
	}
	else if (GET_HP_PERC(patient) <= number(50, 65))
	{
		if (GET_SPELL_MEM(caster, SPELL_CURE_CRITIC))
			*spellnum = SPELL_CURE_CRITIC;
		else if (GET_SPELL_MEM(caster, SPELL_CURE_SERIOUS))
			*spellnum = SPELL_CURE_SERIOUS;
		else if (GET_SPELL_MEM(caster, SPELL_CURE_LIGHT))
			*spellnum = SPELL_CURE_LIGHT;
	}
	if (*spellnum)
		return (patient);
	else
		return (NULL);
}

void mob_casting(CHAR_DATA * ch)
{
	CHAR_DATA *victim;
	int battle_spells[MAX_STRING_LENGTH];
	int lag = GET_WAIT(ch), i, spellnum, spells, sp_num;
	OBJ_DATA *item;

	if (AFF_FLAGGED(ch, AFF_CHARM) || AFF_FLAGGED(ch, AFF_HOLD) || AFF_FLAGGED(ch, AFF_SIELENCE) || lag > 0)
		return;

	memset(&battle_spells, 0, sizeof(battle_spells));
	for (i = 1, spells = 0; i <= MAX_SPELLS; i++)
		if (GET_SPELL_MEM(ch, i) && IS_SET(spell_info[i].routines, NPC_CALCULATE))
			battle_spells[spells++] = i;

	for (item = ch->carrying;
			spells < MAX_STRING_LENGTH &&
			item &&
			GET_RACE(ch) == NPC_RACE_HUMAN &&
			!MOB_FLAGGED(ch, MOB_ANGEL) && !AFF_FLAGGED(ch, AFF_CHARM); item = item->next_content)
		switch (GET_OBJ_TYPE(item))
		{
		case ITEM_WAND:
		case ITEM_STAFF:
			if (GET_OBJ_VAL(item, 2) > 0 &&
					IS_SET(spell_info[GET_OBJ_VAL(item, 3)].routines, NPC_CALCULATE))
				battle_spells[spells++] = GET_OBJ_VAL(item, 3);
			break;
		case ITEM_POTION:
			for (i = 1; i <= 3; i++)
				if (IS_SET
						(spell_info[GET_OBJ_VAL(item, i)].routines,
						 NPC_AFFECT_NPC | NPC_UNAFFECT_NPC | NPC_UNAFFECT_NPC_CASTER))
					battle_spells[spells++] = GET_OBJ_VAL(item, i);
			break;
		case ITEM_SCROLL:
			for (i = 1; i <= 3; i++)
				if (IS_SET(spell_info[GET_OBJ_VAL(item, i)].routines, NPC_CALCULATE))
					battle_spells[spells++] = GET_OBJ_VAL(item, i);
			break;
		}

	// �����-�������  -  ����� ����
	spellnum = 0;
	victim = find_cure(ch, ch, &spellnum);
	// ���� ��������� ���������� � ���� ��� ���
	for (i = 0; !victim && spells && i < GET_REAL_INT(ch) / 5; i++)
		if (!spellnum && (spellnum = battle_spells[(sp_num = number(0, spells - 1))])
				&& spellnum > 0 && spellnum <= MAX_SPELLS)  	// sprintf(buf,"$n using spell '%s', %d from %d",
		{
			//         spell_name(spellnum), sp_num, spells);
			// act(buf,FALSE,ch,0,ch->get_fighting(),TO_VICT);
			if (spell_info[spellnum].routines & NPC_DAMAGE_PC_MINHP)
			{
				if (!AFF_FLAGGED(ch, AFF_CHARM))
					victim = find_minhp(ch);
			}
			else if (spell_info[spellnum].routines & NPC_DAMAGE_PC)
			{
				if (!AFF_FLAGGED(ch, AFF_CHARM))
					victim = find_damagee(ch);
			}
			else if (spell_info[spellnum].routines & NPC_AFFECT_PC_CASTER)
			{
				if (!AFF_FLAGGED(ch, AFF_CHARM))
					victim = find_opp_caster(ch);
			}
			else if (spell_info[spellnum].routines & NPC_AFFECT_PC)
			{
				if (!AFF_FLAGGED(ch, AFF_CHARM))
					victim = find_opp_affectee(ch, spellnum);
			}
			else if (spell_info[spellnum].routines & NPC_AFFECT_NPC)
				victim = find_affectee(ch, spellnum);
			else if (spell_info[spellnum].routines & NPC_UNAFFECT_NPC_CASTER)
				victim = find_caster(ch, spellnum);
			else if (spell_info[spellnum].routines & NPC_UNAFFECT_NPC)
				victim = find_friend(ch, spellnum);
			else if (spell_info[spellnum].routines & NPC_DUMMY)
				victim = find_friend_cure(ch, spellnum);
			else
				spellnum = 0;
		}
	if (spellnum && victim)  	// Is this object spell ?
	{
		for (item = ch->carrying;
				!AFF_FLAGGED(ch, AFF_CHARM) &&
				!MOB_FLAGGED(ch, MOB_ANGEL) && item && GET_RACE(ch) == NPC_RACE_HUMAN; item = item->next_content)
			switch (GET_OBJ_TYPE(item))
			{
			case ITEM_WAND:
			case ITEM_STAFF:
				if (GET_OBJ_VAL(item, 2) > 0 && GET_OBJ_VAL(item, 3) == spellnum)
				{
					mag_objectmagic(ch, item, GET_NAME(victim));
					return;
				}
				break;
			case ITEM_POTION:
				for (i = 1; i <= 3; i++)
					if (GET_OBJ_VAL(item, i) == spellnum)
					{
						if (ch != victim)
						{
							obj_from_char(item);
							act("$n �������$g $o3 $N2.", FALSE, ch, item, victim, TO_ROOM);
							obj_to_char(item, victim);
						}
						else
							victim = ch;
						mag_objectmagic(victim, item, GET_NAME(victim));
						return;
					}
				break;
			case ITEM_SCROLL:
				for (i = 1; i <= 3; i++)
					if (GET_OBJ_VAL(item, i) == spellnum)
					{
						mag_objectmagic(ch, item, GET_NAME(victim));
						return;
					}
				break;
			}

		cast_spell(ch, victim, 0, NULL, spellnum, spellnum);
	}
}

#define  MAY_LIKES(ch)   ((!AFF_FLAGGED(ch, AFF_CHARM) || AFF_FLAGGED(ch, AFF_HELPER)) && \
                          AWAKE(ch) && GET_WAIT(ch) <= 0)

#define	MAY_ACT(ch)	(!(AFF_FLAGGED(ch, AFF_STOPFIGHT) || AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT) || GET_MOB_HOLD(ch) || GET_WAIT(ch)))

/* control the fights going on.  Called every 2 seconds from comm.c. */
void perform_violence(void)
{
	CHAR_DATA *ch, *vict, *caster = NULL, *damager = NULL;
	int i, do_this, initiative, max_init = 0, min_init = 100, sk_use = 0, sk_num = 0;
	struct helper_data_type *helpee;
	struct follow_type *k, *k_next;

	// Step 0.0 Summons mob helpers

	for (ch = combat_list; ch; ch = next_combat_list)
	{
		next_combat_list = ch->next_fighting;
		// Extract battler if no opponent
		if (ch->get_fighting() == NULL || IN_ROOM(ch) != IN_ROOM(ch->get_fighting()) || IN_ROOM(ch) == NOWHERE)
		{
			stop_fighting(ch, TRUE);
			continue;
		}
		if (GET_MOB_HOLD(ch) ||
				!IS_NPC(ch) ||
				GET_WAIT(ch) > 0 ||
				GET_POS(ch) < POS_FIGHTING ||
				AFF_FLAGGED(ch, AFF_CHARM) ||
				AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT) ||
				AFF_FLAGGED(ch, AFF_STOPFIGHT) || AFF_FLAGGED(ch, AFF_SIELENCE))
			continue;

		if (!PRF_FLAGGED(ch->get_fighting(), PRF_NOHASSLE))
			for (sk_use = 0, helpee = GET_HELPER(ch); helpee; helpee = helpee->next_helper)
				for (vict = character_list; vict; vict = vict->next)
				{
					if (!IS_NPC(vict) ||
							GET_MOB_VNUM(vict) != helpee->mob_vnum ||
							AFF_FLAGGED(ch, AFF_CHARM) ||
							AFF_FLAGGED(vict, AFF_HOLD) ||
							AFF_FLAGGED(vict, AFF_CHARM) ||
							AFF_FLAGGED(vict, AFF_BLIND) ||
							GET_WAIT(vict) > 0 ||
							GET_POS(vict) < POS_STANDING || IN_ROOM(vict) == NOWHERE || vict->get_fighting())
						continue;
					if (!sk_use && GET_RACE(ch) == NPC_RACE_HUMAN)
						act("$n �������$g : \"�� ������, ��� ������ ��������� !\"",
							FALSE, ch, 0, 0, TO_ROOM);
					if (IN_ROOM(vict) != IN_ROOM(ch))
					{
						char_from_room(vict);
						char_to_room(vict, IN_ROOM(ch));
						act("$n ������$g �� ��� � �������$g �� ������ $N2.", FALSE,
							vict, 0, ch, TO_ROOM);
					}
					else
						act("$n �������$g � ����� �� ������� $N1.", FALSE, vict, 0,
							ch, TO_ROOM);
					set_fighting(vict, ch->get_fighting());
				};
	}


	// Step 1. Define initiative, mob casting and mob flag skills
	for (ch = combat_list; ch; ch = next_combat_list)
	{
		next_combat_list = ch->next_fighting;
		// Initialize initiative
		INITIATIVE(ch) = 0;
		BATTLECNTR(ch) = 0;
		ROUND_COUNTER(ch) += 1;
		DpsSystem::check_round(ch);
		BattleRoundParameters params(ch);
		handle_affects(params);

		round_num_mtrigger(ch, ch->get_fighting());

		SET_AF_BATTLE(ch, EAF_STAND);
		if (affected_by_spell(ch, SPELL_SLEEP))
			SET_AF_BATTLE(ch, EAF_SLEEP);
		if (IN_ROOM(ch) == NOWHERE)
			continue;

		if (GET_MOB_HOLD(ch) || AFF_FLAGGED(ch, AFF_STOPFIGHT)
				|| AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT))
//������
		{
			for (k = ch->followers; k; k = k_next)
			{
				k_next = k->next;
				if (AFF_FLAGGED(k->follower, AFF_HELPER) &&
						MOB_FLAGGED(k->follower, MOB_ANGEL) &&
						!k->follower->get_fighting() &&
						IN_ROOM(k->follower) == IN_ROOM(ch) &&
						CAN_SEE(k->follower, ch) && AWAKE(k->follower) &&
						MAY_ACT(k->follower) && GET_POS(k->follower) >= POS_FIGHTING)
				{
					for (vict = world[IN_ROOM(ch)]->people; vict; vict = vict->next_in_room)
						if (vict->get_fighting() == ch && vict != ch && vict != k->follower)
							break;
					if (vict && k->follower->get_skill(SKILL_RESCUE))  	//if(GET_MOB_VNUM(k->follower)==108)
					{
//       act("TRYING RESC for STOPFIGHT", TRUE, ch, 0, 0, TO_CHAR);
						go_rescue(k->follower, ch, vict);
					}
				}
			}
			continue;
		}
		// Mobs stand up and players sit
		if (GET_POS(ch) < POS_FIGHTING &&
				GET_POS(ch) > POS_STUNNED &&
				GET_WAIT(ch) <= 0 && !GET_MOB_HOLD(ch) && !AFF_FLAGGED(ch, AFF_SLEEP))
		{
			if (IS_NPC(ch))
			{
				act("$n �����$g �� ����.", TRUE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_FIGHTING;
			}
			else if (GET_POS(ch) == POS_SLEEPING)
			{
				act("�� ���������� � ����.", TRUE, ch, 0, 0, TO_CHAR);
				act("$n �������$u � ���$g.", TRUE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_SITTING;
			}
			else if (GET_POS(ch) == POS_RESTING)
			{
				act("�� ���������� ����� � ����.", TRUE, ch, 0, 0, TO_CHAR);
				act("$n ���������$g ����� � ���$g.", TRUE, ch, 0, 0, TO_ROOM);
				GET_POS(ch) = POS_SITTING;
			}
		}
		// For NPC without lags and charms make it likes
		if (IS_NPC(ch) && MAY_LIKES(ch))  	// Get weapon from room
		{
			//edited by WorM 2010.09.03 ������� ������� ������ ����� � ��� ���� � ���� ���� ���-�� � ���� �� �� �����
			//���������� � ��� ���-�� ���� ��������� ��� ��� ���-�� ��� ����� ��������
			if (!AFF_FLAGGED(ch, AFF_CHARM))//���� ���� �����-�� ���, ���������� ���� ������ ����� ����� ���� ��� �������
			{
				npc_battle_scavenge(ch);//����� ����
				if(ch->carrying)//� ���� ���� ���-�� � ���� �� �����������, ���������
				{
					npc_wield(ch);
					npc_armor(ch);
				}
			}
			//end by WorM
			//dzMUDiST. ���������� ���������� ���������� � ��� �� ����� ���� �������
			if (ch->last_comm != NULL)
			{
				command_interpreter(ch, ch->last_comm);
				free(ch->last_comm);
				ch->last_comm = NULL;
			}
			// Set some flag-skills
			// 1) parry
			do_this = number(0, 100);
			sk_use = FALSE;
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_PARRY))
			{
				SET_AF_BATTLE(ch, EAF_PARRY);
				sk_use = TRUE;
			}
			// 2) blocking
			do_this = number(0, 100);
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_BLOCK))
			{
				SET_AF_BATTLE(ch, EAF_BLOCK);
				sk_use = TRUE;
			}
			// 3) multyparry
			do_this = number(0, 100);
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_MULTYPARRY))
			{
				SET_AF_BATTLE(ch, EAF_MULTYPARRY);
				sk_use = TRUE;
			}

			// 4) deviate
			do_this = number(0, 100);
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_DEVIATE))
			{
				SET_AF_BATTLE(ch, EAF_DEVIATE);
				sk_use = TRUE;
			}
			// 5) stupor
			do_this = number(0, 100);
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_STUPOR))
			{
				SET_AF_BATTLE(ch, EAF_STUPOR);
				sk_use = TRUE;
			}
			// 6) mighthit
			do_this = number(0, 100);
			if (!sk_use && do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_MIGHTHIT))
			{
				SET_AF_BATTLE(ch, EAF_MIGHTHIT);
				sk_use = TRUE;
			}
			// 7) styles
			do_this = number(0, 100);
			if (do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_AWAKE) > number(1, 101))
				SET_AF_BATTLE(ch, EAF_AWAKE);
			else
				CLR_AF_BATTLE(ch, EAF_AWAKE);
			do_this = number(0, 100);
			if (do_this <= GET_LIKES(ch) && ch->get_skill(SKILL_PUNCTUAL) > number(1, 101))
				SET_AF_BATTLE(ch, EAF_PUNCTUAL);
			else
				CLR_AF_BATTLE(ch, EAF_PUNCTUAL);
		}

		initiative = size_app[GET_POS_SIZE(ch)].initiative;
		if ((i = number(1, 10)) == 10)
			initiative -= 1;
		else
			initiative += i;

		initiative += GET_INITIATIVE(ch);
		if (!IS_NPC(ch))
			switch (IS_CARRYING_W(ch) * 10 / MAX(1, CAN_CARRY_W(ch)))
			{
			case 10:
			case 9:
			case 8:
				initiative -= 2;
				break;
			case 7:
			case 6:
			case 5:
				initiative -= 1;
				break;
			}

		if (GET_AF_BATTLE(ch, EAF_AWAKE))
			initiative -= 2;
		if (GET_AF_BATTLE(ch, EAF_PUNCTUAL))
			initiative -= 1;
		if (AFF_FLAGGED(ch, AFF_SLOW))
			initiative -= 10;
		if (AFF_FLAGGED(ch, AFF_HASTE))
			initiative += 10;
		if (GET_WAIT(ch) > 0)
			initiative -= 1;
		if (calc_leadership(ch))
			initiative += 5;
		if (GET_AF_BATTLE(ch, EAF_SLOW))
			initiative = 1;

		initiative = MAX(initiative, 1);
		INITIATIVE(ch) = initiative;
		SET_AF_BATTLE(ch, EAF_FIRST);
		max_init = MAX(max_init, initiative);
		min_init = MIN(min_init, initiative);
	}

	/* Process fighting           */
	for (initiative = max_init; initiative >= min_init; initiative--)
		for (ch = combat_list; ch; ch = next_combat_list)
		{
			next_combat_list = ch->next_fighting;
			if (INITIATIVE(ch) != initiative || IN_ROOM(ch) == NOWHERE)
				continue;
			// If mob cast 'hold' when initiative setted
			if (AFF_FLAGGED(ch, AFF_HOLD) ||
					AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT) || AFF_FLAGGED(ch, AFF_STOPFIGHT) || !AWAKE(ch))
				continue;
			// If mob cast 'fear', 'teleport', 'recall', etc when initiative setted
			if (!ch->get_fighting() || IN_ROOM(ch) != IN_ROOM(ch->get_fighting()))
				continue;

			if (IS_NPC(ch))  	// Select extra_attack type
			{
				// if (GET_AF_BATTLE(ch,EAF_MULTYPARRY))
				//    continue;

				// �������� ������� ����� ������� ������ ���� (���������� ��� ����������)
				fight_mtrigger(ch);

				// ������������
				if (MAY_LIKES(ch)
					&& !AFF_FLAGGED(ch, AFF_CHARM)
					&& !AFF_FLAGGED(ch, AFF_NOT_SWITCH)
					&& GET_REAL_INT(ch) > number(15, 25))
				{
					perform_mob_switch(ch);
				}

				// Cast spells
				if (MAY_LIKES(ch))
					mob_casting(ch);
				if (!ch->get_fighting() || IN_ROOM(ch) != IN_ROOM(ch->get_fighting()) || AFF_FLAGGED(ch, AFF_HOLD) ||	// mob_casting ��� �� ������� ����������
						AFF_FLAGGED(ch, AFF_STOPFIGHT) || !AWAKE(ch) || AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT))
					continue;

				if ((AFF_FLAGGED(ch, AFF_CHARM) || MOB_FLAGGED(ch, MOB_ANGEL)) && AFF_FLAGGED(ch, AFF_HELPER) && ch->master &&	// !IS_NPC(ch->master)  &&
						CAN_SEE(ch, ch->master) &&
						IN_ROOM(ch) == IN_ROOM(ch->master) && AWAKE(ch) &&
						MAY_ACT(ch) && GET_POS(ch) >= POS_FIGHTING)
				{
					for (vict = world[IN_ROOM(ch)]->people; vict; vict = vict->next_in_room)
						if (vict->get_fighting() == ch->master && vict != ch && vict != ch->master)
							break;
					if (vict && (ch->get_skill(SKILL_RESCUE)	// ���� �����-�� || GET_REAL_INT(ch) < number(0,100)
								))  	//if(GET_MOB_VNUM(ch)==108 && ch->master)
					{
//                       act("TRYING to RESCUE in FIGHT", TRUE, ch->master, 0, 0, TO_CHAR);
						go_rescue(ch, ch->master, vict);
					}
					else if (vict && ch->get_skill(SKILL_PROTECT))
						go_protect(ch, ch->master);
				}
				else if (!AFF_FLAGGED(ch, AFF_CHARM))
					for (sk_num = 0, sk_use = GET_REAL_INT(ch)
											  /*,sprintf(buf,"sk_use {%d}-GET_WAIT {%d}\r\n",sk_use,GET_WAIT(ch))
											  ,send_to_char(buf,ch->get_fighting())*/  ;
							MAY_LIKES(ch) && sk_use > 0; sk_use--)
					{
						do_this = number(0, 100);
						if (do_this > GET_LIKES(ch))
							continue;
						do_this = number(0, 100);
						//sprintf(buf,"<%d>\r\n",do_this);
						//send_to_char(buf,ch->get_fighting());
						if (do_this < 10)
							sk_num = SKILL_BASH;
						else if (do_this < 20)
							sk_num = SKILL_DISARM;
						else if (do_this < 30)
							sk_num = SKILL_KICK;
						else if (do_this < 40)
							sk_num = SKILL_PROTECT;
						else if (do_this < 50)
							sk_num = SKILL_RESCUE;
						else if (do_this < 60 && !ch->get_touching())
							sk_num = SKILL_TOUCH;
						else if (do_this < 70)
							sk_num = SKILL_CHOPOFF;
						//edited by WorM 2010.09.03 ������� ������� ����, ���� �� ���� ��� ��� ���� � ������ ������������
						else if (do_this < 80)
							sk_num = SKILL_THROW;
						//end by WorM
						else
							sk_num = SKILL_BASH;
						if (ch->get_skill(sk_num) <= 0)
							sk_num = 0;
						if (!sk_num)
							continue;
						//else
						//   act("Victim prepare to skill '$F'.",FALSE,ch->get_fighting(),0,skill_name(sk_num),TO_CHAR);
						/* ���� ����� ������ � �������� �����������, �� ������ ������� */
						//edited by WorM 2010.09.03 ���������� ���� ���� ��� ����� ��� � ����������� ������� � ������ �������
						/*if (ch->get_skill(SKILL_THROW))
							if (GET_EQ(ch, WEAR_WIELD))
								if (OBJ_FLAGGED(GET_EQ(ch, WEAR_WIELD), ITEM_THROWING))
									if (GET_REAL_INT(ch) > number(1, 36))
										sk_num = SKILL_THROW;*/

						if (sk_num == SKILL_TOUCH)
						{
							sk_use = 0;
							go_touch(ch, ch->get_fighting());
						}

						if (sk_num == SKILL_THROW)
						{
							sk_use = 0;
							/* ���� �������� �� ������� */
							for (vict = world[IN_ROOM(ch)]->people, i = 0; vict;
									vict = vict->next_in_room)
							{
								if (!IS_NPC(vict))
									i++;
							}
							if (i > 0)
							{
								caster = NULL;
								i = number(1, i);
								for (vict = world[IN_ROOM(ch)]->people; i;
										vict = vict->next_in_room)
								{
									if (!IS_NPC(vict))
									{
										i--;
										caster = vict;
									}
								}
							}
							/* ������ */
							if (caster)
								go_throw(ch, caster);
						}
// �������� �� ������ ������, �������� �� ��� �������, ���� ����� �� ����� ������ �� ����
						if (!(MOB_FLAGGED(ch, MOB_ANGEL) && ch->master)
								&& (sk_num == SKILL_RESCUE || sk_num == SKILL_PROTECT))
						{
							CHAR_DATA *attacker;
							int dumb_mob;
							caster = NULL;
							damager = NULL;
							dumb_mob = (int)(GET_REAL_INT(ch) < number(5, 20));
							for (attacker = world[IN_ROOM(ch)]->people;
									attacker; attacker = attacker->next_in_room)
							{
								vict = attacker->get_fighting();	// ��������� ������
								if (!vict ||	// ������ ���
										(!IS_NPC(vict) || AFF_FLAGGED(vict, AFF_CHARM) || AFF_FLAGGED(vict, AFF_HELPER)) ||	// ������ - �� ���
										(IS_NPC(attacker) &&
										 !(AFF_FLAGGED(attacker, AFF_CHARM)
										   && attacker->master && !IS_NPC(attacker->master))
										 && !(MOB_FLAGGED(attacker, MOB_ANGEL)
											  && attacker->master
											  && !IS_NPC(attacker->master))
										 //�� ������ �������, ����� ��� ���� && !AFF_FLAGGED(attacker,AFF_HELPER)
										) ||	// ���� ������� (����)
										!CAN_SEE(ch, vict) ||	// �� �����, ���� ����� �������
										ch == vict	// ���� ������� �� �����
								   )
									continue;

								// ���� ������� vict �� attacker
								if (!caster ||	// ��� ���� ������ �� ������
										(GET_HIT(vict) < GET_HIT(caster))	// ����� ���� ����
								   )
								{
									caster = vict;
									damager = attacker;
									if (dumb_mob)
										break;	// ����� ��� ������� �������
								}
							}

							if (sk_num == SKILL_RESCUE && caster && damager)
							{
								sk_use = 0;
								go_rescue(ch, caster, damager);
							}
							if (sk_num == SKILL_PROTECT && caster)
							{
								sk_use = 0;
								go_protect(ch, caster);
							}
						}

						if (sk_num == SKILL_BASH || sk_num == SKILL_CHOPOFF
								|| sk_num == SKILL_DISARM)
						{
							caster = NULL;
							damager = NULL;
							if (GET_REAL_INT(ch) < number(15, 25))
							{
								caster = ch->get_fighting();
								damager = ch->get_fighting();
							}
							else
							{
								for (vict = world[IN_ROOM(ch)]->people; vict;
										vict = vict->next_in_room)
								{
									if ((IS_NPC(vict)
											&& !AFF_FLAGGED(vict, AFF_CHARM))
											|| !vict->get_fighting())
										continue;
									if ((AFF_FLAGGED(vict, AFF_HOLD)
											&& GET_POS(vict) < POS_FIGHTING)
											|| (IS_CASTER(vict)
												&& (AFF_FLAGGED(vict, AFF_HOLD)
													|| AFF_FLAGGED(vict, AFF_SIELENCE)
													|| GET_WAIT(vict) > 0)))
										continue;
									if (!caster || (IS_CASTER(vict)
													&& GET_CASTER(vict) >
													GET_CASTER(caster)))
										caster = vict;
									if (!damager
											|| GET_DAMAGE(vict) > GET_DAMAGE(damager))
										damager = vict;
								}
							}
							if (caster &&
									(CAN_SEE(ch, caster) || ch->get_fighting() == caster) &&
									GET_CASTER(caster) > POOR_CASTER &&
									(sk_num == SKILL_BASH || sk_num == SKILL_CHOPOFF))
							{
								if (sk_num == SKILL_BASH)
								{
									if (GET_POS(caster) >= POS_FIGHTING ||
											calculate_skill(ch, SKILL_BASH, 200,
															caster) > number(50, 80))
									{
										sk_use = 0;
										go_bash(ch, caster);
									}
								}
								else
								{
									if (GET_POS(caster) >= POS_FIGHTING ||
											calculate_skill(ch, SKILL_CHOPOFF, 200,
															caster) > number(50, 80))
									{
										sk_use = 0;
										go_chopoff(ch, caster);
									}
								}
							}
							if (sk_use &&
									damager &&
									(CAN_SEE(ch, damager) || ch->get_fighting() == damager))
							{
								if (sk_num == SKILL_BASH)
								{
									if (on_horse(damager))
									{
										// �������. ������ ����. ������ �� ������ ������ ����, ���� ������� � ����������.
										if (get_horse(damager) == ch)
										{
											horse_drop(ch);
										}
										else
										{
											sk_use = 0;
											go_bash(ch, get_horse(damager));
										}
									}
									else
										if (GET_POS(damager) >= POS_FIGHTING ||
												calculate_skill(ch, SKILL_BASH, 200,
																damager) > number(50,
																				  80))
										{
											sk_use = 0;
											go_bash(ch, damager);
										}
								}
								else if (sk_num == SKILL_CHOPOFF)
								{
									if (on_horse(damager))
									{
										sk_use = 0;
										go_chopoff(ch, get_horse(damager));
									}
									else
										if (GET_POS(damager) >= POS_FIGHTING ||
												calculate_skill(ch, SKILL_CHOPOFF, 200,
																damager) > number(50,
																				  80))
										{
											sk_use = 0;
											go_chopoff(ch, damager);
										}
								}
								else
									if (sk_num == SKILL_DISARM &&
											(GET_EQ(damager, WEAR_WIELD) ||
											 GET_EQ(damager, WEAR_BOTHS) ||
											 (GET_EQ(damager, WEAR_HOLD)
											  /* shapirus: �������� �� ����� �������, �.�. ��� ���� � do_disarm() � go_disarm()
											                                    &&
											  				  GET_OBJ_TYPE (GET_EQ (damager, WEAR_HOLD))
											  				  == ITEM_WEAPON*/
											 )))
									{
										sk_use = 0;
										go_disarm(ch, damager);
									}
							}
						}

						if (sk_num == SKILL_KICK && !on_horse(ch->get_fighting()))
						{
							sk_use = 0;
							go_kick(ch, ch->get_fighting());
						}
					}

				if (!ch->get_fighting() || IN_ROOM(ch) != IN_ROOM(ch->get_fighting()))
					continue;

				/***** ���� �������� ������� ��� ����� */
				if (!AFF_FLAGGED(ch, AFF_STOPRIGHT))
					exthit(ch, TYPE_UNDEFINED, RIGHT_WEAPON);

				/***** ����������� */
				for (i = 1; i <= ch->mob_specials.ExtraAttack; i++)
				{
					if (AFF_FLAGGED(ch, AFF_STOPFIGHT) ||
							AFF_FLAGGED(ch, AFF_MAGICSTOPFIGHT) ||
							(i == 1 && AFF_FLAGGED(ch, AFF_STOPLEFT)))
						continue;
					exthit(ch, TYPE_UNDEFINED, i + RIGHT_WEAPON);
				}
			}
			else  	/* PLAYERS - only one hit per round */
			{

				if (GET_POS(ch) > POS_STUNNED &&
						GET_POS(ch) < POS_FIGHTING && GET_AF_BATTLE(ch, EAF_STAND))
				{
					sprintf(buf, "%s��� ����� ������ �� ���� !%s\r\n",
							CCWHT(ch, C_NRM), CCNRM(ch, C_NRM));
					send_to_char(buf, ch);
					CLR_AF_BATTLE(ch, EAF_STAND);
				}

				if (ch->get_cast_spell() && GET_WAIT(ch) <= 0)
				{
					if (AFF_FLAGGED(ch, AFF_SIELENCE))
						send_to_char("�� �� ������ ��������� � �����.\r\n", ch);
					else
					{
						cast_spell(ch, ch->get_cast_char(), ch->get_cast_obj(),
								   0, ch->get_cast_spell(), ch->get_cast_subst());
						if (!(IS_IMMORTAL(ch) || GET_GOD_FLAG(ch, GF_GODSLIKE)
								|| CHECK_WAIT(ch)))
							WAIT_STATE(ch, PULSE_VIOLENCE);
						ch->set_cast(0, 0, 0, 0, 0);
					}
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (GET_AF_BATTLE(ch, EAF_MULTYPARRY))
					continue;

				// TODO: � switch ���� ���������
				if (ch->get_extra_skill() == SKILL_THROW && ch->get_extra_victim() && GET_WAIT(ch) <= 0)
				{
					go_throw(ch, ch->get_extra_victim());
					ch->set_extra_attack(0, 0);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (ch->get_extra_skill() == SKILL_BASH && ch->get_extra_victim() && GET_WAIT(ch) <= 0)
				{
					go_bash(ch, ch->get_extra_victim());
					ch->set_extra_attack(0, 0);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (ch->get_extra_skill() == SKILL_KICK && ch->get_extra_victim() && GET_WAIT(ch) <= 0)
				{
					go_kick(ch, ch->get_extra_victim());
					ch->set_extra_attack(0, 0);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (ch->get_extra_skill() == SKILL_CHOPOFF && ch->get_extra_victim() && GET_WAIT(ch) <= 0)
				{
					go_chopoff(ch, ch->get_extra_victim());
					ch->set_extra_attack(0, 0);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (ch->get_extra_skill() == SKILL_DISARM && ch->get_extra_victim() && GET_WAIT(ch) <= 0)
				{
					go_disarm(ch, ch->get_extra_victim());
					ch->set_extra_attack(0, 0);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				if (!ch->get_fighting() || IN_ROOM(ch) != IN_ROOM(ch->get_fighting()))
					continue;
				/***** ���� �������� ������� ��� ����� */
				if (GET_AF_BATTLE(ch, EAF_FIRST))
				{
					if (!AFF_FLAGGED(ch, AFF_STOPRIGHT) &&
							(IS_IMMORTAL(ch) ||
							 GET_GOD_FLAG(ch, GF_GODSLIKE) || !GET_AF_BATTLE(ch, EAF_USEDRIGHT)))
						exthit(ch, TYPE_UNDEFINED, RIGHT_WEAPON);
					CLR_AF_BATTLE(ch, EAF_FIRST);
					SET_AF_BATTLE(ch, EAF_SECOND);
					if (INITIATIVE(ch) > min_init)
					{
						INITIATIVE(ch)--;
						continue;
					}
				}

				/***** ���� ������ ������� ���� ��� ���� � ������ ��������� */
				if (GET_EQ(ch, WEAR_HOLD) &&
						GET_OBJ_TYPE(GET_EQ(ch, WEAR_HOLD)) == ITEM_WEAPON &&
						GET_AF_BATTLE(ch, EAF_SECOND) &&
						!AFF_FLAGGED(ch, AFF_STOPLEFT) &&
						(IS_IMMORTAL(ch) ||
						 GET_GOD_FLAG(ch, GF_GODSLIKE) || ch->get_skill(SKILL_SATTACK) > number(1, 101)))
				{
					if (IS_IMMORTAL(ch) || GET_GOD_FLAG(ch, GF_GODSLIKE) ||
							!GET_AF_BATTLE(ch, EAF_USEDLEFT))
						exthit(ch, TYPE_UNDEFINED, LEFT_WEAPON);
					CLR_AF_BATTLE(ch, EAF_SECOND);
				}
				else
					/***** ���� ������ ����� ���� ��� �������� � ������ ��������� */
					if (!GET_EQ(ch, WEAR_HOLD) && !GET_EQ(ch, WEAR_LIGHT) &&
							!GET_EQ(ch, WEAR_SHIELD) && !GET_EQ(ch, WEAR_BOTHS) &&
							!AFF_FLAGGED(ch, AFF_STOPLEFT) &&
							GET_AF_BATTLE(ch, EAF_SECOND) && ch->get_skill(SKILL_SHIT))
					{
						if (IS_IMMORTAL(ch) || !GET_AF_BATTLE(ch, EAF_USEDLEFT))
							exthit(ch, TYPE_UNDEFINED, LEFT_WEAPON);
						CLR_AF_BATTLE(ch, EAF_SECOND);
					}

// ������� ������, �.�. ������� �� ���������� �������
// check if angel is in fight, and go_rescue if it is not
				for (k = ch->followers; k; k = k_next)
				{
					k_next = k->next;
					if (AFF_FLAGGED(k->follower, AFF_HELPER) &&
							MOB_FLAGGED(k->follower, MOB_ANGEL) &&
							!k->follower->get_fighting() &&
							IN_ROOM(k->follower) == IN_ROOM(ch) &&
							CAN_SEE(k->follower, ch) && AWAKE(k->follower) &&
							MAY_ACT(k->follower) && GET_POS(k->follower) >= POS_FIGHTING)
					{
						for (vict = world[IN_ROOM(ch)]->people; vict; vict = vict->next_in_room)
							if (vict->get_fighting() == ch && vict != ch && vict != k->follower)
								break;
						if (vict && k->follower->get_skill(SKILL_RESCUE))
						{
//                if(GET_MOB_VNUM(k->follower)==108)
//                  act("TRYING to RESCUE without FIGHTING", TRUE, ch, 0, 0, TO_CHAR);
							go_rescue(k->follower, ch, vict);
						}
					}
				}




			}
		}

	/* Decrement mobs lag */
	for (ch = combat_list; ch; ch = ch->next_fighting)
	{
		if (IN_ROOM(ch) == NOWHERE)
			continue;

		CLR_AF_BATTLE(ch, EAF_FIRST);
		CLR_AF_BATTLE(ch, EAF_SECOND);
		CLR_AF_BATTLE(ch, EAF_USEDLEFT);
		CLR_AF_BATTLE(ch, EAF_USEDRIGHT);
		CLR_AF_BATTLE(ch, EAF_MULTYPARRY);
		if (GET_AF_BATTLE(ch, EAF_SLEEP))
			affect_from_char(ch, SPELL_SLEEP);
		if (GET_AF_BATTLE(ch, EAF_BLOCK))
		{
			CLR_AF_BATTLE(ch, EAF_BLOCK);
			if (!WAITLESS(ch) && GET_WAIT(ch) < PULSE_VIOLENCE)
				WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
		}
		if (GET_AF_BATTLE(ch, EAF_DEVIATE))
		{
			CLR_AF_BATTLE(ch, EAF_DEVIATE);
			if (!WAITLESS(ch) && GET_WAIT(ch) < PULSE_VIOLENCE)
				WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
		}
		if (GET_AF_BATTLE(ch, EAF_POISONED))
			CLR_AF_BATTLE(ch, EAF_POISONED);
		battle_affect_update(ch);
	}
}

// returns 1 if only ch was outcasted
// returns 2 if only victim was outcasted
// returns 4 if both were outcasted
// returns 0 if none was outcasted
int check_agro_follower(CHAR_DATA * ch, CHAR_DATA * victim)
{
	CHAR_DATA *cleader, *vleader;
	int return_value = 0;
	if (ch == victim)
		return return_value;
// translating pointers from charimces to their leaders
	if (IS_NPC(ch) && ch->master && (AFF_FLAGGED(ch, AFF_CHARM) || MOB_FLAGGED(ch, MOB_ANGEL) || IS_HORSE(ch)))
		ch = ch->master;
	if (IS_NPC(victim) && victim->master &&
			(AFF_FLAGGED(victim, AFF_CHARM) || MOB_FLAGGED(victim, MOB_ANGEL) || IS_HORSE(victim)))
		victim = victim->master;
	cleader = ch;
	vleader = victim;
// finding leaders
	while (cleader->master)
	{
		if (IS_NPC(cleader) &&
				!AFF_FLAGGED(cleader, AFF_CHARM) && !MOB_FLAGGED(cleader, MOB_ANGEL) && !IS_HORSE(cleader))
			break;
		cleader = cleader->master;
	}
	while (vleader->master)
	{
		if (IS_NPC(vleader) &&
				!AFF_FLAGGED(vleader, AFF_CHARM) && !MOB_FLAGGED(vleader, MOB_ANGEL) && !IS_HORSE(vleader))
			break;
		vleader = vleader->master;
	}
	if (cleader != vleader)
		return return_value;


// finding closest to the leader nongrouped agressor
// it cannot be a charmice
	while (ch->master && ch->master->master)
	{
		if (!AFF_FLAGGED(ch->master, AFF_GROUP) && !IS_NPC(ch->master))
		{
			ch = ch->master;
			continue;
		}
		else if (IS_NPC(ch->master)
				 && !AFF_FLAGGED(ch->master->master, AFF_GROUP)
				 && !IS_NPC(ch->master->master) && ch->master->master->master)
		{
			ch = ch->master->master;
			continue;
		}
		else
			break;
	}

// finding closest to the leader nongrouped victim
// it cannot be a charmice
	while (victim->master && victim->master->master)
	{
		if (!AFF_FLAGGED(victim->master, AFF_GROUP)
				&& !IS_NPC(victim->master))
		{
			victim = victim->master;
			continue;
		}
		else if (IS_NPC(victim->master)
				 && !AFF_FLAGGED(victim->master->master, AFF_GROUP)
				 && !IS_NPC(victim->master->master)
				 && victim->master->master->master)
		{
			victim = victim->master->master;
			continue;
		}
		else
			break;
	}
	if (!AFF_FLAGGED(ch, AFF_GROUP) || cleader == victim)
	{
		stop_follower(ch, SF_EMPTY);
		return_value |= 1;
	}
	if (!AFF_FLAGGED(victim, AFF_GROUP) || vleader == ch)
	{
		stop_follower(victim, SF_EMPTY);
		return_value |= 2;
	}
	return return_value;
}
