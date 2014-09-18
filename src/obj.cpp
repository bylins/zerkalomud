// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#include <sstream>
#include "obj.hpp"
#include "utils.h"
#include "pk.h"
#include "celebrates.hpp"
#include "cache.hpp"
#include "screen.h"
#include "comm.h"
#include "char.hpp"
#include "db.h"
#include "constants.h"

extern void print_obj_affects(CHAR_DATA *ch, const obj_affected_type &affect);
extern void tascii(int *pointer, int num_planes, char *ascii);

id_to_set_info_map obj_data::set_table;

obj_data::obj_data() :
	uid(0),
	item_number(NOTHING),
	in_room(NOWHERE),
	name(NULL),
	description(NULL),
	short_description(NULL),
	action_description(NULL),
	ex_description(NULL),
	carried_by(NULL),
	worn_by(NULL),
	worn_on(NOWHERE),
	in_obj(NULL),
	contains(NULL),
	id(0),
	proto_script(NULL),
	script(NULL),
	next_content(NULL),
	next(NULL),
	room_was_in(NOWHERE),
	max_in_world(0),
	skills(NULL),
	serial_num_(0),
	timer_(0),
	mort_req_(0)
{
	caching::obj_cache.add(this);
	memset(&obj_flags, 0, sizeof(obj_flag_data));

	for (int i = 0; i < 6; i++)
		PNames[i] = NULL;
}

obj_data::obj_data(const obj_data& other)
{
	*this = other;
	caching::obj_cache.add(this);
}

obj_data::~obj_data()
{
	caching::obj_cache.remove(this);
	bloody::remove_obj(this); //��. ����������� � ��������� BloodyInfo �� pk.cpp
	//weak_ptr ��� �� ��� ������ � ����
	Celebrates::remove_from_obj_lists(this->uid);
}

int obj_data::get_serial_num()
{
	return serial_num_;
}

void obj_data::set_serial_num(int num)
{
	serial_num_ = num;
}

const std::string obj_data::activate_obj(const activation& __act)
{
	if (item_number >= 0)
	{
		obj_flags.affects = __act.get_affects();
		for (int i = 0; i < MAX_OBJ_AFFECT; i++)
			affected[i] = __act.get_affected_i(i);

		int weight = __act.get_weight();
		if (weight > 0)
			obj_flags.weight = weight;

		if (obj_flags.type_flag == ITEM_WEAPON)
		{
			int nsides, ndices;
			__act.get_dices(ndices, nsides);
			// ���� ����� �������� �� ��, ��������������� �� ��� ���������.
			if (ndices > 0 && nsides > 0)
			{
				obj_flags.value[1] = ndices;
				obj_flags.value[2] = nsides;
			}
		}

		// ���������� ������.
		if (__act.has_skills())
		{
			// � ���� �������� � �������� skills ��������� �� ���� � ��� ��
			// ������. � � ����������. ������� ��� ���� ��������� �����,
			// ���� ��� ������� "������������" ����� ��� ����� �������.
			// ������, ������������� � ����, �������� ������ ������ ��������.
			skills = new std::map<int, int>;
			__act.get_skills(*skills);
		}

		return __act.get_actmsg() + "\n" + __act.get_room_actmsg();
	}
	else
		return "\n";
}

const std::string obj_data::deactivate_obj(const activation& __act)
{
	if (item_number >= 0)
	{
		obj_flags.affects = obj_proto[item_number]->obj_flags.affects;
		for (int i = 0; i < MAX_OBJ_AFFECT; i++)
			affected[i] = obj_proto[item_number]->affected[i];

		obj_flags.weight = obj_proto[item_number]->obj_flags.weight;

		if (obj_flags.type_flag == ITEM_WEAPON)
		{
			obj_flags.value[1] = obj_proto[item_number]->obj_flags.value[1];
			obj_flags.value[2] = obj_proto[item_number]->obj_flags.value[2];
		}

		// ������������ ������.
		if (__act.has_skills())
		{
			// ��� ��������� �� ��������� ����� ������ � ��������. ���
			// ����� ����� �������.
			delete skills;
			skills = obj_proto[item_number]->skills;
		}

		return __act.get_deactmsg() + "\n" + __act.get_room_deactmsg();
	}
	else
		return "\n";
}

