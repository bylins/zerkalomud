/**************************************************************************
*   File: exchange.h                                 Part of Bylins       *
*  Usage: Exhange headers functions used by the MUD                       *
*                                                                         *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

#ifndef _EXCHANGE_HPP_
#define _EXCHANGE_HPP_

SPECIAL(exchange);


typedef struct exchange_item_data
			EXCHANGE_ITEM_DATA;


extern EXCHANGE_ITEM_DATA *exchange_item_list;
extern std::vector<bool> lot_usage;


#define EXCHANGE_AUTOSAVETIME 300	//���-�� ������ ����� ���������������� ������ (0 ��� ����������)
#define EXCHANGE_AUTOSAVEBACKUPTIME 750	//���-�� ������ ����� ���������������� ������ (0 ��� ����������)
#define EXCHANGE_SAVEONEVERYOPERATION FALSE	//��������� ����� ����� ������ ��������
#define EXCHANGE_DATABASE_FILE LIB_PLRSTUFF"exchange.db"
#define EXCHANGE_DATABASE_BACKUPFILE LIB_PLRSTUFF"exchange.backup"
#define EX_NEW_ITEM_CHAR '#'
#define EX_END_CHAR '$'
#define FILTER_LENGTH 25
#define EXCHANGE_EXHIBIT_PAY 100	// ����� �� ����������� �� �����
#define EXCHANGE_EXHIBIT_PAY_COEFF 0.05	// ����������� ������ � ����������� �� ���� ������
#define EXCHANGE_IDENT_PAY 110	//���� �� ���������
#define EXCHANGE_MIN_CHAR_LEV 8	//����������� ������� ��� ������� � ������
#define EXCHANGE_MAX_EXHIBIT_PER_CHAR 20	//������������ ���-�� ������������ �������� ����� �����


#define GET_EXCHANGE_ITEM_LOT(item)  ((item)->lot_id)
#define GET_EXCHANGE_ITEM_SELLERID(item)  ((item)->seller_id)
#define GET_EXCHANGE_ITEM_COST(item)  ((item)->obj_cost)
#define GET_EXCHANGE_ITEM_COMMENT(item)  ((item)->comment)
#define GET_EXCHANGE_ITEM(item)  ((item)->obj)


void extract_exchange_item(EXCHANGE_ITEM_DATA * item);
void check_exchange(OBJ_DATA * obj);

struct exchange_item_data
{
	int lot_id;		//����� ����
	int seller_id;		//����� ��������
	int obj_cost;		//���� ����
	char *comment;		//����������
	OBJ_DATA *obj;		//���������� �������
	EXCHANGE_ITEM_DATA *next;	//��� ������ �������� ������
};

void exchange_database_save(bool backup = false);

#endif
