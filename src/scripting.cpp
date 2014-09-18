// Copyright (c) 2011 Posvist
// Part of Bylins http://www.mud.ru

#include <string>
#include <vector>
#include <boost/python.hpp>
#include "utils.h"
#include "comm.h"
#include "char.hpp"
#include "interpreter.h"
#include "obj.hpp"
#include "db.h"
#include "cache.hpp"
#include "spells.h"
#include "handler.h"
#include "constants.h"
#include "modify.h"
#include "scripting.hpp"

#define DEFINE_CONSTANT(X) scope().attr(#X) = X

using namespace boost::python;
namespace py=boost::python;
using namespace scripting;

std::string parse_python_exception();

template <class t>
inline t pass_through(const t& o) { return o; }

//�����-������� ������ �������� ������� ������� �����
template <class t, class nextfunc, class getfunc, class result_t=t>
class CIterator
{
public:
	typedef t object_t;
	CIterator(const object_t& o, const nextfunc nfunc, const getfunc gfunc):
		_object(o), _next_func(nfunc), _get_func(gfunc) { }
	result_t next();
	private:
	object_t _object;
	const nextfunc _next_func;
	const getfunc _get_func;
};

py::handle<PyObject> ObjectDoesNotExist;

template <class t>
class Wrapper
{
public:
typedef t wrapped_t;
typedef caching::Cache<wrapped_t*> cache_t;
Wrapper(t* obj, cache_t& cache_):
cache(cache_), id(cache_.get_id(obj))
	{
	}

Wrapper(const Wrapper& other):
	cache(other.cache), id(other.id) { }

Wrapper& operator=(const Wrapper& other)
{
	id = other.id;
	return *this;
}

operator t*() const
{
	t* r = cache.get_obj(id);
	if (!r)
		raise_error();
	return r;
}

operator bool() { return id!=0; }

void raise_error() const
{
	PyErr_SetString(ObjectDoesNotExist.get(), "Object you are referencing doesn't exist anymore");
	throw_error_already_set();
}

struct Ensurer
{
Ensurer(const Wrapper& w): ptr(w) { }

t* operator->() { return ptr; }
operator t*() { return ptr; }
t* ptr;
};
private:
	cache_t& cache;
	caching::id_t id;
};

class CharacterWrapper: public Wrapper<Character>
{
public:
CharacterWrapper(Character* ch):Wrapper<Character>(ch, caching::character_cache) { }
const char* get_name() const
{
	Ensurer ch(*this);
	return ch->get_name();
}

void set_name(const char* name)
{
	Ensurer ch(*this);
	ch->set_name(name);
}

void send(const char* msg)
{
	Ensurer ch(*this);
	send_to_char(msg, (Character*)ch);
}

void _page_string(const string& msg)
{
	Ensurer ch(*this);
	page_string(ch->desc, msg);
}

void close_console()
{
	Ensurer ch(*this);
	if (STATE(ch->desc) == CON_CONSOLE)
	{
		//ch->desc->console.reset();
		STATE(ch->desc) = CON_PLAYING;
	}
}

void act_on_char(const char *str, bool hide_invisible, const OBJ_DATA * obj, const CharacterWrapper& victim, int type)
{
	Ensurer ch(*this);
	Ensurer v(victim);
	act(str, hide_invisible, ch, obj, (Character*)victim, type);
}

void act_on_obj(const char *str, bool hide_invisible, const OBJ_DATA * obj, const OBJ_DATA *victim, int type)
{
	Ensurer ch(*this);
	act(str, hide_invisible, ch, obj, victim, type);
}

py::list get_followers()
{
	Ensurer ch(*this);
	py::list result;
	for (follow_type* i=ch->followers; i; i=i->next)
		result.append(CharacterWrapper(i->follower));
	return result;
}

bool is_immortal()
{
	Ensurer ch(*this);
	return IS_IMMORTAL(ch);
}

bool is_impl()
{
	Ensurer ch(*this);
	return IS_IMPL(ch);
}

bool is_NPC()
{
	Ensurer ch(*this);
	return IS_NPC(ch);
}

const char* get_long_descr() const
{
	Ensurer ch(*this);
	return ch->get_long_descr();
}

void set_long_descr(const char* v)
{
	Ensurer ch(*this);
	ch->set_long_descr(v);
}

const char* get_description() const
{
	Ensurer ch(*this);
	return ch->get_description();
}

void set_description(const char* v)
{
	Ensurer ch(*this);
	ch->set_description(v);
}

const short get_class() const
{
	Ensurer ch(*this);
	return ch->get_class();
}

void set_class(const short v)
{
	Ensurer ch(*this);
	ch->set_class(v);
}

const short get_level() const
{
	Ensurer ch(*this);
	return ch->get_level();
}

void set_level(const short v)
{
	Ensurer ch(*this);
	ch->set_level(v);
}

const long get_exp() const
{
	Ensurer ch(*this);
	return ch->get_exp();
}

void set_exp(const long v)
{
	Ensurer ch(*this);
	gain_exp(ch, v-ch->get_exp(), 0);
}

const long get_gold() const
{
	Ensurer ch(*this);
	return ch->get_gold();
}

void set_gold(const long v)
{
	Ensurer ch(*this);
	ch->set_gold(v);
}

const long get_bank() const
{
	Ensurer ch(*this);
	return ch->get_bank();
}

void set_bank(const long v)
{
	Ensurer ch(*this);
	ch->set_bank(v);
}

const int get_str() const
{
	Ensurer ch(*this);
	return ch->get_str();
}

void set_str(const int v)
{
	Ensurer ch(*this);
	ch->set_str(v);
}

const int get_dex() const
{
	Ensurer ch(*this);
	return ch->get_dex();
}

void set_dex(const int v)
{
	Ensurer ch(*this);
	ch->set_dex(v);
}

const int get_con() const
{
	Ensurer ch(*this);
	return ch->get_con();
}

void set_con(const int v)
{
	Ensurer ch(*this);
	ch->set_con(v);
}

const int get_wis() const
{
	Ensurer ch(*this);
	return ch->get_wis();
}

void set_wis(const int v)
{
	Ensurer ch(*this);
	ch->set_wis(v);
}

const int get_int() const
{
	Ensurer ch(*this);
	return ch->get_int();
}

void set_int(const int v)
{
	Ensurer ch(*this);
	ch->set_int(v);
}

const int get_cha() const
{
	Ensurer ch(*this);
	return ch->get_cha();
}

void set_cha(const int v)
{
	Ensurer ch(*this);
	ch->set_cha(v);
}

const byte get_sex() const
{
	Ensurer ch(*this);
	return ch->get_sex();
}

void set_sex(const byte v)
{
	Ensurer ch(*this);
	ch->set_sex(v);
}

const ubyte get_weight() const
{
	Ensurer ch(*this);
	return ch->get_weight();
}

void set_weight(const ubyte v)
{
	Ensurer ch(*this);
	ch->set_weight(v);
}

const ubyte get_height() const
{
	Ensurer ch(*this);
	return ch->get_height();
}

void set_height(const ubyte v)
{
	Ensurer ch(*this);
	ch->set_height(v);
}

const ubyte get_religion() const
{
	Ensurer ch(*this);
	return ch->get_religion();
}

void set_religion(const ubyte v)
{
	Ensurer ch(*this);
	ch->set_religion(v);
}

const ubyte get_kin() const
{
	Ensurer ch(*this);
	return ch->get_kin();
}

void set_kin(const ubyte v)
{
	Ensurer ch(*this);
	ch->set_kin(v);
}

const ubyte get_race() const
{
	Ensurer ch(*this);
	return ch->get_race();
}

void set_race(const ubyte v)
{
	Ensurer ch(*this);
	ch->set_race(v);
}

const int get_hit() const
{
	Ensurer ch(*this);
	return ch->get_hit();
}

void set_hit(const int v)
{
	Ensurer ch(*this);
	ch->set_hit(v);
}

const int get_max_hit() const
{
	Ensurer ch(*this);
	return ch->get_max_hit();
}

void set_max_hit(const int v)
{
	Ensurer ch(*this);
	ch->set_max_hit(v);
}

const sh_int get_move() const
{
	Ensurer ch(*this);
	return ch->get_move();
}

void set_move(const sh_int v)
{
	Ensurer ch(*this);
	ch->set_move(v);
}

const sh_int get_max_move() const
{
	Ensurer ch(*this);
	return ch->get_max_move();
}

void set_max_move(const sh_int v)
{
	Ensurer ch(*this);
	ch->set_max_move(v);
}

const char* get_pad(const int v) const
{
	Ensurer ch(*this);
	return ch->get_pad(v);
}

void set_pad(const int pad, const char* v)
{
	Ensurer ch(*this);
	ch->set_pad(pad, v);
}

void remove_gold(const long num, const bool log=true)
{
	Ensurer ch(*this);
	ch->remove_gold(num, log);
}

void remove_bank(const long num, const bool log=true)
{
	Ensurer ch(*this);
	ch->remove_bank(num, log);
}

void remove_both_gold(const long num, const bool log=true)
{
	Ensurer ch(*this);
	ch->remove_both_gold(num, log);
}

void add_gold(const long num, const bool log=true)
{
	Ensurer ch(*this);
	ch->add_gold(num, log);
}

void add_bank(const long num, const bool log=true)
{
	Ensurer ch(*this);
	ch->add_bank(num, log);
}

const long get_total_gold() const
{
	Ensurer ch(*this);
	return ch->get_total_gold();
	}

const int get_uid() const
{
	Ensurer ch(*this);
	return ch->get_uid();
}

const short get_remort() const
{
	Ensurer ch(*this);
	return ch->get_remort();
}

int get_skill(int skill_num) const
{
	Ensurer ch(*this);
	return ch->get_skill(skill_num);
}

void set_skill(int skill_num, int percent)
{
	Ensurer ch(*this);
	ch->set_skill(skill_num, percent);
}

void clear_skills()
{
	Ensurer ch(*this);
	ch->clear_skills();
}

int get_skills_count() const
{
	Ensurer ch(*this);
	return ch->get_skills_count();
}

int get_equipped_skill(int skill_num) const
{
	Ensurer ch(*this);
	return ch->get_equipped_skill(skill_num);
}

int get_trained_skill(int skill_num) const
{
	Ensurer ch(*this);
	return ch->get_trained_skill(skill_num);
}

ubyte get_spell(int spell_num) const
{
	Ensurer ch(*this);
	return GET_SPELL_TYPE(ch, spell_num);
}

void set_spell(int spell_num, ubyte value)
{
	Ensurer ch(*this);
	GET_SPELL_TYPE(ch, spell_num) = value;
}

void interpret(char* command)
{
	Ensurer ch(*this);
	command_interpreter(ch, command);
}

mob_rnum get_nr() const
{
	Ensurer ch(*this);
	return ch->nr;
}

void set_nr(const mob_rnum nr)
{
	Ensurer ch(*this);
	ch->nr = nr;
}

room_rnum get_in_room() const
{
	Ensurer ch(*this);
	return ch->in_room;
}

void set_in_room(room_rnum in_room)
{
	Ensurer ch(*this);
	char_from_room(ch);
	char_to_room(ch, in_room);
	check_horse(ch);
}

bool is_affected_by_spell(int spell_num) const
{
	Ensurer ch(*this);
	return affected_by_spell(ch, spell_num);
}

void add_affect(affect_data& af)
{
	Ensurer ch(*this);
	affect_to_char(ch, &af);
}

CharacterWrapper get_vis(const char* name, int where) const
{
	Ensurer ch(*this);
	Character* r = get_char_vis(ch, name, where);
	if (!r)
	{
		PyErr_SetString(PyExc_ValueError, "Character not found");
		throw_error_already_set();
	}
	return r;
}

void restore()
{
	Ensurer vict(*this);
	GET_HIT(vict) = GET_REAL_MAX_HIT(vict);
	GET_MOVE(vict) = GET_REAL_MAX_MOVE(vict);
	if (IS_MANA_CASTER(vict))
	{
		GET_MANA_STORED(vict) = GET_MAX_MANA(vict);
	}
	else
	{
		GET_MEM_COMPLETED(vict) = GET_MEM_TOTAL(vict);
	}
}

void quested_add(const CharacterWrapper& ch, int vnum, char *text)
{
	Ensurer self(*this);
	self->quested_add((Character*)Ensurer(ch), vnum, text);
}

bool quested_remove(int vnum)
{
	Ensurer ch(*this);
	return ch->quested_remove(vnum);
}

string quested_get_text(int vnum)
{
	Ensurer ch(*this);
	return ch->quested_get_text(vnum);
}

string quested_print() const
{
	Ensurer ch(*this);
	return ch->quested_print();
}
};