void obj_data::set_skill(int skill_num, int percent)
{
	if (skills)
	{
		std::map<int, int>::iterator skill = skills->find(skill_num);
		if (skill == skills->end())
		{
			if (percent != 0)
				skills->insert(std::make_pair(skill_num, percent));
		}
		else
		{
			if (percent != 0)
				skill->second = percent;
			else
				skills->erase(skill);
		}
	}
	else
	{
		if (percent != 0)
		{
			skills = new std::map<int, int>;
			skills->insert(std::make_pair(skill_num, percent));
		}
	}
}

int obj_data::get_skill(int skill_num) const
{
	if (skills)
	{
		std::map<int, int>::iterator skill = skills->find(skill_num);
		if (skill != skills->end())
			return skill->second;
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

/**
 * @warning ��������������, ��� __out_skills.empty() == true.
 */
void obj_data::get_skills(std::map<int, int>& out_skills) const
{
	if (skills)
		out_skills.insert(skills->begin(), skills->end());
}

bool obj_data::has_skills() const
{
	if (skills)
		return !skills->empty();
	else
		return false;
}

void obj_data::set_timer(int timer)
{
	timer_ = MAX(0, timer);
}

int obj_data::get_timer() const
{
	return timer_;
}

/**
* �������� �������� ������ (��� ������ ����������� ����� ������� �� ����).
* ������ ������� ����� ������ ��������� ������ �� ���������� �������.
* \param time �� ������� 1.
*/
void obj_data::dec_timer(int time)
{
	if (time > 0)
	{
		timer_ -= time;
		if (!timed_spell.empty())
		{
			timed_spell.dec_timer(this, time);
		}
	}
}

int obj_data::get_mort_req() const
{
	return mort_req_;
}

void obj_data::set_mort_req(int param)
{
	mort_req_ = MAX(0, param);
}

namespace ObjSystem
{

bool is_armor_type(const OBJ_DATA *obj)
{
	switch GET_OBJ_TYPE(obj)
	{
	case ITEM_ARMOR:
	case ITEM_ARMOR_LIGHT:
	case ITEM_ARMOR_MEDIAN:
	case ITEM_ARMOR_HEAVY:
		return true;
	}
	return false;
}

} // namespace ObjSystem

////////////////////////////////////////////////////////////////////////////////

AcquiredAffects::AcquiredAffects()
	: type_(0), weight_(0)
{
	affects_flags_ = clear_flags;
	extra_flags_ = clear_flags;
	no_flags_ = clear_flags;
}

AcquiredAffects::AcquiredAffects(OBJ_DATA *obj)
{
	name_ = GET_OBJ_PNAME(obj, 4) ? GET_OBJ_PNAME(obj, 4) : "<null>";
	type_ = ACQUIRED_ENCHANT;

	for (int i = 0; i < MAX_OBJ_AFFECT; i++)
	{
		if (obj->affected[i].location != APPLY_NONE
			&& obj->affected[i].modifier != 0)
		{
			affected_.push_back(obj->affected[i]);
		}
	}

	affects_flags_ = GET_OBJ_AFFECTS(obj);
	extra_flags_ = obj->obj_flags.extra_flags;
	REMOVE_BIT(GET_FLAG(extra_flags_, ITEM_TICKTIMER), ITEM_TICKTIMER);
	no_flags_ = GET_OBJ_NO(obj);
	weight_ = GET_OBJ_VAL(obj, 0);
}

void AcquiredAffects::print(CHAR_DATA *ch) const
{
	send_to_char(ch, "\r\n���������� %s :%s\r\n", name_.c_str(), CCCYN(ch, C_NRM));

	for (std::vector<obj_affected_type>::const_iterator i = affected_.begin(),
		iend = affected_.end(); i != iend; ++i)
	{
		print_obj_affects(ch, *i);
	}

	if (sprintbits(affects_flags_, weapon_affects, buf2, ","))
	{
		send_to_char(ch, "%s   �������: %s%s\r\n",
				CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
	}

	if (sprintbits(extra_flags_, extra_bits, buf2, ","))
	{
		send_to_char(ch, "%s   �����������: %s%s\r\n",
				CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
	}

	if (sprintbits(no_flags_, no_bits, buf2, ","))
	{
		send_to_char(ch, "%s   ��������: %s%s\r\n",
				CCCYN(ch, C_NRM), buf2, CCNRM(ch, C_NRM));
	}

	if (weight_ != 0)
	{
		send_to_char(ch, "%s   %s ��� �������� �� %d%s\r\n", CCCYN(ch, C_NRM),
				weight_ > 0 ? "�����������" : "���������",
				abs(weight_), CCNRM(ch, C_NRM));
	}
}

void AcquiredAffects::apply_to_obj(OBJ_DATA *obj) const
{
	for (std::vector<obj_affected_type>::const_iterator i = affected_.begin(),
		iend = affected_.end(); i != iend; ++i)
	{
		for (int k = 0; k < MAX_OBJ_AFFECT; k++)
		{
			if (obj->affected[k].location == i->location)
			{
				obj->affected[k].modifier += i->modifier;
				break;
			}
			else if (obj->affected[k].location == APPLY_NONE)
			{
				obj->affected[k].location = i->location;
				obj->affected[k].modifier = i->modifier;
				break;
			}
		}
	}

	GET_FLAG(GET_OBJ_AFFECTS(obj), INT_ZERRO) |= GET_FLAG(affects_flags_, INT_ZERRO);
	GET_FLAG(GET_OBJ_AFFECTS(obj), INT_ONE) |= GET_FLAG(affects_flags_, INT_ONE);
	GET_FLAG(GET_OBJ_AFFECTS(obj), INT_TWO) |= GET_FLAG(affects_flags_, INT_TWO);
	GET_FLAG(GET_OBJ_AFFECTS(obj), INT_THREE) |= GET_FLAG(affects_flags_, INT_THREE);

	GET_FLAG(obj->obj_flags.extra_flags, INT_ZERRO) |= GET_FLAG(extra_flags_, INT_ZERRO);
	GET_FLAG(obj->obj_flags.extra_flags, INT_ONE) |= GET_FLAG(extra_flags_, INT_ONE);
	GET_FLAG(obj->obj_flags.extra_flags, INT_TWO) |= GET_FLAG(extra_flags_, INT_TWO);
	GET_FLAG(obj->obj_flags.extra_flags, INT_THREE) |= GET_FLAG(extra_flags_, INT_THREE);

	GET_FLAG(obj->obj_flags.no_flag, INT_ZERRO) |= GET_FLAG(no_flags_, INT_ZERRO);
	GET_FLAG(obj->obj_flags.no_flag, INT_ONE) |= GET_FLAG(no_flags_, INT_ONE);
	GET_FLAG(obj->obj_flags.no_flag, INT_TWO) |= GET_FLAG(no_flags_, INT_TWO);
	GET_FLAG(obj->obj_flags.no_flag, INT_THREE) |= GET_FLAG(no_flags_, INT_THREE);

	GET_OBJ_WEIGHT(obj) += weight_;
	if (GET_OBJ_WEIGHT(obj) <= 0)
	{
		GET_OBJ_WEIGHT(obj) = 1;
	}
}

int AcquiredAffects::get_type() const
{
	return type_;
}

std::string AcquiredAffects::print_to_file() const
{
	std::stringstream out;
	out << "Ench: I " << name_ << "\n" << " T "<< type_ << "\n";

	for (std::vector<obj_affected_type>::const_iterator i = affected_.begin(),
		iend = affected_.end(); i != iend; ++i)
	{
		out << " A " << i->location << " " << i->modifier << "\n";
	}

	*buf = '\0';
	tascii((int *) &affects_flags_, 4, buf);
	out << " F " << buf << "\n";

	*buf = '\0';
	tascii((int *) &extra_flags_, 4, buf);
	out << " E " << buf << "\n";

	*buf = '\0';
	tascii((int *) &no_flags_, 4, buf);
	out << " N " << buf << "\n";

	out << " W " << weight_ << "\n~\n";

	return out.str();
}

////////////////////////////////////////////////////////////////////////////////
