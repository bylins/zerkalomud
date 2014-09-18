// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#include <sstream>
#include <bitset>
#include <boost/lexical_cast.hpp>
#include "char_player.hpp"
#include "utils.h"
#include "db.h"
#include "dg_scripts.h"
#include "handler.h"
#include "boards.h"
#include "file_crc.hpp"
#include "spells.h"
#include "constants.h"
#include "skills.h"
#include "im.h"
#include "olc.h"
#include "comm.h"
#include "pk.h"
#include "diskio.h"
#include "interpreter.h"
#include "genchar.h"
#include "AffectHandler.hpp"
#include "player_races.hpp"
#include "morph.hpp"

void tascii(int *pointer, int num_planes, char *ascii);
int level_exp(CHAR_DATA * ch, int level);

Player::Player()
	:
	pfilepos_(-1),
	was_in_room_(NOWHERE),
	from_room_(0),
	answer_id_(NOBODY),
	motion_(true)
{
	for (int i = 0; i < START_STATS_TOTAL; ++i)
		start_stats_.at(i) = 0;
	set_morph(NormalMorph::GetNormalMorph(this));
}

int Player::get_pfilepos() const
{
	return pfilepos_;
}

void Player::set_pfilepos(int pfilepos)
{
	pfilepos_ = pfilepos;
}

room_rnum Player::get_was_in_room() const
{
	return was_in_room_;
}

void Player::set_was_in_room(room_rnum was_in_room)
{
	was_in_room_ = was_in_room;
}

std::string const & Player::get_passwd() const
{
	return passwd_;
}

void Player::set_passwd(std::string const & passwd)
{
	passwd_ = passwd;
}

void Player::remort()
{
	quested_.clear();
	mobmax_.clear();
}

void Player::reset()
{
	remember_.reset();
	last_tell_ = "";
	answer_id_ = NOBODY;
}

room_rnum Player::get_from_room() const
{
	return from_room_;
}

void Player::set_from_room(room_rnum from_room)
{
	from_room_ = from_room;
}

int Player::get_start_stat(int stat_num)
{
	int stat = 0;
	try
	{
		stat = start_stats_.at(stat_num);
	}
	catch (...)
	{
		log("SYSERROR : bad start_stat %d (%s %s %d)", stat_num, __FILE__, __func__, __LINE__);
	}
	return stat;
}

void Player::set_start_stat(int stat_num, int number)
{
	try
	{
		start_stats_.at(stat_num) = number;
	}
	catch (...)
	{
		log("SYSERROR : bad start_stat num %d (%s %s %d)", stat_num, __FILE__, __func__, __LINE__);
	}
}

void Player::set_answer_id(int id)
{
	answer_id_ = id;
}

int Player::get_answer_id() const
{
	return answer_id_;
}

void Player::remember_add(std::string text, int flag)
{
	remember_.add_str(text, flag);
}

std::string Player::remember_get(int flag) const
{
	return remember_.get_text(flag);
}

bool Player::remember_set_num(unsigned int num)
{
	return remember_.set_num_str(num);
}

unsigned int Player::remember_get_num() const
{
	return remember_.get_num_str();
}

void Player::set_last_tell(const char *text)
{
	if (text)
	{
		last_tell_ = text;
	}
}

std::string const & Player::get_last_tell()
{
	return last_tell_;
}

void Player::quested_add(CHAR_DATA *ch, int vnum, char *text)
{
	quested_.add(ch, vnum, text);
}

bool Player::quested_remove(int vnum)
{
	return quested_.remove(vnum);
}

bool Player::quested_get(int vnum) const
{
	return quested_.get(vnum);
}

std::string Player::quested_get_text(int vnum) const
{
	return quested_.get_text(vnum);
}

std::string Player::quested_print() const
{
	return quested_.print();
}

void Player::quested_save(FILE *saved) const
{
	quested_.save(saved);
}

int Player::mobmax_get(int vnum) const
{
	return mobmax_.get_kill_count(vnum);
}

void Player::mobmax_add(CHAR_DATA *ch, int vnum, int count, int level)
{
	mobmax_.add(ch, vnum, count, level);
}

void Player::mobmax_load(CHAR_DATA *ch, int vnum, int count, int level)
{
	mobmax_.load(ch, vnum, count, level);
}

void Player::mobmax_remove(int vnum)
{
	mobmax_.remove(vnum);
}

void Player::mobmax_save(FILE *saved) const
{
	mobmax_.save(saved);
}

void Player::dps_add_dmg(int type, int dmg, int over_dmg, CHAR_DATA *ch)
{
	dps_.add_dmg(type, ch, dmg, over_dmg);
}

void Player::dps_clear(int type)
{
	dps_.clear(type);
}

void Player::dps_print_stats(CHAR_DATA *coder)
{
	dps_.print_stats(this, coder);
}

void Player::dps_print_group_stats(CHAR_DATA *ch, CHAR_DATA *coder)
{
	dps_.print_group_stats(ch, coder);
}

/**
* ��� dps_copy.
*/
void Player::dps_set(DpsSystem::Dps *dps)
{
	dps_ = *dps;
}

/**
* ����� ������ ��� ����������� ����� ����� ���� ��� �������� ������.
*/
void Player::dps_copy(CHAR_DATA *ch)
{
	ch->dps_set(&dps_);
}

void Player::dps_end_round(int type, CHAR_DATA *ch)
{
	dps_.end_round(type, ch);
}

void Player::dps_add_exp(int exp, bool battle)
{
	if (battle)
	{
		dps_.add_battle_exp(exp);
	}
	else
	{
		dps_.add_exp(exp);
	}
}

