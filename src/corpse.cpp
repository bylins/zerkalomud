// $RCSfile$     $Date$     $Revision$
// Part of Bylins http://www.mud.ru

#include <sstream>
#include <iostream>
#include <set>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "corpse.hpp"
#include "db.h"
#include "utils.h"
#include "char.hpp"
#include "comm.h"
#include "handler.h"
#include "dg_scripts.h"
#include "im.h"
#include "room.hpp"
#include "pugixml.hpp"
#include "modify.h"
#include "house.h"

extern int max_npc_corpse_time, max_pc_corpse_time;
extern MobRaceListType mobraces_list;
extern void obj_to_corpse(OBJ_DATA *corpse, CHAR_DATA *ch, int rnum, bool setload);
extern int top_of_helpt;
extern struct help_index_element *help_table;
extern int hsort(const void *a, const void *b);
extern void go_boot_xhelp(void);

////////////////////////////////////////////////////////////////////////////////

namespace FullSetDrop
{

// ������ ����� �� ����
const char *CONFIG_FILE = LIB_MISC"full_set_drop.xml";
// ����������� ������� ���� ��� ������� � ����-������ �����
const int MIN_GROUP_MOB_LVL = 32;
// ���/���� ������ ����� ��� ������� ����-�����
const int MIN_SOLO_MOB_LVL = 25;
const int MAX_SOLO_MOB_LVL = 31;
// ����. ���-�� ���������� � ������ ����������� � ����������
const int MAX_GROUP_SIZE = 12;
const char *MOB_STAT_FILE = LIB_PLRSTUFF"mob_stat.xml";

enum { SOLO_MOB, GROUP_MOB, SOLO_ZONE, GROUP_ZONE };

// ������ ����-����� �� ���� (vnum)
std::list<int> group_obj_list;
// ������ ����-����� �� ���� (vnum)
std::list<int> solo_obj_list;

// ���������� �� �����: vnum ����, ������ ������, ���-�� �������
std::map<int, std::vector<int> > mob_stat;

struct MobNode
{
	MobNode() : vnum(-1), rnum(-1), miw(-1),
		type(-1), kill_stat(MAX_GROUP_SIZE + 1, 0) {};

	int vnum;
	int rnum;
	// ����.�.����
	int miw;
	// ��� ���� (��� ����������)
	std::string name;
	// ����/���� ���
	int type;
	// ���������� ������ �� ������� ������
	std::vector<int> kill_stat;
};

struct ZoneNode
{
	ZoneNode() : zone(-1), type(-1) {};

