// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2007 Krodo
// Part of Bylins http://www.mud.ru

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "comm.h"
#include "utils.h"

extern char mudname[];

/**
* ���� ���� - ������������� ��� ������� �������������� ������� � ��� ���������� ���� ������.
*/

void show_code_date(CHAR_DATA *ch)
{
#ifdef CYGWIN
	send_to_char(ch, "�������-���, ������ ��� Windows �� %s %s\r\n������� ���̣� http://mud.kharkov.org\r\n", __DATE__, __TIME__);
#else
#ifdef STATIC_LIBS
	send_to_char(ch, "�������-���, ������ �� ������������ ������������, %s %s\r\n������� ���̣� http://mud.kharkov.org\r\n", __DATE__, __TIME__);
#else
	send_to_char(ch, "�������-���, ������ �� %s %s\r\n������� ���̣� http://mud.kharkov.org\r\n", __DATE__, __TIME__);
#endif
#endif

	if (mudname[0])
		send_to_char(ch, "&R%s&n\r\n",mudname);
}

void log_code_date()
{
	log("Code version %s %s", __DATE__, __TIME__);
}
