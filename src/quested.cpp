// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "quested.hpp"
#include "utils.h"
#include "char.hpp"

void smash_tilde(char *str);

/**
* ���������� ������������ ������ �����/������ ������ (128 ��������).
*/
void Quested::add(CHAR_DATA *ch, int vnum, char *text)
{
	if (!IS_NPC(ch) && !IS_IMMORTAL(ch))
	{
		smash_tilde(text);
		skip_spaces(&text);
		std::string text_node = *text ? text : "";
		if (text_node.size() > 128)
		{
			text_node = text_node.substr(0, 128);
		}
		quested_[vnum] = text_node;
	}
}

bool Quested::remove(int vnum)
{
	QuestedType::iterator it = quested_.find(vnum);
	if (it != quested_.end())
	{
		quested_.erase(it);
		return true;
	}
	return false;
}

bool Quested::get(int vnum) const
{
	QuestedType::const_iterator it = quested_.find(vnum);
	if (it != quested_.end())
		return true;
	return false;
}

/**
* ���������� ������ ������ ��� ��������� ����� ������ ��� ������ ������.
*/
std::string Quested::get_text(int vnum) const
{
	QuestedType::const_iterator it = quested_.find(vnum);
	if (it != quested_.end())
	{
		return it->second;
	}
	return "";
}

std::string Quested::print() const
{
	std::stringstream text;
	for (QuestedType::const_iterator it = quested_.begin(); it != quested_.end(); ++it)
		text << " " << it->first << " " << it->second << "\r\n";
	return text.str();
}

void Quested::save(FILE *saved) const
{
	for (QuestedType::const_iterator it = quested_.begin(); it != quested_.end(); ++it)
		fprintf(saved, "Qst : %d %s~\n", it->first, it->second.c_str());
}

void Quested::clear()
{
	quested_.clear();
}
