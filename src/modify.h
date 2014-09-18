// -*- coding: koi8-r -*-

#ifndef _MODIFY_H_
#define _MODIFY_H_

#include "structs.h"

void string_add(DESCRIPTOR_DATA * d, char *str);
void string_write(DESCRIPTOR_DATA * d, char **txt, size_t len, long mailto, void *data);
void page_string(DESCRIPTOR_DATA * d, char *str, int keep_internal);
void page_string(DESCRIPTOR_DATA * d, std::string buf);

#endif