CharacterWrapper create_mob_from_proto(mob_rnum proto_rnum, bool is_virtual=true)
{
	return read_mobile(proto_rnum, is_virtual);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(create_mob_from_proto_overloads, create_mob_from_proto, 1, 2)

struct CharacterListWrapper
{
typedef CharacterWrapper (*func_type)(const CharacterWrapper& );
typedef CIterator<CharacterWrapper, func_type, func_type> iterator;

static CharacterWrapper my_next_func(const CharacterWrapper& w)
{
	CharacterWrapper::Ensurer ch(w);
	if (ch->next)
		return ch->next;
	PyErr_SetString(PyExc_StopIteration, "End of list.");
	throw_error_already_set();
	return CharacterWrapper(NULL); //to prevent compiler warning
}

static iterator* iter() { return new iterator(character_list, my_next_func, pass_through); }
};

CharacterWrapper get_mob_proto(const mob_rnum rnum)
{
	if (rnum>=0 && rnum <= top_of_mobt)
		return &mob_proto[rnum];
		PyErr_SetString(PyExc_ValueError, "mob rnum is out of range");
	throw_error_already_set();
	return CharacterWrapper(NULL);
	}

Character* character_get_master(Character* ch)
{
	return ch->master;
}

void character_set_master(Character* ch, Character* master)
{
	ch->master = master;
}

string get_spell_type_str(const affect_data& af)
{
	return spell_info[af.type].name;
}

string get_location_str(const affect_data& af)
{
	char buf[MAX_STRING_LENGTH];
	sprinttype(af.location, apply_types, buf);
	return buf;
}

string get_bitvector_str(const affect_data& af)
{
	char buf[MAX_STRING_LENGTH];
	sprintbitwd(af.bitvector, affected_bits, buf, ", ");
	return buf;
}

typedef boost::array<obj_affected_type, MAX_OBJ_AFFECT> affected_t;

template<class T, int N>
struct _arrayN
{
    typedef boost::array<T,N> arrayN;

    static T get(arrayN const& self, int idx)
    {
      if( !(0<=idx && idx<N ))
	  {
      PyErr_SetString(PyExc_KeyError,"index out of range");
      throw_error_already_set();
	  }
	  return self[idx];
    }

    static boost::python::list getslice(arrayN const& self, int a,int b)
    {
      if( !(a>=0 && a<N && b>0 && b<=N) )
      {
       PyErr_SetString(PyExc_KeyError,"index out of range");
       throw_error_already_set();
      }
      if(b>N){b=N;}
      if(a<0){a=0;}
      boost::python::list t;
      for(int i=a;i<b;++i)
          t.append(self[i]);
       return t;
    }
    static void setslice(arrayN& self, int a,int b,boost::python::object& v)
    {
      if( !(a>=0 && a<N && b>0 && b<=N) )
      {
       PyErr_SetString(PyExc_KeyError,"index out of range");
       throw_error_already_set();
      }
      if(b>N){b=N;}
      if(a<0){a=0;}

      for(int i=a;i<b;++i)
          self[i]=extract<T>(v[i]);
    }
        static void set(arrayN& self, int idx, const T val) { self[idx]=val; }

    static boost::python::list values(arrayN const& self)
    {
        boost::python::list t;
        for(int i=0;i<N;++i)
            t.append(self[i]);
        return t;
    }
    static int size(arrayN const & self)
    {
     return N;
    }
};

class ObjWrapper: public Wrapper<obj_data>
{
public:
ObjWrapper(obj_data* obj):Wrapper<obj_data>(obj, caching::obj_cache) { }

string get_name() const
{
	Ensurer obj(*this);
	return obj->name;
}

void set_name(const char* name)
{
	Ensurer obj(*this);
	obj_rnum i = GET_OBJ_RNUM(obj);
	if (i == -1 || obj->name != obj_proto[i]->name)
		if (obj->name) free(obj->name);
	obj->name = str_dup(name);
}

string get_description() const
{
	Ensurer obj(*this);
	return obj->description;
}

void set_description(const char* description)
{
	Ensurer obj(*this);
	obj_rnum i = GET_OBJ_RNUM(obj);
	if (i == -1 || obj->description != obj_proto[i]->description)
		if (obj->description) free(obj->description);
	obj->description = str_dup(description);
}
string get_short_description() const
{
	Ensurer obj(*this);
	return obj->short_description;
}

void set_short_description(const char* short_description)
{
	Ensurer obj(*this);
	obj_rnum i = GET_OBJ_RNUM(obj);
	if (i == -1 || obj->short_description != obj_proto[i]->short_description)
		if (obj->short_description) free(obj->short_description);
	obj->short_description = str_dup(short_description);
}
string get_action_description() const
{
	Ensurer obj(*this);
	return obj->action_description;
}

void set_action_description(const char* action_description)
{
	Ensurer obj(*this);
	obj_rnum i = GET_OBJ_RNUM(obj);
	if (i == -1 || obj->action_description != obj_proto[i]->action_description)
		if (obj->action_description) free(obj->action_description);
	obj->action_description = str_dup(action_description);
}
string get_pad(const unsigned pad) const
{
	Ensurer obj(*this);
	if (pad < 6)
		return obj->PNames[pad];
	else return "";
}

void set_pad(const unsigned pad, const char* s)
{
	if (pad >= 6) return;
	Ensurer obj(*this);
	obj_rnum i = GET_OBJ_RNUM(obj);
	if (i == -1 || obj->PNames[pad] != obj_proto[i]->PNames[pad])
		if (obj->PNames[pad]) free(obj->PNames[pad]);
	obj->PNames[pad] = str_dup(s);
}

int get_value(const unsigned i) const
{
	if (i >= NUM_OBJ_VAL_POSITIONS)
	{
		PyErr_SetString(PyExc_ValueError, "argument out of range");
		throw_error_already_set();
	}
	Ensurer obj(*this);
	return obj->obj_flags.value[i];
}

void set_value(const int i, const int v)
{
	if (i >= NUM_OBJ_VAL_POSITIONS)
	{
		PyErr_SetString(PyExc_ValueError, "argument out of range");
		throw_error_already_set();
	}
	Ensurer obj(*this);
	obj->obj_flags.value[i] = v;
}

int get_obj_type() const
{
	Ensurer obj(*this);
	return obj->obj_flags.type_flag;
}

void set_obj_type(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.type_flag = v;
}

int get_wear_flags() const
{
	Ensurer obj(*this);
	return obj->obj_flags.wear_flags;
}

void set_wear_flags(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.wear_flags = v;
}

unsigned get_weight() const
{
	Ensurer obj(*this);
	return obj->obj_flags.weight;
}

void set_weight(const unsigned v)
{
	Ensurer obj(*this);
	obj->obj_flags.weight = v;
}
unsigned get_cost() const
{
	Ensurer obj(*this);
	return obj->obj_flags.cost;
}

void set_cost(const unsigned v)
{
	Ensurer obj(*this);
	obj->obj_flags.cost = v;
}
unsigned get_cost_per_day_on() const
{
	Ensurer obj(*this);
	return obj->obj_flags.cost_per_day_on;
}

void set_cost_per_day_on(const unsigned v)
{
	Ensurer obj(*this);
	obj->obj_flags.cost_per_day_on = v;
}
unsigned get_cost_per_day_off() const
{
	Ensurer obj(*this);
	return obj->obj_flags.cost_per_day_off;
}

void set_cost_per_day_off(const unsigned v)
{
	Ensurer obj(*this);
	obj->obj_flags.cost_per_day_off = v;
}
int get_sex() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_sex;
}

void set_sex(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_sex = v;
}
int get_spell() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_spell;
}