	// ���� ����
	int zone;
	// ��� ���� (����/���������), ����������� �� ���������� ������� �����
	int type;
	// ������ ����� � ����
	std::list<MobNode> mobs;
};

// ��������� ������ ��� ������ ������������ ���� ������ ����� ����
std::list<ZoneNode> mob_name_list;
// ��������� ������ ����� �� ���� ����-�����
std::list<ZoneNode> group_mob_list;
// ��������� ������ ����� �� ���� ����-�����
std::list<ZoneNode> solo_mob_list;

struct DropNode
{
	DropNode() : obj_rnum(0), chance(0) {};
	// ���� ������
	int obj_rnum;
	// ���� ����� (�������� * 10), chance �� 1000
	int chance;
};
// ��������� ������ ����� �� ����� (mob_rnum)
std::map<int, DropNode> drop_list;

struct HelpNode
{
	// ������ ������� ��� �������
	std::string alias_list;
	// ������ ��� ���� ��� ������ �������
	std::string title;
	// ������ ���������
	std::set<int> vnum_list;
};
// ������ ������������ ������� ����� ��� �������
std::vector<HelpNode> help_list;

/**
 * ������������� ������ ����� �� ����.
 */
void init_obj_list()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(CONFIG_FILE);
	if (!result)
	{
		snprintf(buf, MAX_STRING_LENGTH, "...%s", result.description());
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}
    pugi::xml_node node_list = doc.child("set_list");
    if (!node_list)
    {
		mudlog("...<set_list> read fail", CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
    }
	for (pugi::xml_node set_node = node_list.child("set");
		set_node; set_node = set_node.next_sibling("set"))
	{
		HelpNode node;

		node.alias_list = xmlparse_str(set_node, "help_alias");
		if (node.alias_list.empty())
		{
			mudlog("...bad set attributes (empty help_alias)",
				CMP, LVL_IMMORT, SYSLOG, TRUE);
			continue;
		}

		std::string type = xmlparse_str(set_node, "type");
		if (type.empty() || (type != "auto" && type != "manual"))
		{
			snprintf(buf, sizeof(buf),
				"...bad set attributes (type=%s)", type.c_str());
			mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
			continue;
		}

		if (type == "manual")
		{
			for (pugi::xml_node obj_node = set_node.child("obj");
				obj_node; obj_node = obj_node.next_sibling("obj"))
			{
				const int obj_vnum = xmlparse_int(obj_node, "vnum");
				if (real_object(obj_vnum) < 0)
				{
					snprintf(buf, sizeof(buf),
						"...bad obj_node attributes (vnum=%d)", obj_vnum);
					mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
					continue;
				}

				std::string list_type = xmlparse_str(obj_node, "list");
				if (list_type.empty()
					|| (list_type != "solo" && list_type != "group"))
				{
					snprintf(buf, sizeof(buf),
						"...bad manual obj attributes (list=%s, obj_vnum=%d)",
						list_type.c_str(), obj_vnum);
					mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
					continue;
				}

				if (list_type == "solo")
				{
					solo_obj_list.push_back(obj_vnum);
				}
				else
				{
					group_obj_list.push_back(obj_vnum);
				}
				// ������ ����� ��� �������
				node.vnum_list.insert(obj_vnum);
				// ��� ����
				if (node.title.empty())
				{
					for (id_to_set_info_map::const_iterator it = obj_data::set_table.begin(),
						iend = obj_data::set_table.end(); it != iend; ++it)
					{
						set_info::const_iterator k = it->second.find(obj_vnum);
						if (k != it->second.end())
						{
							node.title = it->second.get_name();
						}
					}
				}
			}
		}
		else
		{
			// ������ ���� ������������� �� ����.�����������
			std::multimap<int, int> set_sort_list;

			for (pugi::xml_node obj_node = set_node.child("obj");
				obj_node; obj_node = obj_node.next_sibling("obj"))
			{
				const int obj_vnum = xmlparse_int(obj_node, "vnum");
				if (real_object(obj_vnum) < 0)
				{
					snprintf(buf, sizeof(buf),
						"...bad obj_node attributes (vnum=%d)", obj_vnum);
					mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
					continue;
				}
				// ���������� ������ �����������
				for (id_to_set_info_map::const_iterator it = obj_data::set_table.begin(),
					iend = obj_data::set_table.end(); it != iend; ++it)
				{
					set_info::const_iterator k = it->second.find(obj_vnum);
					if (k != it->second.end() && !k->second.empty())
					{
						// ������� ��������� (������������) � ������ ���������
						set_sort_list.insert(
							std::make_pair(k->second.rbegin()->first, obj_vnum));
						// ������ ����� ��� �������
						node.vnum_list.insert(obj_vnum);
						// ��� ����
						if (node.title.empty())
						{
							node.title = it->second.get_name();
						}
					}
				}
			}
			// ������ �������� ����������� � ����-����, ������ � �����
			int num = 0, total_num = set_sort_list.size();
			for (std::multimap<int, int>::const_iterator i = set_sort_list.begin(),
				iend = set_sort_list.end(); i != iend; ++i, ++num)
			{
				if (num < total_num / 2)
				{
					solo_obj_list.push_back(i->second);
				}
				else
				{
					group_obj_list.push_back(i->second);
				}
			}
		}
		// ������ ������� � ����� ��� �������
		help_list.push_back(node);
	}
}

/**
 * ���� ���������� �� ��������� ����� � �������� �� ������ ������.
 */
void init_mob_stat()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(MOB_STAT_FILE);
	if (!result)
	{
		snprintf(buf, MAX_STRING_LENGTH, "...%s", result.description());
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}
    pugi::xml_node node_list = doc.child("mob_list");
    if (!node_list)
    {
		snprintf(buf, MAX_STRING_LENGTH, "...<mob_list> read fail");
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
    }
	for (pugi::xml_node mob_node = node_list.child("mob"); mob_node; mob_node = mob_node.next_sibling("mob"))
	{
		int mob_vnum = xmlparse_int(mob_node, "vnum");
		if (real_mobile(mob_vnum) < 0)
		{
			snprintf(buf, MAX_STRING_LENGTH, "...bad mob attributes (vnum=%d)", mob_vnum);
			mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
			continue;
		}

		std::vector<int> node(MAX_GROUP_SIZE + 1, 0);

		for (int k = 1; k <= MAX_GROUP_SIZE; ++k)
		{
			snprintf(buf, sizeof(buf), "n%d", k);
			pugi::xml_attribute attr = mob_node.attribute(buf);
			if (attr && attr.as_int() > 0)
			{
				node[k] = attr.as_int();
			}
		}

		mob_stat.insert(std::make_pair(mob_vnum, node));
	}
}

void add_to_zone_list(std::list<ZoneNode> &cont, MobNode &node)
{
	int zone = node.vnum/100;
	std::list<ZoneNode>::iterator k = std::find_if(cont.begin(), cont.end(),
		boost::bind(std::equal_to<int>(),
		boost::bind(&ZoneNode::zone, _1), zone));

	if (k != cont.end())
	{
		k->mobs.push_back(node);
	}
	else
	{
		ZoneNode tmp_node;
		tmp_node.zone = zone;
		tmp_node.mobs.push_back(node);
		cont.push_back(tmp_node);
	}
}

/**
 * ��������� ���������������� ������ ������ ����� ��� ������������
 * ������ ������������ �� ������ ������ ����� ����.
 * ������ � ����: ����, ���, ���
 * ����������: ���� ��� ���������
 *             ����� ����: ����-�����, ������ � ������, ������ � ������
 */
void init_mob_name_list()
{
	std::set<int> bad_zones;

	for (ClanListType::const_iterator clan = Clan::ClanList.begin();
		clan != Clan::ClanList.end(); ++clan)
	{
		bad_zones.insert((*clan)->GetRent()/100);
	}

	int curr_zone = 0;
	bool rent = false, mail = false, banker = false;
	for (std::vector<ROOM_DATA *>::const_iterator i = world.begin(),
		iend = world.end(); i != iend; ++i)
	{
		if (curr_zone != zone_table[(*i)->zone].number)
		{
			if (rent && mail && banker)
			{
				bad_zones.insert(curr_zone);
			}
			rent = false;
			mail = false;
			banker = false;
			curr_zone = zone_table[(*i)->zone].number;
		}
		for (CHAR_DATA *ch = (*i)->people; ch; ch = ch->next_in_room)
		{
			if (IS_RENTKEEPER(ch))
			{
				rent = true;
			}
			else if (IS_POSTKEEPER(ch))
			{
				mail = true;
			}
			else if (IS_BANKKEEPER(ch))
			{
				banker = true;
			}
		}
	}

	for (std::map<int, std::vector<int> >::iterator i = mob_stat.begin(),
		iend = mob_stat.end(); i != iend; ++i)
	{
		const int rnum = real_mobile(i->first);
		const int zone = i->first/100;
		std::set<int>::const_iterator k = bad_zones.find(zone);

		if (rnum < 0
			|| zone < 100
			|| k != bad_zones.end())
		{
			continue;
		}

		MobNode node;
		node.vnum = i->first;
		node.rnum = rnum;
		node.name = mob_proto[rnum].get_name();
		node.kill_stat = i->second;

		add_to_zone_list(mob_name_list, node);
	}
}

