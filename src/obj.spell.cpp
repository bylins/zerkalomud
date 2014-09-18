// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#include <sstream>
#include <string>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "spells.h"
#include "utils.h"
#include "comm.h"
#include "screen.h"
#include "poison.hpp"
#include "char.hpp"
#include "db.h"
#include "room.hpp"

/**
* ������ ����� �� ������.
* \param send_message - true �������� ������� ��� ������ �����
*                     - false �� ��������
*/
void TimedSpell::clear(OBJ_DATA *obj, bool send_message)
{
	// ���� ���-�� ���� ������� �� ������� ��� ������ �������
	switch (spell_)
	{
	case SPELL_ACONITUM_POISON:
	case SPELL_SCOPOLIA_POISON:
	case SPELL_BELENA_POISON:
	case SPELL_DATURA_POISON:
		break;
	case SPELL_FLY:
	{
		const OBJ_DATA * const proto = read_object_mirror(GET_OBJ_VNUM(obj));
		if (!OBJ_FLAGGED(proto, ITEM_FLYING))
		{
			REMOVE_BIT(GET_OBJ_EXTRA(obj, ITEM_FLYING), ITEM_FLYING);
		}
		if (!OBJ_FLAGGED(proto, ITEM_SWIMMING))
		{
			REMOVE_BIT(GET_OBJ_EXTRA(obj, ITEM_SWIMMING), ITEM_SWIMMING);
		}
		break;
	}
	default:
		log("SYSERROR: %s:%d, spell_ = %d", __FILE__, __LINE__, spell_);
	} // switch

	// ������ ����������� ����
	if (send_message && (obj->carried_by || obj->worn_by))
	{
		CHAR_DATA *ch = obj->carried_by ? obj->carried_by : obj->worn_by;
		switch (spell_)
		{
		case SPELL_ACONITUM_POISON:
		case SPELL_SCOPOLIA_POISON:
		case SPELL_BELENA_POISON:
		case SPELL_DATURA_POISON:
			send_to_char(ch, "� %s ���������� ��������� �������� ���.\r\n",
					GET_OBJ_PNAME(obj, 1));
			break;
		case SPELL_FLY:
			send_to_char(ch, "���%s %s ��������%s ������ � �������.\r\n",
					GET_OBJ_VIS_SUF_7(obj, ch), GET_OBJ_PNAME(obj, 0),
					GET_OBJ_VIS_SUF_1(obj, ch));
			break;
		default:
			send_to_char(ch, "� %s ���-�� ������� (%d)... �� �������� �����!\r\n",
					GET_OBJ_PNAME(obj, 1), spell_);
		}
	}
	spell_ = -1;
	timer_ = -1;
}

/**
* ��� ���.������ �� ������ (��� � ������).
* \param time �� ������� = 1.
*/
void TimedSpell::dec_timer(OBJ_DATA *obj, int time)
{
	if (timer_ == -1 || !obj || time <= 0)
	{
		log("SYSERROR: func: %s, timer_ = %d, obj = %s, time = %d", __func__, timer_, obj ? "true" : "false", time);
		return;
	}

	timer_ -= time;
	if (timer_ <= 0)
	{
		this->clear(obj, true);
	}
}

/**
* ��� ���.����� � �������� �� ������.
*/
void TimedSpell::set(OBJ_DATA *obj, int spell, int time)
{
	if (!obj || spell < 0 || spell >= LAST_USED_SPELL || time < 0)
	{
		log("SYSERROR: func: %s, obj = %s, spell = %d, time = %d", __func__, obj ? "true" : "false", spell, time);
		return;
	}
	// ����� ��������� ��������
	if (!this->empty())
	{
		this->clear(obj, false);
	}
	timer_ = time;
	spell_ = spell;
}

/**
* ����� ����������� ������� ��� �� ����� ��� �������.
*/
std::string TimedSpell::diag_to_char(CHAR_DATA *ch)
{
	if (spell_ != -1 && ch)
	{
		std::stringstream out;
		switch (spell_)
		{
		case SPELL_ACONITUM_POISON:
		case SPELL_SCOPOLIA_POISON:
		case SPELL_BELENA_POISON:
		case SPELL_DATURA_POISON:
			out << CCGRN(ch, C_NRM) << "��������� " << get_poison_by_spell(spell_)
					<< " ��� " << timer_ << " " << desc_count(timer_, WHAT_MINu)
					<< "." << CCCYN(ch, C_NRM) << "\r\n";
			break;
		case SPELL_FLY:
			out << CCCYN(ch, C_NRM) << "�������� ���������� '�����' (" << time_format(timer_, true)
					<< ").\r\n" << CCCYN(ch, C_NRM);
			break;
		default:
			out << "�������� ����������� ���������� (" << spell_ << ")... �� ��������� �����!\r\n";
		}
		return out.str();
	}
	else
		return "";
}

bool TimedSpell::is_spell_poisoned() const
{
	if (check_poison(spell_))
	{
		return true;
	}
	else
		return false;
}

/**
* ��� ����� �������.
*/
bool TimedSpell::empty() const
{
	if (spell_ != -1)
		return false;
	else
		return true;
}

/**
* ���������� ������ � ����.
*/
std::string TimedSpell::print() const
{
	std::stringstream out;
	if (spell_ != -1)
		out << "TSpl: " << spell_ << " " << timer_ << "~\n";
	return out.str();
}

/**
* ���������� ����� ���������� (-1 � ������ ��� ����������).
*/
int TimedSpell::get() const
{
	return spell_;
}
