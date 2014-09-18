/**************************************************************************
*   File: exchange.cpp                                 Part of Bylins     *
*  Usage: Exchange functions used by the MUD                              *
*                                                                         *
*                                                                         *
*  $Author$                                                         *
*  $Date$                                           *
*  $Revision$                                                      *
************************************************************************ */

#include <stdexcept>
#include <sstream>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "screen.h"
#include "exchange.h"
#include "im.h"
#include "constants.h"
#include "skills.h"
#include "char.hpp"
#include "char_player.hpp"
#include "modify.h"
#include "room.hpp"
#include "mail.h"
#include "objsave.h"
#include "pk.h"

//������������ ������� �-��.
extern OBJ_DATA *read_one_object_new(char **data, int *error);
extern int get_buf_line(char **source, char *target);
extern int get_buf_lines(char **source, char *target);
extern void tascii(int *pointer, int num_planes, char *ascii);
extern void asciiflag_conv(const char *flag, void *value);

extern int invalid_anti_class(CHAR_DATA * ch, const OBJ_DATA * obj);
extern int invalid_unique(CHAR_DATA * ch, const OBJ_DATA * obj);
extern int invalid_no_class(CHAR_DATA * ch, const OBJ_DATA * obj);
extern int invalid_align(CHAR_DATA * ch, const OBJ_DATA * obj);
extern char *diag_weapon_to_char(const OBJ_DATA * obj, int show_wear);
extern char *diag_obj_timer(const OBJ_DATA * obj);
extern char *diag_timer_to_char(const OBJ_DATA * obj);
extern void imm_show_obj_values(OBJ_DATA * obj, CHAR_DATA * ch);
extern void mort_show_obj_values(const OBJ_DATA * obj, CHAR_DATA * ch, int fullness);
extern void set_wait(CHAR_DATA * ch, int waittime, int victim_in_room);
extern bool is_dig_stone(OBJ_DATA *obj);

//���� �-��
int exchange_exhibit(CHAR_DATA * ch, char *arg);
int exchange_change_cost(CHAR_DATA * ch, char *arg);
int exchange_withdraw(CHAR_DATA * ch, char *arg);
int exchange_information(CHAR_DATA * ch, char *arg);
int exchange_identify(CHAR_DATA * ch, char *arg);
int exchange_purchase(CHAR_DATA * ch, char *arg);
int exchange_offers(CHAR_DATA * ch, char *arg);
int exchange_setfilter(CHAR_DATA * ch, char *arg);


int exchange_database_load();
int exchange_database_reload(bool loadbackup);
void check_exchange(OBJ_DATA * obj);
void extract_exchange_item(EXCHANGE_ITEM_DATA * item);
int get_unique_lot(void);
void message_exchange(char *message, CHAR_DATA * ch, EXCHANGE_ITEM_DATA * j);
int obj_matches_filter(EXCHANGE_ITEM_DATA * j, char *filter_name, char *filter_owner, int *filter_type,
					   int *filter_cost, int *filter_timer, int *filter_wereon, int *filter_weaponclass);
void show_lots(char *filter, short int show_type, CHAR_DATA * ch);
int parse_exch_filter(char *buf, char *filter_name, char *filter_owner, int *filter_type,
					  int *filter_cost, int *filter_timer, int *filter_wereon, int *filter_weaponclass);
void clear_exchange_lot(EXCHANGE_ITEM_DATA * lot);
extern void obj_info(CHAR_DATA * ch, OBJ_DATA *obj, char buf[MAX_STRING_LENGTH]);

//Polud
ACMD(do_exchange);

int newbase;

EXCHANGE_ITEM_DATA *create_exchange_item(void);

EXCHANGE_ITEM_DATA *exchange_item_list = NULL;
std::vector<bool> lot_usage;

char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

char info_message[] = ("����� ��������� <�������> <����>        - ��������� ������� �� �������\r\n"
					   "����� ���� <#���> <����>                - �������� ���� �� ���� ���\r\n"
					   "����� ����� <#���>                      - ����� � ������� ���� ���\r\n"
					   "����� ���������� <#���>                 - ���������� � ����\r\n"
					   "����� �������������� <#���>             - �������������� ���� (���� ������ 110 ���)\r\n"
					   "����� ������ <#���>                     - ������ ���\r\n"
					   "����� ����������� ��� <�������>         - ��� �����������\r\n"
					   "����� ����������� ��� <�������>         - ��� �����������\r\n"
					   "����� ����������� ���� <�������>        - ����������� ���\r\n"
					   "����� ����������� ����� <�������>       - ����������� ������ � �����\r\n"
					   "����� ����������� ������ <�������>      - ����������� ������ ��������\r\n"
					   "����� ����������� ������� <�������>     - ����������� ������� ��������\r\n"
					   "����� ����������� ������� <�������>     - ����������� ������� ��������\r\n"
					   "����� ����������� ������ <�������>      - ����������� ������\r\n"
					   "����� ����������� ����� <�������>       - ����������� ����\r\n"
					   "����� ����������� ����������� <�������> - ����������� ������������\r\n"
					   "����� ����������� ������ <�������>      - ������ �����������\r\n"
					   "����� ���������� <������>               - ���������� ������ �� ������\r\n");

SPECIAL(exchange)
{
	if (CMD_IS("exchange") || CMD_IS("�����"))
	{
		if (IS_NPC(ch))
			return 0;
		if (AFF_FLAGGED(ch, AFF_SIELENCE))
		{
			send_to_char("�� ����, ��� ���� �� ���.\r\n", ch);
			return 1;
		}
		if (!IS_NPC(ch) && PLR_FLAGGED(ch, PLR_DUMB))
		{
			send_to_char("��� ��������� �������� c ����������!\r\n", ch);
			return 1;
		}
		/*
				if (PLR_FLAGGED(ch, PLR_MUTE)) {
					send_to_char("��� �� � ���� �����������.\r\n", ch);
					return 1;
				}
		*/
		if (GET_LEVEL(ch) < EXCHANGE_MIN_CHAR_LEV && !GET_REMORT(ch))
		{
			sprintf(buf1,
					"��� ����� ������� ���� �� %d ������, ����� ������������ �������.\r\n",
					EXCHANGE_MIN_CHAR_LEV);
			send_to_char(buf1, ch);
			return 1;
		}
		if (RENTABLE(ch))
		{
			send_to_char("��������� ������� ������ ��������.\r\n", ch);
			return 1;
		}

		argument = one_argument(argument, arg1);

		if (is_abbrev(arg1, "���������") || is_abbrev(arg1, "exhibit"))
			exchange_exhibit(ch, argument);
		else if (is_abbrev(arg1, "����") || is_abbrev(arg1, "cost"))
			exchange_change_cost(ch, argument);
		else if (is_abbrev(arg1, "�����") || is_abbrev(arg1, "withdraw"))
			exchange_withdraw(ch, argument);
		else if (is_abbrev(arg1, "����������") || is_abbrev(arg1, "information"))
			exchange_information(ch, argument);
		else if (is_abbrev(arg1, "��������������") || is_abbrev(arg1, "identify"))
			exchange_identify(ch, argument);
		else if (is_abbrev(arg1, "������") || is_abbrev(arg1, "purchase"))
			exchange_purchase(ch, argument);
		else if (is_abbrev(arg1, "�����������") || is_abbrev(arg1, "offers"))
			exchange_offers(ch, argument);
		else if (is_abbrev(arg1, "����������") || is_abbrev(arg1, "filter"))
			exchange_setfilter(ch, argument);
		else if (is_abbrev(arg1, "save") && (GET_LEVEL(ch) >= LVL_IMPL))
			exchange_database_save();
		else if (is_abbrev(arg1, "savebackup") && (GET_LEVEL(ch) >= LVL_IMPL))
			exchange_database_save(true);
		else if (is_abbrev(arg1, "reload") && (GET_LEVEL(ch) >= LVL_IMPL))
			exchange_database_reload(false);
		else if (is_abbrev(arg1, "reloadbackup") && (GET_LEVEL(ch) >= LVL_IMPL))
			exchange_database_reload(true);
		else
			send_to_char(info_message, ch);

		return 1;
	}
	else
		return 0;

}

