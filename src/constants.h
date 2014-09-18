/**************************************************************************
*  File: constants.h                                     Part of Bylins   *
*  Usage: header file for mud contstants.                                 *
*                                                                         *
*                                                                         *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
**************************************************************************/

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

extern const char *circlemud_version;
extern const char *dirs[];
extern const char *DirsFrom[];
extern const char *DirsTo[];
extern const char *room_bits[];
extern const char *exit_bits[];
extern const char *sector_types[];
extern const char *genders[];
extern const char *position_types[];
extern const char *resistance_types[];
extern const char *player_bits[];
extern const char *action_bits[];
extern const char *preference_bits[];
extern const char *affected_bits[];
extern const char *connected_types[];
extern const char *where[];
extern const char *item_types[];
extern const char *wear_bits[];
extern const char *extra_bits[];
extern const char *apply_negative[];
extern const char *apply_types[];
extern const char *weapon_affects[];
extern const char *anti_bits[];
extern const char *no_bits[];
extern const char *material_type[];
extern const char *container_bits[];
extern const char *fullness[];
extern const char *spell_wear_off_msg[];
extern const char *npc_class_types[];
extern const char *npc_race_types[];
extern const char *places_of_birth[];
extern const char *weekdays[];
extern const char *month_name[];
extern const char *weekdays_poly[];
extern const char *month_name_poly[];
extern const char *ingradient_bits[];
extern const char *function_bits[];
extern const char *religion_name[][NUM_SEXES];
extern const char *pray_metter[];
extern const char *pray_whom[];
extern const char *room_aff_visib_bits[];
extern const char *room_aff_invis_bits[];
extern const char *equipment_types[];
extern struct str_app_type str_app[];
extern struct dex_skill_type dex_app_skill[];
extern struct dex_app_type dex_app[];
extern struct con_app_type con_app[];
extern struct int_app_type int_app[];
extern struct wis_app_type wis_app[];
extern struct cha_app_type cha_app[];
extern struct size_app_type size_app[];
extern struct class_app_type class_app[];
extern struct weapon_app_type weapon_app[];
extern struct pray_affect_type pray_affect[];
extern int rev_dir[];
extern int movement_loss[];
extern struct weapon_affect_types weapon_affect[];
extern int mana[];
extern int mana_gain_cs[];
extern int mana_cost_cs[][9];
extern const char *material_name[];

//MZ.load
extern struct zone_type * zone_types;
//-MZ.load

/*The number of changing coefficients (the others are unchanged) */
#define	MAX_EXP_COEFFICIENTS_USED 15

/* unless you change this, Puff casts all your dg spells */
#define DG_CASTER_PROXY 113

#define FIRST_ROOM       1
#define STRANGE_ROOM     3

#define FIRE_MOVES       20
#define LOOKING_MOVES    5
#define HEARING_MOVES    2
#define LOOKHIDE_MOVES   3
#define SNEAK_MOVES      1
#define CAMOUFLAGE_MOVES 1
#define PICKLOCK_MOVES   10
#define TRACK_MOVES      3
#define SENSE_MOVES      4
#define HIDETRACK_MOVES  10

#define MOB_ARMOUR_MULT  5
#define MOB_AC_MULT      5
#define MOB_DAMAGE_MULT  3

#define MAX_GROUPED_FOLLOWERS 7

extern int HORSE_VNUM;
extern int HORSE_COST;
extern int START_SWORD;
extern int START_CLUB;
extern int START_KNIFE;
extern int START_BREAD;
extern int START_SCROLL;
extern int CREATE_LIGHT;
extern int START_LIGHT;
extern int START_ARMOR;
extern int START_BOTTLE;
extern int START_BOW;
extern int START_WRUNE;
extern int START_ARUNE;
extern int START_ERUNE;
extern int START_FRUNE;

/* ����� ����-������� 0�� ������. ��� ����� ������� ��� � ���� ������� +
   ������� ������. �.� ����� ����� ������ ���� �� 250�� ���� ������������
   ������� ���� 50 */
#define CHARM_MOB_VNUM 200
#endif

