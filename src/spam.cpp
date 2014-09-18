// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#include <list>
#include <sstream>
#include "spam.hpp"
#include "utils.h"
#include "char.hpp"
#include "comm.h"

using namespace SpamSystem;

namespace
{

// ���� ��������� ������ �� ������ ����
const int MAX_MESSAGE_RUNNING = 10;
// � ��������� ���������, �������������� ��� ��������
const unsigned MESSAGE_LIST_SIZE = 31;
// ���� ��������� �� ������ ���� �� ��������� MESSAGE_LIST_SIZE ���������
const int MAX_MESSAGE_TOTAL = 15;
// ���-�� ���������� � MAX_MESSAGE_TOTAL ������ ����� ������ � ������� ��������
// �.�. ���� ���� ��� ������ MAX_MESSAGE_TOTAL ������ �� ��������� MESSAGE_LIST_SIZE,
// �� ��� ���� ����� ���� ���� ��������� ��������� �����
const int MAX_TIME_TOTAL = 45;
// ������ ����� ��������� �� ������ ���� (������ ���� ����)
const int MIN_MESSAGE_TIME = 1;

enum { NORMAL_FLAG, RUNNING_FLAG, MIN_TIME_FLAG, TOTAL_FLAG };

typedef std::list<std::pair<long, time_t> > UidListType;
UidListType offtop_list;

/**
*
*/
void add_to_list(UidListType &list, long uid)
{
	list.push_front(std::make_pair(uid, time(0)));
	if (list.size() > MESSAGE_LIST_SIZE)
	{
		list.pop_back();
	}
};

/**
*
*/
int check_list(const UidListType &list, long uid)
{
	int total = 0, running = 0;
	time_t first_message = time(0);
	for (UidListType::const_iterator i = offtop_list.begin(); i != offtop_list.end(); ++i)
	{
		if (uid != i->first)
		{
			running = -1;
		}
		else
		{
			if (running != -1)
			{
				++running;
				if (running >= MAX_MESSAGE_RUNNING)
				{
					return RUNNING_FLAG;
				}
			}
			if ((time(0) - i->second) < MIN_MESSAGE_TIME)
			{
					return MIN_TIME_FLAG;
			}
			++total;
			first_message = i->second;
		}
	}
	if (total >= MAX_MESSAGE_TOTAL
		&& (time(0) - first_message) < MAX_TIME_TOTAL)
	{
		return TOTAL_FLAG;
	}
	return NORMAL_FLAG;
};

/**
*
*/
int add_message(int mode, long uid)
{
	switch(mode)
	{
	case OFFTOP_MODE:
	{
		int flag = check_list(offtop_list, uid);
		if (NORMAL_FLAG == flag)
		{
			add_to_list(offtop_list, uid);
		}
		else
		{
			return flag;
		}
		break;
	}
	default:
		log("SYSERROR: �� �� ������ ���� ���� ������� (%s %s %d)", __FILE__, __func__, __LINE__);
		return NORMAL_FLAG;
	};

	return NORMAL_FLAG;
}

} // namespace

namespace SpamSystem
{

/**
*
*/
bool check(CHAR_DATA *ch, int mode)
{
	int flag = add_message(mode, GET_UNIQUE(ch));
	if (NORMAL_FLAG != flag)
	{
		std::stringstream text;
		text << "����-��������: ";
		switch (flag)
		{
		case RUNNING_FLAG:
			text << "������ " << MAX_MESSAGE_RUNNING << " ��������� ������";
			break;
		case MIN_TIME_FLAG:
			text << "�������� ����� ����������� ������ " << MIN_MESSAGE_TIME << " �������";
			break;
		case TOTAL_FLAG:
			text << "������� ������� ����� ��������� �� ��������� " << MAX_TIME_TOTAL << " ������";
			break;
		default:
			log("SYSERROR: �� �� ������ ���� ���� ������� (%s %s %d)", __FILE__, __func__, __LINE__);
			return true;
		};
		text << ".\r\n";
		send_to_char(text.str(), ch);
		return false;
	}
	return true;
}

} // SpamSystem