/**
 * ������ � ����: ���
 */
void init_zone_type()
{
	for (std::list<ZoneNode>::iterator i = mob_name_list.begin(),
		iend = mob_name_list.end(); i != iend; ++i)
	{
		int killed_solo = 0;
		for (std::list<MobNode>::iterator k = i->mobs.begin(),
			kend = i->mobs.end(); k != kend; ++k)
		{
			int group_cnt = 0;
			for (int cnt = 2; cnt <= MAX_GROUP_SIZE; ++cnt)
			{
				group_cnt += k->kill_stat[cnt];
			}
			if (k->kill_stat[1] > group_cnt)
			{
				++killed_solo;
			}
		}
		if (killed_solo >= i->mobs.size() * 0.8)
		{
			i->type = SOLO_ZONE;
		}
		else
		{
			i->type = GROUP_ZONE;
		}
	}
}

/**
 * ������ � ����: ���
 */
void init_mob_type()
{
	for (std::list<ZoneNode>::iterator i = mob_name_list.begin(),
		iend = mob_name_list.end(); i != iend; ++i)
	{
		for (std::list<MobNode>::iterator k = i->mobs.begin(),
			kend = i->mobs.end(); k != kend; ++k)
		{
			int group_cnt = 0;
			for (int cnt = 2; cnt <= MAX_GROUP_SIZE; ++cnt)
			{
				group_cnt += k->kill_stat[cnt];
			}
			if (i->type == SOLO_ZONE && k->kill_stat[1] > group_cnt)
			{
				k->type = SOLO_MOB;
			}
			else if (i->type == GROUP_ZONE && k->kill_stat[1] < group_cnt)
			{
				k->type = GROUP_MOB;
			}
		}
	}
}

/**
 * ������ ����.�.���� �� ������ ���� ������ ����.
 * ���� ���� ����� ��������� - ������� ������������ ��������.
 */
int calc_max_in_world(int mob_rnum)
{
	int max_in_world = 0;
	for (int i = 0; i <= top_of_zone_table; ++i)
	{
		for (int cmd_no = 0; zone_table[i].cmd[cmd_no].command != 'S'; ++cmd_no)
		{
			if (zone_table[i].cmd[cmd_no].command == 'M'
				&& zone_table[i].cmd[cmd_no].arg1 == mob_rnum)
			{
				max_in_world = MAX(max_in_world, zone_table[i].cmd[cmd_no].arg2);
			}
		}
	}
	return max_in_world;
}

/**
 * � ����� ������� �������� ���������� ��������� ����
 * ����������: ���� � ������������ ������ ������ ����� ����,
 *             ���� ������ 1 ����.�.����
 *             ���� ���� ���������� ������ ��� ������� ����
 *             ���� � ������� ������ ����������/������� ����
 * ������ � ����: ����.�.����
 */
void filter_dupe_names()
{
	for (std::list<ZoneNode>::iterator it = mob_name_list.begin(),
		iend = mob_name_list.end(); it != iend; ++it)
	{
		std::list<MobNode> tmp_list;
		// ��������� (������� ��������) ���������� ����� � ������� �������
		for (std::list<MobNode>::iterator k = it->mobs.begin(),
			kend = it->mobs.end(); k != kend; ++k)
		{
			bool good = true;

			for (std::list<MobNode>::iterator l = it->mobs.begin(),
				lend = it->mobs.end(); l != lend; ++l)
			{
				if (k->vnum != l->vnum && k->name == l->name)
				{
					good = false;
					break;
				}
			}
			// ���� ������ ���������� ����
			k->miw = calc_max_in_world(k->rnum);
			if (!good || k->miw != 1 || k->type == -1)
			{
				continue;
			}
			// �������� �� ����� ����
			if (k->type == SOLO_MOB
				&& (mob_proto[k->rnum].get_level() < MIN_SOLO_MOB_LVL
					|| mob_proto[k->rnum].get_level() > MAX_SOLO_MOB_LVL))
			{
				continue;
			}
			if (k->type == GROUP_MOB
				&& mob_proto[k->rnum].get_level() < MIN_GROUP_MOB_LVL)
			{
				continue;
			}
			// ����� ������������ ����, ���� ��� �����
			const CHAR_DATA *mob = &mob_proto[k->rnum];
			if (MOB_FLAGGED(mob, MOB_LIKE_FULLMOON)
				|| MOB_FLAGGED(mob, MOB_LIKE_WINTER)
				|| MOB_FLAGGED(mob, MOB_LIKE_SPRING)
				|| MOB_FLAGGED(mob, MOB_LIKE_SUMMER)
				|| MOB_FLAGGED(mob, MOB_LIKE_AUTUMN)
				|| mob->get_exp() <= 0)
			{
				continue;
			}

			tmp_list.push_back(*k);
		}
		it->mobs = tmp_list;
	}
}

/**
 * ����������: ������ ���� � ����� � ����� ������� ������� �����
 * ��� ����� ������������ ������������� �� ������ �����
 */
