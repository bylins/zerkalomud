#ifndef CELEBRATES_HPP_INCLUDED
#define CELEBRATES_HPP_INCLUDED
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace Celebrates
{

const int CLEAN_PERIOD = 10;

typedef std::vector<int> TrigList;
struct ToLoad;

typedef boost::shared_ptr<ToLoad> LoadPtr;
typedef std::vector<LoadPtr> LoadList;

struct ToLoad
{
	TrigList triggers;
	int vnum;
	int max;
	LoadList objects;
};

typedef std::map<int, TrigList> AttachList; //mob vnum, ����� ���������
typedef std::map<int, AttachList> AttachZonList; //zone_num, ������ ��� ������

struct CelebrateRoom
{
	int vnum;
	TrigList triggers;
	LoadList mobs;
	LoadList objects;
};

typedef boost::shared_ptr<CelebrateRoom> CelebrateRoomPtr;
typedef std::vector<CelebrateRoomPtr> CelebrateRoomsList;
typedef std::map<int, CelebrateRoomsList> CelebrateZonList;//����� ����, ������ ������
typedef std::map<long, CHAR_DATA *> CelebrateMobs;
typedef std::map<long, OBJ_DATA *> CelebrateObjs;

struct CelebrateData
{
	CelebrateData() : is_clean(true) {};
	std::string name;
	bool is_clean;
	CelebrateZonList rooms;
	AttachZonList mobsToAttach;
	AttachZonList objsToAttach;
};

typedef boost::shared_ptr<CelebrateData> CelebrateDataPtr;

struct CelebrateDay
{
	CelebrateDay() : last(false), start_at(0), finish_at(24) {};
	bool last;
	int start_at;
	int finish_at;
	CelebrateDataPtr celebrate;
};

typedef boost::shared_ptr<CelebrateDay> CelebrateDayPtr;
typedef std::map<int, CelebrateDayPtr> CelebrateList; //����� ��� � ����, ��������


CelebrateDataPtr get_mono_celebrate();
CelebrateDataPtr get_poly_celebrate();
CelebrateDataPtr get_real_celebrate();

std::string get_name_mono(int day);
std::string get_name_poly(int day);
std::string get_name_real(int day);

int get_mud_day();
int get_real_day();

void load();
void sanitize();

void add_mob_to_attach_list(long, CHAR_DATA *);
void add_mob_to_load_list(long, CHAR_DATA *);
void add_obj_to_attach_list(long, OBJ_DATA *);
void add_obj_to_load_list(long, OBJ_DATA *);

void remove_from_obj_lists(long uid);
void remove_from_mob_lists(long uid);

};

#endif //CELEBRATES_HPP_INCLUDED