void Player::save_char()
{
	FILE *saved;
	char filename[MAX_STRING_LENGTH];
	int i;
	time_t li;
	AFFECT_DATA *aff, tmp_aff[MAX_AFFECT];
	OBJ_DATA *char_eq[NUM_WEARS];
	struct timed_type *skj;
	struct char_portal_type *prt;
	int tmp = time(0) - this->player_data.time.logon;
	if (!now_entrycount)
		if (IS_NPC(this) || this->get_pfilepos() < 0)
			return;

	log("Save char %s", GET_NAME(this));
	save_char_vars(this);

	/* ������ ���� � ����� ������� */
	get_filename(GET_NAME(this), filename, PLAYERS_FILE);
	if (!(saved = fopen(filename, "w")))
	{
		perror("Unable open charfile");
		return;
	}
	/* ���������� */
	/* ������� ��� ��������� �������  */
	for (i = 0; i < NUM_WEARS; i++)
	{
		if (GET_EQ(this, i))
		{
			char_eq[i] = unequip_char(this, i | 0x80);
#ifndef NO_EXTRANEOUS_TRIGGERS
			remove_otrigger(char_eq[i], this);
#endif
		}
		else
			char_eq[i] = NULL;
	}

	for (aff = this->affected, i = 0; i < MAX_AFFECT; i++)
	{
		if (aff)
		{
			if (aff->type == SPELL_ARMAGEDDON || aff->type < 1 || aff->type > LAST_USED_SPELL)
				i--;
			else
			{
				tmp_aff[i] = *aff;
				tmp_aff[i].next = 0;
			}
			aff = aff->next;
		}
		else
		{
			tmp_aff[i].type = 0;	/* Zero signifies not used */
			tmp_aff[i].duration = 0;
			tmp_aff[i].modifier = 0;
			tmp_aff[i].location = 0;
			tmp_aff[i].bitvector = 0;
			tmp_aff[i].next = 0;
		}
	}

	/*
	 * remove the affections so that the raw values are stored; otherwise the
	 * effects are doubled when the char logs back in.
	 */
	while (this->affected)
		affect_remove(this, this->affected);

	if ((i >= MAX_AFFECT) && aff && aff->next)
		log("SYSERR: WARNING: OUT OF STORE ROOM FOR AFFECTED TYPES!!!");

	// ������� ���� ����, ����������� ��� ������ ����, ��� ��� ������������� ������� ������ �� ����
	if (GET_NAME(this))
		fprintf(saved, "Name: %s\n", GET_NAME(this));
	fprintf(saved, "Levl: %d\n", GET_LEVEL(this));
	fprintf(saved, "Clas: %d\n", GET_CLASS(this));
	fprintf(saved, "UIN : %d\n", GET_UNIQUE(this));
	fprintf(saved, "LstL: %ld\n", static_cast<long int>(LAST_LOGON(this)));
	if (this->desc)//edited WorM 2010.08.27 ���������� ���� ��������� ��� ���������� � ������� �������
		strcpy(buf, this->desc->host);
	else//�� ���� ��� ������ ���� ��������� ��������� ����
	{
		li = 0;
		if (LOGON_LIST(this))
		{
			struct logon_data * cur_log = LOGON_LIST(this);
			while (cur_log)
			{
				if ((cur_log)->lasttime > li)
				{
					strcpy(buf, cur_log->ip);
					li = cur_log->lasttime;
					log("%s\r\n", buf);
				}
				cur_log = cur_log->next;
			}
		}
		else
			strcpy(buf, "Unknown");
	}
	fprintf(saved, "Host: %s\n", buf);
	free(player_table[this->get_pfilepos()].last_ip);
	player_table[this->get_pfilepos()].last_ip = str_dup(buf);
	fprintf(saved, "Id  : %ld\n", GET_IDNUM(this));
	fprintf(saved, "Exp : %ld\n", GET_EXP(this));
	if (GET_REMORT(this) > 0)
		fprintf(saved, "Rmrt: %d\n", GET_REMORT(this));
	// �����
	*buf = '\0';
	tascii(&PLR_FLAGS(this, 0), 4, buf);
	fprintf(saved, "Act : %s\n", buf);
	if (GET_EMAIL(this))//edited WorM 2010.08.27 ���������� ���� ��������� ��� ���������� � ������� �������
		fprintf(saved, "EMal: %s\n", GET_EMAIL(this));
	// ��� ����� ����������� ��������, ������ ��� ����� ���� ������ �� �����������
	fprintf(saved, "Rebt: ��������� ����� ���� ��� ������������ �� ��������\n\n");
	// ������ ����� ��� ����� � ��� �����

	if (GET_PAD(this, 0))
		fprintf(saved, "NmI : %s\n", GET_PAD(this, 0));
	if (GET_PAD(this, 0))
		fprintf(saved, "NmR : %s\n", GET_PAD(this, 1));
	if (GET_PAD(this, 0))
		fprintf(saved, "NmD : %s\n", GET_PAD(this, 2));
	if (GET_PAD(this, 0))
		fprintf(saved, "NmV : %s\n", GET_PAD(this, 3));
	if (GET_PAD(this, 0))
		fprintf(saved, "NmT : %s\n", GET_PAD(this, 4));
	if (GET_PAD(this, 0))
		fprintf(saved, "NmP : %s\n", GET_PAD(this, 5));
	if (!this->get_passwd().empty())
		fprintf(saved, "Pass: %s\n", this->get_passwd().c_str());
	if (GET_TITLE(this))
		fprintf(saved, "Titl: %s\n", GET_TITLE(this));
	if (this->player_data.description && *this->player_data.description)
	{
		strcpy(buf, this->player_data.description);
		kill_ems(buf);
		fprintf(saved, "Desc:\n%s~\n", buf);
	}
	if (POOFIN(this))
		fprintf(saved, "PfIn: %s\n", POOFIN(this));
	if (POOFOUT(this))
		fprintf(saved, "PfOt: %s\n", POOFOUT(this));
	fprintf(saved, "Sex : %d %s\n", GET_SEX(this), genders[(int) GET_SEX(this)]);
	fprintf(saved, "Kin : %d %s\n", GET_KIN(this), string(PlayerRace::GetKinNameByNum(GET_KIN(this),GET_SEX(this))).c_str());
	li = this->player_data.time.birth;
	fprintf(saved, "Brth: %ld %s\n", static_cast<long int>(li), ctime(&li));
	// Gunner
	tmp += this->player_data.time.played;
	fprintf(saved, "Plyd: %d\n", tmp);
	// Gunner end
	li = this->player_data.time.logon;
	fprintf(saved, "Last: %ld %s\n", static_cast<long int>(li), ctime(&li));
	fprintf(saved, "Hite: %d\n", GET_HEIGHT(this));
	fprintf(saved, "Wate: %d\n", GET_WEIGHT(this));
	fprintf(saved, "Size: %d\n", GET_SIZE(this));
	/* ��������� */
	fprintf(saved, "Alin: %d\n", GET_ALIGNMENT(this));
	*buf = '\0';
	tascii(&AFF_FLAGS(this, 0), 4, buf);
	fprintf(saved, "Aff : %s\n", buf);

	/* ������ �� �� ������� */
	/* ������ */
	fprintf(saved, "Str : %d\n", this->get_inborn_str());
	fprintf(saved, "Int : %d\n", this->get_inborn_int());
	fprintf(saved, "Wis : %d\n", this->get_inborn_wis());
	fprintf(saved, "Dex : %d\n", this->get_inborn_dex());
	fprintf(saved, "Con : %d\n", this->get_inborn_con());
	fprintf(saved, "Cha : %d\n", this->get_inborn_cha());

	/* ����������� - added by Gorrah */
	if (GET_LEVEL(this) < LVL_IMMORT)
	{
		fprintf(saved, "Feat:\n");
		for (i = 1; i < MAX_FEATS; i++)
		{
			if (HAVE_FEAT(this, i))
				fprintf(saved, "%d %s\n", i, feat_info[i].name);
		}
		fprintf(saved, "0 0\n");
	}

	/* �������� �� c���������� */
	if (GET_LEVEL(this) < LVL_IMMORT)
	{
		fprintf(saved, "FtTm:\n");
		for (skj = this->timed_feat; skj; skj = skj->next)
		{
			fprintf(saved, "%d %d %s\n", skj->skill, skj->time, feat_info[skj->skill].name);
		}
		fprintf(saved, "0 0\n");
	}

	/* ����� */
	if (GET_LEVEL(this) < LVL_IMMORT)
	{
		fprintf(saved, "Skil:\n");
		int skill;
		for (i = 1; i <= MAX_SKILL_NUM; i++)
		{
			skill = this->get_inborn_skill(i);
			if (skill)
				fprintf(saved, "%d %d %s\n", i, skill, skill_info[i].name);
		}
		fprintf(saved, "0 0\n");
	}

	/* �������� �� ����� */
	if (GET_LEVEL(this) < LVL_IMMORT)
	{
		fprintf(saved, "SkTm:\n");
		for (skj = this->timed; skj; skj = skj->next)
		{
			fprintf(saved, "%d %d\n", skj->skill, skj->time);
		}
		fprintf(saved, "0 0\n");
	}

	/* ����� */
	// ������� �������� �������� ���� ��� ������, ������ �� ������ �� ����
	if (GET_LEVEL(this) < LVL_IMMORT && GET_CLASS(this) != CLASS_DRUID)
	{
		fprintf(saved, "Spel:\n");
		for (i = 1; i <= MAX_SPELLS; i++)
			if (GET_SPELL_TYPE(this, i))
				fprintf(saved, "%d %d %s\n", i, GET_SPELL_TYPE(this, i), spell_info[i].name);
		fprintf(saved, "0 0\n");
	}

	/* ���������� ����� */
	if (GET_LEVEL(this) < LVL_IMMORT)
	{
		fprintf(saved, "SpMe:\n");
		for (i = 1; i <= MAX_SPELLS; i++)
		{
			if (GET_SPELL_MEM(this, i))
				fprintf(saved, "%d %d\n", i, GET_SPELL_MEM(this, i));
		}
		fprintf(saved, "0 0\n");
	}

	/* ������� */
//    if (GET_LEVEL(this) < LVL_IMMORT)
	{
		im_rskill *rs;
		im_recipe *r;
		fprintf(saved, "Rcps:\n");
		for (rs = GET_RSKILL(this); rs; rs = rs->link)
		{
			if (rs->perc <= 0)
				continue;
			r = &imrecipes[rs->rid];
			fprintf(saved, "%d %d %s\n", r->id, rs->perc, r->name);
		}
		fprintf(saved, "-1 -1\n");
	}

	fprintf(saved, "Hrol: %d\n", GET_HR(this));
	fprintf(saved, "Drol: %d\n", GET_DR(this));
	fprintf(saved, "Ac  : %d\n", GET_AC(this));

	fprintf(saved, "Hit : %d/%d\n", GET_HIT(this), GET_MAX_HIT(this));
	fprintf(saved, "Mana: %d/%d\n", GET_MEM_COMPLETED(this), GET_MEM_TOTAL(this));
	fprintf(saved, "Move: %d/%d\n", GET_MOVE(this), GET_MAX_MOVE(this));
	fprintf(saved, "Gold: %ld\n", get_gold());
	fprintf(saved, "Bank: %ld\n", get_bank());

	fprintf(saved, "Wimp: %d\n", GET_WIMP_LEV(this));
	fprintf(saved, "Frez: %d\n", GET_FREEZE_LEV(this));
	fprintf(saved, "Invs: %d\n", GET_INVIS_LEV(this));
	fprintf(saved, "Room: %d\n", GET_LOADROOM(this));

	fprintf(saved, "Badp: %d\n", GET_BAD_PWS(this));

	if (GET_BOARD(this))
		for (int i = 0; i < BOARD_TOTAL; ++i)
			fprintf(saved, "Br%02d: %ld\n", i + 1, static_cast<long int>(GET_BOARD_DATE(this, i)));

	for (int i = 0; i < START_STATS_TOTAL; ++i)
		fprintf(saved, "St%02d: %i\n", i, this->get_start_stat(i));

	if (GET_LEVEL(this) < LVL_IMMORT)
		fprintf(saved, "Hung: %d\n", GET_COND(this, FULL));
	if (GET_LEVEL(this) < LVL_IMMORT)
		fprintf(saved, "Thir: %d\n", GET_COND(this, THIRST));
	if (GET_LEVEL(this) < LVL_IMMORT)
		fprintf(saved, "Drnk: %d\n", GET_COND(this, DRUNK));

	fprintf(saved, "Reli: %d %s\n", GET_RELIGION(this), religion_name[GET_RELIGION(this)][(int) GET_SEX(this)]);
	fprintf(saved, "Race: %d %s\n", GET_RACE(this), string(PlayerRace::GetRaceNameByNum(GET_KIN(this),GET_RACE(this),GET_SEX(this))).c_str());
	fprintf(saved, "DrSt: %d\n", GET_DRUNK_STATE(this));
	fprintf(saved, "Olc : %d\n", GET_OLC_ZONE(this));
	*buf = '\0';
	tascii(&PRF_FLAGS(this, 0), 4, buf);
	fprintf(saved, "Pref: %s\n", buf);

	if (MUTE_DURATION(this) > 0 && PLR_FLAGGED(this, PLR_MUTE))
		fprintf(saved, "PMut: %ld %d %ld %s~\n", MUTE_DURATION(this), GET_MUTE_LEV(this), MUTE_GODID(this), MUTE_REASON(this));
	if (NAME_DURATION(this) > 0 && PLR_FLAGGED(this, PLR_NAMED))
		fprintf(saved, "PNam: %ld %d %ld %s~\n", NAME_DURATION(this), GET_NAME_LEV(this), NAME_GODID(this), NAME_REASON(this));
	if (DUMB_DURATION(this) > 0 && PLR_FLAGGED(this, PLR_DUMB))
		fprintf(saved, "PDum: %ld %d %ld %s~\n", DUMB_DURATION(this), GET_DUMB_LEV(this), DUMB_GODID(this), DUMB_REASON(this));
	if (HELL_DURATION(this) > 0 && PLR_FLAGGED(this, PLR_HELLED))
		fprintf(saved, "PHel: %ld %d %ld %s~\n", HELL_DURATION(this), GET_HELL_LEV(this), HELL_GODID(this), HELL_REASON(this));
	if (GCURSE_DURATION(this) > 0)
		fprintf(saved, "PGcs: %ld %d %ld %s~\n", GCURSE_DURATION(this), GET_GCURSE_LEV(this), GCURSE_GODID(this), GCURSE_REASON(this));
	if (FREEZE_DURATION(this) > 0 && PLR_FLAGGED(this, PLR_FROZEN))
		fprintf(saved, "PFrz: %ld %d %ld %s~\n", FREEZE_DURATION(this), GET_FREEZE_LEV(this), FREEZE_GODID(this), FREEZE_REASON(this));
	if (UNREG_DURATION(this) > 0)
		fprintf(saved, "PUnr: %ld %d %ld %s~\n", UNREG_DURATION(this), GET_UNREG_LEV(this), UNREG_GODID(this), UNREG_REASON(this));


	if (KARMA(this) > 0)
	{
		strcpy(buf, KARMA(this));
		kill_ems(buf);
		fprintf(saved, "Karm:\n%s~\n", buf);
	}
	if (LOGON_LIST(this) > 0)
	{
		log("Saving logon list.");
		struct logon_data * next_log = LOGON_LIST(this);
		std::stringstream buffer;
		while (next_log)
		{
			buffer << next_log->ip << " " << next_log->count << " " << next_log->lasttime << "\n";
			next_log = next_log->next;
		}
		fprintf(saved, "LogL:\n%s~\n", buffer.str().c_str());
	}
	fprintf(saved, "GdFl: %ld\n", this->player_specials->saved.GodsLike);
	fprintf(saved, "NamG: %d\n", NAME_GOD(this));
	fprintf(saved, "NaID: %ld\n", NAME_ID_GOD(this));
	fprintf(saved, "StrL: %d\n", STRING_LENGTH(this));
	fprintf(saved, "StrW: %d\n", STRING_WIDTH(this));
	fprintf(saved, "NtfE: %ld\n", NOTIFY_EXCH_PRICE(this)); //Polud ���. ���� ��� �������-����������

	if (this->remember_get_num() != Remember::DEF_REMEMBER_NUM)
	{
		fprintf(saved, "Rmbr: %d\n", this->remember_get_num());
	}

	if (EXCHANGE_FILTER(this))
		fprintf(saved, "ExFl: %s\n", EXCHANGE_FILTER(this));

	// shapirus: ����� ����
	{
		struct ignore_data *cur = IGNORE_LIST(this);
		if (cur)
		{
			for (; cur; cur = cur->next)
			{
				if (!cur->id)
					continue;
				fprintf(saved, "Ignr: [%ld]%ld\n", cur->mode, cur->id);
			}
		}
	}

	/* affected_type */
	if (tmp_aff[0].type > 0)
	{
		fprintf(saved, "Affs:\n");
		for (i = 0; i < MAX_AFFECT; i++)
		{
			aff = &tmp_aff[i];
			if (aff->type)
				fprintf(saved, "%d %d %d %d %d %d %s\n", aff->type, aff->duration,
						aff->modifier, aff->location, static_cast<int>(aff->bitvector),
						static_cast<int>(aff->battleflag), spell_name(aff->type));
		}
		fprintf(saved, "0 0 0 0 0 0\n");
	}

	/* ������� */
	for (prt = GET_PORTALS(this); prt; prt = prt->next)
	{
		fprintf(saved, "Prtl: %d\n", prt->vnum);
	}
	for (i = 0; i < NLOG; ++i)
	{
		if (!GET_LOGS(this))
		{
			log("SYSERR: Saving NULL logs for char %s", GET_NAME(this));
			break;
		}
		fprintf(saved, "Logs: %d %d\n", i, GET_LOGS(this)[i]);
	}

	fprintf(saved, "Disp: %ld\n", disposable_flags_.to_ulong());

    fprintf(saved, "Ripa: %d\n", GET_RIP_ARENA(this)); //Rip_arena
    fprintf(saved, "Wina: %d\n", GET_WIN_ARENA(this)); //Win_arena
    fprintf(saved, "Expa: %llu\n", GET_EXP_ARENA(this)); //Exp_arena
    fprintf(saved, "Ripm: %d\n", GET_RIP_MOB(this)); //Rip_mob
    fprintf(saved, "Expm: %llu\n", GET_EXP_MOB(this)); //Exp_mob
    fprintf(saved, "Ripd: %d\n", GET_RIP_DT(this)); //Rip_dt
    fprintf(saved, "Expd: %llu\n", GET_EXP_DT(this));//Exp_dt
    fprintf(saved, "Ripo: %d\n", GET_RIP_OTHER(this));//Rip_other
    fprintf(saved, "Expo: %llu\n", GET_EXP_OTHER(this));//Exp_other
    fprintf(saved, "Ripp: %d\n", GET_RIP_PK(this));//Rip_pk
    fprintf(saved, "Expp: %llu\n", GET_EXP_PK(this)); //Exp_pk
    fprintf(saved, "Rimt: %d\n", GET_RIP_MOBTHIS(this)); //Rip_mob_this
    fprintf(saved, "Exmt: %llu\n", GET_EXP_MOBTHIS(this));//Exp_mob_this
    fprintf(saved, "Ridt: %d\n", GET_RIP_DTTHIS(this)); //Rip_dt_this
    fprintf(saved, "Exdt: %llu\n", GET_EXP_DTTHIS(this)); //Exp_dt_this
    fprintf(saved, "Riot: %d\n", GET_RIP_OTHERTHIS(this)); //Rip_other_this
    fprintf(saved, "Exot: %llu\n", GET_EXP_OTHERTHIS(this)); ////Exp_other_this
    fprintf(saved, "Ript: %d\n", GET_RIP_PKTHIS(this)); //Rip_pk_this
    fprintf(saved, "Expt: %llu\n", GET_EXP_PKTHIS(this));//Exp_pk_this

	// added by WorM (�������) 2010.06.04 ����� ����������� �� ����(������������ ��� �����)
	i = 0;
	if (this->followers
		&& GET_CLASS(this) == CLASS_MERCHANT
		&& !IS_IMMORTAL(this))
	{
		struct follow_type *k;
		for (k = this->followers; k; k = k->next)
		{
			if (k->follower
				&& AFF_FLAGGED(k->follower, AFF_HELPER)
				&& AFF_FLAGGED(k->follower, AFF_CHARM))
			{
				break;
			}
		}
		if(k && k->follower->affected)
		{
			for (AFFECT_DATA *aff = k->follower->affected; aff; aff = aff->next)
			{
				if (aff->type == SPELL_CHARM)
				{
					if(k->follower->mob_specials.hire_price == 0)
					{
						break;
					}
					int i = ((aff->duration-1)/2)*k->follower->mob_specials.hire_price;
					if(i != 0)
					{
						fprintf(saved, "GldH: %d\n", i);
					}
					break;
				}
			}
		}
	}

	this->quested_save(saved);
	this->mobmax_save(saved);
	save_pkills(this, saved);
	morphs_save(this, saved);

	fclose(saved);
	FileCRC::check_crc(filename, FileCRC::UPDATE_PLAYER, GET_UNIQUE(this));

	/* ��������������� ������� */
	/* add spell and eq affections back in now */
	for (i = 0; i < MAX_AFFECT; i++)
	{
		if (tmp_aff[i].type)
			affect_to_char(this, &tmp_aff[i]);
	}

	for (i = 0; i < NUM_WEARS; i++)
	{
		if (char_eq[i])
		{
#ifndef NO_EXTRANEOUS_TRIGGERS
			if (wear_otrigger(char_eq[i], this, i))
#endif
				equip_char(this, char_eq[i], i | 0x80 | 0x40);
#ifndef NO_EXTRANEOUS_TRIGGERS
			else
				obj_to_char(char_eq[i], this);
#endif
		}
	}
	affect_total(this);

	if ((i = get_ptable_by_name(GET_NAME(this))) >= 0)
	{
		player_table[i].last_logon = LAST_LOGON(this);
		player_table[i].level = GET_LEVEL(this);
		//added by WorM 2010.08.27 � ������� ��������� ����
		if(player_table[i].mail)
			free(player_table[i].mail);
		player_table[i].mail = str_dup(GET_EMAIL(this));
		//end by WorM
	}
}