void filter_extra_mobs(int total, int type)
{
	std::list<ZoneNode> &cont = (type == GROUP_MOB) ? group_mob_list : solo_mob_list;
	const int obj_total = (type == GROUP_MOB) ? group_obj_list.size() : solo_obj_list.size();
	// ��������� ������ ����� � ����� ����������� �����
	int num_del = total - obj_total;
	while (num_del > 0)
	{
		unsigned max_num = 0;
		for (std::list<ZoneNode>::iterator it = cont.begin(),
			iend = cont.end(); it != iend; ++it)
		{
			if (it->mobs.size() > max_num)
			{
				max_num = it->mobs.size();
			}
		}
		for (std::list<ZoneNode>::iterator it = cont.begin(),
			iend = cont.end(); it != iend; ++it)
		{
			if (it->mobs.size() >= max_num)
			{
				std::list<MobNode>::iterator l = it->mobs.begin();
				std::advance(l, number(0, it->mobs.size() - 1));
				it->mobs.erase(l);
				if (it->mobs.empty())
				{
					cont.erase(it);
				}
				--num_del;
				break;
			}
		}
	}
}

/**
 * ���������� ������ ������ ����� �� �����- � ����- ������.
 */
void split_mob_name_list()
{
	int total_group_mobs = 0, total_solo_mobs = 0;

	for (std::list<ZoneNode>::iterator it = mob_name_list.begin(),
		iend = mob_name_list.end(); it != iend; ++it)
	{
		for (std::list<MobNode>::iterator k = it->mobs.begin(),
			kend = it->mobs.end(); k != kend; ++k)
		{
			if (k->type == GROUP_MOB)
			{
				add_to_zone_list(group_mob_list, *k);
				++total_group_mobs;
			}
			else if (k->type == SOLO_MOB)
			{
				add_to_zone_list(solo_mob_list, *k);
				++total_solo_mobs;
			}
		}
	}
	mob_name_list.clear();

	filter_extra_mobs(total_group_mobs, GROUP_MOB);
	filter_extra_mobs(total_solo_mobs, SOLO_MOB);
}

int calc_drop_chance(std::list<MobNode>::iterator &mob, int obj_rnum)
{
	int chance = 0;

	if (mob->type == GROUP_MOB)
	{
		// ��� ������� ������������ �� ���-�� �������
		int max_kill = 0;
		int num1 = 2;
		// � ��� ����� ���-�� ���������
		for (int i = 2; i <= MAX_GROUP_SIZE; ++i)
		{
			if (mob->kill_stat[i] > max_kill)
			{
				max_kill = mob->kill_stat[i];
				num1 = i;
			}
		}
		int max_kill2 = 0;
		int num2 = 2;
		for (int i = 2; i <= MAX_GROUP_SIZE; ++i)
		{
			if (i != num1
				&& mob->kill_stat[i] > max_kill2)
			{
				max_kill2 = mob->kill_stat[i];
				num2 = i;
			}
		}
		// � ������� ����� ����
		double num_mod = (num1 + num2) / 2.0;
		// 5.8% .. 14.8%
		chance = (40 + num_mod * 9) / mob->miw;
	}
	else
	{
		// 3% .. 4.2%
		int mob_lvl = mob_proto[mob->rnum].get_level();
		int lvl_mod = MIN(MAX(0, mob_lvl - MIN_SOLO_MOB_LVL), 6);
		chance = (30 + lvl_mod * 2) / mob->miw;
		// ���� ���� � ���� ����������� ���� �� ����
		const OBJ_DATA *obj = obj_proto[obj_rnum];
		if (!SetSystem::is_big_set(obj))
		{
			chance *= 1.75;
		}
	}

	return chance;
}

/**
 * ��������� ��������� ������� ����� � �����.
 */
void init_drop_table(int type)
{
	std::list<ZoneNode> &mob_list = (type == GROUP_MOB) ? group_mob_list : solo_mob_list;
	std::list<int> &obj_list = (type == GROUP_MOB) ? group_obj_list : solo_obj_list;

	while(!obj_list.empty() && !mob_list.empty())
	{
		std::list<int>::iterator it = obj_list.begin();
		std::advance(it, number(0, obj_list.size() - 1));
		const int obj_rnum = real_object(*it);

		std::list<ZoneNode>::iterator k = mob_list.begin();
		std::advance(k, number(0, mob_list.size() - 1));

		// �� ���� ������� ���� ����� ����� �������� ����������
		// ��������, �� ��� ����� ���������� ��������, ��� �� �����
		// ���������� ��� ������ ������ ����� ������ ����������
		if (k->mobs.empty())
		{
			mob_list.erase(k);
			continue;
		}

		std::list<MobNode>::iterator l = k->mobs.begin();
		std::advance(l, number(0, k->mobs.size() - 1));

		DropNode tmp_node;
		tmp_node.obj_rnum = obj_rnum;
		tmp_node.chance = calc_drop_chance(l, obj_rnum);

		drop_list.insert(std::make_pair(l->rnum, tmp_node));

		obj_list.erase(it);
		k->mobs.erase(l);
	}

	obj_list.clear();
	mob_list.clear();
}

void add_to_help_table(const std::vector<std::string> &key_list, const std::string &text)
{
	if (key_list.empty() || text.empty())
	{
		snprintf(buf, sizeof(buf), "add_to_help_table error: key_list or text empty");
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}

	int num = key_list.size();
	RECREATE(help_table, struct help_index_element, top_of_helpt + num + 1);

	struct help_index_element el;
	el.min_level = 0;
	el.duplicate = 0;
	el.entry = str_dup(text.c_str());

	for (std::vector<std::string>::const_iterator i = key_list.begin(),
		iend = key_list.end(); i != iend; ++i)
	{
		el.keyword = str_dup((*i).c_str());
		help_table[++top_of_helpt] = el;
		++el.duplicate;
	}
	// ������� ��������� � �����, �� ���� ��� � ����� �����
	qsort(help_table, top_of_helpt + 1, sizeof(struct help_index_element), hsort);
}

/**
 * ��������� ��������� � �������.
 */
