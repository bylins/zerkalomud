/*************************************************************************
*   File: features.cpp                                 Part of Bylins    *
*   Features code                                                        *
*                                                                        *
*  $Author$                                                     *
*  $Date$                                          *
*  $Revision$                	                                 *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "handler.h"
#include "comm.h"
#include "db.h"
#include "interpreter.h"
#include "skills.h"
#include "spells.h"
#include "features.hpp"
#include "char.hpp"
#include "player_races.hpp"
#include "room.hpp"
#include "house.h"
#include "screen.h"
#include "pk.h"
#include "dg_scripts.h"
#include "room.hpp"
#include "house.h"
#include "screen.h"
#include "pk.h"
#include "dg_scripts.h"


extern const char *unused_spellname;

struct feat_info_type feat_info[MAX_FEATS];

void unused_feat(int feat);
void assign_feats(void);
int find_feat_num(char *name);
bool can_use_feat(CHAR_DATA *ch, int feat);
bool can_get_feat(CHAR_DATA *ch, int feat);
bool find_feat_slot(CHAR_DATA *ch, int feat);
int feature_mod(int feat, int location);
void check_berserk(CHAR_DATA * ch);

ACMD(do_lightwalk);

/*
   ��������� ����� ��� �������� �������� �������� � ������ affected ��������� �����������
   ���� � ����-�� ���� �������, ����� ������ ������� ������������ ��� ���� �����, ����������
   ������ ������� � ���������, ������ ������� ���� � ���������� �����������
   ������ ����� �������� ������� ��������� � ���������� feat_info �� ����
*/
class aff_array
{
public:
	explicit aff_array() : _pos(0), i(MAX_FEAT_AFFECT) {}

	int pos(int pos = -1)
	{
		if (pos == -1)
		{
			return _pos;
		}
		else if (pos >= 0 && pos < MAX_FEAT_AFFECT)
		{
			_pos = pos;
			return _pos;
		}
		sprintf(buf, "SYSERR: invalid arg passed to features::aff_aray.pos!");
		mudlog(buf, BRF, LVL_GOD, SYSLOG, TRUE);
	}

	void insert(byte location, sbyte modifier)
	{
		affected[_pos].location = location;
		affected[_pos].modifier = modifier;
		_pos++;
		if (_pos >= MAX_FEAT_AFFECT)
			_pos = 0;
	}

	void clear()
	{
		_pos = 0;
		for (i = 0; i < MAX_FEAT_AFFECT; i++)
		{
			affected[i].location = APPLY_NONE;
			affected[i].modifier = 0;
		}
	}

	struct obj_affected_type
				affected[MAX_FEAT_AFFECT];
private:
	int _pos, i;
};

/* ����� ������ ����������� �� ����� */
int find_feat_num(char *name)
{
	int index, ok;
	char const *temp, *temp2;
	char first[256], first2[256];
	for (index = 1; index < MAX_FEATS; index++)
	{
		if (is_abbrev(name, feat_info[index].name))
			return (index);
		ok = TRUE;
		/* It won't be changed, but other uses of this function elsewhere may. */
		temp = any_one_arg(feat_info[index].name, first);
		temp2 = any_one_arg(name, first2);
		while (*first && *first2 && ok)
		{
			if (!is_abbrev(first2, first))
				ok = FALSE;
			temp = any_one_arg(temp, first);
			temp2 = any_one_arg(temp2, first2);
		}

		if (ok && !*first2)
			return (index);
	}
	return (-1);
}

/* ������������� ����������� ��������� ���������� */
void feato(int feat, const char *name, int type, bool can_up_slot, aff_array app)
{
	int i, j;
	for (i = 0; i < NUM_CLASSES; i++)
		for (j = 0; j < NUM_KIN; j++)
		{
			feat_info[feat].min_remort[i][j] = 0;
			feat_info[feat].slot[i][j] = 0;
		}
	feat_info[feat].name = name;
	feat_info[feat].type = type;
	feat_info[feat].up_slot = can_up_slot;
	for (i = 0; i < MAX_FEAT_AFFECT; i++)
	{
		feat_info[feat].affected[i].location = app.affected[i].location;
		feat_info[feat].affected[i].modifier = app.affected[i].modifier;
	}
}

/* ������������� ��� unused features */
void unused_feat(int feat)
{
	int i, j;

	for (i = 0; i < NUM_CLASSES; i++)
		for (j = 0; j < NUM_KIN; j++)
		{
			feat_info[feat].min_remort[i][j] = 0;
			feat_info[feat].slot[i][j] = 0;
			feat_info[feat].natural_classfeat[i][j] = FALSE;
			feat_info[feat].classknow[i][j] = FALSE;
		}

	feat_info[feat].name = unused_spellname;
	feat_info[feat].type = UNUSED_FTYPE;
	feat_info[feat].up_slot = FALSE;

	for (i = 0; i < MAX_FEAT_AFFECT; i++)
	{
		feat_info[feat].affected[i].location = APPLY_NONE;
		feat_info[feat].affected[i].modifier = 0;
	}
}