void set_spell(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_spell = v;
}
int get_level() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_level;
}

void set_level(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_level = v;
}
int get_skill() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_skill;
}

void set_skill(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_skill = v;
}
int get_max() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_max;
}

void set_max(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_max = v;
}
int get_cur() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_cur;
}

void set_cur(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_cur = v;
}

int get_mater() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_mater;
}

void set_mater(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_mater = v;
}
int get_owner() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_owner;
}

void set_owner(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_owner = v;
}

int get_maker() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_maker;
}

void set_maker(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_maker = v;
}

int get_destroyer() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_destroyer;
}

void set_destroyer(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_destroyer = v;
}

int get_zone() const
{
	Ensurer obj(*this);
	return obj->obj_flags.Obj_zone;
}

void set_zone(const int v)
{
	Ensurer obj(*this);
	obj->obj_flags.Obj_zone = v;
}

flag_data get_affects() const
{
	Ensurer obj(*this);
	return obj->obj_flags.affects;
}

void set_affects(const flag_data& f)
{
	Ensurer obj(*this);
	obj->obj_flags.affects = f;
}

flag_data get_anti_flag() const
{
	Ensurer obj(*this);
	return obj->obj_flags.anti_flag;
}

void set_anti_flag(const flag_data& f)
{
	Ensurer obj(*this);
	obj->obj_flags.anti_flag = f;
}

flag_data get_no_flag() const
{
	Ensurer obj(*this);
	return obj->obj_flags.no_flag;
}

void set_no_flag(const flag_data& f)
{
	Ensurer obj(*this);
	obj->obj_flags.no_flag = f;
}

flag_data get_extra_flags() const
{
	Ensurer obj(*this);
	return obj->obj_flags.extra_flags;
}

void set_extra_flags(const flag_data& f)
{
	Ensurer obj(*this);
	obj->obj_flags.extra_flags = f;
}

affected_t& get_affected()
{
	Ensurer obj(*this);
	return obj->affected;
}

object get_carried_by() const
{
	Ensurer obj(*this);
	if (obj->carried_by)
		return object(CharacterWrapper(obj->carried_by));
	return object();
}

object get_worn_by() const
{
	Ensurer obj(*this);
	if (obj->worn_by)
		return object(CharacterWrapper(obj->worn_by));
	return object();
}

obj_rnum get_item_number() const
{
	Ensurer obj(*this);
	return GET_OBJ_RNUM(obj);
}

void set_item_number(const obj_rnum n)
{
	Ensurer obj(*this);
	GET_OBJ_RNUM(obj)=n;
}
};