void init_xhelp()
{
	std::stringstream out;
	out << "������ ���������, ����������� � ������� ��������������� ���������:\r\n";

	for (id_to_set_info_map::const_iterator it = obj_data::set_table.begin(),
		iend = obj_data::set_table.end(); it != iend; ++it)
	{
		bool print_set_name = true;
		for (set_info::const_iterator obj = it->second.begin(),
			iend = it->second.end(); obj != iend; ++obj)
		{
			for (std::map<int, DropNode>::iterator k = drop_list.begin(),
					kend = drop_list.end(); k != kend; ++k)
			{
				if (obj_index[k->second.obj_rnum].vnum == obj->first)
				{
					if (print_set_name)
					{
						out << "\r\n" << it->second.get_name().c_str() << "\r\n";
						print_set_name = false;
					}
					out.precision(1);
					out << "   " << GET_OBJ_PNAME(obj_proto[k->second.obj_rnum], 0)
						<< " - " << mob_proto[k->first].get_name()
						<< " (" << zone_table[mob_index[k->first].zone].name << ")"
						<< " - " << std::fixed << k->second.chance / 10.0 << "%\r\n";
					break;
				}
			}
		}
	}

	std::vector<std::string> help_list;
	help_list.push_back("����");
	help_list.push_back("����");
	help_list.push_back("���������������");
	add_to_help_table(help_list, out.str());
}

void init_xhelp_full()
{
	for (std::vector<HelpNode>::const_iterator i = help_list.begin(),
		iend = help_list.end(); i != iend; ++i)
	{
		std::stringstream out;
		out << "\r\n" << i->title << "\r\n";

		for (std::set<int>::const_iterator l = i->vnum_list.begin(),
			lend = i->vnum_list.end(); l != lend; ++l)
		{
			for (std::map<int, DropNode>::iterator k = drop_list.begin(),
					kend = drop_list.end(); k != kend; ++k)
			{
				if (obj_index[k->second.obj_rnum].vnum == *l)
				{
					out.precision(1);
					out << "   " << GET_OBJ_PNAME(obj_proto[k->second.obj_rnum], 0)
						<< " - " << mob_proto[k->first].get_name()
						<< " (" << zone_table[mob_index[k->first].zone].name << ")"
						<< " - " << std::fixed << k->second.chance / 10.0 << "%\r\n";
					break;
				}
			}
		}

		std::vector<std::string> str_list;
		boost::split(str_list, i->alias_list, boost::is_any_of(", "));
		add_to_help_table(str_list, out.str());
	}
}

/**
 * ������ ������� �����, ��� ������� ���������� �� ��������� �����.
 * \param zone_vnum - ���� �� �������, ������� �� ���������� �����
 * ��� ��������� ���� � �������� ������ ������ ��� ��� ���
 */
void reload(int zone_vnum)
{
	group_obj_list.clear();
	solo_obj_list.clear();
	mob_name_list.clear();
	group_mob_list.clear();
	solo_mob_list.clear();
	drop_list.clear();
	help_list.clear();

	if (zone_vnum > 0)
	{
		for (std::map<int, std::vector<int> >::iterator i = mob_stat.begin(),
			iend = mob_stat.end(); i != iend; /* ����� */)
		{
			if (i->first/100 == zone_vnum)
			{
				mob_stat.erase(i++);
			}
			else
			{
				++i;
			}
		}
		save_mob_stat();
	}

	init_obj_list();

	init_mob_name_list();
	init_zone_type();
	init_mob_type();
	filter_dupe_names();
	split_mob_name_list();

	init_drop_table(SOLO_MOB);
	init_drop_table(GROUP_MOB);

	// ������� ���� ��������� ����������
	// init_xhelp() � init_xhelp_full() ��������� ��� ��
	go_boot_xhelp();
}

void init()
{
	init_obj_list();

	init_mob_stat();
	init_mob_name_list();
	init_zone_type();
	init_mob_type();
	filter_dupe_names();
	split_mob_name_list();

	init_drop_table(SOLO_MOB);
	init_drop_table(GROUP_MOB);

	init_xhelp();
	init_xhelp_full();
}

void show_stats(CHAR_DATA *ch)
{
	send_to_char(ch, "  ����� � ���������� ��� �����: %d\r\n", mob_stat.size());
}

/**
 * \return ���� ������ ��� -1 ���� ������� ������
 */
int check_mob(int mob_rnum)
{
	std::map<int, DropNode>::iterator it = drop_list.find(mob_rnum);
	if (it != drop_list.end())
	{
		int num = obj_index[it->second.obj_rnum].stored
			+ obj_index[it->second.obj_rnum].number;
		if (num < GET_OBJ_MIW(obj_proto[it->second.obj_rnum])
			&& number(0, 1000) <= it->second.chance)
		{
			return it->second.obj_rnum;
		}
	}
	return -1;
}

void renumber_obj_rnum(int rnum)
{
	for (std::map<int, DropNode>::iterator it = drop_list.begin(),
		iend = drop_list.end(); it != iend; ++it)
	{
		if (it->second.obj_rnum >= rnum)
		{
			it->second.obj_rnum += 1;
		}
	}
}

void save_mob_stat()
{
	pugi::xml_document doc;
	doc.append_child().set_name("mob_list");
	pugi::xml_node mob_list = doc.child("mob_list");

	for (std::map<int, std::vector<int> >::const_iterator i = mob_stat.begin(),
		iend = mob_stat.end(); i != iend; ++i)
	{
		pugi::xml_node mob_node = mob_list.append_child();
		mob_node.set_name("mob");
		mob_node.append_attribute("vnum") = i->first;

		for (int k = 1; k <= MAX_GROUP_SIZE; ++k)
		{
			if (i->second[k] > 0)
			{
				snprintf(buf, sizeof(buf), "n%d", k);
				mob_node.append_attribute(buf) = i->second[k];
			}
		}
	}
	doc.save_file(MOB_STAT_FILE);
}

