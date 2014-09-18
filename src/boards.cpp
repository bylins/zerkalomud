/* ****************************************************************************
* File: boards.cpp                                             Part of Bylins *
* Usage: Handling of multiple bulletin boards                                 *
* (c) 2005 Krodo                                                              *
******************************************************************************/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "conf.h"
#include "house.h"
#include "screen.h"
#include "comm.h"
#include "privilege.hpp"
#include "boards.h"
#include "char.hpp"
#include "modify.h"
#include "room.hpp"

const int GENERAL_BOARD = 0;  // �����
const int NEWS_BOARD = 1;  // �������
const int IDEA_BOARD = 2;  // ����
const int ERROR_BOARD = 3;  // ������
const int GODNEWS_BOARD = 4;  // ������� (������ ��� �����)
const int GODGENERAL_BOARD = 5;  // ����� (������ ��� �����)
const int GODBUILD_BOARD = 6;  // ������� (������ ��� �����)
const int GODCODE_BOARD = 7; // ������ (������ ��� �����)
const int GODPUNISH_BOARD = 8;  // ��������� (������ ��� �����)
const int PERS_BOARD = 9;  // ������������ (������ ��� �����)
const int CLAN_BOARD = 10; // ��������
const int CLANNEWS_BOARD = 11; // �������� �������
const int NOTICE_BOARD = 12;
const int MISPRINT_BOARD = 13;
// �� �������� ��������� BOARD_TOTAL � boards.h

// ������������ ������ ���������
const int MAX_MESSAGE_LENGTH = 4096;
// ���.����� ��� ����� �� ����� ������
const int MIN_WRITE_LEVEL = 6;
// ������������ ���-�� ��������� �� ����� �����
const unsigned int MAX_BOARD_MESSAGES = 200;
// ������������ ���-�� ��������� � ���������/�������
const unsigned int MAX_REPORT_MESSAGES = 999;

BoardListType Board::BoardList;

Board::Board() : type(0), lastWrite(0), clanRent(0), persUnique(0)
{
	// ��� ����� ������ � �� ����
}

void Board::create_board(int type, const std::string &name, const std::string &desc, const std::string &file)
{
	BoardPtr tmp_board(new Board);
	tmp_board->type = type;
	tmp_board->name = name;
	tmp_board->desc = desc;
	tmp_board->file = file;
	tmp_board->Load();
	Board::BoardList.push_back(tmp_board);
}

// �������� ���� �����, ����� �������� � ������������
void Board::BoardInit()
{
	Board::BoardList.clear();

	create_board(GENERAL_BOARD, "����", "�������� �������", ETC_BOARD"general.board");
	create_board(NEWS_BOARD, "�������", "������ � ������� �����", ETC_BOARD"news.board");
	create_board(IDEA_BOARD, "����", "���� � �� ����������", ETC_BOARD"idea.board");
	create_board(ERROR_BOARD, "������", "��������� �� ������� � ����", ETC_BOARD"error.board");
	create_board(GODNEWS_BOARD, "GodNews", "������������ �������", ETC_BOARD"god-news.board");
	create_board(GODGENERAL_BOARD, "��������", "������������ �������� �������", ETC_BOARD"god-general.board");
	create_board(GODBUILD_BOARD, "������", "������� ��������", ETC_BOARD"god-build.board");
	create_board(GODCODE_BOARD, "�����", "������� �������", ETC_BOARD"god-code.board");
	create_board(GODPUNISH_BOARD, "���������", "����������� � ����������", ETC_BOARD"god-punish.board");
	create_board(NOTICE_BOARD, "������", "��������� �� �������������", ETC_BOARD"notice.board");
	create_board(MISPRINT_BOARD, "��������", "�������� � ������� ��������", ETC_BOARD"misprint.board");
}