int exchange_exhibit(CHAR_DATA * ch, char *arg)
{

	char obj_name[MAX_INPUT_LENGTH];
	int item_cost = 0, lot;
	OBJ_DATA *obj;
	char tmpbuf[MAX_INPUT_LENGTH];
	EXCHANGE_ITEM_DATA *item = NULL;
	EXCHANGE_ITEM_DATA *j, *next_thing = NULL;
	int counter;
	int counter_ming; //���������� ������������
	int tax;	//�����

	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(ch) < LVL_IMPL)
	{
		send_to_char("����, �� ������ � ��������� ��������, ��� ��� �� � ����.\r\n", ch);
		return false;
	}


	arg = one_argument(arg, obj_name);
	arg = one_argument(arg, arg2);
	if (!*obj_name)
	{
		send_to_char("������: ����� ��������� ������� ���� �����������\r\n", ch);
		return false;
	}
	if (!sscanf(arg2, "%d", &item_cost))
		item_cost = 0;


	if (!*obj_name)
	{
		send_to_char("�� ������ �������.\r\n", ch);
		return false;
	}
	if (!(obj = get_obj_in_list_vis(ch, obj_name, ch->carrying)))
	{
		send_to_char("� ��� ����� ���.\r\n", ch);
		return false;
	}
	if (!bloody::handle_transfer(ch, NULL, obj))
		return false;
	if (GET_OBJ_TYPE(obj) != ITEM_BOOK)
	{
		if (OBJ_FLAGGED(obj, ITEM_NORENT)
				|| OBJ_FLAGGED(obj, ITEM_NOSELL)
				|| OBJ_FLAGGED(obj, ITEM_ZONEDECAY)
				|| OBJ_FLAGGED(obj, ITEM_REPOP_DECAY)
				|| GET_OBJ_RNUM(obj) < 0)
		{
			send_to_char("���� ������� �� ������������ ��� ������.\r\n", ch);
			return false;
		}
	}
	if (OBJ_FLAGGED(obj, ITEM_DECAY) ||
			OBJ_FLAGGED(obj, ITEM_NODROP) || obj->obj_flags.cost <= 0 || obj->obj_flags.Obj_owner > 0)
	{
		send_to_char("���� ������� �� ������������ ��� ������.\r\n", ch);
		return false;
	}
	if (obj->contains)
	{
		sprintf(tmpbuf, "���������� %s ����� ��������.\r\n", obj->PNames[3]);
		send_to_char(tmpbuf, ch);
		return false;
	}
	else if (SetSystem::is_big_set(obj))
	{
		snprintf(buf, MAX_STRING_LENGTH,
				"%s �������� ������ �������� ������ ���������.\r\n",
				OBJ_PAD(obj, 0));
		send_to_char(CAP(buf), ch);
		return false;
	}


	if (item_cost <= 0)
	{
		item_cost = MAX(1, GET_OBJ_COST(obj));
	}

	(GET_OBJ_TYPE(obj) != ITEM_MING) ?
	tax = EXCHANGE_EXHIBIT_PAY + (int)(item_cost * EXCHANGE_EXHIBIT_PAY_COEFF) :
		  tax = (int)(item_cost * EXCHANGE_EXHIBIT_PAY_COEFF / 2);
	if ((ch->get_total_gold() < tax)
			&& (GET_LEVEL(ch) < LVL_IMPL))
	{
		send_to_char("� ��� �� ������ ����� �� ������ !\r\n", ch);
		return false;
	}
	for (j = exchange_item_list, counter = 0, counter_ming = 0;
			j && (counter + (counter_ming / 20)  <= EXCHANGE_MAX_EXHIBIT_PER_CHAR);
			j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_SELLERID(j) == GET_IDNUM(ch))
			((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_MING) &&
			 (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_INGRADIENT)) ? counter++ : counter_ming++;
	}

	if (counter + (counter_ming / 20)  >= EXCHANGE_MAX_EXHIBIT_PER_CHAR)
	{
		send_to_char("�� ��� ��������� �� ����� ������������ ���������� ��������� !\r\n", ch);
		return false;
	}

	if ((lot = get_unique_lot()) <= 0)
	{
		send_to_char("����� ���������� !\r\n", ch);
		return false;
	}

	item = create_exchange_item();

	GET_EXCHANGE_ITEM_LOT(item) = lot;
	GET_EXCHANGE_ITEM_SELLERID(item) = GET_IDNUM(ch);
	GET_EXCHANGE_ITEM_COST(item) = item_cost;
	skip_spaces(&arg);
	if (*arg)
		GET_EXCHANGE_ITEM_COMMENT(item) = str_dup(arg);
	else
		GET_EXCHANGE_ITEM_COMMENT(item) = NULL;

	GET_EXCHANGE_ITEM(item) = obj;
	obj_from_char(obj);

	sprintf(tmpbuf, "�� ��������� �� ����� $O3 (��� %d) �� %d %s.\r\n",
			GET_EXCHANGE_ITEM_LOT(item), item_cost, desc_count(item_cost, WHAT_MONEYu));
	act(tmpbuf, FALSE, ch, 0, obj, TO_CHAR);
	sprintf(tmpbuf,
			"����� : ����� ��� (%d) - %s - ���� %d %s. \r\n",
			GET_EXCHANGE_ITEM_LOT(item), obj->PNames[0], item_cost, desc_count(item_cost, WHAT_MONEYa));
	message_exchange(tmpbuf, ch, item);

	ch->remove_both_gold(tax);

	if (EXCHANGE_SAVEONEVERYOPERATION)
	{
		exchange_database_save();
		Crash_crashsave(ch);
		ch->save_char();
	}
	return (true);
}


int exchange_change_cost(CHAR_DATA * ch, char *arg)
{
	EXCHANGE_ITEM_DATA *item = NULL, *j, *next_thing = NULL;
	int lot, newcost, pay;
	char tmpbuf[MAX_INPUT_LENGTH];

	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(ch) < LVL_IMPL)
	{
		send_to_char("����, �� ������ � ��������� ��������, ��� ��� �� � ����.\r\n", ch);
		return false;
	}
	if (sscanf(arg, "%d %d", &lot, &newcost) != 2)
	{
		send_to_char("������ �������: ����� ���� <���> <����� ����>.\r\n", ch);
		return false;
	}
	for (j = exchange_item_list; j && (!item); j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_LOT(j) == lot)
			item = j;
	}
	if ((lot < 0) || (!item))
	{
		send_to_char("�������� ����� ����.\r\n", ch);
		return false;
	}
	if ((GET_EXCHANGE_ITEM_SELLERID(item) != GET_IDNUM(ch)) && (GET_LEVEL(ch) < LVL_IMPL))
	{
		send_to_char("��� �� ��� ���.\r\n", ch);
		return false;
	}
	if (newcost == GET_EXCHANGE_ITEM_COST(item))
	{
		send_to_char("���� ����� ���� ��������� � �������.\r\n", ch);
		return false;
	}
	if (newcost <= 0)
	{
		send_to_char("�� ������� ������������ ����.\r\n", ch);
		return false;
	}
	pay = newcost - GET_EXCHANGE_ITEM_COST(item);
	if (pay > 0)
		if ((ch->get_total_gold() < (pay * EXCHANGE_EXHIBIT_PAY_COEFF)) && (GET_LEVEL(ch) < LVL_IMPL))
		{
			send_to_char("� ��� �� ������ ����� �� ������ !\r\n", ch);
			return false;
		}


	GET_EXCHANGE_ITEM_COST(item) = newcost;
	if (pay > 0)
	{
		ch->remove_both_gold(static_cast<long>(pay * EXCHANGE_EXHIBIT_PAY_COEFF));
	}

	sprintf(tmpbuf, "�� ��������� ���� %d %s, �� %s (��� %d).\r\n",
			newcost, desc_count(newcost, WHAT_MONEYu), GET_EXCHANGE_ITEM(item)->PNames[3], GET_EXCHANGE_ITEM_LOT(item));
	send_to_char(tmpbuf, ch);
	sprintf(tmpbuf,
			"����� : ��� (%d) - %s - ��������� �� ����� ���� %d %s.\r\n",
			GET_EXCHANGE_ITEM_LOT(item), GET_EXCHANGE_ITEM(item)->PNames[0], newcost, desc_count(newcost, WHAT_MONEYa));
	message_exchange(tmpbuf, ch, item);
	set_wait(ch, 2, FALSE);
//	send_to_char("�������.\r\n", ch);
	return true;
}

int exchange_withdraw(CHAR_DATA * ch, char *arg)
{
	EXCHANGE_ITEM_DATA *item = NULL, *j, *next_thing = NULL;
	int lot;
	char tmpbuf[MAX_INPUT_LENGTH];


	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(ch) < LVL_IMPL)
	{
		send_to_char("����, �� ������ � ��������� ��������, ��� ��� �� � ����.\r\n", ch);
		return false;
	}

	if (!sscanf(arg, "%d", &lot))
	{
		send_to_char("�� ������ ����� ����.\r\n", ch);
		return false;
	}
	for (j = exchange_item_list; j && (!item); j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_LOT(j) == lot)
			item = j;
	}
	if ((lot < 0) || (!item))
	{
		send_to_char("�������� ����� ����.\r\n", ch);
		return false;
	}
	if ((GET_EXCHANGE_ITEM_SELLERID(item) != GET_IDNUM(ch)) && (GET_LEVEL(ch) < LVL_IMPL))
	{
		send_to_char("��� �� ��� ���.\r\n", ch);
		return false;
	}
	act("�� ����� $O3 � ������.\r\n", FALSE, ch, 0, GET_EXCHANGE_ITEM(item), TO_CHAR);
	if (GET_EXCHANGE_ITEM_SELLERID(item) != GET_IDNUM(ch))
		sprintf(tmpbuf,
				"����� : ��� %d(%s) ����%s � ������ ������.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)));
	else
		sprintf(tmpbuf,
				"����� : ��� %d(%s) ����%s � ������ ����������.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)));
	message_exchange(tmpbuf, ch, item);
	obj_to_char(GET_EXCHANGE_ITEM(item), ch);
	clear_exchange_lot(item);


	if (EXCHANGE_SAVEONEVERYOPERATION)
	{
		exchange_database_save();
		Crash_crashsave(ch);
		ch->save_char();
	}

	return true;
}


int exchange_information(CHAR_DATA * ch, char *arg)
{
	EXCHANGE_ITEM_DATA *item = NULL, *j, *next_thing = NULL;
	int lot;
	char buf[MAX_STRING_LENGTH], buf2[MAX_INPUT_LENGTH];


	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (!sscanf(arg, "%d", &lot))
	{
		send_to_char("�� ������ ����� ����.\r\n", ch);
		return false;
	}
	for (j = exchange_item_list; j && (!item); j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_LOT(j) == lot)
			item = j;
	}
	if ((lot < 0) || (!item))
	{
		send_to_char("�������� ����� ����.\r\n", ch);
		return false;
	}

	sprintf(buf, "��� %d. ���� %d\r\n", GET_EXCHANGE_ITEM_LOT(item), GET_EXCHANGE_ITEM_COST(item));

	sprintf(buf + strlen(buf), "������� \"%s\", ", GET_EXCHANGE_ITEM(item)->short_description);
	if ((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) == ITEM_WAND)
			|| (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) == ITEM_STAFF))
	{
		if (GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 2) < GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 1))
			strcat(buf, "(�/�), ");
	}
	strcat(buf, " ��� ");
	sprinttype(GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)), item_types, buf2);
	if (*buf2)
	{
		strcat(buf, buf2);
		strcat(buf, "\n");
	};
	strcat(buf, diag_timer_to_char(GET_EXCHANGE_ITEM(item)));
	strcpy(buf2, diag_weapon_to_char(GET_EXCHANGE_ITEM(item), TRUE));
	obj_info(ch, GET_EXCHANGE_ITEM(item), buf);
	if (*buf2)
	{
		strcat(buf, buf2);
		strcat(buf, "\n");
	};
	if (invalid_anti_class(ch, GET_EXCHANGE_ITEM(item)) || invalid_unique(ch, GET_EXCHANGE_ITEM(item)))
	{
		sprintf(buf2, "��� ���� ��� ����������!.");
		strcat(buf, buf2);
		strcat(buf, "\n");
	}
	if (invalid_align(ch, GET_EXCHANGE_ITEM(item)) || invalid_no_class(ch, GET_EXCHANGE_ITEM(item)))
	{
		sprintf(buf2, "�� �� ������� ������������ ���� �����.");
		strcat(buf, buf2);
		strcat(buf, "\n");
	}
	sprintf(buf2, "%s",
			get_name_by_id(GET_EXCHANGE_ITEM_SELLERID(item)) ?
			get_name_by_id(GET_EXCHANGE_ITEM_SELLERID(item)) : "(null)");
	*buf2 = UPPER(*buf2);
	strcat(buf, "�������� ");
	strcat(buf, buf2);
	strcat(buf, "\n");

	if (GET_EXCHANGE_ITEM_COMMENT(item))
	{
		strcat(buf, "���������� �������� �� ���� ������: ");
		sprintf(buf2, "'%s'.", GET_EXCHANGE_ITEM_COMMENT(item));
		strcat(buf, buf2);
		strcat(buf, "\n");
	}
	send_to_char(buf, ch);
	return true;
}