void add_mob_stat(CHAR_DATA *mob, int members)
{
	if (members < 1 || members > MAX_GROUP_SIZE)
	{
		snprintf(buf, sizeof(buf), "add_mob_stat error: mob_vnum=%d, members=%d",
			GET_MOB_VNUM(mob), members);
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}
	std::map<int, std::vector<int> >::iterator i = mob_stat.find(GET_MOB_VNUM(mob));
	if (i != mob_stat.end())
	{
		i->second[members] += 1;
	}
	else
	{
		std::vector<int> node(MAX_GROUP_SIZE + 1, 0);
		node[members] += 1;
		mob_stat.insert(std::make_pair(GET_MOB_VNUM(mob), node));
	}
}

void show_zone_stat(CHAR_DATA *ch, int zone_vnum)
{
	std::stringstream out;
	out << "���������� ������� ����� � ���� ����� " << zone_vnum << "\r\n";
	out << "   vnum : ������ ������ = ���-�� �������\r\n\r\n";

	for (std::map<int, std::vector<int> >::const_iterator i = mob_stat.begin(),
		iend = mob_stat.end(); i != iend; ++i)
	{
		if (i->first/100 == zone_vnum)
		{
			out << i->first << " :";
			for (int k = 1; k <= MAX_GROUP_SIZE; ++k)
			{
				if (i->second[k] > 0)
				{
					out << " n" << k << "=" << i->second[k];
				}
			}
			out << "\r\n";
		}
	}

	send_to_char(out.str().c_str(), ch);
}


} // namespace FullSetDrop

////////////////////////////////////////////////////////////////////////////////

namespace GlobalDrop
{

typedef std::map<int /* vnum */, int /* rnum*/> OlistType;

struct global_drop
{
	global_drop() : vnum(0), mob_lvl(0), max_mob_lvl(0), prc(0), mobs(0), rnum(-1) {};
	int vnum; // ���� ������, ���� ����� ������������� - ���� ������ ������
	int mob_lvl;  // ��� ����� ����
	int max_mob_lvl; // ����. ����� ���� (0 - �� �����������)
	int prc;  // ����� ����� (������ � �����)
	int mobs; // ����� ���������� �����
	int rnum; // ���� ������, ���� vnum ��������
	// ������ ������ � ����� ������ (��������� ������ ���������)
	// ��� ������ �� ������ ����������� ���� ��������� � ����
	OlistType olist;
};

typedef std::vector<global_drop> DropListType;
DropListType drop_list;

const char *CONFIG_FILE = LIB_MISC"global_drop.xml";
const char *STAT_FILE = LIB_PLRSTUFF"global_drop.tmp";

void init()
{
	// �� ������ �������
	drop_list.clear();

	// ������
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(CONFIG_FILE);
	if (!result)
	{
		snprintf(buf, MAX_STRING_LENGTH, "...%s", result.description());
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
	}
    pugi::xml_node node_list = doc.child("globaldrop");
    if (!node_list)
    {
		snprintf(buf, MAX_STRING_LENGTH, "...<globaldrop> read fail");
		mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
		return;
    }
	for (pugi::xml_node node = node_list.child("drop"); node; node = node.next_sibling("drop"))
	{
		int obj_vnum = xmlparse_int(node, "obj_vnum");
		int mob_lvl = xmlparse_int(node, "mob_lvl");
		int max_mob_lvl = xmlparse_int(node, "max_mob_lvl");
		int chance = xmlparse_int(node, "chance");

		if (obj_vnum == -1 || mob_lvl <= 0 || chance <= 0 || max_mob_lvl < 0)
		{
			snprintf(buf, MAX_STRING_LENGTH,
					"...bad drop attributes (obj_vnum=%d, mob_lvl=%d, chance=%d, max_mob_lvl=%d)",
					obj_vnum, mob_lvl, chance, max_mob_lvl);
			mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
			return;
		}

		global_drop tmp_node;
		tmp_node.vnum = obj_vnum;
		tmp_node.mob_lvl = mob_lvl;
		tmp_node.max_mob_lvl = max_mob_lvl;
		tmp_node.prc = chance;

		if (obj_vnum >= 0)
		{
			int obj_rnum = real_object(obj_vnum);
			if (obj_rnum < 0)
			{
				snprintf(buf, MAX_STRING_LENGTH, "...incorrect obj_vnum=%d", obj_vnum);
				mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
				return;
			}
			tmp_node.rnum = obj_rnum;
		}
		else
		{
			// ������ ������ � ������ ������
			for (pugi::xml_node item = node.child("obj"); item; item = item.next_sibling("obj"))
			{
				int item_vnum = xmlparse_int(item, "vnum");
				if (item_vnum <= 0)
				{
					snprintf(buf, MAX_STRING_LENGTH,
						"...bad shop attributes (item_vnum=%d)", item_vnum);
					mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
					return;
				}
				// ��������� ������
				int item_rnum = real_object(item_vnum);
				if (item_rnum < 0)
				{
					snprintf(buf, MAX_STRING_LENGTH, "...incorrect item_vnum=%d", item_vnum);
					mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
					return;
				}
				tmp_node.olist[item_vnum] = item_rnum;
			}
			if (tmp_node.olist.empty())
			{
				snprintf(buf, MAX_STRING_LENGTH, "...item list empty (obj_vnum=%d)", obj_vnum);
				mudlog(buf, CMP, LVL_IMMORT, SYSLOG, TRUE);
				return;
			}
		}
		drop_list.push_back(tmp_node);
	}

	// ����������� ����� �� ������ ����� �����
	std::ifstream file(STAT_FILE);
	if (!file.is_open())
	{
		log("SYSERROR: �� ������� ������� ���� �� ������: %s", STAT_FILE);
		return;
	}
	int vnum, mobs;
	while (file >> vnum >> mobs)
	{
		for (DropListType::iterator i = drop_list.begin(); i != drop_list.end(); ++i)
		{
			if (i->vnum == vnum)
			{
				i->mobs = mobs;
			}
		}
	}
}

void save()
{
	std::ofstream file(STAT_FILE);
	if (!file.is_open())
	{
		log("SYSERROR: �� ������� ������� ���� �� ������: %s", STAT_FILE);
		return;
	}
	for (DropListType::iterator i = drop_list.begin(); i != drop_list.end(); ++i)
	{
		file << i->vnum << " " << i->mobs << "\n";
	}
}

/**
 * ����� ������ ��� ����� �� ������ � ������ ���� � ����.
 * \return rnum
 */
int get_obj_to_drop(DropListType::iterator &i)
{
	std::vector<int> tmp_list;
	for (OlistType::iterator k = i->olist.begin(), kend = i->olist.end(); k != kend; ++k)
	{
		if (obj_index[k->second].stored + obj_index[k->second].number < GET_OBJ_MIW(obj_proto[k->second]))
		{
			tmp_list.push_back(k->second);
		}
	}
	if (!tmp_list.empty())
	{
		int rnd = number(0, tmp_list.size() - 1);
		return tmp_list.at(rnd);
	}
	return -1;
}

/**
 * ���������� ���� � ����� �������� ����������.
 * ���� vnum �������������, �� ����� ���� �� ������ ������ �����.
 */
bool check_mob(OBJ_DATA *corpse, CHAR_DATA *ch)
{
	for (DropListType::iterator i = drop_list.begin(), iend = drop_list.end(); i != iend; ++i)
	{
		if (GET_LEVEL(ch) >= i->mob_lvl
			&& (!i->max_mob_lvl || GET_LEVEL(ch) <= i->max_mob_lvl))
		{
			++(i->mobs);
			if (i->mobs >= i->prc)
			{
				int obj_rnum = i->vnum > 0 ? i->rnum : get_obj_to_drop(i);
				if (obj_rnum >= 0)
				{
					obj_to_corpse(corpse, ch, obj_rnum, false);
				}
				i->mobs = 0;
				return true;
			}
		}
	}
	return false;
}

void renumber_obj_rnum(int rnum)
{
	for (DropListType::iterator i = drop_list.begin(); i != drop_list.end(); ++i)
	{
		if (i->vnum >= 0 && i->rnum >= rnum)
		{
			i->rnum += 1;
		}
		else if (i->vnum < 0)
		{
			for (OlistType::iterator k = i->olist.begin(), kend = i->olist.end();
				k != kend; ++k)
			{
				if (k->second >= rnum)
				{
					k->second += 1;
				}
			}
		}
	}
}

} // namespace GlobalDrop

