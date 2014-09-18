// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#ifndef FILE_CRC_HPP_INCLUDED
#define FILE_CRC_HPP_INCLUDED

#include "conf.h"
#include "sysdep.h"
#include "structs.h"

namespace FileCRC
{

// UPDATE_� ���� ����� ������� ������
enum { PLAYER, TEXTOBJS, TIMEOBJS, UPDATE_PLAYER, UPDATE_TEXTOBJS, UPDATE_TIMEOBJS };

void load();
void save(bool force_save = false);
void show(CHAR_DATA *ch);
void check_crc(const char *name, int mode, long uid);

} // namespace FileCRC

#endif // FILE_CRC_HPP_INCLUDED