extern obj_rnum top_of_objt;
extern std::vector < OBJ_DATA * >obj_proto;
ObjWrapper get_obj_proto(const obj_rnum rnum)
{
	if (rnum>=0 && rnum <= top_of_objt)
		return obj_proto[rnum];
		PyErr_SetString(PyExc_ValueError, "obj rnum is out of range");
	throw_error_already_set();
	return ObjWrapper(NULL);
}

object get_char_equipment(const CharacterWrapper& c, const unsigned num)
{
	if (num >= NUM_WEARS)
	{
			PyErr_SetString(PyExc_ValueError, "invalid wear slot");
		throw_error_already_set();
	}
	CharacterWrapper::Ensurer ch(c);
	obj_data* r = ch->equipment[num];
	if (!r)
		return object();
	else
		return object(ObjWrapper(r));
}

bool flag_is_set(const flag_data& flag, const unsigned f)
{
	return IS_SET(GET_FLAG(flag, f), f);
}

void flag_set(flag_data& flag, const unsigned f)
{
	SET_BIT(GET_FLAG(flag, f), f);
}

void flag_remove(flag_data& flag, const unsigned f)
{
	REMOVE_BIT(GET_FLAG(flag, f), f);
}

void flag_toggle(flag_data& flag, const unsigned f)
{
	TOGGLE_BIT(GET_FLAG(flag, f), f);
}

extern void tascii(int *pointer, int num_planes, char *ascii);
str flag_str(const flag_data& flag)
{
	char buf[MAX_STRING_LENGTH];
	*buf='\0';
	tascii((int*)&flag, 4, buf);
	return str(buf);
}

object obj_affected_type_str(const obj_affected_type& affect)
{
	char buf[MAX_STRING_LENGTH];
	sprinttype(affect.location, apply_types, buf);
	bool negative = false;
	for (int j = 0; *apply_negative[j] != '\n'; j++)
	{
		if (!str_cmp(buf2, apply_negative[j]))
		{
			negative = true;
			break;
		}
	}
	if (!negative && affect.modifier < 0)
	{
		negative = true;
	}
	else if (negative && affect.modifier < 0)
	{
		negative = false;
	}
	return str("%s%s%d") %
	make_tuple(buf,
	negative ? " �������� �� " : " �������� �� ",
	affect.modifier>=0 ? affect.modifier : -affect.modifier);
}

