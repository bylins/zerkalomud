// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2009 Krodo
// Part of Bylins http://www.mud.ru

#ifndef POISON_HPP_INCLUDED
#define POISON_HPP_INCLUDED

#include "conf.h"
#include "sysdep.h"
#include "structs.h"

void poison_victim(CHAR_DATA * ch, CHAR_DATA * vict, int modifier);
void try_weap_poison(CHAR_DATA *ch, CHAR_DATA *vict, OBJ_DATA *wielded);

bool poison_in_vessel(int liquid_num);
void set_weap_poison(OBJ_DATA *weapon, int liquid_num);

std::string get_poison_by_spell(int spell);
bool check_poison(int spell);

int same_time_update(CHAR_DATA *ch, AFFECT_DATA *af);

#endif // POISON_HPP_INCLUDED
