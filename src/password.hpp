// password.hpp
// Copyright (c) 2007 Krodo
// Part of Bylins http://www.mud.ru

#ifndef PASSWORD_HPP_INCLUDED
#define PASSWORD_HPP_INCLUDED

#include "conf.h"
#include <string>

#include "sysdep.h"
#include "structs.h"

/**
* ������ ����� ��� ������ � �������� � ���� ���� + ��������� md5, ���� ����� � ���� ������ ���� ��� �� ����� �����.
* ���� ���� ����� ���������� ���������� �����-���� ���� � ������� ���������� - ��� ��� ��� ��� ���� ������.
*/
namespace Password
{

void set_password(CHAR_DATA *ch, const std::string &pwd);
bool compare_password(CHAR_DATA *ch, const std::string &pwd);
bool compare_password(std::string const &hash, std::string const &pass);
bool check_password(const CHAR_DATA *ch, const char *pwd);
std::string generate_md5_hash(const std::string &pwd);

extern const unsigned int MIN_PWD_LENGTH;
extern const unsigned int MAX_PWD_LENGTH;
extern const char *BAD_PASSWORD;

} // namespace Password

#endif // PASSWORD_HPP_INCLUDED