BOOST_PYTHON_MODULE(mud)
{
	def("log", mudlog, ( py::arg("msg"), py::arg("msg_type")=DEF, py::arg("level")=LVL_IMMORT, py::arg("channel")=SYSLOG, py::arg("to_file")=TRUE ) ,
	"���������� ��������� msg ���� msg_type � ����� ���� channel ��� ������ level.\n"
	"\n"
	"msg_type ��������� �������� �������� �� utils.h, defines for mudlog.\n"
	"channel  �����, � ������� ����� �������� ��������� (comm.h). � ��������� ����� ����� ��������� �������� constants.SYSLOG, constants.ERRLOG � constants.IMLOG.\n"
	"to_file  ���������� �� ��������� ��� �� � ����, ������ ������ ��� �����");
	def("send_all", send_to_all,
"���� ��������� msg ���� �������.");
	def("find_skill_num", find_skill_num, "���������� ����� ����� �� ��� ��������.");
	def("find_spell_num", find_spell_num, "���������� ����� ������ �� ��� ��������.");
	def("get_mob_proto", get_mob_proto, "���������� ���� �� ���� ���������� � ������� rnum.");
	def("get_obj_proto", get_obj_proto, "���������� ������ �� ���� ���������� � ������� rnum.");
	def("get_mob_rnum", real_mobile, "���������� rnum ���� �� vnum.\n"
	"\n"
	"rnum - ��� ������ � ���� ����������.\n"
	"vnum - ��� ����������� ����� ����, ������� �������� ��������.");
	def("get_obj_rnum", real_object, "���������� rnum ������� �� vnum."
	"\n"
	"��. ����������� � get_mob_rnum.");
	def("get_room_rnum", real_room, "���������� rnum ������� �� vnum."
	"\n"
	"��. ����������� � get_mob_rnum.");
	def("create_mob", create_mob_from_proto, create_mob_from_proto_overloads((py::args("proto_num"), py::args("virtual")=true), "������� ��������� ���� �� ��������� ���������.\n"
	"\n"
	"num  ����������� (vnum) ��� �������� (rnum) ���������.\n"
	"virtual  ���������� ����� ������� ���������. True - ����� vnum, False - ����� rnum.\n"
	"����� ��������, ���� ����� ��������� ���������� (�������� ���������� ���� in_room)."));

	ObjectDoesNotExist = handle<>(PyErr_NewException((char*)"mud.ObjectDoesNotExist", PyExc_RuntimeError, NULL));
	scope().attr("ObjectDoesNotExist") = ObjectDoesNotExist;
	class_<CharacterWrapper>("Character", "������� ��������.", no_init)
		.def("send", &CharacterWrapper::send, "�������� ��������� �������� ������.")
		.def("page_string", &CharacterWrapper::_page_string, "���������� ������ ��������� � ������������ ������������� ���������.")
		.def("close_console", &CharacterWrapper::close_console, "������� ��������� �� ������ �������, ���� �� � ��� ����� ��������� (��������� �������).")
		.def("act", &CharacterWrapper::act_on_char, (py::arg("msg"), py::arg("hide_invisible")=false, py::arg("obj")=NULL, py::arg("victim")=NULL, py::arg("act_type")=TO_CHAR), "������������ act (��. comm.cpp).")
		.def("act", &CharacterWrapper::act_on_obj)
		.def("interpret", &CharacterWrapper::interpret, "�������� ��������� �������� ������� �������.")
		.add_property("name", &CharacterWrapper::get_name, &CharacterWrapper::set_name)
		.def("get_pad", &CharacterWrapper::get_pad, "�������� ��� � ��������� ������ (0-5).")
		.def("set_pad", &CharacterWrapper::set_pad, "���������� ��������� ����� (0-5).")
		.add_property("long_descr", &CharacterWrapper::get_long_descr, &CharacterWrapper::set_long_descr, "�������� ����, ������������ � �������.")
		.add_property("description", &CharacterWrapper::get_description, &CharacterWrapper::set_description, "�������� ����, ������ �� ������� '�� ���'.")
		.add_property("class", &CharacterWrapper::get_class, &CharacterWrapper::set_class)
		.add_property("level", &CharacterWrapper::get_level, &CharacterWrapper::set_level)
		.add_property("UID", &CharacterWrapper::get_uid)
		.add_property("exp", &CharacterWrapper::get_exp, &CharacterWrapper::set_exp)
		.add_property("remort", &CharacterWrapper::get_remort)
		.add_property("gold", &CharacterWrapper::get_gold, &CharacterWrapper::set_gold)
		.add_property("bank", &CharacterWrapper::get_bank, &CharacterWrapper::set_bank)
		.def("remove_gold", &CharacterWrapper::remove_gold)
		.def("remove_bank", &CharacterWrapper::remove_bank)
		.def("remove_both_gold", &CharacterWrapper::remove_both_gold, "�������� ����� ��������� ���� � ��������� � ���, �, � ������ �������� ����������, � ����������� �����.")
		.def("add_gold", &CharacterWrapper::add_gold)
		.def("add_bank", &CharacterWrapper::add_bank)
		.add_property("total_gold", &CharacterWrapper::get_total_gold, "��������� ���������� ����� � ��������� (���� + �������).")
		.add_property("str", &CharacterWrapper::get_str, &CharacterWrapper::set_str)
		.add_property("dex", &CharacterWrapper::get_dex, &CharacterWrapper::set_dex)
		.add_property("con", &CharacterWrapper::get_con, &CharacterWrapper::set_con)
		.add_property("wis", &CharacterWrapper::get_wis, &CharacterWrapper::set_wis)
		.add_property("int", &CharacterWrapper::get_int, &CharacterWrapper::set_int)
		.add_property("cha", &CharacterWrapper::get_cha, &CharacterWrapper::set_cha)

		.add_property("sex", &CharacterWrapper::get_sex, &CharacterWrapper::set_sex, "��� ���������. �������� �� constants.SEX_XXX")
		.add_property("weight", &CharacterWrapper::get_weight, &CharacterWrapper::set_weight, "���")
		.add_property("height", &CharacterWrapper::get_height, &CharacterWrapper::set_height, "����")
		.add_property("religion", &CharacterWrapper::get_religion, &CharacterWrapper::set_religion, "����������� ��������������. 0 - ���������, 1 - ���������.")
		.add_property("kin", &CharacterWrapper::get_kin, &CharacterWrapper::set_kin, "�����")
		.add_property("race", &CharacterWrapper::get_race, &CharacterWrapper::set_race, "���")
		.add_property("hit", &CharacterWrapper::get_hit, &CharacterWrapper::set_hit, "������� ���������� ������ �����")
		.add_property("max_hit", &CharacterWrapper::get_max_hit, &CharacterWrapper::set_max_hit, "������������ ���������� ������ �����")
		.add_property("move", &CharacterWrapper::get_move, &CharacterWrapper::set_move, "������� ���������� ������ ��������")
		.add_property("max_move", &CharacterWrapper::get_max_move, &CharacterWrapper::set_max_move, "������������ ����������� ������ �������")
		//.add_property("master", make_getter(&CharacterWrapper::master, return_value_policy<reference_existing_object>()), make_setter(Character::master, ))
		.def("followers", &CharacterWrapper::get_followers, "���������� ������ �������������� ���������.")
		.add_property("is_immortal", &CharacterWrapper::is_immortal)
		.add_property("is_impl", &CharacterWrapper::is_impl)
		.add_property("is_NPC", &CharacterWrapper::is_NPC)
		.def("get_skill", &CharacterWrapper::get_skill, "���������� ������� �������� ������� �������.\n"
		"\n"
		"����� � ������ ���� ������ � ������� �� ������/���.\n"
		"��� ��������� ������ ������ �� �������� ��. get_skill_num.")
		.def("set_skill", &CharacterWrapper::set_skill, "������������� ������� �������� �������� �������.")
		.def("clear_skills", &CharacterWrapper::clear_skills, "������� ������  ������.")
		.add_property("skills_count", &CharacterWrapper::get_skills_count, "�������� ����� ������, �������� ������� ��������.")
		.def("get_equipped_skill", &CharacterWrapper::get_equipped_skill, "����� �� ������.")
		.def("get_trained_skill", &CharacterWrapper::get_trained_skill, "������ ������������� ����� ����.")
		.def("get_spell", &CharacterWrapper::get_spell, "������� �� �������� ��������� �����������. .\n"
		"\n"
		"��� ��������� ������ ���������� �� �������� ��. get_spell_num.")
		.def("set_spell", &CharacterWrapper::set_spell, "������������� ����������� ���������� � ���������.")
		.add_property("rnum", &CharacterWrapper::get_nr, &CharacterWrapper::set_nr, "�������� ����� ��������� � ������� ����������.")
		.add_property("in_room", &CharacterWrapper::get_in_room, &CharacterWrapper::set_in_room, "rnum �������, � ������� ��������� ��������.\n"
		"\n"
		"��� ��������� ����� �������� �������� ����������� � �������� �������.")
		.def("affected_by_spell", &CharacterWrapper::is_affected_by_spell, "��������� �� �������� �������� ���������� ����������.")
		.def("add_affect", &CharacterWrapper::add_affect, "�������� �� ��������� �������� ������.")
		.def("get_char_vis", &CharacterWrapper::get_vis, "���� ���������� ��������� � ������ ���������.\n"
		"\n"
		"��������� �������� ��������� �������� �� constants.FIND_XXX.")
		.def("get_equipment", get_char_equipment, "���������� ������ �� ��������� ����� ���������� ���������.")
		.def("restore", &CharacterWrapper::restore, "��������������� ���� �����, ������� � ����/���.")
		.def("quested_add", &CharacterWrapper::quested_add, "���������� ������������ ������ �����/������ ������ (128 ��������).")
		.def("quested_remove", &CharacterWrapper::quested_remove, "�������� ���������� � ������.")
		.def("quested_get", &CharacterWrapper::quested_get_text, "���������� ������ ��������� ����������, ����������� ��� ������� ������� vnum.")
		.add_property("quested_text", &CharacterWrapper::quested_print, "��� ���������� �� ������� � ��������� ����.")
	;

	class_<affected_t>("ObjAffectedArray", "������ �� ����� ������������� �������.", no_init)
		.def("__len__", &_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::size)
		.def("__getitem__",&_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::get)
		.def("__getslice__",&_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::getslice)
		.def("__setslice__",&_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::setslice)
		.def("__setitem__",&_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::set)
		.def("values", &_arrayN<obj_affected_type, MAX_OBJ_AFFECT>::values)
	;

	//wraps obj_data (see obj.hpp)
	class_<ObjWrapper>("Object", "������� ������ (����).", no_init)
	.add_property("name", &ObjWrapper::get_name, &ObjWrapper::set_name, "������ �������")
	.add_property("description", &ObjWrapper::get_description, &ObjWrapper::set_description, "�������� ������� ����� �����.")
	.add_property("short_description", &ObjWrapper::get_short_description, &ObjWrapper::set_short_description, "�������� ������� (������������ �����)")
	.add_property("action_description", &ObjWrapper::get_action_description, &ObjWrapper::set_action_description, "�������� ��� �������� (������� � �.�.)")
	.def("get_pad", &ObjWrapper::get_pad, "�������� �������� �������� � �������� ������.")
	.def("set_pad", &ObjWrapper::set_pad, "������������� �������� ����� �������� � �������� ��������.")
	.add_property("item_type", &ObjWrapper::get_obj_type, &ObjWrapper::set_obj_type, "��� �������� (�������� �� constants.ITEM_XXX)")
	.def("get_value", &ObjWrapper::get_value, "�������� ���� �� �������� �������� (0-3).\n"
	"\n"
	"���� �������� ����� ������ ������������� ��� ������ ����� ���������.\n")
	.def("set_value", &ObjWrapper::set_value, "���������� ���� �������� �������� (���� 0-3)")
	.add_property("wear_flags", &ObjWrapper::get_wear_flags, &ObjWrapper::set_wear_flags, "����� ������ �������� (���� ������� ����� ��� ����?)")
	.add_property("weight", &ObjWrapper::get_weight, &ObjWrapper::set_weight, "���")
	.add_property("cost", &ObjWrapper::get_cost, &ObjWrapper::set_cost, "���� ��� ������� � ��������")
	.add_property("cost_per_day_on", &ObjWrapper::get_cost_per_day_on, &ObjWrapper::set_cost_per_day_on, "���� �����, ����� ������� ���� � ����������")
	.add_property("cost_per_day_off", &ObjWrapper::get_cost_per_day_off, &ObjWrapper::set_cost_per_day_off, "���� �����, ����� ������� �� � ����������")
		.add_property("sex", &ObjWrapper::get_sex, &ObjWrapper::set_sex, "��� ��������")
		.add_property("spell", &ObjWrapper::get_spell, &ObjWrapper::set_spell)
		.add_property("level", &ObjWrapper::get_level, &ObjWrapper::set_level)
		.add_property("skill", &ObjWrapper::get_skill, &ObjWrapper::set_skill)
		.add_property("max", &ObjWrapper::get_max, &ObjWrapper::set_max, "������������ ���������")
		.add_property("cur", &ObjWrapper::get_cur, &ObjWrapper::set_cur, "������� ���������")
		.add_property("material", &ObjWrapper::get_mater, &ObjWrapper::set_mater, "��������")
		.add_property("owner", &ObjWrapper::get_owner, &ObjWrapper::set_owner, "ID ��������� (��� �����?)")
		.add_property("destroyer", &ObjWrapper::get_destroyer, &ObjWrapper::set_destroyer)
		.add_property("maker", &ObjWrapper::get_maker, &ObjWrapper::set_maker, "ID �������� (?)")
		.add_property("zone", &ObjWrapper::get_zone, &ObjWrapper::set_zone, "rnum ����, �� ������� ������� �����")
		.add_property("rnum", &ObjWrapper::get_item_number, &ObjWrapper::set_item_number, "�������� ����� �������, ���������� �������� � ������� ����������.")
		.add_property("affects", &ObjWrapper::get_affects, &ObjWrapper::set_affects, "������������� �������")
		.add_property("extra_flags", &ObjWrapper::get_extra_flags, &ObjWrapper::set_extra_flags, "����������� (�����, ����� � �.�.)")
		.add_property("no_flags", &ObjWrapper::get_no_flag, &ObjWrapper::set_no_flag)
		.add_property("anti_flags", &ObjWrapper::get_anti_flag, &ObjWrapper::set_anti_flag)
		.add_property("modifiers", make_function(&ObjWrapper::get_affected, return_internal_reference<>()), "������ ������������� (XXX �������� �� YYY)")
		.add_property("carried_by", &ObjWrapper::get_carried_by, "� ���� ������� � ���������")
		.add_property("worn_by", &ObjWrapper::get_worn_by, "�� ��� ������� ���� � ����������")
	;

	//implicitly_convertible<Character*, CharacterWrapper>();
	implicitly_convertible<CharacterWrapper, Character* >();

	class_<CharacterListWrapper::iterator>("CharacterListIterator", "�������� �� ������ mud.character_list", no_init)
		.def("next", &CharacterListWrapper::iterator::next);
	scope().attr("character_list") = class_<CharacterListWrapper>("CharacterListWrapper", "������ ���� ���������� � ����.")
		.def("__iter__", &CharacterListWrapper::iter, return_value_policy<manage_new_object>())
		.staticmethod("__iter__")
	();

	class_<affect_data, std::auto_ptr<affect_data> >("Affect", "������� ������.")
		.def_readwrite("spell_type", &affect_data::type, "����� ����������, ������� �������� ���� ������")
		.add_property("spell_type_str", get_spell_type_str, "�������� ���������� (������ ��� ������)")
		.add_property("location_str", get_location_str, "�������� ����������� ��������� (������ ��� ������)")
		.add_property("bitvector_str", get_bitvector_str, "����� ������� ����������� (� ��������� ����, ������ ��� ������)")
		.def_readwrite("duration", &affect_data::duration, "����������������� � ��������")
		.def_readwrite("modifier", &affect_data::modifier, "�������� ��������� ���������")
		.def_readwrite("location", &affect_data::location, "���������� �������� (constants.APLY_XXX)")
		.def_readwrite("battleflag", &affect_data::battleflag, "���� ��� ��� (����, � �.�.)")
		.def_readwrite("bitvector", &affect_data::bitvector, "������������� �������")
		.def_readwrite("caster_id", &affect_data::caster_id, "ID �������������")
		.def_readwrite("apply_time", &affect_data::apply_time, "��������� ������� ������ ����� (���� ������������ ������ � ��������)")
	;

	class_<flag_data>("FlagData", "����� ����-������.")
		.def("__contains__", flag_is_set, "���������� �� ���� � ���� ����?")
		.def("set", flag_set, "���������� ��������� ����")
		.def("remove", flag_remove, "������ ��������� ����")
		.def("toggle", flag_toggle, "����������� ��������� ����")
		.def("__str__", flag_str)
	;

	class_<obj_affected_type>("ObjectModifier", "����������� ���������, ������������� ��������.")
	.def(py::init<int, int>(py::args("location", "modifier")))
		.def_readwrite("location", &obj_affected_type::location, "�������, ������� ��������")
		.def_readwrite("modifier", &obj_affected_type::modifier, "�������� ���������")
		.def("__str__", obj_affected_type_str, "��������� ����������� � ������ ���� (XXX �������� �� YYY)")
	;
}