// �� ���� reboot: ������������ ������ ��� ������ ���� � ������� �� entrycount
// ��� ���������� ����� ���� �������� ������ �� ���� Rebt, ��� ��������� ���� ������������
// ������� ��� �����-�� ���������� � entrycount, must_be_deleted � TopPlayer::Refresh �������
// ���������, ��� �������� ��� �������� � ������������� ���������������������� ������ ���������
// �� ������ ������ ���: PLR_FLAGS, GET_CLASS, GET_EXP, GET_IDNUM, LAST_LOGON, GET_LEVEL, GET_NAME, GET_REMORT, GET_UNIQUE, GET_EMAIL
/**
* \param reboot - �� ������� = false
*/
int Player::load_char_ascii(const char *name, bool reboot)
{
	int id, num = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, i;
	long int lnum = 0, lnum3 = 0;
	unsigned long long llnum = 0;
	FBFILE *fl;
	char filename[40];
	char buf[MAX_RAW_INPUT_LENGTH], line[MAX_RAW_INPUT_LENGTH], tag[6];
	char line1[MAX_RAW_INPUT_LENGTH];
	AFFECT_DATA af;
	struct timed_type timed;


	*filename = '\0';
	log("Load ascii char %s", name);
	if (now_entrycount)
	{
		id = 1;
	}
	else
	{
		id = find_name(name);
	}
	if (!(id >= 0 && get_filename(name, filename, PLAYERS_FILE) && (fl = fbopen(filename, FB_READ))))
	{
		log("Cann't load ascii %d %s", id, filename);
		return (-1);
	}

///////////////////////////////////////////////////////////////////////////////

	// ������� ���� � ������ ���� ��� ������ �� ���� "Rebt", ���� reboot �� ����� = 1, �� �� ���� ���� � ���������
	if (this->player_specials == NULL)
		CREATE(this->player_specials, struct player_special_data, 1);

	set_level(1);
	set_class(1);
	set_uid(0);
	set_last_logon(time(0));
	set_idnum(0);
	set_exp(0);
	set_remort(0);
	GET_LASTIP(this)[0] = 0;
	GET_EMAIL(this)[0] = 0;
	asciiflag_conv("", &PLR_FLAGS(this, 0));

	bool skip_file = 0;

	do
	{
		if (!fbgetline(fl, line))
		{
			log("SYSERROR: Wrong file ascii %d %s", id, filename);
			return (-1);
		}

		tag_argument(line, tag);
		for (i = 0; !(line[i] == ' ' || line[i] == '\0'); i++)
		{
			line1[i] = line[i];
		}
		line1[i] = '\0';
		num = atoi(line1);
		lnum = atol(line1);
		try
		{
			llnum = boost::lexical_cast<unsigned long long>(line1);
		}
		catch(boost::bad_lexical_cast &)
        {
			llnum = 0;
		}


		switch (*tag)
		{
		case 'A':
			if (!strcmp(tag, "Act "))
				asciiflag_conv(line, &PLR_FLAGS(this, 0));
			break;
		case 'C':
			if (!strcmp(tag, "Clas"))
			{
				set_class(num);
			}
			break;
		case 'E':
			if (!strcmp(tag, "Exp "))
			{
				set_exp(lnum);
			}
			//added by WorM 2010.08.27 ������ ���� � ���� ���� ��� ������
			else if (!strcmp(tag, "EMal"))
				strcpy(GET_EMAIL(this), line);
			break;
		case 'H':
			if (!strcmp(tag, "Host"))
			{
				strcpy(GET_LASTIP(this), line);
			}
			//end by WorM
		  break;
		case 'I':
			if (!strcmp(tag, "Id  "))
			{
				set_idnum(lnum);
			}
			break;
		case 'L':
			if (!strcmp(tag, "LstL"))
			{
				set_last_logon(lnum);
			}
			else if (!strcmp(tag, "Levl"))
			{
				set_level(num);
			}
			break;
		case 'N':
			if (!strcmp(tag, "Name"))
			{
				set_name(line);
			}
			break;
		case 'R':
			if (!strcmp(tag, "Rebt"))
				skip_file = 1;
			else if (!strcmp(tag, "Rmrt"))
			{
				set_remort(num);
			}
			break;
		case 'U':
			if (!strcmp(tag, "UIN "))
			{
				set_uid(num);
			}
			break;
		default:
			sprintf(buf, "SYSERR: Unknown tag %s in pfile %s", tag, name);
		}
	}
	while (!skip_file);

	//added by WorM 2010.08.27 ������ ���� � ��������� ip ���� ��� ���������� ��������
	while((reboot) && (!*GET_EMAIL(this) || !*GET_LASTIP(this)))
	{
		if (!fbgetline(fl, line))
		{
			log("SYSERROR: Wrong file ascii %d %s", id, filename);
			return (-1);
		}

		tag_argument(line, tag);

		if (!strcmp(tag, "EMal"))
			strcpy(GET_EMAIL(this), line);
		else if (!strcmp(tag, "Host"))
			strcpy(GET_LASTIP(this), line);
	}
	//end by WorM

	// ���� � ������������ ���� ������ ���-�� ������� ������ ����� ����� - ������� ��� �����

	//Indexing experience - if his exp is lover than required for his level - set it to required
	if (GET_EXP(this) < level_exp(this, GET_LEVEL(this)))
	{
		set_exp(level_exp(this, GET_LEVEL(this)));
	}

	if (reboot)
	{
		fbclose(fl);
		return id;
	}

	// ���� ���������� ������� ���� ������, �� ������ ���� ������ � ���� ��� ��������� ����

///////////////////////////////////////////////////////////////////////////////


	/* character init */
	/* initializations necessary to keep some things straight */

	this->set_npc_name(0);
	this->player_data.long_descr = NULL;

	this->real_abils.Feats.reset();

	// ������� ����� ��� ������ �� �����, ��������� ���� ������
	if (GET_CLASS(this) != CLASS_DRUID)
		for (i = 1; i <= MAX_SPELLS; i++)
			GET_SPELL_TYPE(this, i) = 0;
	else
		for (i = 1; i <= MAX_SPELLS; i++)
			GET_SPELL_TYPE(this, i) = SPELL_RUNES;

	for (i = 1; i <= MAX_SPELLS; i++)
		GET_SPELL_MEM(this, i) = 0;
	this->char_specials.saved.affected_by = clear_flags;
	POOFIN(this) = NULL;
	POOFOUT(this) = NULL;
	GET_RSKILL(this) = NULL;	// �������� �� �����
	this->char_specials.carry_weight = 0;
	this->char_specials.carry_items = 0;
	this->real_abils.armor = 100;
	GET_MEM_TOTAL(this) = 0;
	GET_MEM_COMPLETED(this) = 0;
	MemQ_init(this);

	GET_AC(this) = 10;
	GET_ALIGNMENT(this) = 0;
	GET_BAD_PWS(this) = 0;
	this->player_data.time.birth = time(0);
	GET_KIN(this) = 0;

	this->set_str(10);
	this->set_dex(10);
	this->set_con(10);
	this->set_int(10);
	this->set_wis(10);
	this ->set_cha(10);

	GET_COND(this, DRUNK) = 0;
	GET_DRUNK_STATE(this) = 0;

// Punish Init
	DUMB_DURATION(this) = 0;
	DUMB_REASON(this) = 0;
	GET_DUMB_LEV(this) = 0;
	DUMB_GODID(this) = 0;

	MUTE_DURATION(this) = 0;
	MUTE_REASON(this) = 0;
	GET_MUTE_LEV(this) = 0;
	MUTE_GODID(this) = 0;

	HELL_DURATION(this) = 0;
	HELL_REASON(this) = 0;
	GET_HELL_LEV(this) = 0;
	HELL_GODID(this) = 0;

	FREEZE_DURATION(this) = 0;
	FREEZE_REASON(this) = 0;
	GET_FREEZE_LEV(this) = 0;
	FREEZE_GODID(this) = 0;

	GCURSE_DURATION(this) = 0;
	GCURSE_REASON(this) = 0;
	GET_GCURSE_LEV(this) = 0;
	GCURSE_GODID(this) = 0;

	NAME_DURATION(this) = 0;
	NAME_REASON(this) = 0;
	GET_NAME_LEV(this) = 0;
	NAME_GODID(this) = 0;

	UNREG_DURATION(this) = 0;
	UNREG_REASON(this) = 0;
	GET_UNREG_LEV(this) = 0;
	UNREG_GODID(this) = 0;

// End punish init

	GET_DR(this) = 0;

	set_gold(0, false);
	set_bank(0, false);

	this->player_specials->saved.GodsLike = 0;
	GET_HIT(this) = 21;
	GET_MAX_HIT(this) = 21;
	GET_HEIGHT(this) = 50;
	GET_HR(this) = 0;
	GET_COND(this, FULL) = 0;
	GET_INVIS_LEV(this) = 0;
	this->player_data.time.logon = time(0);
	GET_MOVE(this) = 44;
	GET_MAX_MOVE(this) = 44;
	KARMA(this) = 0;
	LOGON_LIST(this) = 0;
	NAME_GOD(this) = 0;
	STRING_LENGTH(this) = 80;
	STRING_WIDTH(this) = 25;
	NAME_ID_GOD(this) = 0;
	GET_OLC_ZONE(this) = 0;
	this->player_data.time.played = 0;
	GET_LOADROOM(this) = NOWHERE;
	GET_RELIGION(this) = 1;
	GET_RACE(this) = 1;
	GET_SEX(this) = 0;
	GET_COND(this, THIRST) = 0;
	GET_WEIGHT(this) = 50;
	GET_WIMP_LEV(this) = 0;
	asciiflag_conv("", &PRF_FLAGS(this, 0));
	asciiflag_conv("", &AFF_FLAGS(this, 0));
	GET_PORTALS(this) = NULL;
	EXCHANGE_FILTER(this) = NULL;
	IGNORE_LIST(this) = NULL;
	CREATE(GET_LOGS(this), int, NLOG);
	NOTIFY_EXCH_PRICE(this) = 0;
	this->player_specials->saved.HiredCost = 0;

	GET_BOARD(this) = new(struct board_data);
	// ����� ����� ������� ����, � ������� ������ ������ ����� ���������,
	// ��������, ����� �� ������ ������ 200+ ��������� ��� ������ ������� �������
	for (int i = 0; i < BOARD_TOTAL; ++i)
		GET_BOARD_DATE(this, i) = 1143706650;

	while (fbgetline(fl, line))
	{
		tag_argument(line, tag);
		for (i = 0; !(line[i] == ' ' || line[i] == '\0'); i++)
		{
			line1[i] = line[i];
		}
		line1[i] = '\0';
		num = atoi(line1);
		lnum = atol(line1);
		try
		{
			llnum = boost::lexical_cast<unsigned long long>(line1);
		}
		catch(boost::bad_lexical_cast &)
        {
			llnum = 0;
		}

		switch (*tag)
		{
		case 'A':
			if (!strcmp(tag, "Ac  "))
				GET_AC(this) = num;
			else if (!strcmp(tag, "Aff "))
				asciiflag_conv(line, &AFF_FLAGS(this, 0));
			else if (!strcmp(tag, "Affs"))
			{
				i = 0;
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d %d %d %d %d", &num, &num2, &num3, &num4, &num5, &num6);
					if (num > 0)
					{
						af.type = num;
						af.duration = num2;
						af.modifier = num3;
						af.location = num4;
						af.bitvector = num5;
						af.battleflag = num6;
						if (af.type == SPELL_LACKY)
						{
							af.handler = boost::shared_ptr<LackyAffectHandler>(new LackyAffectHandler());
						}
						affect_to_char(this, &af);
						i++;
					}
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "Alin"))
				GET_ALIGNMENT(this) = num;
			break;

		case 'B':
			if (!strcmp(tag, "Badp"))
				GET_BAD_PWS(this) = num;
			else if (!strcmp(tag, "Bank"))
			{
				set_bank(lnum, false);
			}
			else if (!strcmp(tag, "Br01"))
				GET_BOARD_DATE(this, GENERAL_BOARD) = lnum;
			else if (!strcmp(tag, "Br02"))
				GET_BOARD_DATE(this, NEWS_BOARD) = lnum;
			else if (!strcmp(tag, "Br03"))
				GET_BOARD_DATE(this, IDEA_BOARD) = lnum;
			else if (!strcmp(tag, "Br04"))
				GET_BOARD_DATE(this, ERROR_BOARD) = lnum;
			else if (!strcmp(tag, "Br05"))
				GET_BOARD_DATE(this, GODNEWS_BOARD) = lnum;
			else if (!strcmp(tag, "Br06"))
				GET_BOARD_DATE(this, GODGENERAL_BOARD) = lnum;
			else if (!strcmp(tag, "Br07"))
				GET_BOARD_DATE(this, GODBUILD_BOARD) = lnum;
			else if (!strcmp(tag, "Br08"))
				GET_BOARD_DATE(this, GODCODE_BOARD) = lnum;
			else if (!strcmp(tag, "Br09"))
				GET_BOARD_DATE(this, GODPUNISH_BOARD) = lnum;
			else if (!strcmp(tag, "Br10"))
				GET_BOARD_DATE(this, PERS_BOARD) = lnum;
			else if (!strcmp(tag, "Br11"))
				GET_BOARD_DATE(this, CLAN_BOARD) = lnum;
			else if (!strcmp(tag, "Br12"))
				GET_BOARD_DATE(this, CLANNEWS_BOARD) = lnum;
			else if (!strcmp(tag, "Br13"))
				GET_BOARD_DATE(this, NOTICE_BOARD) = lnum;
			else if (!strcmp(tag, "Br14"))
				GET_BOARD_DATE(this, MISPRINT_BOARD) = lnum;

			else if (!strcmp(tag, "Brth"))
				this->player_data.time.birth = lnum;
			break;

		case 'C':
			if (!strcmp(tag, "Cha "))
				this->set_cha(num);
			else if (!strcmp(tag, "Con "))
				this->set_con(num);
			break;

		case 'D':
			if (!strcmp(tag, "Desc"))
			{
				this->player_data.description = fbgetstring(fl);
			}
			else if (!strcmp(tag, "Disp"))
			{
				std::bitset<TOTAL_DISPOSABLE_NUM> tmp_flags(lnum);
				disposable_flags_ = tmp_flags;
			}
			else if (!strcmp(tag, "Dex "))
				this->set_dex(num);
			else if (!strcmp(tag, "Drnk"))
				GET_COND(this, DRUNK) = num;
			else if (!strcmp(tag, "DrSt"))
				GET_DRUNK_STATE(this) = num;
			else if (!strcmp(tag, "Drol"))
				GET_DR(this) = num;
			break;

		case 'E':
			if (!strcmp(tag, "ExFl"))
				EXCHANGE_FILTER(this) = str_dup(line);
			else if (!strcmp(tag, "EMal"))
				strcpy(GET_EMAIL(this), line);
/*29.11.09. (c) ��������*/
//edited by WorM 2011.05.21
			else if (!strcmp(tag, "Expa"))
				GET_EXP_ARENA(this) = llnum;
			else if (!strcmp(tag, "Expm"))
				GET_EXP_MOB(this) = llnum;
			else if (!strcmp(tag, "Exmt"))
				GET_EXP_MOBTHIS(this) = llnum;
			else if (!strcmp(tag, "Expp"))
				GET_EXP_PK(this) = llnum;
			else if (!strcmp(tag, "Expt"))
				GET_EXP_PKTHIS(this) = llnum;
			else if (!strcmp(tag, "Expt"))
				GET_EXP_OTHER(this) = llnum;
			else if (!strcmp(tag, "Exot"))
				GET_EXP_OTHERTHIS(this) = llnum;
			else if (!strcmp(tag, "Expd"))
				GET_EXP_DT(this) = llnum;
			else if (!strcmp(tag, "Exdt"))
				GET_EXP_DTTHIS(this) = llnum;
//end by WorM
/*����� ������ (�) ��������*/
			break;

		case 'F':
			// ��������� ��� ������������� �� ������ �������� ���������
			if (!strcmp(tag, "Frez"))
				GET_FREEZE_LEV(this) = num;
			else if (!strcmp(tag, "Feat"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d", &num);
					if (num > 0 && num < MAX_FEATS)
						if (feat_info[num].classknow[(int) GET_CLASS(this)][(int) GET_KIN(this)] || PlayerRace::FeatureCheck((int)GET_KIN(this),(int)GET_RACE(this),num))
							SET_FEAT(this, num);
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "FtTm"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num != 0)
					{
						timed.skill = num;
						timed.time = num2;
						timed_feat_to_char(this, &timed);
					}
				}
				while (num != 0);
			}
			break;

		case 'G':
			if (!strcmp(tag, "Gold"))
			{
				set_gold(lnum, false);
			}
			else if (!strcmp(tag, "GodD"))
				GCURSE_DURATION(this) = lnum;
			else if (!strcmp(tag, "GdFl"))
				this->player_specials->saved.GodsLike = lnum;
			// added by WorM (�������) 2010.06.04 ����� ����������� �� ����(������������ ��� �����)
			else if (!strcmp(tag, "GldH"))
			{
				if(num != 0 && !IS_IMMORTAL(this) && GET_CLASS(this) == CLASS_MERCHANT)
				{
					this->player_specials->saved.HiredCost = num;
				}
			}
			// end by WorM
			break;

		case 'H':
			if (!strcmp(tag, "Hit "))
			{
				sscanf(line, "%d/%d", &num, &num2);
				GET_HIT(this) = num;
				GET_MAX_HIT(this) = num2;
			}
			else if (!strcmp(tag, "Hite"))
				GET_HEIGHT(this) = num;
			else if (!strcmp(tag, "Hrol"))
				GET_HR(this) = num;
			else if (!strcmp(tag, "Hung"))
				GET_COND(this, FULL) = num;
			else if (!strcmp(tag, "Host"))
				strcpy(GET_LASTIP(this), line);
			break;

		case 'I':
			if (!strcmp(tag, "Int "))
				this->set_int(num);
			else if (!strcmp(tag, "Invs"))
				GET_INVIS_LEV(this) = num;
// shapirus
			else if (!strcmp(tag, "Ignr"))
				load_ignores(this, line);
			break;

		case 'K':
			if (!strcmp(tag, "Kin "))
				GET_KIN(this) = num;
			else if (!strcmp(tag, "Karm"))
				KARMA(this) = fbgetstring(fl);
			break;
		case 'L':
			if (!strcmp(tag, "LogL"))
			{
				i = 0;
				struct logon_data * cur_log = 0;
				long  lnum, lnum2;
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%s %ld %ld", &buf[0], &lnum, &lnum2);
					if (buf[0] != '~')
					{
						if (i == 0)
							cur_log = LOGON_LIST(this) = new(struct logon_data);
						else
						{
							cur_log->next = new(struct logon_data);
							cur_log = cur_log->next;
						}
						// ��������� � ������.
						cur_log->ip = str_dup(buf);
						cur_log->count = lnum;
						cur_log->lasttime = lnum2;
						cur_log->next = 0;   // ���������� ������
						i++;
					}
					else break;
				}
				while (true);
			}
