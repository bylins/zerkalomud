// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#include <fstream>
#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include "conf.h"
#include "file_crc.hpp"
#include "db.h"
#include "utils.h"
#include "interpreter.h"
#include "comm.h"

bool need_warn = true;
namespace FileCRC
{

class PlayerCRC
{
public:
	PlayerCRC() : player(0), textobjs(0), timeobjs(0) {};
	std::string name; // ��� ������
	boost::uint32_t player; // crc .player
	boost::uint32_t textobjs; // crc .textobjs
	boost::uint32_t timeobjs; // crc .timeobjs
	// TODO: �� ����� ���
};

typedef boost::shared_ptr<PlayerCRC> CRCListPtr;
typedef std::map<long, CRCListPtr> CRCListType;
const char CRC_UID = '*';

CRCListType crc_list;
// ��� �����
std::string message;
// ������ ������������� ������ ����� �������
bool need_save = false;

/**
* ����� ��������� � ������, ������ � ��� show crc.
*/
void add_message(const char *text, ...)
{
	if (!text) return;
	va_list args;
	char out[MAX_STRING_LENGTH];

	va_start(args, text);
	vsprintf(out, text, args);
	va_end(args);

	mudlog(out, DEF, LVL_IMMORT, SYSLOG, TRUE);
	message += out + std::string("\r\n");
}

/**
* ������� crc ��� ������.
*/
boost::uint32_t calculate_str_crc(const std::string &text)
{
	boost::crc_32_type crc;
	crc = std::for_each(text.begin(), text.end(), crc);
	return crc();
}

/**
* ������� crc ��� �����.
*/
boost::uint32_t calculate_file_crc(const char *name)
{
	std::ifstream in(name, std::ios::binary);
	if (!in.is_open())
		return 0;

	std::ostringstream t_out;
	t_out << in.rdbuf();
	return calculate_str_crc(t_out.str());
}

/**
* �������� ����������� ������ crc.
*/
void load()
{
	const char *file_name = LIB_PLRSTUFF"crc.lst";
	std::ifstream file(file_name);
	if (!file.is_open())
	{
		add_message("SYSERROR: �� ������� ������� ���� �� ������: %s", file_name);
		return;
	}

	std::string buffer, textobjs_buf, timeobjs_buf;
	std::getline(file, buffer, CRC_UID);
	std::stringstream stream(buffer);

	long uid;
	while (stream >> uid)
	{
		if (!(stream >> buffer >> textobjs_buf >> timeobjs_buf))
		{
			add_message("SYSERROR: �� ������� ��������� ����: %s, last uid: %ld", file_name, uid);
			return;
		}

		std::string name = GetNameByUnique(uid);
		if (name.empty())
		{
			log("FileCRC: UID %ld - ��������� �� ����������.", uid);
			continue;
		}
		CRCListPtr tmp_crc(new PlayerCRC);
		tmp_crc->name = name;
		try
		{
			tmp_crc->player = boost::lexical_cast<boost::uint32_t>(buffer);
			tmp_crc->textobjs = boost::lexical_cast<boost::uint32_t>(textobjs_buf);
			tmp_crc->timeobjs = boost::lexical_cast<boost::uint32_t>(timeobjs_buf);
		}
		catch (boost::bad_lexical_cast &)
		{
			add_message("FileCrc: ������ ������ crc (%s, %s), uid: %ld", buffer.c_str(), textobjs_buf.c_str(), uid);
			break;
		}
		crc_list[uid] = tmp_crc;
	}

	bool checked = false;
	if ((file >> buffer))
	{
		boost::uint32_t prev_crc;
		try
		{
			prev_crc = boost::lexical_cast<boost::uint32_t>(buffer);
		}
		catch (boost::bad_lexical_cast &)
		{
			add_message("SYSERROR: ������ ������ total crc (%s)", buffer.c_str());
			return;
		}
		stream.clear();
		const boost::uint32_t crc = calculate_str_crc(stream.str());
		if (crc != prev_crc)
		{
			add_message("SYSERROR: ������������ ����������� ����� �����: %s", file_name);
			return;
		}
		checked = true;
	}

	if (!checked)
		add_message("SYSERROR: �� ����������� ������ ����������� ����� �����: %s", file_name);
}

/**
* ������ ����������� ������ ���-���� (��������� ��� � ������� ��� �������� ������������� �����).
* \param force_save - ������ ����� � ����� ������ (��� �������� ����), �� ��������� false.
*/
void save(bool force_save)
{
	if (!need_save && !force_save) return;

	need_save = false;
	std::stringstream out;

	for (CRCListType::const_iterator it = crc_list.begin(); it != crc_list.end(); ++it)
	{
		out << it->first << " "
			<< it->second->player << " "
			<< it->second->textobjs << " "
			<< it->second->timeobjs << "\r\n";
	}

	// crc32 ������ ����� ������� ��� �� � �����
	const boost::uint32_t crc = calculate_str_crc(out.str());

	// ��� ��� � �����, ����� �� �������� �� ����� ����
	const char *file_name = LIB_PLRSTUFF"crc.lst";
	std::ofstream file(file_name);
	if (!file.is_open())
	{
		add_message("SYSERROR: �� ������� ������� ���� �� ������: %s", file_name);
		return;
	}
	file << out.rdbuf() << CRC_UID << " " << crc << "\r\n";
	file.close();
}

void create_message(std::string &name, int mode)
{
	if(!need_warn)
		return;
	char time_buf[20];
	time_t ct = time(0);
	strftime(time_buf, sizeof(time_buf), "%d-%m-%y %H:%M:%S", localtime(&ct));
	std::string file_type;
	switch (mode)
	{
	case PLAYER:
		file_type = "player";
		break;
	case TEXTOBJS:
		file_type = "textobjs";
		break;
	case TIMEOBJS:
		file_type = "timeobjs";
		break;
	default:
		file_type = "error mode";
		break;
	}
	add_message("%s ������������ ����������� ����� %s �����: %s", time_buf, file_type.c_str(), name.c_str());
}

/**
* �������� crc ������ ������.
* \param mode - PLAYER ��� ������ ������, UPDATE_PLAYER - ��� ������ � ����������� ������ crc.
*/
void check_crc(const char *filename, int mode, long uid)
{
	CRCListType::const_iterator it = crc_list.find(uid);
	if (it != crc_list.end())
	{
		switch (mode)
		{
		case PLAYER:
		{
			const boost::uint32_t crc = calculate_file_crc(filename);
			if (it->second->player != crc)
				create_message(it->second->name, mode);
			break;
		}
		case TEXTOBJS:
		{
			const boost::uint32_t crc = calculate_file_crc(filename);
			if (it->second->textobjs != crc)
				create_message(it->second->name, mode);
			break;
		}
		case TIMEOBJS:
		{
			const boost::uint32_t crc = calculate_file_crc(filename);
			if (it->second->timeobjs != crc)
				create_message(it->second->name, mode);
			break;
		}
		case UPDATE_PLAYER:
			it->second->player = calculate_file_crc(filename);
			it->second->name = GetNameByUnique(uid);
			break;
		case UPDATE_TEXTOBJS:
			it->second->textobjs = calculate_file_crc(filename);
			it->second->name = GetNameByUnique(uid);
			break;
		case UPDATE_TIMEOBJS:
			it->second->timeobjs = calculate_file_crc(filename);
			it->second->name = GetNameByUnique(uid);
			break;
		default:
			add_message("SYSERROR: �� �� ������ ���� ���� �������, uid: %ld, mode: %d, func: %s",
						uid, mode, __func__);
			return;
		}
	}
	else
	{
		CRCListPtr tmp_crc(new PlayerCRC);
		tmp_crc->name = GetNameByUnique(uid);
		switch (mode)
		{
		case PLAYER:
		case UPDATE_PLAYER:
			tmp_crc->player = calculate_file_crc(filename);
			break;
		case TEXTOBJS:
		case UPDATE_TEXTOBJS:
			tmp_crc->textobjs = calculate_file_crc(filename);
			break;
		case TIMEOBJS:
		case UPDATE_TIMEOBJS:
			tmp_crc->timeobjs = calculate_file_crc(filename);
			break;
		default:
			add_message("SYSERROR: �� �� ������ ���� ���� �������2, mode: %d, func: %s",
					mode, __func__);
			break;
		}
		crc_list[uid] = tmp_crc;
	}

	if (mode >= UPDATE_PLAYER)
		need_save = true;
}

/**
* ����� ���� ������� ���� �� show crc.
*/
void show(CHAR_DATA *ch)
{
	if (message.empty())
		send_to_char("����� ������ �� �����������...\r\n", ch);
	else
		send_to_char(message.c_str(), ch);
}

} // namespace FileCRC