BOOST_PYTHON_MODULE(constants)
{
	//log channels
	DEFINE_CONSTANT(SYSLOG);
	DEFINE_CONSTANT(ERRLOG);
	DEFINE_CONSTANT(IMLOG);

	//act
	DEFINE_CONSTANT(TO_ROOM);
	DEFINE_CONSTANT(TO_VICT);
	DEFINE_CONSTANT(TO_NOTVICT);
	DEFINE_CONSTANT(TO_CHAR);
	DEFINE_CONSTANT(TO_ROOM_HIDE);
	DEFINE_CONSTANT(CHECK_NODEAF);
	DEFINE_CONSTANT(CHECK_DEAF);
	DEFINE_CONSTANT(TO_SLEEP);

	//sex
	DEFINE_CONSTANT(SEX_NEUTRAL);
	DEFINE_CONSTANT(SEX_MALE);
	DEFINE_CONSTANT(SEX_FEMALE);
	DEFINE_CONSTANT(SEX_POLY);

	//religion
	DEFINE_CONSTANT(RELIGION_POLY);
	DEFINE_CONSTANT(RELIGION_MONO);

	//aff_xxx
	DEFINE_CONSTANT(AFF_BLIND);
	DEFINE_CONSTANT(AFF_INVISIBLE);
	DEFINE_CONSTANT(AFF_DETECT_ALIGN);
	DEFINE_CONSTANT(AFF_DETECT_INVIS);
	DEFINE_CONSTANT(AFF_DETECT_MAGIC);
	DEFINE_CONSTANT(AFF_SENSE_LIFE);
	DEFINE_CONSTANT(AFF_WATERWALK);
	DEFINE_CONSTANT(AFF_SANCTUARY);
	DEFINE_CONSTANT(AFF_GROUP);
	DEFINE_CONSTANT(AFF_CURSE);
	DEFINE_CONSTANT(AFF_INFRAVISION);
	DEFINE_CONSTANT(AFF_POISON);
	DEFINE_CONSTANT(AFF_PROTECT_EVIL);
	DEFINE_CONSTANT(AFF_PROTECT_GOOD);
	DEFINE_CONSTANT(AFF_SLEEP);
	DEFINE_CONSTANT(AFF_NOTRACK);
	DEFINE_CONSTANT(AFF_TETHERED);
	DEFINE_CONSTANT(AFF_BLESS);
	DEFINE_CONSTANT(AFF_SNEAK);
	DEFINE_CONSTANT(AFF_HIDE);
	DEFINE_CONSTANT(AFF_COURAGE);
	DEFINE_CONSTANT(AFF_CHARM);
	DEFINE_CONSTANT(AFF_HOLD);
	DEFINE_CONSTANT(AFF_FLY);
	DEFINE_CONSTANT(AFF_SIELENCE);
	DEFINE_CONSTANT(AFF_AWARNESS);
	DEFINE_CONSTANT(AFF_BLINK);
	DEFINE_CONSTANT(AFF_HORSE);
	DEFINE_CONSTANT(AFF_NOFLEE);
	DEFINE_CONSTANT(AFF_SINGLELIGHT);
	DEFINE_CONSTANT(AFF_HOLYLIGHT);
	DEFINE_CONSTANT(AFF_HOLYDARK);
	DEFINE_CONSTANT(AFF_DETECT_POISON);
	DEFINE_CONSTANT(AFF_DRUNKED);
	DEFINE_CONSTANT(AFF_ABSTINENT);
	DEFINE_CONSTANT(AFF_STOPRIGHT);
	DEFINE_CONSTANT(AFF_STOPLEFT);
	DEFINE_CONSTANT(AFF_STOPFIGHT);
	DEFINE_CONSTANT(AFF_HAEMORRAGIA);
	DEFINE_CONSTANT(AFF_CAMOUFLAGE);
	DEFINE_CONSTANT(AFF_WATERBREATH);
	DEFINE_CONSTANT(AFF_SLOW);
	DEFINE_CONSTANT(AFF_HASTE);
	DEFINE_CONSTANT(AFF_SHIELD);
	DEFINE_CONSTANT(AFF_AIRSHIELD);
	DEFINE_CONSTANT(AFF_FIRESHIELD);
	DEFINE_CONSTANT(AFF_ICESHIELD);
	DEFINE_CONSTANT(AFF_MAGICGLASS);
	DEFINE_CONSTANT(AFF_STAIRS);
	DEFINE_CONSTANT(AFF_STONEHAND);
	DEFINE_CONSTANT(AFF_PRISMATICAURA);
	DEFINE_CONSTANT(AFF_HELPER);
	DEFINE_CONSTANT(AFF_EVILESS);
	DEFINE_CONSTANT(AFF_AIRAURA);
	DEFINE_CONSTANT(AFF_FIREAURA);
	DEFINE_CONSTANT(AFF_ICEAURA);
	DEFINE_CONSTANT(AFF_DEAFNESS);
	DEFINE_CONSTANT(AFF_CRYING);
	DEFINE_CONSTANT(AFF_PEACEFUL);
	DEFINE_CONSTANT(AFF_MAGICSTOPFIGHT);
	DEFINE_CONSTANT(AFF_BERSERK);
	DEFINE_CONSTANT(AFF_LIGHT_WALK);
	DEFINE_CONSTANT(AFF_BROKEN_CHAINS);
	DEFINE_CONSTANT(AFF_CLOUD_OF_ARROWS);
	DEFINE_CONSTANT(AFF_SHADOW_CLOAK);
	DEFINE_CONSTANT(AFF_GLITTERDUST);
	DEFINE_CONSTANT(AFF_AFFRIGHT);
	DEFINE_CONSTANT(AFF_SCOPOLIA_POISON);
	DEFINE_CONSTANT(AFF_DATURA_POISON);
	DEFINE_CONSTANT(AFF_SKILLS_REDUCE);
	DEFINE_CONSTANT(AFF_NOT_SWITCH);
	DEFINE_CONSTANT(AFF_BELENA_POISON);
	DEFINE_CONSTANT(AFF_NOTELEPORT);
	DEFINE_CONSTANT(AFF_LACKY);
	DEFINE_CONSTANT(AFF_BANDAGE);
	DEFINE_CONSTANT(AFF_NO_BANDAGE);
	DEFINE_CONSTANT(AFF_MORPH);

	//apply_xxx
	DEFINE_CONSTANT(APPLY_NONE);
	DEFINE_CONSTANT(APPLY_STR);
	DEFINE_CONSTANT(APPLY_DEX);
	DEFINE_CONSTANT(APPLY_INT);
	DEFINE_CONSTANT(APPLY_WIS);
	DEFINE_CONSTANT(APPLY_CON);
	DEFINE_CONSTANT(APPLY_CHA);
	DEFINE_CONSTANT(APPLY_CLASS);
	DEFINE_CONSTANT(APPLY_LEVEL);
	DEFINE_CONSTANT(APPLY_AGE);
	DEFINE_CONSTANT(APPLY_CHAR_WEIGHT);
	DEFINE_CONSTANT(APPLY_CHAR_HEIGHT);
	DEFINE_CONSTANT(APPLY_MANAREG);
	DEFINE_CONSTANT(APPLY_HIT);
	DEFINE_CONSTANT(APPLY_MOVE);
	DEFINE_CONSTANT(APPLY_GOLD);
	DEFINE_CONSTANT(APPLY_EXP);
	DEFINE_CONSTANT(APPLY_AC);
	DEFINE_CONSTANT(APPLY_HITROLL);
	DEFINE_CONSTANT(APPLY_DAMROLL);
	DEFINE_CONSTANT(APPLY_SAVING_WILL);
	DEFINE_CONSTANT(APPLY_RESIST_FIRE);
	DEFINE_CONSTANT(APPLY_RESIST_AIR);
	DEFINE_CONSTANT(APPLY_SAVING_CRITICAL);
	DEFINE_CONSTANT(APPLY_SAVING_STABILITY);
	DEFINE_CONSTANT(APPLY_HITREG);
	DEFINE_CONSTANT(APPLY_MOVEREG);
	DEFINE_CONSTANT(APPLY_C1);
	DEFINE_CONSTANT(APPLY_C2);
	DEFINE_CONSTANT(APPLY_C3);
	DEFINE_CONSTANT(APPLY_C4);
	DEFINE_CONSTANT(APPLY_C5);
	DEFINE_CONSTANT(APPLY_C6);
	DEFINE_CONSTANT(APPLY_C7);
	DEFINE_CONSTANT(APPLY_C8);
	DEFINE_CONSTANT(APPLY_C9);
	DEFINE_CONSTANT(APPLY_SIZE);
	DEFINE_CONSTANT(APPLY_ARMOUR);
	DEFINE_CONSTANT(APPLY_POISON);
	DEFINE_CONSTANT(APPLY_SAVING_REFLEX);
	DEFINE_CONSTANT(APPLY_CAST_SUCCESS);
	DEFINE_CONSTANT(APPLY_MORALE);
	DEFINE_CONSTANT(APPLY_INITIATIVE);
	DEFINE_CONSTANT(APPLY_RELIGION);
	DEFINE_CONSTANT(APPLY_ABSORBE);
	DEFINE_CONSTANT(APPLY_LIKES);
	DEFINE_CONSTANT(APPLY_RESIST_WATER);
	DEFINE_CONSTANT(APPLY_RESIST_EARTH);
	DEFINE_CONSTANT(APPLY_RESIST_VITALITY);
	DEFINE_CONSTANT(APPLY_RESIST_MIND);
	DEFINE_CONSTANT(APPLY_RESIST_IMMUNITY);
	DEFINE_CONSTANT(APPLY_AR);
	DEFINE_CONSTANT(APPLY_MR);
	DEFINE_CONSTANT(APPLY_ACONITUM_POISON);
	DEFINE_CONSTANT(APPLY_SCOPOLIA_POISON);
	DEFINE_CONSTANT(APPLY_BELENA_POISON);
	DEFINE_CONSTANT(APPLY_DATURA_POISON);
	DEFINE_CONSTANT(NUM_APPLIES);

	//find_xxx (handler.h)
	DEFINE_CONSTANT(FIND_CHAR_ROOM);
	DEFINE_CONSTANT(FIND_CHAR_WORLD);

	DEFINE_CONSTANT(WEAR_LIGHT);
	DEFINE_CONSTANT(WEAR_FINGER_R);
	DEFINE_CONSTANT(WEAR_FINGER_L);
	DEFINE_CONSTANT(WEAR_NECK_1);
	DEFINE_CONSTANT(WEAR_NECK_2);
	DEFINE_CONSTANT(WEAR_BODY);
	DEFINE_CONSTANT(WEAR_HEAD);
	DEFINE_CONSTANT(WEAR_LEGS);
	DEFINE_CONSTANT(WEAR_FEET);
	DEFINE_CONSTANT(WEAR_HANDS);
	DEFINE_CONSTANT(WEAR_ARMS);
	DEFINE_CONSTANT(WEAR_SHIELD);
	DEFINE_CONSTANT(WEAR_ABOUT);
	DEFINE_CONSTANT(WEAR_WAIST);
	DEFINE_CONSTANT(WEAR_WRIST_R);
	DEFINE_CONSTANT(WEAR_WRIST_L);
	DEFINE_CONSTANT(WEAR_WIELD);
	DEFINE_CONSTANT(WEAR_HOLD);
	DEFINE_CONSTANT(WEAR_BOTHS);
	DEFINE_CONSTANT(NUM_WEARS);

	DEFINE_CONSTANT(ITEM_LIGHT);
	DEFINE_CONSTANT(ITEM_SCROLL);
	DEFINE_CONSTANT(ITEM_WAND);
	DEFINE_CONSTANT(ITEM_STAFF);
	DEFINE_CONSTANT(ITEM_WEAPON);
	DEFINE_CONSTANT(ITEM_FIREWEAPON);
	DEFINE_CONSTANT(ITEM_MISSILE);
	DEFINE_CONSTANT(ITEM_TREASURE);
	DEFINE_CONSTANT(ITEM_ARMOR);
	DEFINE_CONSTANT(ITEM_POTION);
	DEFINE_CONSTANT(ITEM_WORN);
	DEFINE_CONSTANT(ITEM_OTHER);
	DEFINE_CONSTANT(ITEM_TRASH);
	DEFINE_CONSTANT(ITEM_TRAP);
	DEFINE_CONSTANT(ITEM_CONTAINER);
	DEFINE_CONSTANT(ITEM_NOTE);
	DEFINE_CONSTANT(ITEM_DRINKCON);
	DEFINE_CONSTANT(ITEM_KEY);
	DEFINE_CONSTANT(ITEM_FOOD);
	DEFINE_CONSTANT(ITEM_MONEY);
	DEFINE_CONSTANT(ITEM_PEN);
	DEFINE_CONSTANT(ITEM_BOAT);
	DEFINE_CONSTANT(ITEM_FOUNTAIN);
	DEFINE_CONSTANT(ITEM_BOOK);
	DEFINE_CONSTANT(ITEM_INGRADIENT);
	DEFINE_CONSTANT(ITEM_MING);
	DEFINE_CONSTANT(ITEM_MATERIAL);
	DEFINE_CONSTANT(ITEM_BANDAGE);
	DEFINE_CONSTANT(ITEM_ARMOR_LIGHT);
	DEFINE_CONSTANT(ITEM_ARMOR_MEDIAN);
	DEFINE_CONSTANT(ITEM_ARMOR_HEAVY);

	DEFINE_CONSTANT(ITEM_WEAR_TAKE);
	DEFINE_CONSTANT(ITEM_WEAR_FINGER);
	DEFINE_CONSTANT(ITEM_WEAR_NECK);
	DEFINE_CONSTANT(ITEM_WEAR_BODY);
	DEFINE_CONSTANT(ITEM_WEAR_HEAD);
	DEFINE_CONSTANT(ITEM_WEAR_LEGS);
	DEFINE_CONSTANT(ITEM_WEAR_FEET);
	DEFINE_CONSTANT(ITEM_WEAR_HANDS);
	DEFINE_CONSTANT(ITEM_WEAR_ARMS);
	DEFINE_CONSTANT(ITEM_WEAR_SHIELD);
	DEFINE_CONSTANT(ITEM_WEAR_ABOUT);
	DEFINE_CONSTANT(ITEM_WEAR_WAIST);
	DEFINE_CONSTANT(ITEM_WEAR_WRIST);
	DEFINE_CONSTANT(ITEM_WEAR_WIELD);
	DEFINE_CONSTANT(ITEM_WEAR_HOLD);
	DEFINE_CONSTANT(ITEM_WEAR_BOTHS);
}