// ����/������ �������� �����
void Board::ClanInit()
{
	// ������ ��� �������
	for (BoardListType::iterator board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
	{
		if ((*board)->type == CLAN_BOARD || (*board)->type == CLANNEWS_BOARD)
		{
			Board::BoardList.erase(board);
			board = Board::BoardList.begin();
		}
	}

	for (ClanListType::const_iterator clan = Clan::ClanList.begin(); clan != Clan::ClanList.end(); ++clan)
	{
		std::string name = (*clan)->GetAbbrev();
		CreateFileName(name);
		// ������ �������� �����
		BoardPtr board(new Board);
		board->type = CLAN_BOARD;
		board->name = "������";
		board->desc = "�������� ������ ������� ";
		board->desc += (*clan)->GetAbbrev();
		board->clanRent = (*clan)->GetRent();
		board->file = LIB_HOUSE + name + "/" + name + ".board";
		board->Load();
		Board::BoardList.push_back(board);
		// ������ �������� �������
		BoardPtr newsboard(new Board);
		newsboard->type = CLANNEWS_BOARD;
		newsboard->name = "���������";
		newsboard->desc = "������� ������� ";
		newsboard->desc += (*clan)->GetAbbrev();
		newsboard->clanRent = (*clan)->GetRent();
		newsboard->file = LIB_HOUSE + name + "/" + name + "-news.board";
		newsboard->Load();
		Board::BoardList.push_back(newsboard);
	}
}

// ������ ��� �������
void Board::clear_god_boards()
{
	for (BoardListType::iterator board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
	{
		if ((*board)->type == PERS_BOARD)
		{
			Board::BoardList.erase(board);
			board = Board::BoardList.begin();
		}
	}
}

// ������� ������� ����
void Board::init_god_board(long uid, std::string name)
{
	BoardPtr board(new Board);
	board->type = PERS_BOARD;
	board->name = "�������";
	board->desc = "��� ������ ��� �������";
	board->persUnique = uid;
	board->persName = name;
	// ������� ��� � ������ �� �����������
	std::string tmp_name = name;
	CreateFileName(tmp_name);
	board->file = ETC_BOARD + tmp_name + ".board";
	board->Load();
	Board::BoardList.push_back(board);
}

/**
* ������ ���� ����� �����.
*/
void Board::reload_all()
{
	BoardInit();
	Privilege::load_god_boards();
	ClanInit();
}

// ���������� �����, ���� ��� ����������
void Board::Load()
{
	std::ifstream file((this->file).c_str());
	if (!file.is_open())
	{
		this->Save();
		return;
	}

	std::string buffer;
	while (file >> buffer)
	{
		if (buffer == "Message:")
		{
			MessagePtr message(new Message);
			file >> message->num;
			file >> message->author;
			file >> message->unique;
			file >> message->level;
			file >> message->date;
			file >> message->rank;

			ReadEndString(file);
			std::getline(file, message->subject, '~');
			ReadEndString(file);
			std::getline(file, message->text, '~');
			// �� �������� ������� �� �� ��� ���������
			if (message->author.empty() || !message->unique || !message->level || !message->date)
				continue;
			this->messages.push_back(message);
		}
		else if (buffer == "Type:")
			file >> this->type;
		else if (buffer == "Clan:")
			file >> this->clanRent;
		else if (buffer == "PersUID:")
			file >> this->persUnique;
		else if (buffer == "PersName:")
			file >> this->persName;
	}
	file.close();

	this->Save();
}

// ��������� �����
void Board::Save()
{
	std::ofstream file((this->file).c_str());
	if (!file.is_open())
	{
		log("Error open file: %s! (%s %s %d)", (this->file).c_str(), __FILE__, __func__, __LINE__);
		return;
	}

	file << "Type: " << this->type << " "
	<< "Clan: " << this->clanRent << " "
	<< "PersUID: " << this->persUnique << " "
	<< "PersName: " << (this->persName.empty() ? "none" : this->persName) << "\n";
	for (MessageListType::const_iterator message = this->messages.begin(); message != this->messages.end(); ++message)
	{
		file << "Message: " << (*message)->num << "\n"
		<< (*message)->author << " "
		<< (*message)->unique << " "
		<< (*message)->level << " "
		<< (*message)->date << " "
		<< (*message)->rank << "\n"
		<< (*message)->subject << "~\n"
		<< (*message)->text << "~\n";
	}
	file.close();
}

// ������� ��������� ���������
void Board::ShowMessage(CHAR_DATA * ch, MessagePtr message)
{
	std::ostringstream buffer;
	char timeBuf[17];
	strftime(timeBuf, sizeof(timeBuf), "%H:%M %d-%m-%Y", localtime(&message->date));

	buffer << "[" << message->num + 1 << "] "
	<< timeBuf << " "
	<< "(" << message->author << ") :: "
	<< message->subject << "\r\n\r\n"
	<< message->text << "\r\n";

	page_string(ch->desc, buffer.str());
}

void set_last_read(CHAR_DATA *ch, int type, time_t date)
{
	if (GET_BOARD_DATE(ch, type) < date)
		GET_BOARD_DATE(ch, type) = date;
}

bool Board::can_write(CHAR_DATA *ch)
{
	if (this->Access(ch) < 2
			|| PLR_FLAGGED(ch, PLR_HELLED)
			|| PLR_FLAGGED(ch, PLR_NAMED)
			|| PLR_FLAGGED(ch, PLR_DUMB))
	{
		send_to_char("� ��� ��� ����������� ������ � ���� ������.\r\n", ch);
		return false;
	}
	return true;
}

ACMD(DoBoard)
{
	if (!ch->desc)
		return;
	if (AFF_FLAGGED(ch, AFF_BLIND))
	{
		send_to_char("�� ���������!\r\n", ch);
		return;
	}

	BoardListType::const_iterator board;
	// �� � � ��������� ������� ����� ���� �� ���� � ��������������� �����, �� ������ ��� ��������
	for (board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
	{
		if ((*board)->type == subcmd && (*board)->Access(ch))
			break;
	}
	if (board == Board::BoardList.end())
	{
		send_to_char("���� ?\r\n", ch);
		return;
	}

	std::string buffer, buffer2 = argument;
	time_t const date = GET_BOARD_DATE(ch, (*board)->type);
	GetOneParam(buffer2, buffer);
	boost::trim_if(buffer2, boost::is_any_of(std::string(" \'")));
	// ������ �������� � buffer, ������ � buffer2

	if (CompareParam(buffer, "������") || CompareParam(buffer, "list"))
	{
		if ((*board)->Access(ch) == 2)
		{
			send_to_char("� ��� ��� ����������� ������ ���� ������.\r\n", ch);
			return;
		}
		std::ostringstream body;
		body << "��� �����, �� ������� ������ ������ �������� �������� ���� IMHO.\r\n"
		<< "������: ������/�������� <����� ���������>, ������ <���� ���������>.\r\n";
		if ((*board)->messages.empty())
		{
			body << "����� ���� �� ���������, ����� �����.\r\n";
			send_to_char(body.str(), ch);
			return;
		}
		else
			body << "����� ���������: " << (*board)->messages.size() << "\r\n";
		char timeBuf[17];
		for (MessageListType::reverse_iterator message = (*board)->messages.rbegin(); message != (*board)->messages.rend(); ++message)
		{
			if ((*message)->date > date)
				body << CCWHT(ch, C_NRM); // ��� ��������� ����� ������ ������-�����

			strftime(timeBuf, sizeof(timeBuf), "%H:%M %d-%m-%Y", localtime(&(*message)->date));
			body << "[" << (*message)->num + 1 << "] " << timeBuf << " "
			<< "(" << (*message)->author << ") :: "
			<< (*message)->subject << "\r\n" <<  CCNRM(ch, C_NRM);
		}
		page_string(ch->desc, body.str());
		return;
	}

	// ��� ������ ������� ��� '������' ��� ����� - ���������� ������ ������������� ���������, ���� ����� ����
	if (buffer.empty() || ((CompareParam(buffer, "������") || CompareParam(buffer, "read")) && buffer2.empty()))
	{
		if ((*board)->Access(ch) == 2)
		{
			send_to_char("� ��� ��� ����������� ������ ���� ������.\r\n", ch);
			return;
		}
		// ������� ���� � ��������� ��������
		if (((*board)->type == NEWS_BOARD || (*board)->type == GODNEWS_BOARD) && !PRF_FLAGGED(ch, PRF_NEWS_MODE))
		{
			std::ostringstream body;
			char timeBuf[17];
			MessageListType::reverse_iterator message;
			for (message = (*board)->messages.rbegin(); message != (*board)->messages.rend(); ++message)
			{
				strftime(timeBuf, sizeof(timeBuf), "%d-%m-%Y", localtime(&(*message)->date));
				if ((*message)->date > date)
					body << CCWHT(ch, C_NRM); // ����� ���������
				body << timeBuf << CCNRM(ch, C_NRM) << "\r\n"
				<< (*message)->text;
			}
			set_last_read(ch, (*board)->type, time(0));
			page_string(ch->desc, body.str());
			return;
		}
		// ��������� � ��������� ��������
		if ((*board)->type == CLANNEWS_BOARD && !PRF_FLAGGED(ch, PRF_NEWS_MODE))
		{
			std::ostringstream body;
			MessageListType::reverse_iterator message;
			for (message = (*board)->messages.rbegin(); message != (*board)->messages.rend(); ++message)
			{
				if ((*message)->date > date)
					body << CCWHT(ch, C_NRM); // ����� ���������
				body << "[" << (*message)->author << "] " << CCNRM(ch, C_NRM) << (*message)->text;
			}
			set_last_read(ch, (*board)->type, time(0));
			page_string(ch->desc, body.str());
			return;
		}

		for (MessageListType::const_iterator message = (*board)->messages.begin(); message != (*board)->messages.end(); ++message)
		{
			if ((*message)->date > date)
			{
				Board::ShowMessage(ch, *message);
				set_last_read(ch, (*board)->type, (*message)->date);
				return;
			}
		}
		send_to_char("� ��� ��� ������������� ���������.\r\n", ch);
		return;
	}

	unsigned num = 0;
	// ���� ����� ������� ����� ����� ��� '������ �����' - �������� �������� ��� �������, ��� ��������� - ����� �� ��������� '������ �����...' ��� ������
	if (is_number(buffer.c_str()) || ((CompareParam(buffer, "������") || CompareParam(buffer, "read")) && is_number(buffer2.c_str())))
	{
		if ((*board)->Access(ch) == 2)
		{
			send_to_char("� ��� ��� ����������� ������ ���� ������.\r\n", ch);
			return;
		}
		if (CompareParam(buffer, "������"))
			num = atoi(buffer2.c_str());
		else
			num = atoi(buffer.c_str());
		if (num <= 0 || num > (*board)->messages.size())
		{
			send_to_char("��� ��������� ����� ��� ������ ����������.\r\n", ch);
			return;
		}
		num = (*board)->messages.size() - num;
		Board::ShowMessage(ch, (*board)->messages[num]);
		set_last_read(ch, (*board)->type, (*board)->messages[num]->date);
		return;
	}

	if (CompareParam(buffer, "������") || CompareParam(buffer, "write"))
	{
		if (!(*board)->can_write(ch))
			return;
		if (!IS_GOD(ch) && (*board)->lastWrite == GET_UNIQUE(ch)
				&& (*board)->type != CLAN_BOARD
				&& (*board)->type != CLANNEWS_BOARD
				&& (*board)->type != PERS_BOARD
				&& !Privilege::check_flag(ch, Privilege::BOARDS))
		{
			send_to_char("�� �� ���� ������ ��� ������ ����.\r\n", ch);
			return;
		}

		// ��������� �������� �� ������� �����
		std::string name = GET_NAME(ch);
		if (((*board)->type == NEWS_BOARD || (*board)->type == NOTICE_BOARD) && Privilege::check_flag(ch, Privilege::KRODER))
		{
			GetOneParam(buffer2, buffer);
			if (buffer.empty())
			{
				send_to_char("������� ���� �� ���, �� ���� ����� ������������ ���������.\r\n", ch);
				return;
			}
			name = buffer;
		}

		// ������� �������
		MessagePtr tempMessage(new Message);
		tempMessage->author = name;
		tempMessage->unique = GET_UNIQUE(ch);
		// ��� ����� ����� �������� � ������������ ����� ����� ������ (��� ��������� ������� ���-��)
		Privilege::check_flag(ch, Privilege::KRODER) ? tempMessage->level = LVL_IMPL : tempMessage->level = GET_LEVEL(ch);
		// �������� ��� ����
		if (CLAN(ch))
			tempMessage->rank = CLAN_MEMBER(ch)->rank_num;
		else
			tempMessage->rank = 0;
		boost::trim(buffer2);
		// ������� ��� ��������������� ����
		if (buffer2.length() > 40)
		{
			buffer2.erase(40, std::string::npos);
			send_to_char(ch, "���� ��������� ��������� �� '%s'.\r\n", buffer2.c_str());
		}
		if (subcmd == ERROR_BOARD)
			buffer2 += " Room: [" + boost::lexical_cast<std::string>(GET_ROOM_VNUM(ch->in_room)) + "]";
		tempMessage->subject = buffer2;
		// ���� ����� � ����� ����� ��� �� ����� ����������
		ch->desc->message = tempMessage;
		ch->desc->board = *board;

		char **text;
		CREATE(text, char *, 1);
		send_to_char(ch, "������ ������ ���������.  (/s �������� /h ������)\r\n", buffer2.c_str());
		STATE(ch->desc) = CON_WRITEBOARD;
		string_write(ch->desc, text, MAX_MESSAGE_LENGTH, 0, NULL);
	}

	if (CompareParam(buffer, "��������") || CompareParam(buffer, "remove"))
	{
		if (!is_number(buffer2.c_str()))
		{
			send_to_char("������� ���������� ����� ���������.\r\n", ch);
			return;
		}
		num = atoi(buffer2.c_str());
		if (num <= 0 || num > (*board)->messages.size())
		{
			send_to_char("��� ��������� ����� ��� ������ ����������.\r\n", ch);
			return;
		}
		num = (*board)->messages.size() - num;
		set_last_read(ch, (*board)->type, (*board)->messages[num]->date);
		// ��� �� ����� �������� ����� ������� (�� ������/�����), ��� ������ ����
		if ((*board)->Access(ch) < 4)
		{
			if ((*board)->messages[num]->unique != GET_UNIQUE(ch))
			{
				send_to_char("� ��� ��� ����������� ������� ��� ���������.\r\n", ch);
				return;
			}
		}
		else if ((*board)->type != CLAN_BOARD
				 && (*board)->type != CLANNEWS_BOARD
				 && (*board)->type != PERS_BOARD
				 && !Privilege::check_flag(ch, Privilege::KRODER)
				 && GET_LEVEL(ch) < (*board)->messages[num]->level)
		{
			// ��� ������� ����� ������� ������ (��� �������� �����)
			// �������� ����, � ������������ ������ ���
			send_to_char("� ��� ��� ����������� ������� ��� ���������.\r\n", ch);
			return;
		}
		else if ((*board)->type == CLAN_BOARD || (*board)->type == CLANNEWS_BOARD)
		{
			// � ���� ���������� �� �������, �� ����� ������� ����� �����, ���� ���� ������ ����� �� ��� ����
			if (CLAN_MEMBER(ch)->rank_num > (*board)->messages[num]->rank)
			{
				send_to_char("� ��� ��� ����������� ������� ��� ���������.\r\n", ch);
				return;
			}
		}
		// ���������� ������� � ����������� ����� �������
		(*board)->messages.erase((*board)->messages.begin() + num);
		int count = 0;
		for (MessageListType::reverse_iterator it = (*board)->messages.rbegin(); it != (*board)->messages.rend(); ++it)
			(*it)->num = count++;
		if ((*board)->lastWrite == GET_UNIQUE(ch))
			(*board)->lastWrite = 0;
		(*board)->Save();
		send_to_char("��������� �������.\r\n", ch);
		return;
	}
}

ACMD(DoBoardList)
{
	if (IS_NPC(ch))
		return;

	std::ostringstream buffer;
	buffer << " ���         ���  �����|�����                        ��������  ������\r\n"
	<< " ===  ==========  ===========  ==============================  ======\r\n";
	boost::format boardFormat(" %2d)  %10s  [%3d|%3d]    %30s  %6s\r\n");

	int num = 1;
	std::string access;
	for (BoardListType::const_iterator board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
	{
		int unread = 0;
		int cansee = 1;
		switch ((*board)->Access(ch))
		{
		case 0:
			cansee = 0;
			break;
		case 1:
			access = "������";
			break;
		case 2:
//			access = "������";
			cansee = 0;
			break;
		case 3:
			access = "������";
			break;
		case 4:
			access = "������";
			break;
		}
		if (!cansee)
			continue;
		if ((*board)->Access(ch) == 2)
			unread = 0;
		else
		{
			for (MessageListType::reverse_iterator message = (*board)->messages.rbegin(); message != (*board)->messages.rend(); ++message)
			{
				if ((*message)->date > GET_BOARD_DATE(ch, (*board)->type))
					++unread;
				else
					break;
			}
		}
		buffer << boardFormat % num % (*board)->name % unread % (*board)->messages.size() % (*board)->desc % access;
		++num;
	}
	send_to_char(buffer.str(), ch);
}

// 0 - ��� �������, 1 - ������, 2 - ������, 3 - ������/������, 4 - ������(� ��������� �����)
int Board::Access(CHAR_DATA * ch)
{
	switch (this->type)
	{
	case GENERAL_BOARD:
	case IDEA_BOARD:
		// ��� ������, ����� � ���.������, 32 � �� ���������� ������
		if (IS_GOD(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		if (GET_LEVEL(ch) < MIN_WRITE_LEVEL && !GET_REMORT(ch))
			return 1;
		else
			return 3;
	case ERROR_BOARD:
		// ��� ����� � ���.������, 34 � �� ���������� ������
		if (IS_IMPL(ch)
			|| Privilege::check_flag(ch, Privilege::BOARDS)
			|| Privilege::check_flag(ch, Privilege::MISPRINT))
		{
			return 4;
		}
		if (GET_LEVEL(ch) < MIN_WRITE_LEVEL && !GET_REMORT(ch))
			return 0;
		else
			return 2;
	case NEWS_BOARD:
		// ��� ������, 34 � �� ���������� ������
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else
			return 1;
	case GODNEWS_BOARD:
		// 32 ������, 34 � �� ���������� ������
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else if (IS_GOD(ch))
			return 1;
		else
			return 0;
	case GODGENERAL_BOARD:
		// 32 ������/�����, 34 ������
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else if (IS_GOD(ch))
			return 3;
		else
			return 0;
	case GODBUILD_BOARD:
	case GODCODE_BOARD:
		// 33 ������/�����, 34 � �� ���������� ������
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else if (IS_GRGOD(ch))
			return 3;
		else
			return 0;
	case GODPUNISH_BOARD:
		// 32 ������/�����, 34 ������
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else if (IS_GOD(ch))
			return 3;
		else
			return 0;
	case PERS_BOARD:
		if (IS_GOD(ch) && this->persUnique == GET_UNIQUE(ch) && CompareParam(this->persName, GET_NAME(ch), 1))
			return 4;
		else
			return 0;
	case CLAN_BOARD:
		// �� ����-�������� ���������� ���, ��� ������ ����� ��� ������
		if (CLAN(ch) && CLAN(ch)->GetRent() == this->clanRent)
		{
			// �������
			if (CLAN(ch)->CheckPrivilege(CLAN_MEMBER(ch)->rank_num, MAY_CLAN_NEWS))
				return 4;
			else
				return 3;
		}
		else
			return 0;
	case CLANNEWS_BOARD:
		// ���������� �� �����, �������� ����� ������ ���, ������ ����� �� ����������, ������� ����� ������� �����
		if (CLAN(ch) && CLAN(ch)->GetRent() == this->clanRent)
		{
			if (CLAN(ch)->CheckPrivilege(CLAN_MEMBER(ch)->rank_num, MAY_CLAN_NEWS))
				return 4;
			return 1;
		}
		else
			return 0;
	case NOTICE_BOARD:
		// 32+ � �� ���������� ������, ��������� ������ ������
		if (IS_GOD(ch) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else
			return 1;
	case MISPRINT_BOARD:
		// �� ���������� (������ olc) � 34 ������, ��������� ������ ������ � ��� ������/�����
		if (IS_IMPL(ch) || Privilege::check_flag(ch, Privilege::MISPRINT) || Privilege::check_flag(ch, Privilege::BOARDS))
			return 4;
		else if (GET_LEVEL(ch) < MIN_WRITE_LEVEL && !GET_REMORT(ch))
			return 0;
		else
			return 2;
	default:
		log("Error board type! (%s %s %d)", __FILE__, __func__, __LINE__);
		return 0;
	}
	return 0;
}

// ����� �� ������������ ����� �������� ������ �� ������ ����� � ����� ����������
SPECIAL(Board::Special)
{
	OBJ_DATA *board = (OBJ_DATA *) me;
	if (!ch->desc)
		return 0;
	// ������ ���������
	std::string buffer = argument, buffer2;
	GetOneParam(buffer, buffer2);
	boost::trim_if(buffer, boost::is_any_of(std::string(" \'")));
	if ((CMD_IS("��������") || CMD_IS("���������") || CMD_IS("look") || CMD_IS("examine")
			|| CMD_IS("������") || CMD_IS("read")) && (CompareParam(buffer2, "�����") || CompareParam(buffer2, "board")))
	{
		// ��� ������ ������ ��� ������ '�� �� �����' � �������� ��������� � ��� �����
		if (buffer2.empty()
				|| (buffer.empty() && !CompareParam(buffer2, "�����") && !CompareParam(buffer2, "board"))
				|| (!buffer.empty() && !CompareParam(buffer, "�����") && !CompareParam(buffer2, "board")))
			return 0;

		// ����� �����
		if (board->item_number == real_object(GENERAL_BOARD_OBJ))
		{
			char *temp_arg = str_dup("������");
			DoBoard(ch, temp_arg, 0, GENERAL_BOARD);
			free(temp_arg);
			return 1;
		}
		// ����� ���-�����
		if (board->item_number == real_object(GODGENERAL_BOARD_OBJ))
		{
			char *temp_arg = str_dup("������");
			DoBoard(ch, temp_arg, 0, GODGENERAL_BOARD);
			free(temp_arg);
			return 1;
		}
		// ����� ���������
		if (board->item_number == real_object(GODPUNISH_BOARD_OBJ))
		{
			char *temp_arg = str_dup("������");
			DoBoard(ch, temp_arg, 0, GODPUNISH_BOARD);
			free(temp_arg);
			return 1;
		}
		// ����� ��������
		if (board->item_number == real_object(GODBUILD_BOARD_OBJ))
		{
			char *temp_arg = str_dup("������");
			DoBoard(ch, temp_arg, 0, GODBUILD_BOARD);
			free(temp_arg);
			return 1;
		}
		// ����� �������
		if (board->item_number == real_object(GODCODE_BOARD_OBJ))
		{
			char *temp_arg = str_dup("������");
			DoBoard(ch, temp_arg, 0, GODCODE_BOARD);
			free(temp_arg);
			return 1;
		}
		return 0;
	}

	// ��� �����
	if ((CMD_IS("������") || CMD_IS("read")) && !buffer2.empty() && isdigit(buffer2[0]))
		if (buffer2.find(".") != std::string::npos)
			return 0;

	// ����� ��������� �������� ��������� �������� ���������
	if (((CMD_IS("������") || CMD_IS("read")) && !buffer2.empty() && isdigit(buffer2[0]))
			|| CMD_IS("������") || CMD_IS("write") || CMD_IS("��������") || CMD_IS("remove"))
	{

		// ����� �����
		char *temp_arg = str_dup(cmd_info[cmd].command);
		temp_arg = str_add(temp_arg, argument);
		if (board->item_number == real_object(GENERAL_BOARD_OBJ))
		{
			DoBoard(ch, temp_arg, 0, GENERAL_BOARD);
			free(temp_arg);
			return 1;
		}
		if (board->item_number == real_object(GODGENERAL_BOARD_OBJ))
		{
			DoBoard(ch, temp_arg, 0, GODGENERAL_BOARD);
			free(temp_arg);
			return 1;
		}
		if (board->item_number == real_object(GODPUNISH_BOARD_OBJ))
		{
			DoBoard(ch, temp_arg, 0, GODPUNISH_BOARD);
			free(temp_arg);
			return 1;
		}
		if (board->item_number == real_object(GODBUILD_BOARD_OBJ))
		{
			DoBoard(ch, temp_arg, 0, GODBUILD_BOARD);
			free(temp_arg);
			return 1;
		}
		if (board->item_number == real_object(GODCODE_BOARD_OBJ))
		{
			DoBoard(ch, temp_arg, 0, GODCODE_BOARD);
			free(temp_arg);
			return 1;
		}
		free(temp_arg);
	}
	return 0;
}

// ������� ��� ������ � ���� ���� � ����� ���������� �� ������
void Board::LoginInfo(CHAR_DATA * ch)
{
	std::ostringstream buffer, news;
	bool has_message = 0;
	buffer << "\r\n��� ������� ���������:\r\n";

	for (BoardListType::const_iterator board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
	{
		int unread = 0;
		// ����� �� ����� ��� ����� ������ ������, �������� ��� �� ������
		if (!(*board)->Access(ch) || (*board)->Access(ch) == 2 || (*board)->type == MISPRINT_BOARD)
			continue;

		for (MessageListType::reverse_iterator message = (*board)->messages.rbegin(); message != (*board)->messages.rend(); ++message)
		{
			if ((*message)->date > GET_BOARD_DATE(ch, (*board)->type))
				++unread;
			else
				break;
		}

		if (unread)
		{
			has_message = 1;
			if ((*board)->type == NEWS_BOARD || (*board)->type == GODNEWS_BOARD || (*board)->type == CLANNEWS_BOARD)
				news << std::setw(4) << unread << " � ������� '" << (*board)->desc << "' " << CCWHT(ch, C_NRM) << "(" << (*board)->name << ")" << CCNRM(ch, C_NRM) << ".\r\n";
			else
				buffer << std::setw(4) << unread << " � ������� '" << (*board)->desc << "' " << CCWHT(ch, C_NRM) << "(" << (*board)->name << ")" << CCNRM(ch, C_NRM) << ".\r\n";
		}
	}

	if (has_message)
	{
		buffer << news.str();
		send_to_char(buffer.str(), ch);
	}
}

ACMD(report_on_board)
{
	if (IS_NPC(ch)) return;
	skip_spaces(&argument);
	delete_doubledollar(argument);

	if (!*argument)
	{
		send_to_char("������ ��������� ���������.\r\n", ch);
		return;
	}

	BoardListType::const_iterator board;
	for (board = Board::BoardList.begin(); board != Board::BoardList.end(); ++board)
		if ((*board)->type == subcmd)
			break;
	if (board == Board::BoardList.end())
	{
		send_to_char("����� ���� �� �������... :/\r\n", ch);
		return;
	}
	if (!(*board)->can_write(ch))
		return;
	if ((*board)->messages.size() >= MAX_REPORT_MESSAGES)
	{
		send_to_char("��, ��������� ����� �������, ��� ���� ����������. ��������� �� ���� �����!\r\n", ch);
		return;
	}
	// ������� ������� (TODO: �������� � ��������� �� �����, ���� �� �������)
	MessagePtr temp_message(new Message);
	temp_message->author = GET_NAME(ch) ? GET_NAME(ch) : "����������";
	temp_message->unique = GET_UNIQUE(ch);
	// ��� ����� ����� �������� � ������������ ����� ����� ������ (��� ��������� ������� ���-��)
	temp_message->level = GET_LEVEL(ch);
	temp_message->rank = 0;
	temp_message->subject = " Room: [" + boost::lexical_cast<std::string>(GET_ROOM_VNUM(ch->in_room)) + "]";
	temp_message->text = argument;
	temp_message->date = time(0);
	(*board)->messages.push_back(temp_message);
	int count = 0;
	for (MessageListType::reverse_iterator it = (*board)->messages.rbegin(); it != (*board)->messages.rend(); ++it)
		(*it)->num = count++;
	(*board)->Save();
	// ��������� ����� ������ � ����� ������� �� ����� ��-����� ������ �� �����,
	// �� � ��� ����� � ���� ����, �� ���� ����������� - ����� ����� ������� ��������
	send_to_char("��������. ������� ����������.\r\n" "                        ����.\r\n", ch);
}
