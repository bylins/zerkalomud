/* ************************************************************************
*   File: im.cpp                                        Part of Bylins    *
*  Usage: Ingradient handling function                                    *
*                                                                         *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

#ifndef _IM_H_
#define _IM_H_

#include <boost/array.hpp>

// ����������� �������� ������� ������������: �����, ����, ������
#define		IM_CLASS_ROSL		0
#define		IM_CLASS_JIV		1
#define		IM_CLASS_TVERD		2

#define 	IM_POWER_SLOT		1
#define		IM_TYPE_SLOT		2
#define     IM_INDEX_SLOT       3

#define		IM_NPARAM			3

struct _im_tlist_tag
{
	long size;		// ���������� �����
	long *types;		// ������� �����
};
typedef struct _im_tlist_tag im_tlist;

struct _im_memb_tag
{
	int power;		// ���� �����������
	int sex;		// ��� ��������� (0-����,1-���,2-���,3-��.�)
	char **aliases;		// ������ ��� �������
	struct _im_memb_tag *next;	// ������ �� ���������
};
typedef struct _im_memb_tag im_memb;

// �������� ������������ ���� �����������
struct _im_type_tag
{
	int id;			// ����� �� im.lst
	char *name;		// �������� ���� �����������
	int proto_vnum;		// vnum �������-���������
	im_tlist tlst;		// ������ ID �����/���������, �������
	// ����������� ������ ���
	im_memb *head;		// ������ ���������� �����
	// �� ������������ ��� ������������ ������ ����
};
typedef struct _im_type_tag im_type;

// �������� �������� �� ���������� �� �������� ������������ ����
// ���� cls � �������� �������� �� ������������
// ���� members � �������� �������� �� ������������

// �������� ��������������� ����������
struct _im_addon_tag
{
	int id;			// ��� �����������, ������ �������
	int k0, k1, k2;		// ������������� �������
	OBJ_DATA *obj;		// ������������� ������
	struct _im_addon_tag *link;	// ������
};
typedef struct _im_addon_tag im_addon;

#define IM_MSG_OK		0
#define IM_MSG_FAIL		1
#define IM_MSG_DAM		2

// +newbook.patch (Alisher)
#define KNOW_RECIPE  1
// -newbook.patch (Alisher)

// �������� �������
struct _im_recipe_tag
{
	int id;			// ����� �� im.lst
	char *name;		// �������� �������
	int k_improove;		// ��������� ��������
	int result;		// VNUM ��������� ����������
	float k[IM_NPARAM], kp;	// ����� ��������
	int *require;		// ������ ������������ �����������
	int nAddon;		// ���������� ���������� �����������
	im_addon *addon;	// ������ ���������� �����������
	boost::array<char *, 3> msg_char;	// ��������� OK,FAIL,DAM
	boost::array<char *, 3> msg_room;	// ��������� OK,FAIL,DAM
	int x, y;		// XdY - �����������
// +newbook.patch (Alisher)
	boost::array<int, NUM_CLASSES> classknow; // ������� �� ����� ������ ��������
	int level; // �� ����� ������ ����� ������� ������
	int remort; // ������� �������� ���������� ��� �������
// -newbook.patch (Alisher)
};
typedef struct _im_recipe_tag im_recipe;

// �������� �������-������
struct _im_rskill_tag
{
	int rid;		// ������ � ������� ������� ��������
	int perc;		// ������� �������� �������
	struct _im_rskill_tag *link;	// ��������� �� ��������� ������ � �������
};
typedef struct _im_rskill_tag im_rskill;

extern im_recipe *imrecipes;

void im_parse(int **ing_list, char *line);
//MZ.load
void im_reset_room(ROOM_DATA * room, int level, int type);
//-MZ.load
void im_make_corpse(OBJ_DATA * corpse, int *ing_list, int max_prob);
int im_assign_power(OBJ_DATA * obj);
int im_get_recipe(int id);
int im_get_type_by_name(char *name, int mode);
OBJ_DATA *load_ingredient(int index, int power, int rnum);
int im_ing_dump(int *ping, char *s);
void im_inglist_copy(int **pdst, int *src);
void im_inglist_save_to_disk(FILE * f, int *ping);
void im_extract_ing(int **pdst, int num);
int im_get_char_rskill_count(CHAR_DATA * ch);
void trg_recipeturn(CHAR_DATA * ch, int rid, int recipediff);
void trg_recipeadd(CHAR_DATA * ch, int rid, int recipediff);
int im_get_recipe_by_name(char *name);
im_rskill *im_get_char_rskill(CHAR_DATA * ch, int rid);
void compose_recipe(CHAR_DATA * ch, char *argument, int subcmd);
void forget_recipe(CHAR_DATA * ch, char *argument, int subcmd);
int im_get_idx_by_type(int type);

#endif