void scripting::init()
{
	Py_InitializeEx(0); //pass 0 to skip initialization registration of signal handlers
	log("Using python version %s", Py_GetVersion());
	try
	{
	initmud();
	initconstants();
	//add "scripts" to python module path
	import("sys").attr("path").attr("insert")(0, "scripts");
	/*object main_module = import("__main__");
	object main_namespace = main_module.attr("__dict__");*/
	import("console");
	} catch(error_already_set const &)
	{
		log(parse_python_exception().c_str());
		//����� �� ���� ���� ����������� � ����������� �������, ��� ����� ������������ ��� ��������� ����� �� ����
		puts("SYSERR: error initializing Python");
		exit(1);
	}
}

void scripting::terminate()
{
	Py_Finalize();
}

//Converts last python exception into string for pretty printing
//Precondition: catch(boost::python::error_already_set const &)
std::string parse_python_exception()
{
	PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
	PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
	std::string ret("Unfetchable Python error");
	if (type_ptr != NULL)
	try
	{
		handle<> h_type(type_ptr);
		str type_pstr(extract<object>(h_type.get())().attr("__name__"));
		ret = extract<std::string>(type_pstr);
	} catch (error_already_set const &)
	{
		ret = "Internal error getting exception type";
		PyErr_Print();
	}
	if (value_ptr != NULL)
	{
		handle<> h_val(value_ptr);
		str val_pstr(h_val);
		extract<std::string> e_val_pstr(val_pstr);
		if (e_val_pstr.check())
			ret += ": " + e_val_pstr();
		else
			ret += ": internal error getting exception value";
	}
	if (traceback_ptr != NULL)
	try
	{
		handle<> h_tb(traceback_ptr);
		object tb_list = import("traceback").attr("format_tb")(h_tb);
		object tb_str(str("\n").join(tb_list));
		extract<std::string> e_tb_pstr(tb_str);
		if (e_tb_pstr.check())
			ret = "Traceback (most recent call last):\n" + e_tb_pstr() + ret;
		else
			ret += "\nInternal error while formatting traceback";
	} catch (error_already_set const &)
	{
		ret += "\nInternal error while formatting traceback";
		PyErr_Print();
	}

	return ret;
}