// Gunner
			else if (!strcmp(tag, "Logs"))
			{
				sscanf(line, "%d %d", &num, &num2);
				if (num >= 0 && num < NLOG)
					GET_LOGS(this)[num] = num2;
			}
			break;

		case 'M':
			if (!strcmp(tag, "Mana"))
			{
				sscanf(line, "%d/%d", &num, &num2);
				GET_MEM_COMPLETED(this) = num;
				GET_MEM_TOTAL(this) = num2;
			}
			else if (!strcmp(tag, "Move"))
			{
				sscanf(line, "%d/%d", &num, &num2);
				GET_MOVE(this) = num;
				GET_MAX_MOVE(this) = num2;
			}
			else if (!strcmp(tag, "Mobs"))
			{
				do
				{
					if (!fbgetline(fl, line))
						break;
					if (*line == '~')
						break;
					sscanf(line, "%d %d", &num, &num2);
					this->mobmax_load(this, num, num2, MobMax::get_level_by_vnum(num));
				}
				while (true);
			}
			else if (!strcmp(tag, "Mrph"))
			{
				morphs_load(this, string(line));
			}
			break;

		case 'N':
			if (!strcmp(tag, "NmI "))
				GET_PAD(this, 0) = str_dup(line);
			else if (!strcmp(tag, "NmR "))
				GET_PAD(this, 1) = str_dup(line);
			else if (!strcmp(tag, "NmD "))
				GET_PAD(this, 2) = str_dup(line);
			else if (!strcmp(tag, "NmV "))
				GET_PAD(this, 3) = str_dup(line);
			else if (!strcmp(tag, "NmT "))
				GET_PAD(this, 4) = str_dup(line);
			else if (!strcmp(tag, "NmP "))
				GET_PAD(this, 5) = str_dup(line);
			else if (!strcmp(tag, "NamD"))
				NAME_DURATION(this) = lnum;
			else if (!strcmp(tag, "NamG"))
				NAME_GOD(this) = num;
			else if (!strcmp(tag, "NaID"))
				NAME_ID_GOD(this) = lnum;
			else if (!strcmp(tag, "NtfE"))//Polud ���. ���� ��� �������-����������
				NOTIFY_EXCH_PRICE(this) = lnum;
			break;

		case 'O':
			if (!strcmp(tag, "Olc "))
				GET_OLC_ZONE(this) = num;
			break;


		case 'P':
			if (!strcmp(tag, "Pass"))
				this->set_passwd(line);
			else if (!strcmp(tag, "Plyd"))
				this->player_data.time.played = num;
			else if (!strcmp(tag, "PfIn"))
				POOFIN(this) = str_dup(line);
			else if (!strcmp(tag, "PfOt"))
				POOFOUT(this) = str_dup(line);
			else if (!strcmp(tag, "Pref"))
				asciiflag_conv(line, &PRF_FLAGS(this, 0));
			else if (!strcmp(tag, "Pkil"))
			{
				do
				{
					if (!fbgetline(fl, line))
						break;
					if (*line == '~')
						break;
					sscanf(line, "%ld %d", &lnum, &num);

					if (lnum < 0 || !correct_unique(lnum))
						continue;
					struct PK_Memory_type * pk_one = NULL;
					for (pk_one = this->pk_list; pk_one; pk_one = pk_one->next)
						if (pk_one->unique == lnum)
							break;
					if (pk_one)
					{
						log("SYSERROR: duplicate entry pkillers data for %d %s", id, filename);
						continue;
					}

					CREATE(pk_one, struct PK_Memory_type, 1);
					pk_one->unique = lnum;
					pk_one->kill_num = num;
					pk_one->next = this->pk_list;
					this->pk_list = pk_one;
				}
				while (true);
			}
			else if (!strcmp(tag, "Prtl"))
				add_portal_to_char(this, num);
			// Loads Here new punishment strings
			else if (!strcmp(tag, "PMut"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				MUTE_DURATION(this) = lnum;
				GET_MUTE_LEV(this) = num2;
				MUTE_GODID(this) = lnum3;
				MUTE_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PHel"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				HELL_DURATION(this) = lnum;
				GET_HELL_LEV(this) = num2;
				HELL_GODID(this) = lnum3;
				HELL_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PDum"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				DUMB_DURATION(this) = lnum;
				GET_DUMB_LEV(this) = num2;
				DUMB_GODID(this) = lnum3;
				DUMB_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PNam"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				NAME_DURATION(this) = lnum;
				GET_NAME_LEV(this) = num2;
				NAME_GODID(this) = lnum3;
				NAME_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PFrz"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				FREEZE_DURATION(this) = lnum;
				GET_FREEZE_LEV(this) = num2;
				FREEZE_GODID(this) = lnum3;
				FREEZE_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PGcs"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				GCURSE_DURATION(this) = lnum;
				GET_GCURSE_LEV(this) = num2;
				GCURSE_GODID(this) = lnum3;
				GCURSE_REASON(this) = str_dup(buf);
			}
			else if (!strcmp(tag, "PUnr"))
			{
				sscanf(line, "%ld %d %ld %[^~]", &lnum, &num2, &lnum3, &buf[0]);
				UNREG_DURATION(this) = lnum;
				GET_UNREG_LEV(this) = num2;
				UNREG_GODID(this) = lnum3;
				UNREG_REASON(this) = str_dup(buf);
			}

			break;

		case 'Q':
			if (!strcmp(tag, "Qst "))
			{
				buf[0] = '\0';
				sscanf(line, "%d %[^~]", &num, &buf[0]);
				this->quested_add(this, num, buf);
			}
			break;

		case 'R':
			if (!strcmp(tag, "Room"))
				GET_LOADROOM(this) = num;
/*29.11.09. (c) ��������*/
			else if (!strcmp(tag, "Ripa"))
				GET_RIP_ARENA(this) = num;
			else if (!strcmp(tag, "Ripm"))
				GET_RIP_MOB(this) = num;
			else if (!strcmp(tag, "Rimt"))
				GET_RIP_MOBTHIS(this) = num;
			else if (!strcmp(tag, "Ripp"))
				GET_RIP_PK(this) = num;
			else if (!strcmp(tag, "Ript"))
				GET_RIP_PKTHIS(this) = num;
			else if (!strcmp(tag, "Ripo"))
				GET_RIP_OTHER(this) = num;
			else if (!strcmp(tag, "Riot"))
				GET_RIP_OTHERTHIS(this) = num;
			else if (!strcmp(tag, "Ripd"))
				GET_RIP_DT(this) = num;
			else if (!strcmp(tag, "Ridt"))
				GET_RIP_DTTHIS(this) = num;
/*(�) ��������*/
			else if (!strcmp(tag, "Rmbr"))
				this->remember_set_num(num);
			else if (!strcmp(tag, "Reli"))
				GET_RELIGION(this) = num;
			else if (!strcmp(tag, "Race"))
				GET_RACE(this) = num;
			else if (!strcmp(tag, "Rcps"))
			{
				im_rskill *last = NULL;
				for (;;)
				{
					im_rskill *rs;
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num < 0)
						break;
					num = im_get_recipe(num);
// +newbook.patch (Alisher)
					if (num < 0 || imrecipes[num].classknow[(int) GET_CLASS(this)] != KNOW_RECIPE)
// -newbook.patch (Alisher)
						continue;
					CREATE(rs, im_rskill, 1);
					rs->rid = num;
					rs->perc = num2;
					rs->link = NULL;
					if (last)
						last->link = rs;
					else
						GET_RSKILL(this) = rs;
					last = rs;
				}
			}
			break;

		case 'S':
			if (!strcmp(tag, "Size"))
				GET_SIZE(this) = num;
			else if (!strcmp(tag, "Sex "))
				GET_SEX(this) = num;
			else if (!strcmp(tag, "Skil"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num != 0)
						if (skill_info[num].classknow[(int) GET_CLASS(this)][(int) GET_KIN(this)] == KNOW_SKILL)
							this->set_skill(num, num2);
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "SkTm"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num != 0)
					{
						timed.skill = num;
						timed.time = num2;
						timed_to_char(this, &timed);
					}
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "Spel"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num != 0 && spell_info[num].name)
						GET_SPELL_TYPE(this, num) = num2;
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "SpMe"))
			{
				do
				{
					fbgetline(fl, line);
					sscanf(line, "%d %d", &num, &num2);
					if (num != 0)
						GET_SPELL_MEM(this, num) = num2;
				}
				while (num != 0);
			}
			else if (!strcmp(tag, "Str "))
				this->set_str(num);
			else if (!strcmp(tag, "StrL"))
				STRING_LENGTH(this) = num;
			else if (!strcmp(tag, "StrW"))
				STRING_WIDTH(this) = num;
			else if (!strcmp(tag, "St00"))
				this->set_start_stat(G_STR, lnum);
			else if (!strcmp(tag, "St01"))
				this->set_start_stat(G_DEX, lnum);
			else if (!strcmp(tag, "St02"))
				this->set_start_stat(G_INT, lnum);
			else if (!strcmp(tag, "St03"))
				this->set_start_stat(G_WIS, lnum);
			else if (!strcmp(tag, "St04"))
				this->set_start_stat(G_CON, lnum);
			else if (!strcmp(tag, "St05"))
				this->set_start_stat(G_CHA, lnum);
			break;

		case 'T':
			if (!strcmp(tag, "Thir"))
				GET_COND(this, THIRST) = num;
			else if (!strcmp(tag, "Titl"))
				GET_TITLE(this) = str_dup(line);
			break;

		case 'W':
			if (!strcmp(tag, "Wate"))
				GET_WEIGHT(this) = num;
			else if (!strcmp(tag, "Wimp"))
				GET_WIMP_LEV(this) = num;
			else if (!strcmp(tag, "Wis "))
				this->set_wis(num);