int exchange_identify(CHAR_DATA * ch, char *arg)
{
	EXCHANGE_ITEM_DATA *item = NULL, *j, *next_thing = NULL;
	int lot;
	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (!sscanf(arg, "%d", &lot))
	{
		send_to_char("�� ������ ����� ����.\r\n", ch);
		return false;
	}
	for (j = exchange_item_list; j && (!item); j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_LOT(j) == lot)
			item = j;
	}
	if ((lot < 0) || (!item))
	{
		send_to_char("�������� ����� ����.\r\n", ch);
		return false;
	}

	if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(ch) < LVL_IMPL)
	{
		send_to_char("�������, � ���� �������� �� ��� ������ ������.\r\n", ch);
		return false;
	}
	if ((ch->get_total_gold() < (EXCHANGE_IDENT_PAY)) && (GET_LEVEL(ch) < LVL_IMPL))
	{
		send_to_char("� ��� �� ������ �� ��� �����!\r\n", ch);
		return false;
	}
	if (GET_LEVEL(ch) < LVL_IMPL)
		mort_show_obj_values(GET_EXCHANGE_ITEM(item), ch, 200);	//200 - ������ ���������
	else
		imm_show_obj_values(GET_EXCHANGE_ITEM(item), ch);

	ch->remove_both_gold(EXCHANGE_IDENT_PAY);
	send_to_char(ch, "\r\n%s�� ���������� � �������� � ������ ����������� ����� ����� %d %s%s\r\n",
		CCIGRN(ch, C_NRM), EXCHANGE_IDENT_PAY, desc_count(EXCHANGE_IDENT_PAY, WHAT_MONEYu), CCNRM(ch, C_NRM));

	return true;
}

CHAR_DATA *get_char_by_id(int id)
{
	for (CHAR_DATA *i = character_list; i; i = i->next)
		if (!IS_NPC(i) && GET_IDNUM(i) == id)
			return (i);
	return 0;
}

int exchange_purchase(CHAR_DATA * ch, char *arg)
{
	EXCHANGE_ITEM_DATA *item = NULL, *j, *next_thing = NULL;
	int lot;
	char tmpbuf[MAX_INPUT_LENGTH];
	CHAR_DATA *seller;

	if (!*arg)
	{
		send_to_char(info_message, ch);
		return false;
	}
	if (GET_LEVEL(ch) >= LVL_IMMORT && GET_LEVEL(ch) < LVL_IMPL)
	{
		send_to_char("����, �� ������ � ��������� ��������, ��� ��� �� � ����.\r\n", ch);
		return false;
	}

	if (!sscanf(arg, "%d", &lot))
	{
		send_to_char("�� ������ ����� ����.\r\n", ch);
		return false;
	}
	for (j = exchange_item_list; j && (!item); j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM_LOT(j) == lot)
			item = j;
	}
	if ((lot < 0) || (!item))
	{
		send_to_char("�������� ����� ����.\r\n", ch);
		return false;
	}
	if (GET_EXCHANGE_ITEM_SELLERID(item) == GET_IDNUM(ch))
	{
		send_to_char("��� �� ��� ���. �������������� �������� '����� ����� <���>'\r\n", ch);
		return false;
	}
	if ((ch->get_total_gold() < (GET_EXCHANGE_ITEM_COST(item))) && (GET_LEVEL(ch) < LVL_IMPL))
	{
		send_to_char("� ��� � ����� �� ������� ����� �� ���� ���!\r\n", ch);
		return false;
	}



	seller = get_char_by_id(GET_EXCHANGE_ITEM_SELLERID(item));

	if (seller == NULL)
	{
		const char *seller_name = get_name_by_id(GET_EXCHANGE_ITEM_SELLERID(item));
		seller = new Player; // TODO: ���������� �� ����
		if ((seller_name == NULL) || (load_char(seller_name, seller) < 0))
		{
			ch->remove_both_gold(GET_EXCHANGE_ITEM_COST(item));
			//edited by WorM 2011.05.21
			act("�� ������ $O3 �� ������.\r\n", FALSE, ch, 0, GET_EXCHANGE_ITEM(item), TO_CHAR);
			sprintf(tmpbuf,
					"����� : ��� %d(%s) ������%s �� %d %s.\r\n", lot,
					GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)),
					GET_EXCHANGE_ITEM_COST(item), desc_count(GET_EXCHANGE_ITEM_COST(item), WHAT_MONEYu));
			/*act("�� ��������� $O3 �� ������ �����, ��� ��� ��������� ����� ���� �������.\r\n",
				FALSE, ch, 0, GET_EXCHANGE_ITEM(item), TO_CHAR);
			sprintf(tmpbuf,
					"����� : ��� %d(%s) �������%s � �������� ����.\r\n", lot,
					GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)));*/
			//end by WorM
			message_exchange(tmpbuf, ch, item);
			obj_to_char(GET_EXCHANGE_ITEM(item), ch);
			clear_exchange_lot(item);
			if (EXCHANGE_SAVEONEVERYOPERATION)
			{
				exchange_database_save();
				Crash_crashsave(ch);
				ch->save_char();
			}

			delete seller;
			return true;
		}

		seller->add_bank(GET_EXCHANGE_ITEM_COST(item));
		ch->remove_both_gold(GET_EXCHANGE_ITEM_COST(item));
//Polud ������� �������� ����, ��� �� ��� ���� ������
		if (NOTIFY_EXCH_PRICE(seller) && GET_EXCHANGE_ITEM_COST(item) >= NOTIFY_EXCH_PRICE(seller))
		{
			sprintf(tmpbuf,
				"����� : ��� %d(%s) ������%s. %d %s ���������� �� ��� ����.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)),
				GET_EXCHANGE_ITEM_COST(item), desc_count(GET_EXCHANGE_ITEM_COST(item), WHAT_MONEYa));
			store_mail(GET_EXCHANGE_ITEM_SELLERID(item), -1, tmpbuf);
		}
//-Polud
		seller->save_char();
		delete seller;
		act("�� ������ $O3 �� ������.\r\n", FALSE, ch, 0, GET_EXCHANGE_ITEM(item), TO_CHAR);
		sprintf(tmpbuf,
				"����� : ��� %d(%s) ������%s �� %d %s.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)),
				GET_EXCHANGE_ITEM_COST(item), desc_count(GET_EXCHANGE_ITEM_COST(item), WHAT_MONEYu));
		message_exchange(tmpbuf, ch, item);
		obj_to_char(GET_EXCHANGE_ITEM(item), ch);
		clear_exchange_lot(item);
		if (EXCHANGE_SAVEONEVERYOPERATION)
		{
			exchange_database_save();
			Crash_crashsave(ch);
			ch->save_char();
		}

		return true;
	}
	else
	{
		seller->add_bank(GET_EXCHANGE_ITEM_COST(item));
		ch->remove_both_gold(GET_EXCHANGE_ITEM_COST(item));

		act("�� ������ $O3 �� ������.\r\n", FALSE, ch, 0, GET_EXCHANGE_ITEM(item), TO_CHAR);
		sprintf(tmpbuf,
				"����� : ��� %d(%s) ������%s �� %d %s.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)),
				GET_EXCHANGE_ITEM_COST(item), desc_count(GET_EXCHANGE_ITEM_COST(item), WHAT_MONEYu));
		message_exchange(tmpbuf, seller, item);
		sprintf(tmpbuf,
				"����� : ��� %d(%s) ������%s. %d %s ���������� �� ��� ����.\r\n", lot,
				GET_EXCHANGE_ITEM(item)->PNames[0], GET_OBJ_SUF_6(GET_EXCHANGE_ITEM(item)),
				GET_EXCHANGE_ITEM_COST(item), desc_count(GET_EXCHANGE_ITEM_COST(item), WHAT_MONEYa));
		act(tmpbuf, FALSE, seller, 0, NULL, TO_CHAR);

		obj_to_char(GET_EXCHANGE_ITEM(item), ch);
		clear_exchange_lot(item);
		if (EXCHANGE_SAVEONEVERYOPERATION)
		{
			exchange_database_save();
			Crash_crashsave(ch);
			ch->save_char();
			Crash_crashsave(seller);
			seller->save_char();
		}

		return true;
	}

}

