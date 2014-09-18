// description.h
// Copyright (c) 2006 Krodo
// Part of Bylins http://www.mud.ru

#ifndef _DESCRIPTION_H_INCLUDED
#define _DESCRIPTION_H_INCLUDED

#include "conf.h"
#include <string>
#include <vector>
#include <map>

/**
* ���������� �����-��������� ���������� �������� ������, ���� �������� 70+�� ������.
* ��������: ������ �������� ������, ������� ������� ���������. � ������� �������
* �� ���� ��������, � ��� ���������� ����� � ���������� ������� ���� ��������.
* ���� � ��� ��������� ����� ������ ���� ����� ���������, �� �� ���������,
* �.�. �� ������ �� ���� �� ����������, �� ������� ���� � ������� �������� ���.
* ��� �������������� � ��� ������ �������� �������� � �������, �.�. ��� ��� �����

* \todo � ����������� ����� ����������� � class room, � ����� ������ ������ �� ����,
* �.�. ���� ���� ����� ���������� ���� � ��������� � ������ ���.
*/
class RoomDescription
{
public:
	static int add_desc(const std::string &text);
	static const std::string show_desc(int desc_num);

private:
	RoomDescription();
	~RoomDescription();
	// ������ ������� �������� ��� ������ ����
	static std::vector<std::string> _desc_list;
	// � ��� ����� ��� �� �������� ��� ����. ��-�� ����������� ����������� ����� ���
	// ����� ��������� �� ��� ����� ������ ���� ��� ���, � ��� � ����������� ������� ��������
	static std::map<std::string, int> _reboot_map;
};

#endif // _DESCRIPTION_H_INCLUDED
