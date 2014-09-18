// description.cpp
// Copyright (c) 2006 Krodo
// Part of Bylins http://www.mud.ru

#include "conf.h"
#include <stdexcept>

#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "description.h"

std::vector<std::string> RoomDescription::_desc_list;
std::map<std::string, int> RoomDescription::_reboot_map;

/**
* ���������� �������� � ������ � ��������� �� ������������
* \param text - �������� �������
* \return ����� �������� � ���������� �������
*/
int RoomDescription::add_desc(const std::string &text)
{
	std::map<std::string, int>::const_iterator it = _reboot_map.find(text);
	if (it != _reboot_map.end())
		return it->second;
	else
	{
		_desc_list.push_back(text);
		_reboot_map[text] = _desc_list.size();
		return _desc_list.size();
	}
}

/**
* ����� �������� �� ��� ����������� ������ � �������
* \param desc_num - ���������� ����� �������� (descripton_num � room_data)
* \return ������ �������� ��� ������ ������ � ������ ����������� ������
*/
const std::string RoomDescription::show_desc(int desc_num)
{
	try
	{
		return _desc_list.at(--desc_num);
	}
	catch (std::out_of_range)
	{
		log("SYSERROR : bad room description num '%d' (%s %s %d)", desc_num, __FILE__, __func__, __LINE__);
		return "";
	}
}