/* ������������� ������� �������� ������������ */
void assign_feats(void)
{
	int i;
	aff_array feat_app;
	for (i = 0; i < MAX_FEATS; i++)
	{
		unused_feat(i);
	}

//1
	feato(BERSERK_FEAT, "������������ ������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//2
	feato(PARRY_ARROW_FEAT, "������ ������", NORMAL_FTYPE, TRUE, feat_app);
//3
	feato(BLIND_FIGHT_FEAT, "������ ���", NORMAL_FTYPE, TRUE, feat_app);
//4
	feat_app.insert(APPLY_MR, 1);
	feat_app.insert(APPLY_AR, 1);
	feato(IMPREGNABLE_FEAT, "�������������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//5-*
	feato(APPROACHING_ATTACK_FEAT, "��������� �����", NORMAL_FTYPE, TRUE, feat_app);
//6-*
	feato(DODGE_FEAT, "�������", NORMAL_FTYPE, TRUE, feat_app);
//7-*
	feato(TWO_WEAPON_FIGHTING_FEAT, "��������� ���", NORMAL_FTYPE, TRUE, feat_app);
//8
	feato(LIGHT_WALK_FEAT, "������ �������", NORMAL_FTYPE, TRUE, feat_app);
//9-*
	feato(DEPOSIT_FINDING_FEAT, "�����������", NORMAL_FTYPE, TRUE, feat_app);
//10
	feato(SPELL_SUBSTITUTE_FEAT, "������� ����������", NORMAL_FTYPE, TRUE, feat_app);
//11
	feato(POWER_ATTACK_FEAT, "������ �����", NORMAL_FTYPE, TRUE, feat_app);
//12
	feat_app.insert(APPLY_RESIST_FIRE, 5);
	feat_app.insert(APPLY_RESIST_AIR, 5);
	feat_app.insert(APPLY_RESIST_WATER, 5);
	feat_app.insert(APPLY_RESIST_EARTH, 5);
	feato(WOODEN_SKIN_FEAT, "���������� ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//13
	feat_app.insert(APPLY_RESIST_FIRE, 10);
	feat_app.insert(APPLY_RESIST_AIR, 10);
	feat_app.insert(APPLY_RESIST_WATER, 10);
	feat_app.insert(APPLY_RESIST_EARTH, 10);
	feat_app.insert(APPLY_ABSORBE, 5);
	feato(IRON_SKIN_FEAT, "�������� ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//14
	feat_app.insert(FEAT_TIMER, 8);
	feato(CONNOISEUR_FEAT, "������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//15
	feato(EXORCIST_FEAT, "���������� ������", SKILL_MOD_FTYPE, TRUE, feat_app);
//16
	feato(HEALER_FEAT, "��������", NORMAL_FTYPE, TRUE, feat_app);
//17
	feat_app.insert(APPLY_SAVING_REFLEX, -4);
	feato(LIGHTING_REFLEX_FEAT, "���������� �������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//18
	feat_app.insert(FEAT_TIMER, 8);
	feato(DRUNKARD_FEAT, "�������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//19 -*
	feato(DIEHARD_FEAT, "�������� ������", NORMAL_FTYPE, TRUE, feat_app);
//20
	feat_app.insert(APPLY_MOVEREG, 40);
	feato(ENDURANCE_FEAT, "������������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//21
	feat_app.insert(APPLY_SAVING_WILL, -4);
	feat_app.insert(APPLY_SAVING_STABILITY, -4);
	feato(GREAT_FORTITUDE_FEAT, "���� ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//22
	feat_app.insert(APPLY_HITREG, 35);
	feato(FAST_REGENERATION_FEAT, "������� ����������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//23
	feato(STEALTHY_FEAT, "������������", SKILL_MOD_FTYPE, TRUE, feat_app);
//24
	feat_app.insert(APPLY_CAST_SUCCESS, 25);
	feato(RELATED_TO_MAGIC_FEAT, "���������� �������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//25 -*
	feat_app.insert(APPLY_HITREG, 10);
	feat_app.insert(APPLY_SAVING_CRITICAL, -4);
	feato(SPLENDID_HEALTH_FEAT, "����������� ��������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//26
	feato(TRACKER_FEAT, "��������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//27
	feato(WEAPON_FINESSE_FEAT, "������ ����", NORMAL_FTYPE, TRUE, feat_app);
//28
	feato(COMBAT_CASTING_FEAT, "������ ����������", NORMAL_FTYPE, TRUE, feat_app);
//29
	feat_app.insert(SKILL_PUNCH, APPLY_NONE);
	feat_app.insert(PUNCH_FOCUS_FEAT, APPLY_NONE);
	feato(PUNCH_MASTER_FEAT, "������ ��������� ���", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//30
	feat_app.insert(SKILL_CLUBS, APPLY_NONE);
	feat_app.insert(CLUB_FOCUS_FEAT, APPLY_NONE);
	feato(CLUBS_MASTER_FEAT, "������ ������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//31
	feat_app.insert(SKILL_AXES, APPLY_NONE);
	feat_app.insert(AXES_FOCUS_FEAT, APPLY_NONE);
	feato(AXES_MASTER_FEAT, "������ ������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//32
	feat_app.insert(SKILL_LONGS, APPLY_NONE);
	feat_app.insert(LONGS_FOCUS_FEAT, APPLY_NONE);
	feato(LONGS_MASTER_FEAT, "������ ����", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//33
	feat_app.insert(SKILL_SHORTS, APPLY_NONE);
	feat_app.insert(SHORTS_FOCUS_FEAT, APPLY_NONE);
	feato(SHORTS_MASTER_FEAT, "������ ����", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//34
	feat_app.insert(SKILL_NONSTANDART, APPLY_NONE);
	feat_app.insert(NONSTANDART_FOCUS_FEAT, APPLY_NONE);
	feato(NONSTANDART_MASTER_FEAT, "������ ���������� ������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//35
	feat_app.insert(SKILL_BOTHHANDS, APPLY_NONE);
	feat_app.insert(BOTHHANDS_FOCUS_FEAT, APPLY_NONE);
	feato(BOTHHANDS_MASTER_FEAT, "������ ����������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//36
	feat_app.insert(SKILL_PICK, APPLY_NONE);
	feat_app.insert(PICK_FOCUS_FEAT, APPLY_NONE);
	feato(PICK_MASTER_FEAT, "������ �������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//37
	feat_app.insert(SKILL_SPADES, APPLY_NONE);
	feat_app.insert(SPADES_FOCUS_FEAT, APPLY_NONE);
	feato(SPADES_MASTER_FEAT, "������ �����", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//38
	feat_app.insert(SKILL_BOWS, APPLY_NONE);
	feat_app.insert(BOWS_FOCUS_FEAT, APPLY_NONE);
	feato(BOWS_MASTER_FEAT, "������-������", NORMAL_FTYPE, TRUE, feat_app);
	feat_app.clear();
//39
	feato(FOREST_PATHS_FEAT, "������ �����", NORMAL_FTYPE, TRUE, feat_app);
//40
	feato(MOUNTAIN_PATHS_FEAT, "������ �����", NORMAL_FTYPE, TRUE, feat_app);
//41
	feat_app.insert(APPLY_MORALE, 5);
	feato(LUCKY_FEAT, "�����������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//42
	feato(SPIRIT_WARRIOR_FEAT, "������ ���", NORMAL_FTYPE, TRUE, feat_app);
//43
 	feat_app.insert(APPLY_HITREG, 50);
	feato(RELIABLE_HEALTH_FEAT, "������� ��������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//44
	feat_app.insert(APPLY_MANAREG, 100);
	feato(EXCELLENT_MEMORY_FEAT, "������������ ������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//45
	feat_app.insert(APPLY_DEX, 1);
	feato(ANIMAL_DEXTERY_FEAT, "�������� �����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//46
	feat_app.insert(APPLY_MANAREG, 25);
	feato(LEGIBLE_WRITTING_FEAT, "ޣ���� ������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//47
	feat_app.insert(APPLY_DAMROLL, 2);
	feato(IRON_MUSCLES_FEAT, "�������� �����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//48
	feat_app.insert(APPLY_CAST_SUCCESS, 5);
	feato(MAGIC_SIGN_FEAT, "���� �������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//49
	feat_app.insert(APPLY_MOVEREG, 75);
	feato(GREAT_ENDURANCE_FEAT, "������������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//50
	feat_app.insert(APPLY_MORALE, 5);
	feato(BEST_DESTINY_FEAT, "������ ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//51
	feato(BREW_POTION_FEAT, "�������", NORMAL_FTYPE, TRUE, feat_app);
//52
	feato(JUGGLER_FEAT, "�������", NORMAL_FTYPE, TRUE, feat_app);
//53
	feato(NIMBLE_FINGERS_FEAT, "������", SKILL_MOD_FTYPE, TRUE, feat_app);
//54
	feato(GREAT_POWER_ATTACK_FEAT, "���������� ������ �����", NORMAL_FTYPE, TRUE, feat_app);
//55
	feat_app.insert(APPLY_RESIST_IMMUNITY, 15);
	feato(IMMUNITY_FEAT, "�������� � ���", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//56
	feat_app.insert(APPLY_AC, -40);
	feato(MOBILITY_FEAT, "�����������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//57
	feat_app.insert(APPLY_STR, 1);
	feato(NATURAL_STRENGTH_FEAT, "�����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//58
	feat_app.insert(APPLY_DEX, 1);
	feato(NATURAL_DEXTERY_FEAT, "����������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//59
	feat_app.insert(APPLY_INT, 1);
	feato(NATURAL_INTELLECT_FEAT, "��������� ��", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//60
	feat_app.insert(APPLY_WIS, 1);
	feato(NATURAL_WISDOM_FEAT, "������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//61
	feat_app.insert(APPLY_CON, 1);
	feato(NATURAL_CONSTITUTION_FEAT, "��������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//62
	feat_app.insert(APPLY_CHA, 1);
	feato(NATURAL_CHARISMA_FEAT, "��������� �������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//63
	feat_app.insert(APPLY_MANAREG, 25);
	feato(MNEMONIC_ENHANCER_FEAT, "�������� ������", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//64 -*
	feat_app.insert(SKILL_LEADERSHIP, 5);
	feato(MAGNETIC_PERSONALITY_FEAT, "������������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//65
	feat_app.insert(APPLY_DAMROLL, 2);
	feato(DAMROLL_BONUS_FEAT, "����� �� ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//66
	feat_app.insert(APPLY_HITROLL, 1);
	feato(HITROLL_BONUS_FEAT, "������� ����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//67
	feat_app.insert(APPLY_CAST_SUCCESS, 5);
	feato(MAGICAL_INSTINCT_FEAT, "���������� �����", AFFECT_FTYPE, TRUE, feat_app);
	feat_app.clear();
//68
	feat_app.insert(SKILL_PUNCH, APPLY_NONE);
	feato(PUNCH_FOCUS_FEAT, "�������_������: ����� ����", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//69
	feat_app.insert(SKILL_CLUBS, APPLY_NONE);
	feato(CLUB_FOCUS_FEAT, "�������_������: ������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//70
	feat_app.insert(SKILL_AXES, APPLY_NONE);
	feato(AXES_FOCUS_FEAT, "�������_������: ������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//71
	feat_app.insert(SKILL_LONGS, APPLY_NONE);
	feato(LONGS_FOCUS_FEAT, "�������_������: ���", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//72
	feat_app.insert(SKILL_SHORTS, APPLY_NONE);
	feato(SHORTS_FOCUS_FEAT, "�������_������: ���", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//73
	feat_app.insert(SKILL_NONSTANDART, APPLY_NONE);
	feato(NONSTANDART_FOCUS_FEAT, "�������_������: ���������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//74
	feat_app.insert(SKILL_BOTHHANDS, APPLY_NONE);
	feato(BOTHHANDS_FOCUS_FEAT, "�������_������: ���������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//75
	feat_app.insert(SKILL_PICK, APPLY_NONE);
	feato(PICK_FOCUS_FEAT, "�������_������: ������", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//76
	feat_app.insert(SKILL_SPADES, APPLY_NONE);
	feato(SPADES_FOCUS_FEAT, "�������_������: �����", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//77
	feat_app.insert(SKILL_BOWS, APPLY_NONE);
	feato(BOWS_FOCUS_FEAT, "�������_������: ���", SKILL_MOD_FTYPE, TRUE, feat_app);
	feat_app.clear();
//78
	feato(AIMING_ATTACK_FEAT, "���������� �����", NORMAL_FTYPE, TRUE, feat_app);
//79
	feato(GREAT_AIMING_ATTACK_FEAT, "���������� ���������� �����", NORMAL_FTYPE, TRUE, feat_app);
//80
	feato(DOUBLESHOT_FEAT, "������� �������", NORMAL_FTYPE, TRUE, feat_app);
//81
	feato(PORTER_FEAT, "���������", NORMAL_FTYPE, TRUE, feat_app);
//82
	feato(RUNE_NEWBIE_FEAT, "����������� ���", NORMAL_FTYPE, TRUE, feat_app);
//83
	feato(RUNE_USER_FEAT, "������ ����", NORMAL_FTYPE, TRUE, feat_app);
//84
	feato(RUNE_MASTER_FEAT, "�������� ����", NORMAL_FTYPE, TRUE, feat_app);
//85
	feato(RUNE_ULTIMATE_FEAT, "���� �����", NORMAL_FTYPE, TRUE, feat_app);
//86
	feato(TO_FIT_ITEM_FEAT, "����������", NORMAL_FTYPE, TRUE, feat_app);
//87
	feato(TO_FIT_CLOTHCES_FEAT, "��������", NORMAL_FTYPE, TRUE, feat_app);
//88
	feato(STRENGTH_CONCETRATION_FEAT, "������������ ����", NORMAL_FTYPE, TRUE, feat_app);
//89
	feato(DARK_READING_FEAT, "������� ����", NORMAL_FTYPE, TRUE, feat_app);
//90
	feato(SPELL_CAPABLE_FEAT, "����������", NORMAL_FTYPE, TRUE, feat_app);
//91
	feato(ARMOR_LIGHT_FEAT, "������ �������", NORMAL_FTYPE, TRUE, feat_app);
//92
	feato(ARMOR_MEDIAN_FEAT, "������� �������", NORMAL_FTYPE, TRUE, feat_app);
//93
	feato(ARMOR_HEAVY_FEAT, "������� �������", NORMAL_FTYPE, TRUE, feat_app);
//94
	feato(GEMS_INLAY_FEAT, "�����������", NORMAL_FTYPE, TRUE, feat_app);
//95
	feato(WARRIOR_STR_FEAT, "����������� ����", NORMAL_FTYPE, TRUE, feat_app);
//96
	feato(RELOCATE_FEAT, "�������������", NORMAL_FTYPE, TRUE, feat_app);
	/*
	//
		feato(AIR_MAGIC_FOCUS_FEAT, "�������_�����: ������", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(FIRE_MAGIC_FOCUS_FEAT, "�������_�����: �����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(WATER_MAGIC_FOCUS_FEAT, "�������_�����: ����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(EARTH_MAGIC_FOCUS_FEAT, "�������_�����: �����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(LIGHT_MAGIC_FOCUS_FEAT, "�������_�����: ����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(DARK_MAGIC_FOCUS_FEAT, "�������_�����: ����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(MIND_MAGIC_FOCUS_FEAT, "�������_�����: �����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	//
		feato(LIFE_MAGIC_FOCUS_FEAT, "�������_�����: �����", SKILL_MOD_FTYPE, TRUE, feat_app);
		feat_app.clear();
	*/
}

/* ����� �� �������� ������������ �����������? �������� �� ������, ��������, ���������� ���������, �����������. */
bool can_use_feat(CHAR_DATA *ch, int feat)
{
	if (!HAVE_FEAT(ch, feat))
		return FALSE;
	if (IS_NPC(ch))
		return TRUE;
	if (NUM_LEV_FEAT(ch) < feat_info[feat].slot[(int) GET_CLASS(ch)][(int) GET_KIN(ch)])
		return FALSE;
	if (GET_REMORT(ch) < feat_info[feat].min_remort[(int) GET_CLASS(ch)][(int) GET_KIN(ch)])
		return FALSE;

	switch (feat)
	{
	case WEAPON_FINESSE_FEAT:
		if (GET_REAL_DEX(ch) < GET_REAL_STR(ch) || GET_REAL_DEX(ch) < 18)
			return FALSE;
		break;
	case PARRY_ARROW_FEAT:
		if (GET_REAL_DEX(ch) < 16)
			return FALSE;
		break;
	case POWER_ATTACK_FEAT:
		if (GET_REAL_STR(ch) < 20)
			return FALSE;
		break;
	case GREAT_POWER_ATTACK_FEAT:
		if (GET_REAL_STR(ch) < 22)
			return FALSE;
		break;
	case AIMING_ATTACK_FEAT:
		if (GET_REAL_DEX(ch) < 16)
			return FALSE;
		break;
	case GREAT_AIMING_ATTACK_FEAT:
		if (GET_REAL_DEX(ch) < 18)
			return FALSE;
		break;
	case DOUBLESHOT_FEAT:
		if (ch->get_skill(SKILL_BOWS) < 40)
			return FALSE;
		break;
	}
	return TRUE;
}

/* ����� �� �������� ������� ��� �����������? */
bool can_get_feat(CHAR_DATA *ch, int feat)
{
	int i, count = 0;
	if (feat <= 0 || feat >= MAX_FEATS)
	{
		sprintf(buf, "�������� ����� ����������� (feat=%d, ch=%s) ������� � features::can_get_feat!",
			feat, ch->get_name());
		mudlog(buf, BRF, LVL_IMMORT, SYSLOG, TRUE);
		return FALSE;
	}
	/* ����������� �� ������, �������. */
	if ((!feat_info[feat].classknow[(int) GET_CLASS(ch)][(int) GET_KIN(ch)] && !PlayerRace::FeatureCheck(GET_KIN(ch),GET_RACE(ch),feat)) ||
			(GET_REMORT(ch) < feat_info[feat].min_remort[(int) GET_CLASS(ch)][(int) GET_KIN(ch)]))
		return FALSE;

	/* ������� ��������� ������ */
	if (!find_feat_slot(ch, feat))
		return FALSE;

	/* ����������� ���������� ��� �������� */
	switch (feat)
	{
	case PARRY_ARROW_FEAT:
		if (!ch->get_skill(SKILL_MULTYPARRY) && !ch->get_skill(SKILL_PARRY))
			return FALSE;
		break;
	case CONNOISEUR_FEAT:
		if (!ch->get_skill(SKILL_IDENTIFY))
			return FALSE;
		break;
	case EXORCIST_FEAT:
		if (!ch->get_skill(SKILL_TURN_UNDEAD))
			return FALSE;
		break;
	case HEALER_FEAT:
		if (!ch->get_skill(SKILL_AID))
			return FALSE;
		break;
	case STEALTHY_FEAT:
		if (!ch->get_skill(SKILL_HIDE) && !ch->get_skill(SKILL_SNEAK) && !ch->get_skill(SKILL_CAMOUFLAGE))
			return FALSE;
		break;
	case TRACKER_FEAT:
		if (!ch->get_skill(SKILL_TRACK) && !ch->get_skill(SKILL_SENSE))
			return FALSE;
		break;
	case PUNCH_MASTER_FEAT:
	case CLUBS_MASTER_FEAT:
	case AXES_MASTER_FEAT:
	case LONGS_MASTER_FEAT:
	case SHORTS_MASTER_FEAT:
	case NONSTANDART_MASTER_FEAT:
	case BOTHHANDS_MASTER_FEAT:
	case PICK_MASTER_FEAT:
	case SPADES_MASTER_FEAT:
	case BOWS_MASTER_FEAT:
		if (!HAVE_FEAT(ch, (ubyte) feat_info[feat].affected[1].location))
			return FALSE;
		for (i = PUNCH_MASTER_FEAT; i <= BOWS_MASTER_FEAT; i++)
			if (HAVE_FEAT(ch, i))
				count++;
		if (count >= 1+GET_REMORT(ch)/7)
			return FALSE;
		break;
	case SPIRIT_WARRIOR_FEAT:
		if (!HAVE_FEAT(ch, GREAT_FORTITUDE_FEAT))
			return FALSE;
		break;
	case NIMBLE_FINGERS_FEAT:
		if (!ch->get_skill(SKILL_STEAL) && !ch->get_skill(SKILL_PICK_LOCK))
			return FALSE;
		break;
	case GREAT_POWER_ATTACK_FEAT:
		if (!HAVE_FEAT(ch, POWER_ATTACK_FEAT))
			return FALSE;
		break;
	case PUNCH_FOCUS_FEAT:
	case CLUB_FOCUS_FEAT:
	case AXES_FOCUS_FEAT:
	case LONGS_FOCUS_FEAT:
	case SHORTS_FOCUS_FEAT:
	case NONSTANDART_FOCUS_FEAT:
	case BOTHHANDS_FOCUS_FEAT:
	case PICK_FOCUS_FEAT:
	case SPADES_FOCUS_FEAT:
	case BOWS_FOCUS_FEAT:
		if (!ch->get_skill((ubyte) feat_info[feat].affected[0].location))
			return FALSE;
		for (i = PUNCH_FOCUS_FEAT; i <= BOWS_FOCUS_FEAT; i++)
			if (HAVE_FEAT(ch, i))
				count++;
		if (count >= 2+GET_REMORT(ch)/6)
			return FALSE;
		break;
	case GREAT_AIMING_ATTACK_FEAT:
		if (!HAVE_FEAT(ch, AIMING_ATTACK_FEAT))
			return FALSE;
		break;
	case DOUBLESHOT_FEAT:
		if (!HAVE_FEAT(ch, BOWS_FOCUS_FEAT) || ch->get_skill(SKILL_BOWS) < 40)
			return FALSE;
		break;
	case RUNE_USER_FEAT:
		if (!HAVE_FEAT(ch, RUNE_NEWBIE_FEAT))
			return FALSE;
		break;
	case RUNE_MASTER_FEAT:
		if (!HAVE_FEAT(ch, RUNE_USER_FEAT))
			return FALSE;
		break;
	case RUNE_ULTIMATE_FEAT:
		if (!HAVE_FEAT(ch, RUNE_MASTER_FEAT))
			return FALSE;
		break;
	}
	return TRUE;
}

/*
	���� ��������� ���� ��� �����������.
	���������� ����� �� 0 �� MAX_ACC_FEAT-1 - ����� �����,
	0, ���� ����������� ���������� � -1 ���� ������ �� �������.
	�� ��������� ����������� ���������� ����������� �� ������ ������.
*/
bool find_feat_slot(CHAR_DATA *ch, int feat)
{
	int i, lowfeat, hifeat;
	//���� ����������� ���������� - �� ������ ����� ��������
	if (feat_info[feat].natural_classfeat[(int) GET_CLASS(ch)][(int) GET_KIN(ch)] || PlayerRace::FeatureCheck(GET_KIN(ch),GET_RACE(ch),feat))
		return TRUE;
    //������� � ��� ������ ������������, � ������� ���� ������ ����������, � ������� - ���, � ������� ������ ��� �����?
    lowfeat = 0;
    hifeat = 0;
	for (i = 1; i < MAX_FEATS; i++)
	{
        if (feat_info[i].natural_classfeat[(int) GET_CLASS(ch)][(int) GET_KIN(ch)] || PlayerRace::FeatureCheck(GET_KIN(ch),GET_RACE(ch),i))
            continue;
	    if (HAVE_FEAT(ch,i) && (FEAT_SLOT(ch,i) < FEAT_SLOT(ch,feat)))
            lowfeat++;
        if (HAVE_FEAT(ch,i) && (FEAT_SLOT(ch,i) >= FEAT_SLOT(ch,feat)))
            hifeat++;
	}

//�� ���������� ���������� ������ ����� �������:
//����� ������� ������, ������� ��������������� �������������,
//� �����, ��� �������������� ����� � �� �������� ������ ���� �� ����������,
//� ����� ���������� ����� ������, ������� ���������������� �������������
	if (NUM_LEV_FEAT(ch)-FEAT_SLOT(ch, feat)-hifeat-MAX(0, lowfeat-FEAT_SLOT(ch, feat)) > 0)
        return TRUE;

//oops.. ������ ���
	return FALSE;
}

/* ���������� �������� �������� ������������ �� ���� location ��������� affected */
int feature_mod(int feat, int location)
{
	int i;
	for (i = 0; i < MAX_FEAT_AFFECT; i++)
		if (feat_info[feat].affected[i].location == location)
			return (int) feat_info[feat].affected[i].modifier;
	return 0;
}


void check_berserk(CHAR_DATA * ch)
{
	AFFECT_DATA af;
	struct timed_type timed;
	int prob;

	if (affected_by_spell(ch, SPELL_BERSERK) &&
			(GET_HIT(ch) > GET_REAL_MAX_HIT(ch) / 2))
	{
		affect_from_char(ch, SPELL_BERSERK);
		send_to_char("������������ ����������� �������� ���.\r\n", ch);
	}
//!IS_NPC(ch) &&
	if (can_use_feat(ch, BERSERK_FEAT) && ch->get_fighting() &&
			!timed_by_feat(ch, BERSERK_FEAT) && !AFF_FLAGGED(ch, AFF_BERSERK) &&
			(GET_HIT(ch) < GET_REAL_MAX_HIT(ch) / 4))
	{

//		if (!IS_NPC(ch)) {
//Gorrah: ����� �� � ����� ������ ������ ��� ��, ��� ��� ������ ���� �� ������
		timed.skill = BERSERK_FEAT;
		timed.time = 4;
		timed_feat_to_char(ch, &timed);
//		}

		af.type = SPELL_BERSERK;
		af.duration = pc_duration(ch, 1, 60, 30, 0, 0);
		af.modifier = 0;
		af.location = APPLY_NONE;
		af.battleflag = 0;

		prob = IS_NPC(ch) ? 601 : (751 - GET_LEVEL(ch) * 5);
		if (number(1, 1000) <  prob)
		{
			af.bitvector = AFF_BERSERK;
			act("��� ������ ������������ ������!", FALSE, ch, 0, 0, TO_CHAR);
			act("$n0 ����������� �����$g � ������$u �� ����������!", FALSE, ch, 0, 0, TO_ROOM);
		}
		else
		{
			af.bitvector = 0;
			act("�� ������� ��������, ������� �������� ����������. ��� �����.", FALSE, ch, 0, 0, TO_CHAR);
			act("$n0 ������� �������$g, ������� �������� ����������. �������...", FALSE, ch, 0, 0, TO_ROOM);
		}
		affect_join(ch, &af, TRUE, FALSE, TRUE, FALSE);
	}
}

/* ������ ������� */
ACMD(do_lightwalk)
{
	AFFECT_DATA af;
	struct timed_type timed;

	if (IS_NPC(ch) || !can_use_feat(ch, LIGHT_WALK_FEAT))
	{
		send_to_char("�� �� ������ �����.\r\n", ch);
		return;
	}

	if (on_horse(ch))
	{
		act("������������ ������ � ������ �������� ��� $N3...", FALSE, ch, 0, get_horse(ch), TO_CHAR);
		return;
	}

	if (affected_by_spell(ch, SPELL_LIGHT_WALK))
	{
		send_to_char("�� ��� ���������� ������ �����.\r\n", ch);
		return;
	}
	if (timed_by_feat(ch, LIGHT_WALK_FEAT))
	{
		send_to_char("�� ������� �������� ��� �����.\r\n", ch);
		return;
	}

	affect_from_char(ch, SPELL_LIGHT_WALK);

	timed.skill = LIGHT_WALK_FEAT;
	timed.time = 24;
	timed_feat_to_char(ch, &timed);

	send_to_char("������, �� ����������� ����, �� �������� ������ ������.\r\n", ch);
	af.type = SPELL_LIGHT_WALK;
	af.duration = pc_duration(ch, 2, GET_LEVEL(ch), 5, 2, 8);
	af.modifier = 0;
	af.location = APPLY_NONE;
	af.battleflag = 0;
	if (number(1, 1000) > number(1, GET_REAL_DEX(ch) * 50))
	{
		af.bitvector = 0;
		send_to_char("��� �� ������� ��������...\r\n", ch);
	}
	else
	{
		af.bitvector = AFF_LIGHT_WALK;
		send_to_char("���� ���� ����� ����� �������.\r\n", ch);
	}
	affect_to_char(ch, &af);
}

//�������� � �����������
ACMD(do_fit)
{
	OBJ_DATA *obj;
	CHAR_DATA *vict;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];

	/*��������� ���� ��� ��-�����*/
	if (GET_LEVEL(ch) < LVL_IMMORT)
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	};

	//����� �� ����� ������������ ��� �����������?
	if ((subcmd == SCMD_DO_ADAPT) && !can_use_feat(ch, TO_FIT_ITEM_FEAT))
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	};
	if ((subcmd == SCMD_MAKE_OVER) && !can_use_feat(ch, TO_FIT_CLOTHCES_FEAT))
	{
		send_to_char("�� �� ������ �����.", ch);
		return;
	};

	//���� � ��� �������, ������� ����� ����������?
	argument = one_argument(argument, arg1);

	if (!*arg1)
	{
		send_to_char("��� �� ������ ����������?\r\n", ch);
		return;
	};

	if (!(obj = get_obj_in_list_vis(ch, arg1, ch->carrying)))
	{
		sprintf(buf, "� ��� ��� \'%s\'.\r\n", arg1);
		send_to_char(buf, ch);
		return;
	};

	//�� ���� ������������?
	argument = one_argument(argument, arg2);
	if (!(vict = get_char_vis(ch, arg2, FIND_CHAR_ROOM)))
	{
		send_to_char("��� ���� �� ������ ���������� ��� ����?\r\n ��� ������ �������� � ������!\r\n", ch);
		return;
	};

	//������� ��� ����� ���������
	if (GET_OBJ_OWNER(obj) != 0)
	{
		send_to_char("� ���� ���� ��� ���� ��������.\r\n", ch);
		return;

	};

	//������� ������ �� ����������, �������������� ��� �� ���� ���������
	//� �������� ��� ���� �������� ����� ��������, �� ����� ����� �������� ����
	//����� ���� ��� �� ������� �������� �� ��������
	if ((GET_OBJ_WEAR(obj) <= 1) || OBJ_FLAGGED(obj, ITEM_SETSTUFF))
	{
		send_to_char("���� ������� ���������� ����������.\r\n", ch);
		return;
	}

// �� ����������� �������� ��
//(GET_OBJ_MATER(obj) != MAT_CRYSTALL) ���������
// (GET_OBJ_MATER(obj) != MAT_FARFOR) ��������
//(GET_OBJ_MATER(obj) != MAT_ROCK) �����
// (GET_OBJ_MATER(obj) != MAT_PAPER) ������
//(GET_OBJ_MATER(obj) != MAT_DIAMOND) ������������ �����

	//�������� �� ��������?
	switch (subcmd)
	{
	case SCMD_DO_ADAPT:
		if ((GET_OBJ_MATER(obj) != MAT_NONE) &&
				(GET_OBJ_MATER(obj) != MAT_BULAT) &&
				(GET_OBJ_MATER(obj) != MAT_BRONZE) &&
				(GET_OBJ_MATER(obj) != MAT_IRON) &&
				(GET_OBJ_MATER(obj) != MAT_STEEL) &&
				(GET_OBJ_MATER(obj) != MAT_SWORDSSTEEL) &&
				(GET_OBJ_MATER(obj) != MAT_COLOR) &&
				(GET_OBJ_MATER(obj) != MAT_WOOD) &&
				(GET_OBJ_MATER(obj) != MAT_SUPERWOOD) &&
				(GET_OBJ_MATER(obj) != MAT_GLASS))
		{
			sprintf(buf, "� ��������� %s ������%s �� ������������� ���������.\r\n",
					GET_OBJ_PNAME(obj, 0), GET_OBJ_SUF_6(obj));
			send_to_char(buf, ch);
			return;
		}
		break;
	case SCMD_MAKE_OVER:
		if ((GET_OBJ_MATER(obj) != MAT_BONE) &&
				(GET_OBJ_MATER(obj) != MAT_MATERIA) &&
				(GET_OBJ_MATER(obj) != MAT_SKIN) &&
				(GET_OBJ_MATER(obj) != MAT_ORGANIC))
		{
			sprintf(buf, "� ��������� %s ������%s �� ������������� ���������.\r\n",
					GET_OBJ_PNAME(obj, 0), GET_OBJ_SUF_6(obj));
			send_to_char(buf, ch);
			return;
		}
		break;
	};
	GET_OBJ_OWNER(obj) = GET_UNIQUE(vict);
	sprintf(buf, "�� ����� ������� � ������, ����������� ������ �� ������ ���.\r\n");
	sprintf(buf + strlen(buf), "�� ������ ���� ������� � 10000 ��� �������.\r\n");
	sprintf(buf + strlen(buf), "� �����-������ ��������� %s ����� �� ����� %s.\r\n", GET_OBJ_PNAME(obj, 3), GET_PAD(vict, 1));

	send_to_char(buf, ch);

}

int slot_for_char(CHAR_DATA * ch, int i);
#define SpINFO spell_info[spellnum]
/* ������� ���� � ����� */
ACMD(do_spell_capable)
{
	AFFECT_DATA af;
	struct timed_type timed;

	if (IS_NPC(ch) || !can_use_feat(ch, SPELL_CAPABLE_FEAT))
	{
		send_to_char("�� �� ����� �������������.\r\n", ch);
		return;
	}

	if (timed_by_feat(ch, SPELL_CAPABLE_FEAT)
#ifdef TEST_BUILD
		&& !IS_IMMORTAL(ch)
#endif
	  )
	{
		send_to_char("�� �������� ������������ ��� ��� �����.\r\n", ch);
		return;
	}

	char *s;
	int spellnum;

	if (IS_NPC(ch) && AFF_FLAGGED(ch, AFF_CHARM))
		return;

	if (AFF_FLAGGED(ch, AFF_SIELENCE))
	{
		send_to_char("�� �� ������ ��������� � �����.\r\n", ch);
		return;
	}

	s = strtok(argument, "'*!");
	if (s == NULL)
	{
		send_to_char("��� �� ������ ��������� ?\r\n", ch);
		return;
	}
	s = strtok(NULL, "'*!");
	if (s == NULL)
	{
		send_to_char("�������� ���������� ������ ���� ��������� � ������� : ' ��� * ��� !\r\n", ch);
		return;
	}

	spellnum = find_spell_num(s);
	if (spellnum < 1 || spellnum > MAX_SPELLS)
	{
		send_to_char("� ������ �� ��������� ����� ��������� ?\r\n", ch);
		return;
	}

	if ((!IS_SET(GET_SPELL_TYPE(ch, spellnum), SPELL_TEMP | SPELL_KNOW) ||
			GET_REMORT(ch) < MIN_CAST_REM(SpINFO, ch)) &&
			(GET_LEVEL(ch) < LVL_GRGOD) && (!IS_NPC(ch)))
	{
		if (GET_LEVEL(ch) < MIN_CAST_LEV(SpINFO, ch)
				|| GET_REMORT(ch) < MIN_CAST_REM(SpINFO, ch)
				||  slot_for_char(ch, SpINFO.slot_forc[(int) GET_CLASS(ch)][(int) GET_KIN(ch)]) <= 0)
		{
			send_to_char("���� ��� ��� ��������� ������ ������� !\r\n", ch);
			return;
		}
		else
		{
			send_to_char("���� �� ������� �������, ��� ������, ��� ����������...\r\n", ch);
			return;
		}
	}

	if (!GET_SPELL_MEM(ch, spellnum) && !IS_IMMORTAL(ch))
	{
		send_to_char("�� ���������� �� �������, ��� ������������ ��� ����������...\r\n", ch);
		return;
	}

	follow_type *k;
	CHAR_DATA *follower = NULL;
	for (k = ch->followers; k; k = k->next)
	{
		if (AFF_FLAGGED(k->follower, AFF_CHARM) && k->follower->master == ch &&
			MOB_FLAGGED(k->follower, MOB_CLONE) &&
			!affected_by_spell(k->follower, SPELL_CAPABLE) &&
			IN_ROOM(ch) == IN_ROOM(k->follower))
		{
			follower = k->follower;
			break;
		}
	}
	if(!GET_SPELL_MEM(ch, spellnum) && !IS_IMMORTAL(ch))
	{
		send_to_char("�� ���������� �� �������, ��� ������������ ��� ����������...\r\n", ch);
		return;
	}

	if (!follower)
	{
		send_to_char("������ �� ����� ���������� ���� ��� �����.\r\n", ch);
		return;
	}

	act("�� ��������� ������������ $N3.", FALSE, ch, 0, follower, TO_CHAR);
	act("$n ������$u ������ �����-�� ���� � ���-�� ��������� � ������� $N3.", FALSE, ch, 0, follower, TO_ROOM);

	GET_SPELL_MEM(ch, spellnum)--;
	//if (!GET_SPELL_MEM(ch, spellnum))
	//	REMOVE_BIT(GET_SPELL_TYPE(ch, spellnum), SPELL_TEMP);
	if (!IS_NPC(ch) && !IS_IMMORTAL(ch) && PRF_FLAGGED(ch, PRF_AUTOMEM))
		MemQ_remember(ch, spellnum);

	if (!IS_SET(SpINFO.routines, MAG_DAMAGE) || !SpINFO.violent ||
		IS_SET(SpINFO.routines, MAG_MASSES) || IS_SET(SpINFO.routines, MAG_GROUPS) ||
		IS_SET(SpINFO.routines, MAG_AREAS))
	{
		send_to_char("�� ������ ������, �� �� ����� �����.\r\n", ch);
		return;
	}
	affect_from_char(ch, SPELL_CAPABLE_FEAT);

	timed.skill = SPELL_CAPABLE_FEAT;

	switch (SpINFO.slot_forc[GET_CLASS(ch)][GET_KIN(ch)])
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5://1-5 ����� �� 4 ����
			timed.time = 4;
		break;
		case 6:
		case 7://6-7 ����� �� 6 �����
			timed.time = 6;
		break;
		case 8://8 ���� �� 10 �����
			timed.time = 10;
		break;
		case 9://9 ���� �� 12 �����
			timed.time = 12;
		break;
		default://10 ���� ��� ��
			timed.time = 24;
	}
	timed_feat_to_char(ch, &timed);

	GET_CAST_SUCCESS(follower) = GET_REMORT(ch)*4;
	af.type = SPELL_CAPABLE;
	af.duration = 48;
	if(GET_REMORT(ch)>0) {
		af.modifier = GET_REMORT(ch)*4;//������� ������ ������� ���� +����*4 �����
		af.location = APPLY_CAST_SUCCESS;
	} else {
		af.modifier = 0;
		af.location = APPLY_NONE;
	}
	af.battleflag = 0;
	af.bitvector = 0;
	affect_to_char(follower, &af);
	follower->mob_specials.capable_spell = spellnum;
/*
	af.type = SPELL_CAPABLE;
	af.duration = 24;
	af.modifier = spellnum;
	af.location = APPLY_NONE;
	af.battleflag = 0;
	af.bitvector = 0;
	affect_to_char(follower, &af);
	follower->mob_specials.capable_cast = GET_REMORT(ch)*4;
	follower->mob_specials.capable_spell = spellnum;
*/
}

/*
��������� ���� ������� �������.

ACMD(do_blow)
{



}
*/

ACMD(do_relocate)
{
	struct timed_type timed;

	if (!can_use_feat(ch, RELOCATE_FEAT))
	{
		send_to_char("��� ��� ����������.\r\n", ch);
		return;
	}

	if (timed_by_feat(ch, RELOCATE_FEAT)
#ifdef TEST_BUILD
		&& !IS_IMMORTAL(ch)
#endif
	  )
	{
		send_to_char("�� �������� ������������ ��� ��� �����.\r\n", ch);
		return;
	}

	room_rnum to_room, fnd_room;
	one_argument(argument, arg);
	if (!*arg)
	{
		send_to_char("������������� �� ����?", ch);
		return;
	}

	CHAR_DATA* victim = get_player_vis(ch, arg, FIND_CHAR_WORLD);
	if (!victim)
	{
		send_to_char(NOPERSON, ch);
		return;
	}

	/* ���� ����� ������ ������ ��� ��������������� - ���� */
	if (IS_NPC(victim) || (GET_LEVEL(victim) > GET_LEVEL(ch)) || IS_IMMORTAL(victim))
	{
		send_to_char("������� ����������� �� �������.\r\n", ch);
		return;
	}

	/* ��� ����� ������������ ��� ����������� ������� �� ����������� */
	if (!IS_GOD(ch))
	{
		/* ������ ������������ �� ������ ROOM_NOTELEPORTOUT */
		if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_NOTELEPORTOUT))
		{
			send_to_char("������� ����������� �� �������.\r\n", ch);
			return;
		}
		// ������ ������������ ����� ����, ��� ����� ��� ���������� "��������� ����������".
		if (AFF_FLAGGED(ch, AFF_NOTELEPORT))
		{
			send_to_char("������� ����������� �� �������.\r\n", ch);
			return;
		}
	}

	to_room = IN_ROOM(victim);

	if (to_room == NOWHERE)
	{
		send_to_char("������� ����������� �� �������.\r\n", ch);
		return;
	}
	// � ������, ���� ������ �� ����� ����� � ����� (�� ����� �������)
	// ������ � ���� ��������� �����
	if (!Clan::MayEnter(ch, to_room, HCE_PORTAL))
		fnd_room = Clan::CloseRent(to_room);
	else
		fnd_room = to_room;

	if (fnd_room != to_room && !IS_GOD(ch))
	{
		send_to_char("������� ����������� �� �������.\r\n", ch);
		return;
	}

	if (!IS_GOD(ch) &&
			(SECT(fnd_room) == SECT_SECRET ||
			 ROOM_FLAGGED(fnd_room, ROOM_DEATH) ||
			 ROOM_FLAGGED(fnd_room, ROOM_SLOWDEATH) ||
			 ROOM_FLAGGED(fnd_room, ROOM_TUNNEL) ||
			 ROOM_FLAGGED(fnd_room, ROOM_NORELOCATEIN) ||
			 ROOM_FLAGGED(fnd_room, ROOM_ICEDEATH) || (ROOM_FLAGGED(fnd_room, ROOM_GODROOM) && !IS_IMMORTAL(ch))))
	{
		send_to_char("������� ����������� �� �������.\r\n", ch);
		return;
	}

	timed.skill = RELOCATE_FEAT;
	act("$n �������� �����$q �� ����.", TRUE, ch, 0, 0, TO_ROOM);
	send_to_char("�������� ������� ���������� ����� ������ �������.\r\n", ch);
	char_from_room(ch);
	char_to_room(ch, fnd_room);
	check_horse(ch);
	act("$n �������� ������$u ������-��.", TRUE, ch, 0, 0, TO_ROOM);
	if (!(PRF_FLAGGED(victim, PRF_SUMMONABLE) || same_group(ch, victim) || IS_IMMORTAL(ch)))
	{
		send_to_char(ch, "%s��� �������� ��� �������� ��� ������������ �����������.%s\r\n",
			CCIRED(ch, C_NRM), CCINRM(ch, C_NRM));
		pkPortal(ch);
		timed.time = 18 -GET_REMORT(ch);
		WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
		//�� ����� ���� �� ���� ������ ������� �����
			AFFECT_DATA af;
			af.duration = pc_duration(ch, 3, 0, 0, 0, 0);
			af.bitvector = AFF_NOTELEPORT;
			af.battleflag = AF_PULSEDEC;
			affect_to_char(ch, &af);
	}
	else {
		timed.time = 2;
		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
	timed_feat_to_char(ch, &timed);
	look_at_room(ch, 0);
	entry_memory_mtrigger(ch);
	greet_mtrigger(ch, -1);
	greet_otrigger(ch, -1);
	greet_memory_mtrigger(ch);
}

/**
 * ����������� ���� ������� ������������.
 */
void set_race_feats(CHAR_DATA *ch)
{
	std::vector<int> feat_list = PlayerRace::GetRaceFeatures((int)GET_KIN(ch),(int)GET_RACE(ch));
	for (std::vector<int>::iterator i = feat_list.begin(),
		iend = feat_list.end(); i != iend; ++i)
	{
		if (can_get_feat(ch, *i))
		{
			SET_FEAT(ch, *i);
		}
	}
}
