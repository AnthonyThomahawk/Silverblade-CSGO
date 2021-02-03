#include "Globals.h"
#include "Offsets.h"
#include <string>

#ifndef RankReveal_H
#define RankReveal_H


class PlayerRanking
{
public:
	std::string Rank;
	std::string Name;
	int Wins;
};

void getrankdata();

#endif