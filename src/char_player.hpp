// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#ifndef CHAR_PLAYER_HPP_INCLUDED
#define CHAR_PLAYER_HPP_INCLUDED

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "quested.hpp"
#include "mobmax.hpp"
#include "remember.hpp"
#include "char.hpp"
#include "dps.hpp"
#include "morph.hpp"

// ���-�� ����������� ��������� ������ � �����
const int START_STATS_TOTAL = 6;

// ��� ����������� ������
const int OFFTOP_MESSAGE = 0;
const int TOTAL_DISPOSABLE_NUM = 1;
// TOTAL_DISPOSABLE_NUM �� �������� ������

class Player : public Character
{
public:
	Player();
	int get_pfilepos() const;
	void set_pfilepos(int pfilepos);

	room_rnum get_was_in_room() const;
	void set_was_in_room(room_rnum was_in_room);

	std::string const & get_passwd() const;
	void set_passwd(std::string const & passwd);

	room_rnum get_from_room() const;
	void set_from_room(room_rnum was_in_room);

	void remort();
	void reset();

	int get_start_stat(int num);
	void set_start_stat(int stat_num, int number);

	void set_last_tell(const char *text);
	std::string const & get_last_tell();

	void set_answer_id(int id);
	int get_answer_id() const;

	// ������� �� CharRemember
	void remember_add(std::string text, int flag);
	std::string remember_get(int flag) const;
	bool remember_set_num(unsigned int num);
	unsigned int remember_get_num() const;

	// ������� �� Quested
	void quested_add(CHAR_DATA *ch, int vnum, char *text);
	bool quested_remove(int vnum);
	bool quested_get(int vnum) const;
	std::string quested_get_text(int vnum) const;
	std::string quested_print() const;
	void quested_save(FILE *saved) const; // TODO �� ������

	// ������� �� MobMax
	int mobmax_get(int vnum) const;
	void mobmax_add(CHAR_DATA *ch, int vnum, int count, int level);
	void mobmax_load(CHAR_DATA *ch, int vnum, int count, int level);
	void mobmax_remove(int vnum);
	void mobmax_save(FILE *saved) const; // TODO �� ������

	// ������� �� Dps
	void dps_add_dmg(int type, int dmg, int over_dmg, CHAR_DATA *ch = 0);
	void dps_clear(int type);
	void dps_print_stats(CHAR_DATA *coder = 0);
	void dps_print_group_stats(CHAR_DATA *ch, CHAR_DATA *coder = 0);
	void dps_set(DpsSystem::Dps *dps);
	void dps_copy(CHAR_DATA *ch);
	void dps_end_round(int type, CHAR_DATA *ch = 0);
	void dps_add_exp(int exp, bool battle = false);

	void save_char();
	int load_char_ascii(const char *name, bool reboot = 0);

	bool get_disposable_flag(int num);
	void set_disposable_flag(int num);

	// ���� ������ ��� ������� �� motion_, �� ����� ���� ��������
	bool is_active() const;
	void set_motion(bool flag);

private:
	// ���������� ����� � ����� �����-����� (�� ����� �����, �� ������ ������ ������ �� ���)
	// TODO: ������ ��� ����� ���������� ������ ����������� ������ �� ����� ��� ������ ���� � �.�. �����, ����������
	// get_ptable_by_name ��� find_name (������������ ���� ������) � ������ ������ �� ��/���, ���� ��� ����� ���-����
	int pfilepos_;
	// �������, � ������� ��� ��� �� ����, ��� ��� ��������� � �������� (linkdrop)
	room_rnum was_in_room_;
	// ��� ������
	std::string passwd_;
	// �������, ��� ��� ��� �� ������ char_from_room (was_in_room_ ��� ��� ������������ �� �� ������)
	// � ������ ������ ���� ����� ��� �������� ���� �� �� ��� ����� �� ����� ����� ��������, �� ����� � ��� ����� �����������
	room_rnum from_room_;
	// ��������� �����
	boost::array<int, START_STATS_TOTAL> start_stats_;
	// ���������
	CharRemember remember_;
	// ����� ����������� ������� � ������������ �������
	Quested quested_;
	// ������� �� ��������� �����
	MobMax mobmax_;
	// ��������� ��������� ������ (�� �����)
	std::string last_tell_;
	// id ���������� ��������� (��� ������)
	long answer_id_;
	// '�����' ������������ � ������, ���� ��� �����
	DpsSystem::Dps dps_;
	// ����������� �����
	std::bitset<TOTAL_DISPOSABLE_NUM> disposable_flags_;
	// false, ���� ��� ������� ��� ���������� ����� check_idling � ���� �� �������� � �����
	bool motion_;
};

#endif // CHAR_PLAYER_HPP_INCLUDED