/*29.11.09 (c) ��������*/
			else if (!strcmp(tag, "Wina"))
				GET_WIN_ARENA(this) = num;
/*����� ������ (�) ��������*/
			break;

		default:
			sprintf(buf, "SYSERR: Unknown tag %s in pfile %s", tag, name);
		}
	}

	/* initialization for imms */
	if (GET_LEVEL(this) >= LVL_IMMORT)
	{
		set_god_skills(this);
		set_god_morphs(this);
		GET_COND(this, FULL) = -1;
		GET_COND(this, THIRST) = -1;
		GET_COND(this, DRUNK) = -1;
		GET_LOADROOM(this) = NOWHERE;
	}

	/* Set natural & race features - added by Gorrah */
	for (i = 1; i < MAX_FEATS; i++)
	{
		if (can_get_feat(this, i)
			&& feat_info[i].natural_classfeat[(int) GET_CLASS(this)][(int) GET_KIN(this)])
		{
			SET_FEAT(this, i);
		}
	}
	set_race_feats(this);

	if (IS_GRGOD(this))
	{
		for (i = 0; i <= MAX_SPELLS; i++)
			GET_SPELL_TYPE(this, i) = GET_SPELL_TYPE(this, i) |
									SPELL_ITEMS | SPELL_KNOW | SPELL_RUNES | SPELL_SCROLL | SPELL_POTION | SPELL_WAND;
	}
	else if (!IS_IMMORTAL(this))
	{
		for (i = 0; i <= MAX_SPELLS; i++)
		{
			if (spell_info[i].slot_forc[(int) GET_CLASS(this)][(int) GET_KIN(this)] == MAX_SLOT)
				REMOVE_BIT(GET_SPELL_TYPE(this, i), SPELL_KNOW);
// shapirus: ��������� �� ������� �� ������ ������, �� �� ���� ����������,
// ���� ������ ����
			if (GET_REMORT(this) < MIN_CAST_REM(spell_info[i], this))
				GET_SPELL_MEM(this, i) = 0;
		}
	}

	/*
	 * If you're not poisioned and you've been away for more than an hour of
	 * real time, we'll set your HMV back to full
	 */
	if (!AFF_FLAGGED(this, AFF_POISON) && (((long)(time(0) - LAST_LOGON(this))) >= SECS_PER_REAL_HOUR))
	{
		GET_HIT(this) = GET_REAL_MAX_HIT(this);
		GET_MOVE(this) = GET_REAL_MAX_MOVE(this);
	}
	else
		GET_HIT(this) = MIN(GET_HIT(this), GET_REAL_MAX_HIT(this));

	fbclose(fl);
	// ����� �� ������������� �� ��, ��� ��� ������ ��� ��� ������ ������������ � ��� ���������,
	// ����� � ������� crc ����� ������ �����, �.�. ���� �����-������� ��� �� ������������
	// � � ����� ������ ��� ������ ��� ��� ������������� �� �����
	FileCRC::check_crc(filename, FileCRC::PLAYER, GET_UNIQUE(this));

	return (id);
}

bool Player::get_disposable_flag(int num)
{
	if (num < 0 || num >= TOTAL_DISPOSABLE_NUM)
	{
		log("SYSERROR: num=%d (%s %s %d)", num, __FILE__, __func__, __LINE__);
		return false;
	}
	return disposable_flags_[num];
}

void Player::set_disposable_flag(int num)
{
	if (num < 0 || num >= TOTAL_DISPOSABLE_NUM)
	{
		log("SYSERROR: num=%d (%s %s %d)", num, __FILE__, __func__, __LINE__);
		return;
	}
	disposable_flags_.set(num);
}

bool Player::is_active() const
{
	return motion_;
}

void Player::set_motion(bool flag)
{
	motion_ = flag;
}