void make_arena_corpse(CHAR_DATA * ch, CHAR_DATA * killer)
{
	OBJ_DATA *corpse;
	EXTRA_DESCR_DATA *exdesc;

	corpse = create_obj();
	GET_OBJ_SEX(corpse) = SEX_POLY;

	sprintf(buf2, "������� %s ����� �� �����.", GET_PAD(ch, 1));
	corpse->description = str_dup(buf2);

	sprintf(buf2, "������� %s", GET_PAD(ch, 1));
	corpse->short_description = str_dup(buf2);

	sprintf(buf2, "������� %s", GET_PAD(ch, 1));
	corpse->PNames[0] = str_dup(buf2);
	corpse->name = str_dup(buf2);

	sprintf(buf2, "�������� %s", GET_PAD(ch, 1));
	corpse->PNames[1] = str_dup(buf2);
	sprintf(buf2, "�������� %s", GET_PAD(ch, 1));
	corpse->PNames[2] = str_dup(buf2);
	sprintf(buf2, "������� %s", GET_PAD(ch, 1));
	corpse->PNames[3] = str_dup(buf2);
	sprintf(buf2, "��������� %s", GET_PAD(ch, 1));
	corpse->PNames[4] = str_dup(buf2);
	sprintf(buf2, "�������� %s", GET_PAD(ch, 1));
	corpse->PNames[5] = str_dup(buf2);

	GET_OBJ_TYPE(corpse) = ITEM_CONTAINER;
	GET_OBJ_WEAR(corpse) = ITEM_WEAR_TAKE;
	SET_BIT(GET_OBJ_EXTRA(corpse, ITEM_NODONATE), ITEM_NODONATE);
	GET_OBJ_VAL(corpse, 0) = 0;	/* You can't store stuff in a corpse */
	GET_OBJ_VAL(corpse, 2) = IS_NPC(ch) ? GET_MOB_VNUM(ch) : -1;
	GET_OBJ_VAL(corpse, 3) = 1;	/* corpse identifier */
	GET_OBJ_WEIGHT(corpse) = GET_WEIGHT(ch);
	GET_OBJ_RENT(corpse) = 100000;
	corpse->set_timer(max_pc_corpse_time * 2);
	CREATE(exdesc, EXTRA_DESCR_DATA, 1);
	exdesc->keyword = str_dup(corpse->PNames[0]);	// ���������
	if (killer)
		sprintf(buf, "����%s �� ����� %s.\r\n", GET_CH_SUF_6(ch), GET_PAD(killer, 4));
	else
		sprintf(buf, "����%s �� �����.\r\n", GET_CH_SUF_4(ch));
	exdesc->description = str_dup(buf);	// ���������
	exdesc->next = corpse->ex_description;
	corpse->ex_description = exdesc;
	obj_to_room(corpse, IN_ROOM(ch));
}

