/*************************************************************************
*   File: item.creation.hpp                            Part of Bylins    *
*   Item creation from magic ingidients functions header                 *
*                                                                        *
*  $Author$                                                       *
*  $Date$                                          *
*  $Revision$                                                      *
**************************************************************************/

#ifndef __ITEM_CREATION_HPP__
#define __ITEM_CREATION_HPP__

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include "conf.h"

#define MAX_ITEMS 	9

#define MAX_PARTS	3

#define MAX_PROTO 	3
#define COAL_PROTO 	311
#define WOOD_PROTO      313
#define TETIVA_PROTO    314


#define MREDIT_MAIN_MENU	0
#define MREDIT_OBJ_PROTO 	1
#define MREDIT_SKILL		2
#define MREDIT_LOCK	 	3
#define MREDIT_INGR_MENU	4
#define MREDIT_INGR_PROTO	5
#define MREDIT_INGR_WEIGHT	6
#define MREDIT_INGR_POWER	7
#define MREDIT_DEL 	 	8
#define MREDIT_CONFIRM_SAVE     9

#define MAKE_ANY 	0
#define MAKE_POTION	1
#define MAKE_WEAR	2
#define MAKE_METALL	3
#define MAKE_CRAFT	4

// ���������� ����������� ���������� ����� ��� ����������� ���-�� �������.
#define MIN_MAKE_MOVE   10

using std::string;
using std::ifstream;
using std::ofstream;
using std::list;
using std::endl;

void mredit_parse(struct descriptor_data *d, char *arg);
void mredit_disp_menu(struct descriptor_data *d);
void mredit_disp_ingr_menu(struct descriptor_data *d);

ACMD(do_list_make);
ACMD(do_edit_make);
ACMD(do_make_item);

void init_make_items();
// ������ ��������� �� �� ���������� � ���������.
struct create_item_type
{
	int obj_vnum;
	int material_bits;
	int min_weight;
	int max_weight;
	boost::array<int, MAX_PROTO> proto;
	int skill;
	int wear;
};
// ����� ��������� �� �� ���������� ��� �������� ����� �� ������������
struct ingr_part_type
{
	int proto;
	int min_weight;
	int min_power;
};
struct make_skill_type
{
	const char *name;
	const char *short_name;
	int num;
};

class MakeRecept;

class MakeReceptList
{
	list < MakeRecept * >recepts;

public:
	// �������� ���������� �������� ��������.
	MakeReceptList();

	~MakeReceptList();

	// ����� ������ �������� �� ���� ����������� � ���������
	int can_make_list(CHAR_DATA * ch);

	// ��������� ������� .
	int load();

	// ��������� �������.
	int save();

	// ������� ������ �� �������� ��� ��������� �� �����.
	MakeRecept *get_by_name(string & rname);

	MakeReceptList *can_make(CHAR_DATA * ch, MakeReceptList * canlist, int use_skill);

	// ����� ��������� ��������
	int size();

	MakeRecept *operator[](int i);

	// ������������ ������ ��������.
	void clear();

	void add(MakeRecept * recept);

	void add(MakeReceptList * recept);

	void del(MakeRecept * recept);

	void sort();
};

class MakeRecept
{
	int stat_modify(CHAR_DATA * ch, int value, float devider);

	int add_flags(CHAR_DATA * ch, FLAG_DATA * base_flag, FLAG_DATA * add_flag, int delta);

	int add_affects(CHAR_DATA * ch, struct boost::array<obj_affected_type, MAX_OBJ_AFFECT>& base, const boost::array<obj_affected_type, MAX_OBJ_AFFECT>& add, int delta);

	int get_ingr_lev(struct obj_data *ingrobj);

	int get_ingr_pow(struct obj_data *ingrobj);

public:

	bool locked;

	int skill;
	int obj_proto;
	boost::array<ingr_part_type, MAX_PARTS> parts;

	// ����������� ���������� �������� �� �������.
	// ������������ ������� ��������� �����.
	MakeRecept();

	~MakeRecept();

	// ���������� ����� �� � �������� �� ����������� ����������� � ���������
	int can_make(CHAR_DATA *ch);
	// ������� ������� �� �������
	int make(CHAR_DATA *ch);
	// �������� ������ �� ������.
	int load_from_str(string & rstr);
	// ��������� ������ � ������.
	int save_to_str(string & rstr);

	MakeRecept & operator=(MakeRecept & x);


};

#endif