class scripting::Console_impl
{
public:
	Console_impl(const CharacterWrapper& ch):
		console(import("console").attr("PythonConsole")(ch)) { }

	bool push(const char* line)
	{
		try
		{
			return extract<bool>(console.attr("push")(line))();
		} catch(error_already_set const &)
		{
			log(parse_python_exception().c_str());
			return false;
		}
	}

	string get_prompt()
	{
		try
		{
			return extract<string>(console.attr("get_prompt")())();
		} catch(error_already_set const &)
		{
			log(parse_python_exception().c_str());
			return string();
		}
	}

private:
	Console_impl(const Console_impl&);
	void operator=(const Console_impl&);
	object console;
};

Console::Console(CHAR_DATA* ch):
	_impl(new Console_impl(CharacterWrapper(ch)))
{ }

bool Console::push(const char* line)
{
	return _impl->push(line);
}

string Console::get_prompt()
{
	return _impl->get_prompt();
}

ACMD(do_console)
{
	send_to_char(ch, "Python %s on %s\r\n�������� \"help\" ��� ������, \"exit()\", ����� �����.\r\n", Py_GetVersion(), Py_GetPlatform());
	if (!ch->desc->console)
		ch->desc->console = boost::shared_ptr<Console>(new Console(ch));
	//ch->desc->console->print_prompt();
	STATE(ch->desc) = CON_CONSOLE;
}

template <class t, class nextfunc, class getfunc, class result_t>
result_t CIterator<t, nextfunc, getfunc, result_t>::next()
{
	object_t result = _object;
	if (_object)
		_object = _next_func(_object);
	return _get_func(result);
}