OBJ_DATA *make_corpse(CHAR_DATA * ch, CHAR_DATA * killer)
{
	OBJ_DATA *corpse, *o;
	OBJ_DATA *money;
	int i;

	if (IS_NPC(ch) && MOB_FLAGGED(ch, MOB_CORPSE))
		return NULL;

	sprintf(buf2, "���� %s", GET_PAD(ch, 1));
	corpse = create_obj(buf2);

	GET_OBJ_SEX(corpse) = SEX_MALE;

	sprintf(buf2, "���� %s ����� �����.", GET_PAD(ch, 1));
	corpse->description = str_dup(buf2);

	sprintf(buf2, "���� %s", GET_PAD(ch, 1));
	corpse->short_description = str_dup(buf2);

	sprintf(buf2, "���� %s", GET_PAD(ch, 1));
	corpse->PNames[0] = str_dup(buf2);
	sprintf(buf2, "����� %s", GET_PAD(ch, 1));
	corpse->PNames[1] = str_dup(buf2);
	sprintf(buf2, "����� %s", GET_PAD(ch, 1));
	corpse->PNames[2] = str_dup(buf2);
	sprintf(buf2, "���� %s", GET_PAD(ch, 1));
	corpse->PNames[3] = str_dup(buf2);
	sprintf(buf2, "������ %s", GET_PAD(ch, 1));
	corpse->PNames[4] = str_dup(buf2);
	sprintf(buf2, "����� %s", GET_PAD(ch, 1));
	corpse->PNames[5] = str_dup(buf2);

	GET_OBJ_TYPE(corpse) = ITEM_CONTAINER;
	GET_OBJ_WEAR(corpse) = ITEM_WEAR_TAKE;
	SET_BIT(GET_OBJ_EXTRA(corpse, ITEM_NODONATE), ITEM_NODONATE);
	SET_BIT(GET_OBJ_EXTRA(corpse, ITEM_NOSELL), ITEM_NOSELL);
	GET_OBJ_VAL(corpse, 0) = 0;	/* You can't store stuff in a corpse */
	GET_OBJ_VAL(corpse, 2) = IS_NPC(ch) ? GET_MOB_VNUM(ch) : -1;
	GET_OBJ_VAL(corpse, 3) = 1;	/* corpse identifier */
	GET_OBJ_RENT(corpse) = 100000;

	if (IS_NPC(ch))
	{
		corpse->set_timer(max_npc_corpse_time * 2);
	}
	else
	{
		corpse->set_timer(max_pc_corpse_time * 2);
	}

	/* transfer character's equipment to the corpse */
	for (i = 0; i < NUM_WEARS; i++)
		if (GET_EQ(ch, i))
		{
			remove_otrigger(GET_EQ(ch, i), ch);
			obj_to_char(unequip_char(ch, i), ch);
		}
	// ������� ��� ������ ����� ���� ��� �������� ����
	GET_OBJ_WEIGHT(corpse) = GET_WEIGHT(ch) + IS_CARRYING_W(ch);

	/* transfer character's inventory to the corpse */
	corpse->contains = ch->carrying;
	for (o = corpse->contains; o != NULL; o = o->next_content)
	{
		o->in_obj = corpse;
	}
	object_list_new_owner(corpse, NULL);


	/* transfer gold */
	if (ch->get_gold() > 0)  	/* following 'if' clause added to fix gold duplication loophole */
	{
		if (IS_NPC(ch) || (!IS_NPC(ch) && ch->desc))
		{
			money = create_money(ch->get_gold());
			obj_to_obj(money, corpse);
		}
		ch->set_gold(0);
	}

	ch->carrying = NULL;
	IS_CARRYING_N(ch) = 0;
	IS_CARRYING_W(ch) = 0;

//Polud ����������� �������� ������ � ���� (����) ����

	if (IS_NPC(ch) && GET_RACE(ch)>NPC_RACE_BASIC && !ROOM_FLAGGED(IN_ROOM(ch), ROOM_HOUSE))
	{
		MobRaceListType::iterator it = mobraces_list.find(GET_RACE(ch));
		if (it != mobraces_list.end())
		{
			int *ingr_to_load_list, j;
			int num_inrgs = it->second->ingrlist.size();
			CREATE(ingr_to_load_list, int, num_inrgs * 2 + 1);
			for (j=0; j < num_inrgs; j++)
			{
				ingr_to_load_list[2*j] = im_get_idx_by_type(it->second->ingrlist[j].imtype);
				ingr_to_load_list[2*j+1] = it->second->ingrlist[j].prob[GET_LEVEL(ch)-1];
				ingr_to_load_list[2*j+1] |= (GET_LEVEL(ch) << 16);
			}
			ingr_to_load_list[2*j] = -1;
			im_make_corpse(corpse, ingr_to_load_list, 1000);
		}
		else
			if (mob_proto[GET_MOB_RNUM(ch)].ing_list)
				im_make_corpse(corpse, mob_proto[GET_MOB_RNUM(ch)].ing_list, 100);
	}

	// �������� ������ �� �����. - ���������� � raw_kill
	/*  if (IS_NPC (ch))
	    dl_load_obj (corpse, ch); */

	// ���� ������ ���� ������� ��� �� �����(� �������� �� � ��) �� ���� �������� �� � ������ � � ��������� � ��������� �������
	if(IS_CHARMICE(ch) && !MOB_FLAGGED(ch, MOB_CORPSE) && ch->master && ((killer && PRF_FLAGGED(killer, PRF_EXECUTOR))
		|| (ROOM_FLAGGED(IN_ROOM(ch), ROOM_ARENA) && !RENTABLE(ch->master))))
	{
		obj_to_char(corpse, ch->master);
		return NULL;
	}
	else
	{
		obj_to_room(corpse, IN_ROOM(ch));
		return corpse;
	}
}