/**
* �������� ����� �������.
* \return false - �������
*         true - ����������
*/
bool correct_filter_length(CHAR_DATA *ch, const char *str)
{
	if (strlen(str) >= FILTER_LENGTH)
	{
		send_to_char(ch, "������� ������� ������, ������������ �����: %d �������.\r\n", FILTER_LENGTH - 1);
		return false;
	}
	return true;
}

int exchange_offers(CHAR_DATA * ch, char *arg)
{
//���� ����� �������. ���� ��� ����� ��������������, ���������� �����.
	char filter[MAX_STRING_LENGTH];
	char multifilter[MAX_STRING_LENGTH];
	short int show_type;
	bool ignore_filter;
	char arg3[MAX_INPUT_LENGTH], arg4[MAX_INPUT_LENGTH];

	/*
	show_type
	0 - ���
	1 - ���
	2 - ����
	3 - ������
	4 - ������
	5 - �����
	6 - �����
	7 - ������
	*/

	memset(filter, 0, FILTER_LENGTH);
	memset(multifilter, 0, FILTER_LENGTH);

	(arg = one_argument(arg, arg1));
	(arg = one_argument(arg, arg2));
	(arg = one_argument(arg, arg3));
	(arg = one_argument(arg, arg4));

	ignore_filter = ((*arg2 == '*') || (*arg3 == '*') || (*arg4 == '*'));

	if (*arg2 == '!')
	{
		strcpy(multifilter, arg2 + 1);
	}
	if (*arg3 == '!')
	{
		strcpy(multifilter, arg3 + 1);
	}
	if (*arg4 == '!')
	{
		strcpy(multifilter, arg4 + 1);
	}

	if (is_abbrev(arg1, "���") || is_abbrev(arg1, "all"))
	{
		show_type = 0;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "�%s", arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "���") || is_abbrev(arg1, "mine"))
	{
		show_type = 1;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "����") || is_abbrev(arg1, "runes"))
	{
		show_type = 2;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "�����") || is_abbrev(arg1, "armor"))
	{
		show_type = 3;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "������") || is_abbrev(arg1, "weapons"))
	{
		show_type = 4;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "�����") || is_abbrev(arg1, "books"))
	{
		show_type = 5;
		if ((*arg2) && (*arg2 != '*'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
	}
	else if (is_abbrev(arg1, "�����������") || is_abbrev(arg1, "ingradients"))
	{
		show_type = 6;
		if ((*arg2) && (*arg2 != '*'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
	}
	else if (is_abbrev(arg1, "������") || is_abbrev(arg1, "other"))
	{
		show_type = 7;
		if ((*arg2) && (*arg2 != '*'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
	}
	else if (is_abbrev(arg1, "������") || is_abbrev(arg1, "������"))
	{
		show_type = 8;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "�������") || is_abbrev(arg1, "�������"))
	{
		show_type = 9;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}
	else if (is_abbrev(arg1, "�������") || is_abbrev(arg1, "�������"))
	{
		show_type = 10;
		if ((*arg2) && (*arg2 != '*') && (*arg2 != '!'))
		{
			sprintf(filter, "%s �%s", filter, arg2);
		}
		if (*multifilter)
		{
			strcat(filter, " �");
			strcat(filter, multifilter);
		}
	}

	else
	{
		send_to_char(info_message, ch);
		return 0;
	}

	if (!ignore_filter && EXCHANGE_FILTER(ch))
		sprintf(multifilter, "%s %s", EXCHANGE_FILTER(ch), filter);
	else
		strcpy(multifilter, filter);

	if (!correct_filter_length(ch, multifilter))
	{
		return 0;
	}

	show_lots(multifilter, show_type, ch);
	return 1;
}

int exchange_setfilter(CHAR_DATA * ch, char *arg)
{
	char tmpbuf[MAX_INPUT_LENGTH];
	char filter_name[FILTER_LENGTH];
	char filter_owner[FILTER_LENGTH];
	int filter_type = 0;
	int filter_cost = 0;
	int filter_timer = 0;
	int filter_wereon = 0;
	int filter_weaponclass = 0;
	char filter[MAX_INPUT_LENGTH];

	if (!*arg)
	{
		if (!EXCHANGE_FILTER(ch))
		{
			send_to_char("��� ������ ������ ����\r\n", ch);
			return true;
		}
		if (!parse_exch_filter(EXCHANGE_FILTER(ch), filter_name, filter_owner,
							   &filter_type, &filter_cost, &filter_timer, &filter_wereon, &filter_weaponclass))
		{
			free(EXCHANGE_FILTER(ch));
			EXCHANGE_FILTER(ch) = NULL;
			send_to_char("��� ������ ������ ����\r\n", ch);
			return true;
		}
		else
		{
			sprintf(tmpbuf, "��� ������� ������ ������: %s.\r\n", EXCHANGE_FILTER(ch));
			send_to_char(tmpbuf, ch);
			return true;
		}
	}


	skip_spaces(&arg);
	strcpy(filter, arg);
	if (!correct_filter_length(ch, filter))
	{
		return false;
	}
	if (!strncmp(filter, "���", 3))
	{
		if (EXCHANGE_FILTER(ch))
		{
			sprintf(tmpbuf, "��� ������ ������: %s. ����� ������ ����.\r\n", EXCHANGE_FILTER(ch));
			free(EXCHANGE_FILTER(ch));
			EXCHANGE_FILTER(ch) = NULL;
		}
		else
			sprintf(tmpbuf, "����� ������ ����.\r\n");
		send_to_char(tmpbuf, ch);
		return true;
	}
	if (!parse_exch_filter(filter, filter_name, filter_owner,
						   &filter_type, &filter_cost, &filter_timer, &filter_wereon, &filter_weaponclass))
	{
		send_to_char("�������� ������ �������. �������� �������.\r\n", ch);
		free(EXCHANGE_FILTER(ch));
		EXCHANGE_FILTER(ch) = NULL;
		return false;
	}
	if (EXCHANGE_FILTER(ch))
		sprintf(tmpbuf, "��� ������ ������: %s. ����� ������: %s.\r\n", EXCHANGE_FILTER(ch), filter);
	else
		sprintf(tmpbuf, "��� ����� ������: %s.\r\n", filter);

	send_to_char(tmpbuf, ch);

	if (EXCHANGE_FILTER(ch))
		free(EXCHANGE_FILTER(ch));
	EXCHANGE_FILTER(ch) = str_dup(filter);

	return true;


}

EXCHANGE_ITEM_DATA *create_exchange_item(void)
{
	EXCHANGE_ITEM_DATA *item;
	CREATE(item, EXCHANGE_ITEM_DATA, 1);
	GET_EXCHANGE_ITEM_LOT(item) = -1;
	GET_EXCHANGE_ITEM_SELLERID(item) = -1;
	GET_EXCHANGE_ITEM_COST(item) = 0;
	GET_EXCHANGE_ITEM_COMMENT(item) = NULL;
	GET_EXCHANGE_ITEM(item) = NULL;
	item->next = exchange_item_list;
	exchange_item_list = item;
	return (item);
}

void extract_exchange_item(EXCHANGE_ITEM_DATA * item)
{

	EXCHANGE_ITEM_DATA *temp;
	REMOVE_FROM_LIST(item, exchange_item_list, next);
	if (GET_EXCHANGE_ITEM_LOT(item) > 0 && GET_EXCHANGE_ITEM_LOT(item) <= (int) lot_usage.size())
		lot_usage[GET_EXCHANGE_ITEM_LOT(item) - 1] = false;
	if (item->comment)
		free(item->comment);
	if (item->obj)
		extract_obj(item->obj);
	free(item);

}

void check_exchange(OBJ_DATA * obj)
{
	if (!obj)
		return;
	EXCHANGE_ITEM_DATA *j, *next_thing, *temp;

	for (j = exchange_item_list; j; j = next_thing)
	{
		next_thing = j->next;
		if (GET_EXCHANGE_ITEM(j) == obj)
		{
			REMOVE_FROM_LIST(j, exchange_item_list, next);
			lot_usage[GET_EXCHANGE_ITEM_LOT(j) - 1] = false;
			if (j->comment)
				free(j->comment);
			free(j);
			break;
		}
	}
}

EXCHANGE_ITEM_DATA *exchange_read_one_object_new(char **data, int *error)
{
	char buffer[MAX_STRING_LENGTH];
	EXCHANGE_ITEM_DATA *item = NULL;
	char *d;

	*error = 1;
	// ������ �� ������ ��������
	for (; **data != EX_NEW_ITEM_CHAR; (*data)++)
		if (!**data || **data == EX_END_CHAR)
			return (item);

	*error = 2;

	// ��������� #
	(*data)++;

	// ������� lot ��������
	if (!get_buf_line(data, buffer))
		return (item);

	*error = 3;
	item = create_exchange_item();
	if ((GET_EXCHANGE_ITEM_LOT(item) = atoi(buffer)) <= 0)
		return (item);

	*error = 4;
	// ������� seler_id ��������
	if (!get_buf_line(data, buffer))
		return (item);
	*error = 5;
	if (!(GET_EXCHANGE_ITEM_SELLERID(item) = atoi(buffer)))
		return (item);

	*error = 6;
	// ������� ���� ��������
	if (!get_buf_line(data, buffer))
		return (item);
	*error = 7;
	if (!(GET_EXCHANGE_ITEM_COST(item) = atoi(buffer)))
		return (item);

	*error = 8;
	// ������� comment ��������
	char *str_last_symb = strchr(*data, '\n');
	strncpy(buffer, *data, str_last_symb - *data);
	buffer[str_last_symb - *data] = '\0';
	*data = str_last_symb;

	if (strcmp(buffer, "EMPTY"))
		if (!(GET_EXCHANGE_ITEM_COMMENT(item) = str_dup(buffer)))
			return (item);

	*error = 0;
	d = *data;
	GET_EXCHANGE_ITEM(item) = read_one_object_new(data, error);
	if (*error)
		*error = 9;

	return (item);
}

EXCHANGE_ITEM_DATA *exchange_read_one_object(char **data, int *error)
{
	char buffer[MAX_STRING_LENGTH], f0[MAX_STRING_LENGTH], f1[MAX_STRING_LENGTH], f2[MAX_STRING_LENGTH];
	int vnum, i, j, t[5];
	EXTRA_DESCR_DATA *new_descr;
	EXCHANGE_ITEM_DATA *item = NULL;
	*error = 1;
	// ������ �� ������ ��������
	for (; **data != EX_NEW_ITEM_CHAR; (*data)++)
		if (!**data || **data == EX_END_CHAR)
			return (item);

	*error = 2;

	// ��������� #
	(*data)++;

	// ������� lot ��������
	if (!get_buf_line(data, buffer))
		return (item);

	*error = 3;
	item = create_exchange_item();
	if ((GET_EXCHANGE_ITEM_LOT(item) = atoi(buffer)) <= 0)
		return (item);

	*error = 4;
	// ������� seler_id ��������
	if (!get_buf_line(data, buffer))
		return (item);
	*error = 5;
	if (!(GET_EXCHANGE_ITEM_SELLERID(item) = atoi(buffer)))
		return (item);

	*error = 6;
	// ������� ���� ��������
	if (!get_buf_line(data, buffer))
		return (item);
	*error = 7;
	if (!(GET_EXCHANGE_ITEM_COST(item) = atoi(buffer)))
		return (item);

	*error = 8;
	// ������� comment ��������
	char *str_last_symb = strchr(*data, '\n');
	strncpy(buffer, *data, str_last_symb - *data);
	buffer[str_last_symb - *data] = '\0';
	*data = str_last_symb;

	if (strcmp(buffer, "EMPTY"))
		if (!(GET_EXCHANGE_ITEM_COMMENT(item) = str_dup(buffer)))
			return (item);
	*error = 12;
	// ������� vnum ��������
	if (!get_buf_line(data, buffer))
		return (item);
	*error = 13;
	if (!(vnum = atoi(buffer)))
		return (item);

	if (vnum < 0)  		// ������� �� ����� ���������
	{
		GET_EXCHANGE_ITEM(item) = create_obj();
		*error = 14;
		if (!get_buf_lines(data, buffer))
			return (item);
		// ������
		GET_OBJ_ALIAS(GET_EXCHANGE_ITEM(item)) = str_dup(buffer);
		GET_EXCHANGE_ITEM(item)->name = str_dup(buffer);
		// ������
		*error = 15;
		for (i = 0; i < NUM_PADS; i++)
		{
			if (!get_buf_lines(data, buffer))
				return (item);
			GET_OBJ_PNAME(GET_EXCHANGE_ITEM(item), i) = str_dup(buffer);
		}
		// �������� ����� �� �����
		*error = 16;
		if (!get_buf_lines(data, buffer))
			return (item);
		GET_OBJ_DESC(GET_EXCHANGE_ITEM(item)) = str_dup(buffer);
		// �������� ��� ��������
		*error = 17;
		if (!get_buf_lines(data, buffer))
			return (item);
		GET_OBJ_ACT(GET_EXCHANGE_ITEM(item)) = str_dup(buffer);
	}
	else if (!(GET_EXCHANGE_ITEM(item) = read_object(vnum, VIRTUAL)))
	{
		*error = 18;
		return (item);
	}
	*error = 19;
	if (!get_buf_line(data, buffer) || sscanf(buffer, " %s %d %d %d", f0, t + 1, t + 2, t + 3) != 4)
		return (item);
	GET_OBJ_SKILL(GET_EXCHANGE_ITEM(item)) = 0;
	asciiflag_conv(f0, &GET_OBJ_SKILL(GET_EXCHANGE_ITEM(item)));
	GET_OBJ_MAX(GET_EXCHANGE_ITEM(item)) = t[1];
	GET_OBJ_CUR(GET_EXCHANGE_ITEM(item)) = t[2];
	GET_OBJ_MATER(GET_EXCHANGE_ITEM(item)) = t[3];

	*error = 20;
	if (!get_buf_line(data, buffer) || sscanf(buffer, " %d %d %d %d", t, t + 1, t + 2, t + 3) != 4)
		return (item);
	GET_OBJ_SEX(GET_EXCHANGE_ITEM(item)) = t[0];
	GET_EXCHANGE_ITEM(item)->set_timer(t[1]);
	GET_OBJ_SPELL(GET_EXCHANGE_ITEM(item)) = t[2];
	GET_OBJ_LEVEL(GET_EXCHANGE_ITEM(item)) = t[3];

	*error = 21;
	if (!get_buf_line(data, buffer) || sscanf(buffer, " %s %s %s", f0, f1, f2) != 3)
		return (item);
	GET_OBJ_AFFECTS(GET_EXCHANGE_ITEM(item)) = clear_flags;
	GET_OBJ_ANTI(GET_EXCHANGE_ITEM(item)) = clear_flags;
	GET_OBJ_NO(GET_EXCHANGE_ITEM(item)) = clear_flags;
	asciiflag_conv(f0, &GET_OBJ_AFFECTS(GET_EXCHANGE_ITEM(item)));
	asciiflag_conv(f1, &GET_OBJ_ANTI(GET_EXCHANGE_ITEM(item)));
	asciiflag_conv(f2, &GET_OBJ_NO(GET_EXCHANGE_ITEM(item)));

	*error = 22;
	if (!get_buf_line(data, buffer) || sscanf(buffer, " %d %s %s", t, f1, f2) != 3)
		return (item);
	GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) = t[0];
	GET_EXCHANGE_ITEM(item)->obj_flags.extra_flags = clear_flags;
	GET_OBJ_WEAR(GET_EXCHANGE_ITEM(item)) = 0;
	asciiflag_conv(f1, &GET_OBJ_EXTRA(GET_EXCHANGE_ITEM(item), 0));
	asciiflag_conv(f2, &GET_OBJ_WEAR(GET_EXCHANGE_ITEM(item)));

	*error = 23;
	if (!get_buf_line(data, buffer) || sscanf(buffer, "%s %d %d %d", f0, t + 1, t + 2, t + 3) != 4)
		return (item);
	GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 0) = 0;
	asciiflag_conv(f0, &GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 0));
	GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 1) = t[1];
	GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 2) = t[2];
	GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 3) = t[3];

	*error = 24;
	if (!get_buf_line(data, buffer) || sscanf(buffer, "%d %d %d %d", t, t + 1, t + 2, t + 3) != 4)
		return (item);
	GET_OBJ_WEIGHT(GET_EXCHANGE_ITEM(item)) = t[0];
	GET_OBJ_COST(GET_EXCHANGE_ITEM(item)) = t[1];
	GET_OBJ_RENT(GET_EXCHANGE_ITEM(item)) = t[2];
	GET_OBJ_RENTEQ(GET_EXCHANGE_ITEM(item)) = t[3];

	*error = 25;
	if (!get_buf_line(data, buffer) || sscanf(buffer, "%d %d", t, t + 1) != 2)
		return (item);
	GET_EXCHANGE_ITEM(item)->worn_on = t[0];
	GET_OBJ_OWNER(GET_EXCHANGE_ITEM(item)) = t[1];


	// ��������� ��� ���� � �.�.
	if (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) == ITEM_DRINKCON ||
			GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) == ITEM_FOUNTAIN)
	{
		if (GET_OBJ_WEIGHT(GET_EXCHANGE_ITEM(item)) < GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 1))
			GET_OBJ_WEIGHT(GET_EXCHANGE_ITEM(item)) = GET_OBJ_VAL(GET_EXCHANGE_ITEM(item), 1) + 5;
	}

	GET_EXCHANGE_ITEM(item)->ex_description = NULL;	// Exclude doubling ex_description !!!
	j = 0;

	for (;;)
	{
		if (!get_buf_line(data, buffer))
		{
			*error = 0;
			for (; j < MAX_OBJ_AFFECT; j++)
			{
				GET_EXCHANGE_ITEM(item)->affected[j].location = APPLY_NONE;
				GET_EXCHANGE_ITEM(item)->affected[j].modifier = 0;
			}
			if (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(item)) == ITEM_MING)
			{
				int err = im_assign_power(GET_EXCHANGE_ITEM(item));
				if (err)
					*error = 100 + err;
			}
			return (item);
		}
		switch (*buffer)
		{
		case 'E':
			CREATE(new_descr, EXTRA_DESCR_DATA, 1);
			if (!get_buf_lines(data, buffer))
			{
				free(new_descr);
				*error = 26;
				return (item);
			}
			new_descr->keyword = str_dup(buffer);
			if (!get_buf_lines(data, buffer))
			{
				free(new_descr->keyword);
				free(new_descr);
				*error = 27;
				return (item);
			}
			new_descr->description = str_dup(buffer);
			new_descr->next = GET_EXCHANGE_ITEM(item)->ex_description;
			GET_EXCHANGE_ITEM(item)->ex_description = new_descr;
			break;
		case 'A':
			if (j >= MAX_OBJ_AFFECT)
			{
				*error = 28;
				return (item);
			}
			if (!get_buf_line(data, buffer))
			{
				*error = 29;
				return (item);
			}
			if (sscanf(buffer, " %d %d ", t, t + 1) == 2)
			{
				GET_EXCHANGE_ITEM(item)->affected[j].location = t[0];
				GET_EXCHANGE_ITEM(item)->affected[j].modifier = t[1];
				j++;
			}
			break;
		case 'M':
			// ��������� ���� ���������� ����� ���������
			if (!get_buf_line(data, buffer))
			{
				*error = 30;
				return (item);
			}
			if (sscanf(buffer, " %d ", t) == 1)
			{
				GET_OBJ_MAKER(GET_EXCHANGE_ITEM(item)) = t[0];
			}
			break;
		case 'P':
			if (!get_buf_line(data, buffer))
			{
				*error = 31;
				return (item);
			}
			if (sscanf(buffer, " %d ", t) == 1)
			{
				int rnum;
				GET_OBJ_PARENT(GET_EXCHANGE_ITEM(item)) = t[0];
				rnum = real_mobile(GET_OBJ_PARENT(GET_EXCHANGE_ITEM(item)));
				if (rnum > -1)
				{
					trans_obj_name(GET_EXCHANGE_ITEM(item), &mob_proto[rnum]);
				}
			}
			break;

		default:
			break;
		}
	}
	*error = 32;
	return (item);
}

