// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#include <map>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include "mobmax.hpp"
#include "char.hpp"
#include "utils.h"
#include "db.h"

namespace
{

// ����������� ���������� ����� ������ ������ � ������ ������� �� �������
const int MIN_MOB_IN_MOBKILL = 2;
// ������������ ���������� ����� ������ ������ � ������ ������� �� �������
const int MAX_MOB_IN_MOBKILL = 100;
// �� ������� ��� ���� ����� ����� ������, ��� �� ���� � ����, ����� ������ ������������
const int MOBKILL_KOEFF = 3;
// ���-�� ����� ������� ������
boost::array<int, MAX_MOB_LEVEL + 1> num_levels = { {0} };
// ��� ������������ ������ � ������� (��� �������� ������ �����-�����)
typedef std::map<int/* ���� ���� */, int/* ����� ���� */> VnumToLevelType;
VnumToLevelType vnum_to_level;

} // namespace

/**
* ���� ������ ���-�� ����� ������� ������ � ��� ������������ ������ � �������.
*/
void MobMax::init()
{
	for (int i = 0; i <= top_of_mobt; ++i)
	{
		int level = GET_LEVEL(mob_proto + i);
		if (level > MAX_MOB_LEVEL)
			log("Warning! Mob >MAXIMUN lev!");
		else if (level < 0)
			log("Warning! Mob <0 lev!");
		else
		{
			++num_levels[level];
			vnum_to_level[mob_index[i].vnum] = level;
		}
	}

	for (int i = 0; i <= MAX_MOB_LEVEL; ++i)
	{
		log("Mob lev %d. Num of mobs %d", i, num_levels[i]);
		num_levels[i] = num_levels[i] / MOBKILL_KOEFF;
		if (num_levels[i] < MIN_MOB_IN_MOBKILL)
			num_levels[i] = MIN_MOB_IN_MOBKILL;
		if (num_levels[i] > MAX_MOB_IN_MOBKILL)
			num_levels[i] = MAX_MOB_IN_MOBKILL;
		log("Mob lev %d. Max in MobKill file %d", i, num_levels[i]);

	}
}

/**
* ���������� ����� ���������� vnum ���� ��� -1, ���� ������ ���.
*/
int MobMax::get_level_by_vnum(int vnum)
{
	VnumToLevelType::const_iterator it = vnum_to_level.find(vnum);
	if (it != vnum_to_level.end())
		return it->second;
	return -1;
}

/**
* ������ ������� �� ����� ������ level, ���� ����� ������� ������ ����� ����������� ���-��.
* ��������� ��� ������� ����� � ����������� ������� ���������, �.�. ������� ���������� ��
* ��� ���� ����� ������� ������, � ������ ������� � ���-��, �������� ������������ � ������
* ����� ������ �������.
*/
void MobMax::refresh(int level)
{
	int count = 0;
	for (MobMaxType::iterator it = mobmax_.begin(); it != mobmax_.end();/* empty */)
	{
		if (it->level == level)
		{
			if (count > num_levels[level])
				mobmax_.erase(it++);
			else
			{
				++count;
				++it;
			}
		}
		else
			++it;
	}
}

/**
* ���������� ������� �� ���� vnum, ������ level. count ��� ������ ���� ������� �����.
*/
void MobMax::add(CHAR_DATA *ch, int vnum, int count, int level)
{
	if (IS_NPC(ch) || IS_IMMORTAL(ch) || vnum < 0 || count < 1 || level < 0 || level > MAX_MOB_LEVEL) return;

	MobMaxType::iterator it = std::find_if(mobmax_.begin(), mobmax_.end(),
			boost::bind(std::equal_to<int>(),
			boost::bind(&mobmax_data::vnum, _1), vnum));

	if (it != mobmax_.end())
		it->count += count;
	else
	{
		mobmax_data tmp_data(vnum, count, level);
		mobmax_.push_front(tmp_data);
	}
	refresh(level);
}

/**
* ������ add ��� ������ �������� ��� ����� �� ����� �������� ���������.
*/
void MobMax::load(CHAR_DATA *ch, int vnum, int count, int level)
{
	if (IS_NPC(ch) || IS_IMMORTAL(ch) || vnum < 0 || count < 1 || level < 0 || level > MAX_MOB_LEVEL) return;

	mobmax_data tmp_data(vnum, count, level);
	mobmax_.push_front(tmp_data);
}

/**
* �������� ������� �� ���������� ���� vnum.
*/
void MobMax::remove(int vnum)
{
	MobMaxType::iterator it = std::find_if(mobmax_.begin(), mobmax_.end(),
			boost::bind(std::equal_to<int>(),
			boost::bind(&mobmax_data::vnum, _1), vnum));

	if (it != mobmax_.end())
		mobmax_.erase(it);
}

/**
* ���������� ���-�� ������ ����� ������� vnum.
*/
int MobMax::get_kill_count(int vnum) const
{
	MobMaxType::const_iterator it = std::find_if(mobmax_.begin(), mobmax_.end(),
			boost::bind(std::equal_to<int>(),
			boost::bind(&mobmax_data::vnum, _1), vnum));

	if (it != mobmax_.end())
		return it->count;
	return 0;
}

/**
* ���������� � �����-����.
*/
void MobMax::save(FILE *saved) const
{
	fprintf(saved, "Mobs:\n");
	for (MobMaxType::const_reverse_iterator it = mobmax_.rbegin(); it != mobmax_.rend(); ++it)
		fprintf(saved, "%d %d\n", it->vnum, it->count);
	fprintf(saved, "~\n");
}

/**
* ��� ������� ���� �������� ��� �������.
*/
void MobMax::clear()
{
	mobmax_.clear();
}
