// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "conf.h"
#include "sysdep.h"
#include "structs.h"

struct exit_data
{
	char *general_description;	/* When look DIR.         */

	char *keyword;		/* for open/close       */
	char *vkeyword;		// ����� ����� � ����������� ������ ��� ����������/����������

	byte exit_info;	/* Exit info             */
	byte lock_complexity; //Polud ��������� �����
	obj_vnum key;		/* Key's number (-1 for no key) */
	room_rnum to_room;	/* Where direction leads (NOWHERE) */
};

struct track_data
{
	int track_info;		/* bitvector */
	int who;			/* real_number for NPC, IDNUM for PC */
	boost::array<int, 6> time_income;	/* time bitvector */
	boost::array<int, 6> time_outgone;
	struct track_data *next;
};

struct weather_control
{
	int rainlevel;
	int snowlevel;
	int icelevel;
	int sky;
	int weather_type;
	int duration;
};

/* ��������� ������ ������������� �������������� ������ */
struct room_property_data
{
	int poison; /*���� ������ ������� ������������ ��� SPELL_POISONED_FOG*/
};

struct room_data
{
	room_data();

	room_vnum number;	/* Rooms number  (vnum)                */
	zone_rnum zone;		/* Room zone (for resetting)          */
	int sector_type;		/* sector type (move/hide)            */
	int sector_state;		/**** External, change by weather     */

	char *name;		/* Rooms name 'You are ...'           */
	int description_num;    // ����� �������� � ���������� ������
	char *temp_description; // ��� ���, ���� ��������� �� ����� �����������
	EXTRA_DESCR_DATA *ex_description;	/* for examine/look       */
	boost::array<EXIT_DATA *, NUM_OF_DIRS> dir_option;	/* Directions */
	FLAG_DATA room_flags;	/* DEATH,DARK ... etc */

	byte light;		/* Number of lightsources in room */
	byte glight;		/* Number of lightness person     */
	byte gdark;		/* Number of darkness  person     */
	struct weather_control weather;		/* Weather state for room */
	SPECIAL(*func);

	struct trig_proto_list *proto_script;	/* list of default triggers  */
	struct script_data *script;	/* script info for the object */
	struct track_data *track;

	OBJ_DATA *contents;	/* List of items in room              */
	CHAR_DATA *people;	/* List of NPC / PC in room           */

	AFFECT_DATA *affected;	/* affected by what spells       */
	FLAG_DATA affected_by; /* ����� ������� � ������� �� room_flags ���������� �� ��������
				� �� ����� ���� �������� �� ���� */

	// ������ �������������� �������
	ubyte fires;		/* Time when fires - ��������    */
	ubyte ices;		/* Time when ices restore */

	int portal_room;
	ubyte portal_time; 	/* ����� ����� �����������*/
	long pkPenterUnique; //����������� ����� �� �����

	int holes;		/* ����� ��� ����� - ��������� */
	int *ing_list;		/* ����������� ����������� */

	// ��������� ������� ��������� �� ����� (�� ������� ���� ��� �����������)
	struct room_property_data	base_property;
	// ������� � ����������  ������� �������������� ��������� ...
	struct room_property_data	add_property;

	int poison;		/* ������� ��������� ���������� � SPELL_POISONED_FOG */
};

#endif // ROOM_HPP_INCLUDED