void exchange_write_one_object_new(std::stringstream &out, EXCHANGE_ITEM_DATA * item)
{
	out << "#" << GET_EXCHANGE_ITEM_LOT(item) << "\n"
		<< GET_EXCHANGE_ITEM_SELLERID(item) << "\n"
		<< GET_EXCHANGE_ITEM_COST(item) << "\n"
		<< (GET_EXCHANGE_ITEM_COMMENT(item) ? GET_EXCHANGE_ITEM_COMMENT(item) : "EMPTY\n")
		<< "\n";
	write_one_object(out, GET_EXCHANGE_ITEM(item), 0);
	out << "\n";
}

int exchange_database_load()
{
	FILE *fl;
	char *data, *readdata;
	EXCHANGE_ITEM_DATA *item;
	int fsize, error, max_lot = 0;
	char buffer[MAX_STRING_LENGTH];

	log("Exchange: loading database... (exchange.cpp)");

	if (!(fl = fopen(EXCHANGE_DATABASE_FILE, "r")))
	{
		log("SYSERR: Error opening exchange database. (exchange.cpp)");
		return (0);
	}

	fseek(fl, 0L, SEEK_END);
	fsize = ftell(fl);

	CREATE(readdata, char, fsize + 1);
	fseek(fl, 0L, SEEK_SET);
	if (!fread(readdata, fsize, 1, fl) || ferror(fl))
	{
		fclose(fl);
		log("SYSERR: Memory error or cann't read exchange database file. (exchange.cpp)");
		free(readdata);
		return (0);
	};
	fclose(fl);

	data = readdata;
	*(data + fsize) = '\0';

	// ����� ���� ��� ������?
	get_buf_line(&data, buffer);
	if (strstr(buffer, "!NEW!") == NULL)
	{
		newbase = FALSE;
		data = readdata;
	}
	else
	{
		newbase = TRUE;
	}

	for (fsize = 0; *data && *data != EX_END_CHAR; fsize++)
	{
		if (newbase)
		{
			item = exchange_read_one_object_new(&data, &error);
		}
		else
		{
			item = exchange_read_one_object(&data, &error);
		}

		if (item == NULL)
		{
			log("SYSERR: Error #%d reading exchange database file. (exchange.cpp)", error);
			return (0);
		}

		if (error)
		{
			log("SYSERR: Error #%d reading item from exchange database.", error);
			extract_exchange_item(item);
			continue;
		}

		// ������� ������������� �� ��������
		if (GET_EXCHANGE_ITEM(item)->get_timer() <= 0)
		{
			sprintf(buf, "Exchange: - %s ��������%s �� ����������� �������������.\r\n",
					CAP(GET_EXCHANGE_ITEM(item)->PNames[0]), GET_OBJ_SUF_2(GET_EXCHANGE_ITEM(item)));
			log(buf);
			extract_exchange_item(item);
			continue;
		}
		max_lot = MAX(max_lot, GET_EXCHANGE_ITEM_LOT(item));
	}

	free(readdata);

	lot_usage.resize(max_lot, false);
	for (item = exchange_item_list; item; item = item->next)
		lot_usage[GET_EXCHANGE_ITEM_LOT(item) - 1] = true;

	log("Exchange: done loading database.");

	return (1);
}


