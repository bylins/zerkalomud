// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#ifndef DPS_HPP_INCLUDED
#define DPS_HPP_INCLUDED

#include <list>
#include <map>
#include <string>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"

namespace DpsSystem
{

void check_round(CHAR_DATA *ch);

// ������, ����� �� ������� ������� (����, ����� ��������, ���� �� ������, ������� ����� �� ������)
enum { PERS_DPS, PERS_CHARM_DPS, GROUP_DPS, GROUP_CHARM_DPS };

class DpsNode
{
public:
	DpsNode(long id = 0) : dmg_(0), over_dmg_(0), id_(id),
			round_dmg_(0), buf_dmg_(0), rounds_(0) {};
	void add_dmg(int dmg, int over_dmg);
	void set_name(const char *name);
	int get_stat() const;
	unsigned get_dmg() const;
	unsigned get_over_dmg() const;
	long get_id() const;
	const std::string & get_name() const;
	unsigned get_round_dmg() const;
	void end_round();

private:
	// ���������� �����
	unsigned dmg_;
	// ����� ������ �� ��� ������� ����
	unsigned over_dmg_;
	// ��� ��� ������ � ������ ��� ���������� � ����
	std::string name_;
	// ��� �������������
	long id_;
	// ���������� ����� �� �����
	unsigned round_dmg_;
	// ��� ����� ������ �� �����
	unsigned buf_dmg_;
	// ������� ����� ������� ��������� � ���
	int rounds_;
};

typedef std::list<DpsNode> CharmListType;

/**
* ������� �� DpsNode �� ������� �������� (��� ������).
*/
class PlayerDpsNode : public DpsNode
{
public:
	void add_charm_dmg(CHAR_DATA *ch, int dmg, int over_dmg);
	std::string print_charm_stats() const;
	void print_group_charm_stats(CHAR_DATA *ch) const;
	void end_charm_round(CHAR_DATA *ch);

private:
	CharmListType::iterator find_charmice(CHAR_DATA *ch);

	// ������ �������� (MAX_DPS_CHARMICE)
	CharmListType charm_list_;
};

typedef std::map<long /* id */, PlayerDpsNode> GroupListType;

/**
* ������� ��������, ������� � Player.
*/
class Dps
{
public:
	Dps() : exp_(0), battle_exp_(0), lost_exp_(0) {};
	Dps & operator= (const Dps &copy);

	void add_dmg(int type, CHAR_DATA *ch, int dmg, int over_dmg);
	void clear(int type);
	void print_stats(CHAR_DATA *ch, CHAR_DATA *coder = 0);
	void print_group_stats(CHAR_DATA *ch, CHAR_DATA *coder = 0);
	void end_round(int type, CHAR_DATA *ch);

	void add_exp(int exp);
	void add_battle_exp(int exp);

private:
	void add_tmp_group_list(CHAR_DATA *ch);
	void add_group_dmg(CHAR_DATA *ch, int dmg, int over_dmg);
	void end_group_round(CHAR_DATA *ch);
	void add_group_charm_dmg(CHAR_DATA *ch, int dmg, int over_dmg);
	void end_group_charm_round(CHAR_DATA *ch);

	// ��������� ����������
	GroupListType group_dps_;
	// ������������ ���������� � ���� �������
	PlayerDpsNode pers_dps_;
	// ������� �����, ������� ����-�����
	int exp_;
	// ������� ����-�����
	int battle_exp_;
	// �������� �����
	int lost_exp_;
};

} // namespace DpsSystem

#endif // DPS_HPP_INCLUDED
