#include "stdafx.h"
#include "RankReveal.h"

const char* Ranks[] =
{
		"Unranked",

		"Silver I",
		"Silver II",
		"Silver III",
		"Silver IV",
		"Silver Elite",
		"Silver Elite Master",

		"Gold Nova I",
		"Gold Nova II",
		"Gold Nova III",
		"Gold Nova Master",
		"Master Guardian I",
		"Master Guardian II",

		"Master Guardian Elite",
		"Distinguished Master Guardian",
		"Legendary Eagle",
		"Legendary Eagle Master",
		"Supreme Master First Class",
		"The Global Elite"
};

const char* DZRanks[] =
{
		"Unranked",

		"Lab rat 1",
		"Lab rat 2",
		"Sprinting hare 1",
		"Sprinting hare 2",
		"Wild Scout 1",
		"Wild Scout 2",

		"Wild Scout Elite",
		"Hunter Fox 1",
		"Hunter Fox 2",
		"Hunter Fox 3",
		"Hunter Fox Elite",
		"Timber Wolf",

		"Ember Wolf",
		"Wildfire Wolf",
		"The Howling Alpha"
};



void get_name(std::string& out, int index) {
	struct player_info_t {
		char __pad[0x10];
		char name[32];
	};

	std::uintptr_t client_state = Globals::gamememory.Read< std::uintptr_t >(Globals::dwEngine + Offsets::clientstate); //m_dwClientState
	std::uintptr_t user_info_table = Globals::gamememory.Read< std::uintptr_t >(client_state + Offsets::dwClientState_PlayerInfo); //m_dwClientState_PlayerInfo
	std::uintptr_t x = Globals::gamememory.Read< std::uintptr_t >(Globals::gamememory.Read< std::uintptr_t >(user_info_table + 0x40) + 0xC);
	player_info_t p = Globals::gamememory.Read< player_info_t >(Globals::gamememory.Read< uintptr_t >(x + 0x28 + 0x34 * (index - 1)));

	out.resize(32);
	memcpy(out.data(), p.name, 32);
}

int get_rank(int index)
{
	DWORD CCSResource = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwPlayerResource);
	return Globals::gamememory.Read<int>(CCSResource + Offsets::m_iCompetitiveRanking + (index * 4));
}

int get_wins(int index)
{
	DWORD CCSResource = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwPlayerResource);
	return Globals::gamememory.Read<int>(CCSResource + Offsets::m_iCompetitiveWins + (index * 4));
}

void getrankdata()
{
	Globals::playerranklist.clear();

	DWORD gamerules = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwGameRulesProxy);
	DWORD SurvivalDecisionTypes = Globals::gamememory.Read<DWORD>(gamerules + Offsets::m_SurvivalGameRuleDecisionTypes);

	bool isdangerzone = SurvivalDecisionTypes != 0;

	if (isdangerzone)
	{
		for (int i = 1; i < 18; i++)
		{
			std::string pname;
			get_name(pname, i);
			int r = get_rank(i);
			std::string rank = DZRanks[r];

			PlayerRanking tmp;
			tmp.Name = pname;
			tmp.Rank = rank;
			tmp.Wins = get_wins(i);

			Globals::playerranklist.push_back(tmp);
		}
	}
	else
	{
		for (int i = 1; i < 12; i++)
		{
			std::string pname;
			get_name(pname, i);
			int r = get_rank(i);
			std::string rank = Ranks[r];

			PlayerRanking tmp;
			tmp.Name = pname;
			tmp.Rank = rank;
			tmp.Wins = get_wins(i);

			Globals::playerranklist.push_back(tmp);
		}
	}

}