int exchange_database_reload(bool loadbackup)
{
	FILE *fl;
	char *data, *readdata;
	EXCHANGE_ITEM_DATA *item, *j, *next_thing;
	int fsize, error, max_lot = 0;
	char buffer[MAX_STRING_LENGTH];

	for (j = exchange_item_list; j; j = next_thing)
	{
		next_thing = j->next;
		extract_exchange_item(j);
	}
	if (loadbackup)
	{
		log("Exchange: reloading backup of database... (exchange.cpp)");
		if (!(fl = fopen(EXCHANGE_DATABASE_BACKUPFILE, "r")))
		{
			log("SYSERR: Error opening exchange database backup. (exchange.cpp)");
			return (0);
		}
	}
	else
	{
		log("Exchange: reloading database... (exchange.cpp)");
		if (!(fl = fopen(EXCHANGE_DATABASE_FILE, "r")))
		{
			log("SYSERR: Error opening exchange database. (exchange.cpp)");
			return (0);
		}
	}

	fseek(fl, 0L, SEEK_END);
	fsize = ftell(fl);

	CREATE(readdata, char, fsize + 1);
	fseek(fl, 0L, SEEK_SET);
	if (!fread(readdata, fsize, 1, fl) || ferror(fl))
	{
		fclose(fl);
		if (loadbackup)
			log("SYSERR: Memory error or cann't read exchange database backup file. (exchange.cpp)");
		else
			log("SYSERR: Memory error or cann't read exchange database file. (exchange.cpp)");
		free(readdata);
		return (0);
	};
	fclose(fl);

	data = readdata;
	*(data + fsize) = '\0';

	// ����� ���� ��� ������?
	get_buf_line(&data, buffer);
	if (strstr(buffer, "!NEW!") == NULL)
	{
		newbase = FALSE;
		data = readdata;
	}
	else
	{
		newbase = TRUE;
	}

	for (fsize = 0; *data && *data != EX_END_CHAR; fsize++)
	{
		if (newbase)
		{
			item = exchange_read_one_object_new(&data, &error);
		}
		else
		{
			item = exchange_read_one_object(&data, &error);
		}

		if (item == NULL)
		{
			if (loadbackup)
				log("SYSERR: Error #%d reading exchange database backup file. (exchange.cpp)", error);
			else
				log("SYSERR: Error #%d reading exchange database file. (exchange.cpp)", error);
			return (0);
		}

		if (error)
		{
			if (loadbackup)
				log("SYSERR: Error #%d reading item from exchange database backup.", error);
			else
				log("SYSERR: Error #%d reading item from exchange database.", error);
			extract_exchange_item(item);
			continue;
		}

		// ������� ������������� �� ��������
		if (GET_EXCHANGE_ITEM(item)->get_timer() <= 0)
		{
			sprintf(buf, "Exchange: - %s ��������%s �� ����������� �������������.\r\n",
					CAP(GET_EXCHANGE_ITEM(item)->PNames[0]), GET_OBJ_SUF_2(GET_EXCHANGE_ITEM(item)));
			log(buf);
			extract_exchange_item(item);
			continue;
		}
		max_lot = MAX(max_lot, GET_EXCHANGE_ITEM_LOT(item));
	}

	free(readdata);

	lot_usage.resize(max_lot, false);
	for (item = exchange_item_list; item; item = item->next)
		lot_usage[GET_EXCHANGE_ITEM_LOT(item) - 1] = true;

	if (loadbackup)
		log("Exchange: done reloading database backup.");
	else
		log("Exchange: done reloading database.");

	return (1);
}

/**
* \param backup - false (������) ������� ����������
*               - true ���������� � �����, ������ ��������� �����
*/
void exchange_database_save(bool backup)
{
	log("Exchange: Saving exchange database...");

	std::stringstream out;
	out << "!NEW!\n";
	EXCHANGE_ITEM_DATA *j, *next_thing;
	for (j = exchange_item_list; j; j = next_thing)
	{
		next_thing = j->next;
		exchange_write_one_object_new(out, j);
	}
	out << "$\n$\n";

	const char *filename = backup ? EXCHANGE_DATABASE_BACKUPFILE : EXCHANGE_DATABASE_FILE;
	std::ofstream file(filename);
	if (!file.is_open())
	{
		sprintf(buf, "[SYSERROR] open exchange db ('%s')", filename);
		mudlog(buf, BRF, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}
	file << out.rdbuf();
	file.close();

	log("Exchange: done saving database.");
	return;
}

int get_unique_lot(void)
{
	int i;
	for (i = 0; i < (int) lot_usage.size(); i++)
		if (!lot_usage[i])
		{
			lot_usage[i] = true;
			return i + 1;
		}

	if (lot_usage.size() < INT_MAX)
	{
		lot_usage.push_back(true);
		return i + 1;
	}
	else
		return -1;
}


void message_exchange(char *message, CHAR_DATA * ch, EXCHANGE_ITEM_DATA * j)
{
	DESCRIPTOR_DATA *i;

	char filter_name[FILTER_LENGTH];
	char filter_owner[FILTER_LENGTH];
	int filter_type = 0;
	int filter_cost = 0;
	int filter_timer = 0;
	int filter_wereon = 0;
	int filter_weaponclass = 0;

	memset(filter_name, 0, FILTER_LENGTH);
	memset(filter_owner, 0, FILTER_LENGTH);


	for (i = descriptor_list; i; i = i->next)
	{
		if (STATE(i) == CON_PLAYING &&
				(!ch || i != ch->desc) &&
				i->character &&
				!PRF_FLAGGED(i->character, PRF_NOEXCHANGE) &&
				!PLR_FLAGGED(i->character, PLR_WRITING) &&
				!ROOM_FLAGGED(IN_ROOM(i->character), ROOM_SOUNDPROOF) && GET_POS(i->character) > POS_SLEEPING)
		{
			if ((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_INGRADIENT || GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_MING)
				&& !PRF_FLAGGED(i->character, PRF_NOINGR_MODE))
			{
				continue;
			}
			if (!EXCHANGE_FILTER(i->character)
					|| ((parse_exch_filter(EXCHANGE_FILTER(i->character),
										   filter_name, filter_owner, &filter_type, &filter_cost, &filter_timer,
										   &filter_wereon, &filter_weaponclass))
						&&
						(obj_matches_filter
						 (j, filter_name, filter_owner, &filter_type, &filter_cost, &filter_timer,
						  &filter_wereon, &filter_weaponclass))))
			{
				if (COLOR_LEV(i->character) >= C_NRM)
					send_to_char(CCIYEL(i->character, C_NRM), i->character);
				act(message, FALSE, i->character, 0, 0, TO_CHAR | TO_SLEEP);
				if (COLOR_LEV(i->character) >= C_NRM)
					send_to_char(CCNRM(i->character, C_NRM), i->character);
			}
		}
	}
}


int obj_matches_filter(EXCHANGE_ITEM_DATA * j, char *filter_name, char *filter_owner, int *filter_type,
					   int *filter_cost, int *filter_timer, int *filter_wereon, int *filter_weaponclass)
{

	int tm;

	if (*filter_name && !isname(filter_name, GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0)))
	{
		if ((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_MING) &&
				(GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_INGRADIENT))
			return 0;
		//��� ������������, ������������� ��������� ��� ��������a
		else if (!isname(filter_name, GET_OBJ_PNAME(obj_proto[GET_OBJ_RNUM(GET_EXCHANGE_ITEM(j))], 0)))
			return 0;
	}
	if (*filter_owner && !isname(filter_owner, get_name_by_id(GET_EXCHANGE_ITEM_SELLERID(j))))
		return 0;
	if (*filter_type && !(GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == *filter_type))
		return 0;
	if (*filter_cost)
	{
		if ((*filter_cost > 0) && (GET_EXCHANGE_ITEM_COST(j) - *filter_cost < 0))
			return 0;
		else if ((*filter_cost < 0) && (GET_EXCHANGE_ITEM_COST(j) + *filter_cost > 0))
			return 0;
	}
	if (*filter_wereon && (!CAN_WEAR(GET_EXCHANGE_ITEM(j), *filter_wereon)))
		return 0;
	if (*filter_weaponclass
			&& (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_WEAPON || GET_OBJ_SKILL(GET_EXCHANGE_ITEM(j)) != *filter_weaponclass))
		return 0;
	if (*filter_timer)
	{
		// ������ ����� ��� �� �������� �� ������ ����� � ������ ���� �� ������,
		// ������ ���� ������ ������� ���� � -1 ������, �� �� ������ ������� // Krodo
		try
		{
			tm = (GET_EXCHANGE_ITEM(j)->get_timer() * 100 / obj_proto.at(GET_OBJ_RNUM(GET_EXCHANGE_ITEM(j)))->get_timer());
			if ((tm + 1) < *filter_timer)
				return 0;
		}
		catch (std::out_of_range)
		{
			log("SYSERROR: wrong obj_proto in exchange (%s %s %d)", __FILE__, __func__, __LINE__);
			return 0;
		}
	}

	return 1;		// 1 - ������ ��������� ��� ������
}


void show_lots(char *filter, short int show_type, CHAR_DATA * ch)
{
	/*
	show_type
	0 - ���
	1 - ���
	2 - ����
	3 - ������
	4 - ������
	5 - �����
	6 - �����
	7 - ������
	8 - ������ �������
	9 - ������� �������
	10 - ������� �������
	*/
	char tmpbuf[MAX_INPUT_LENGTH];
	bool any_item = 0;

	char filter_name[FILTER_LENGTH];
	char filter_owner[FILTER_LENGTH];
	int filter_type = 0;
	int filter_cost = 0;
	int filter_timer = 0;
	int filter_wereon = 0;
	int filter_weaponclass = 0;

	memset(filter_name, 0, FILTER_LENGTH);
	memset(filter_owner, 0, FILTER_LENGTH);

	if (!parse_exch_filter(filter, filter_name, filter_owner, &filter_type, &filter_cost, &filter_timer,
						   &filter_wereon, &filter_weaponclass))
	{
		send_to_char("�������� ������ ���������� !\r\n", ch);
		log("Exchange: Player uses wrong filter '%s'", filter);
		return;
	}

	std::string buffer =
		" ���     �������                                                     ����  ���������\r\n"
		"--------------------------------------------------------------------------------------------\r\n";

	for (EXCHANGE_ITEM_DATA* j = exchange_item_list; j; j = j->next)
	{
		if ((!obj_matches_filter(j, filter_name, filter_owner, &filter_type,
								 &filter_cost, &filter_timer, &filter_wereon, &filter_weaponclass))
				|| ((show_type == 1) && (!isname(GET_NAME(ch), get_name_by_id(GET_EXCHANGE_ITEM_SELLERID(j)))))
				|| ((show_type == 2) && ((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_INGRADIENT) ||
										 (GET_OBJ_VNUM(GET_EXCHANGE_ITEM(j)) < 200)
										 || (GET_OBJ_VNUM(GET_EXCHANGE_ITEM(j)) > 299)))
				//|| ((show_type == 3) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR))
				|| ((show_type == 3) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR)
							&& (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_LIGHT)
							&& (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_MEDIAN)
							&& (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_HEAVY)
					)
				|| ((show_type == 4) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_WEAPON))
				|| ((show_type == 5) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_BOOK))

				|| (show_type == 6 && GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_MING
					&& (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_INGRADIENT
						|| (GET_OBJ_VNUM(GET_EXCHANGE_ITEM(j)) >= 200 && GET_OBJ_VNUM(GET_EXCHANGE_ITEM(j)) <= 299)))

				|| ((show_type == 7) && ((GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_INGRADIENT)
										 || ObjSystem::is_armor_type(GET_EXCHANGE_ITEM(j))
										 || (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_WEAPON)
										 || (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_BOOK)
										 || (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) == ITEM_MING)))

				|| ((show_type == 8) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_LIGHT))
				|| ((show_type == 9) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_MEDIAN))
				|| ((show_type == 10) && (GET_OBJ_TYPE(GET_EXCHANGE_ITEM(j)) != ITEM_ARMOR_HEAVY))
			)
			continue;

		// �� �������� ������ ������� 5-10 ������� ������
		if (is_abbrev("������ ��������", GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0))
			|| is_abbrev("������� ���������� �������", GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0))
			|| is_abbrev("������ ��������", GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0)))
		{
			sprintbits(GET_EXCHANGE_ITEM(j)->obj_flags.affects, weapon_affects, buf, ",");
			// ��������� ������������ ����, ����� ��� �� ������ �� �������� ���� ������
			if (!strcmp(buf, "������"))  // added by WorM (�������) ����������� �� ������ ��������, �� � ���.������� ��������
			{
				bool found = false;
				int drndice = 0, drsdice = 0;

				for (int n = 0; n < MAX_OBJ_AFFECT; n++)
				{
					drndice = GET_EXCHANGE_ITEM(j)->affected[n].location;
					drsdice = GET_EXCHANGE_ITEM(j)->affected[n].modifier;
					if ((drndice != APPLY_NONE) && (drsdice != 0))
					{
						sprinttype(drndice, apply_types, buf2);
						bool negative = false;
						for (int k = 0; *apply_negative[k] != '\n'; k++)
						{
							if (!str_cmp(buf2, apply_negative[k]))
							{
								negative = true;
								break;
							}
						}
						if (drsdice < 0)
							negative = !negative;
						sprintf(buf, "%s %s%d", buf2, negative ? "-" : "+", abs(drsdice));
						found = true;
						break;
					}
				}

				if (!found)
					sprintf(tmpbuf, "[%4d]   %s", GET_EXCHANGE_ITEM_LOT(j), GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0));
				else
					sprintf(tmpbuf, "[%4d]   %s (%s)", GET_EXCHANGE_ITEM_LOT(j), GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0), buf);
			}
			else  // end by WorM
			{
				sprintf(tmpbuf, "[%4d]   %s (%s)", GET_EXCHANGE_ITEM_LOT(j), GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0), buf);
			}
		}
		else if (is_dig_stone(GET_EXCHANGE_ITEM(j)) && GET_OBJ_MATER(GET_EXCHANGE_ITEM(j)) == MAT_GLASS)
		{
			sprintf(tmpbuf, "[%4d]   %s (������)", GET_EXCHANGE_ITEM_LOT(j), GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0));
		}
		else
		{
			sprintf(tmpbuf, "[%4d]   %s", GET_EXCHANGE_ITEM_LOT(j), GET_OBJ_PNAME(GET_EXCHANGE_ITEM(j), 0));
		}

		sprintf(tmpbuf, "%s %9d  %-s\r\n", colored_name(tmpbuf, 63, true), GET_EXCHANGE_ITEM_COST(j), diag_obj_timer(GET_EXCHANGE_ITEM(j)));
		// ����� ��� ����, �� ��������� ��� ������ ������� ��� ������� �� 0.6 ������� ��� ������ ����� ������. ������� ����� -- Krodo
		buffer += tmpbuf;
		any_item = 1;
	}

	if (!any_item)
		buffer = "����� ���� !\r\n";

	page_string(ch->desc, buffer);
}

int parse_exch_filter(char *buf, char *filter_name, char *filter_owner, int *filter_type,
					  int *filter_cost, int *filter_timer, int *filter_wereon, int *filter_weaponclass)
{
	char sign[FILTER_LENGTH];
	char tmpbuf[FILTER_LENGTH];

	while (*buf && (*buf != '\r') && (*buf != '\n'))
	{
		switch (*buf)
		{
		case ' ':
			buf++;
			break;
		case '�':
			buf = one_argument(++buf, filter_name);
			break;
		case '�':
			buf = one_argument(++buf, filter_owner);
			break;
		case '�':
			buf = one_argument(++buf, tmpbuf);
			if (is_abbrev(tmpbuf, "����") || is_abbrev(tmpbuf, "light"))
				*filter_type = ITEM_LIGHT;
			else if (is_abbrev(tmpbuf, "������") || is_abbrev(tmpbuf, "scroll"))
				*filter_type = ITEM_SCROLL;
			else if (is_abbrev(tmpbuf, "�������") || is_abbrev(tmpbuf, "wand"))
				*filter_type = ITEM_WAND;
			else if (is_abbrev(tmpbuf, "�����") || is_abbrev(tmpbuf, "staff"))
				*filter_type = ITEM_STAFF;
			else if (is_abbrev(tmpbuf, "������") || is_abbrev(tmpbuf, "weapon"))
				*filter_type = ITEM_WEAPON;
			else if (is_abbrev(tmpbuf, "�����") || is_abbrev(tmpbuf, "armor"))
				*filter_type = ITEM_ARMOR;
			else if (is_abbrev(tmpbuf, "�������") || is_abbrev(tmpbuf, "potion"))
				*filter_type = ITEM_POTION;
			else if (is_abbrev(tmpbuf, "������") || is_abbrev(tmpbuf, "other"))
				*filter_type = ITEM_OTHER;
			else if (is_abbrev(tmpbuf, "���������") || is_abbrev(tmpbuf, "container"))
				*filter_type = ITEM_CONTAINER;
			else if (is_abbrev(tmpbuf, "�����") || is_abbrev(tmpbuf, "book"))
				*filter_type = ITEM_BOOK;
			else if (is_abbrev(tmpbuf, "����") || is_abbrev(tmpbuf, "rune"))
				*filter_type = ITEM_INGRADIENT;
			else if (is_abbrev(tmpbuf, "����������") || is_abbrev(tmpbuf, "ingradient"))
				*filter_type = ITEM_MING;
			else if (is_abbrev(tmpbuf, "������") || is_abbrev(tmpbuf, "������"))
				*filter_type = ITEM_ARMOR_LIGHT;
			else if (is_abbrev(tmpbuf, "�������") || is_abbrev(tmpbuf, "�������"))
				*filter_type = ITEM_ARMOR_MEDIAN;
			else if (is_abbrev(tmpbuf, "�������") || is_abbrev(tmpbuf, "�������"))
				*filter_type = ITEM_ARMOR_HEAVY;
			else
				return 0;
			break;
		case '�':
			buf = one_argument(++buf, tmpbuf);
			if (sscanf(tmpbuf, "%d%[-+]", filter_cost, sign) != 2)
				return 0;
			if (*sign == '-')
				*filter_cost = -(*filter_cost);
			break;
		case '�':
			buf = one_argument(++buf, tmpbuf);
			if (is_abbrev(tmpbuf, "������"))
				*filter_timer = 1;
			else if (is_abbrev(tmpbuf, "�����_����������"))
				*filter_timer = 21;
			else if (is_abbrev(tmpbuf, "���������"))
				*filter_timer = 41;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_timer = 61;
			else if (is_abbrev(tmpbuf, "��������"))
				*filter_timer = 81;
			else
				return 0;

			break;
		case '�':
			buf = one_argument(++buf, tmpbuf);
			if (is_abbrev(tmpbuf, "�����"))
				*filter_wereon = ITEM_WEAR_FINGER;
			else if (is_abbrev(tmpbuf, "���") || is_abbrev(tmpbuf, "�����"))
				*filter_wereon = ITEM_WEAR_NECK;
			else if (is_abbrev(tmpbuf, "����"))
				*filter_wereon = ITEM_WEAR_BODY;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_wereon = ITEM_WEAR_HEAD;
			else if (is_abbrev(tmpbuf, "����"))
				*filter_wereon = ITEM_WEAR_LEGS;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_wereon = ITEM_WEAR_FEET;
			else if (is_abbrev(tmpbuf, "�����"))
				*filter_wereon = ITEM_WEAR_HANDS;
			else if (is_abbrev(tmpbuf, "����"))
				*filter_wereon = ITEM_WEAR_ARMS;
			else if (is_abbrev(tmpbuf, "���"))
				*filter_wereon = ITEM_WEAR_SHIELD;
			else if (is_abbrev(tmpbuf, "�����"))
				*filter_wereon = ITEM_WEAR_ABOUT;
			else if (is_abbrev(tmpbuf, "����"))
				*filter_wereon = ITEM_WEAR_WAIST;
			else if (is_abbrev(tmpbuf, "��������"))
				*filter_wereon = ITEM_WEAR_WRIST;
			else if (is_abbrev(tmpbuf, "�����"))
				*filter_wereon = ITEM_WEAR_HOLD;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_wereon = ITEM_WEAR_WIELD;
			else if (is_abbrev(tmpbuf, "���"))
				*filter_wereon = ITEM_WEAR_BOTHS;
			else
				return 0;
			break;
		case '�':
			buf = one_argument(++buf, tmpbuf);
			if (is_abbrev(tmpbuf, "����"))
				*filter_weaponclass = SKILL_BOWS;
			else if (is_abbrev(tmpbuf, "��������"))
				*filter_weaponclass = SKILL_SHORTS;
			else if (is_abbrev(tmpbuf, "�������"))
				*filter_weaponclass = SKILL_LONGS;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_weaponclass = SKILL_AXES;
			else if (is_abbrev(tmpbuf, "������"))
				*filter_weaponclass = SKILL_CLUBS;
			else if (is_abbrev(tmpbuf, "����"))
				*filter_weaponclass = SKILL_NONSTANDART;
			else if (is_abbrev(tmpbuf, "����������"))
				*filter_weaponclass = SKILL_BOTHHANDS;
			else if (is_abbrev(tmpbuf, "�����������"))
				*filter_weaponclass = SKILL_PICK;
			else if (is_abbrev(tmpbuf, "�����"))
				*filter_weaponclass = SKILL_SPADES;
			else
				return 0;
			break;
		default:
			return 0;
		}
	}

	return 1;

}

void clear_exchange_lot(EXCHANGE_ITEM_DATA * lot)
{
	EXCHANGE_ITEM_DATA *temp;
	if (lot == NULL)
		return;


	REMOVE_FROM_LIST(lot, exchange_item_list, next);
	lot_usage[GET_EXCHANGE_ITEM_LOT(lot) - 1] = false;
	if (lot->comment)
		free(lot->comment);
	free(lot);
}

//Polud ��������� ����� ����, ����� ����� ���� ����� ������ ������ ��������� � ����� �������
ACMD(do_exchange)
{
		char* arg = str_dup(argument);
		argument = one_argument(argument, arg1);

		if (IS_NPC(ch)) {
			send_to_char("���������?! �� �� �� �� �������!\r\n", ch);
		} else if (is_abbrev(arg1, "���������") || is_abbrev(arg1, "exhibit")
		|| is_abbrev(arg1, "����") || is_abbrev(arg1, "cost")
		|| is_abbrev(arg1, "�����") || is_abbrev(arg1, "withdraw")
		|| is_abbrev(arg1, "������") || is_abbrev(arg1, "purchase")
		//commented by WorM 2011.05.21 � ����� ���� ���������/��������� ����� ���� � ��������� �������� ?
		/*|| (is_abbrev(arg1, "save") && (GET_LEVEL(ch) >= LVL_IMPL))
		|| (is_abbrev(arg1, "savebackup") && (GET_LEVEL(ch) >= LVL_IMPL))
		|| (is_abbrev(arg1, "reload") && (GET_LEVEL(ch) >= LVL_IMPL))
		|| (is_abbrev(arg1, "reloadbackup") && (GET_LEVEL(ch) >= LVL_IMPL))*/)
		{
			send_to_char("��� ���������� ���������� ����� ��������� ��������, ����� ��������������� ���� ��������.", ch);
		} else
			exchange(ch,NULL,cmd,arg);
		free(arg);
}
