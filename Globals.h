#pragma once
#include "dxoverlay.h"
#include "BladeMemory.h"
#include "RankReveal.h"
#include <string>
#include <vector>

//#include <windows.h>
namespace Globals
{
	extern bool rendernames;
	extern bool renderboxes;
	extern bool renderhps;
	extern bool renderonlyenemy;

	extern bool teamcheck ;
	extern bool vischeckswitch ;
	extern bool walkbotswitch;
	extern bool GlowTeamCheck ;
	extern float GlowTerroristRed ;
	extern float GlowTerroristGreen ;
	extern float GlowTerroristBlue ;
	extern float GlowTerroristAlpha ;

	extern float GlowCounterTerroristRed ;
	extern float GlowCounterTerroristGreen ;
	extern float GlowCounterTerroristBlue ;
	extern float GlowCounterTerroristAlpha ;
	extern int triggerdelay ;
	extern float aimsmooth ;
	extern int aimfov ;
	extern int aimbone;
	extern int aimkey ;
	extern int triggerbotkey;
	extern bool tbotmode;

	extern BladeMemory gamememory;
	extern dx_overlay overlay;
	extern DWORD dwEngine;
	extern DWORD dwClient;

	extern MODULEENTRY32 clientmodule;

	extern int glowesptogglekey;
	extern int triggerbottogglekey;
	extern int aimbottogglekey;

	extern int oldglowsel;
	extern int oldtsel;
	extern int oldaimsel;
	extern int spambind;
	extern bool enablespambind;

	extern int menukey;
	extern int blockertargetkey;

	extern std::string command;

	extern std::vector<PlayerRanking>playerranklist;
}
extern int getgamestate();

extern HANDLE StartThread(LPTHREAD_START_ROUTINE func);
extern HANDLE tbotthread;
extern HANDLE glowthread;
extern HANDLE bhopthread;
extern HANDLE entrefreshthread;
extern HANDLE aimthread;
extern HANDLE walkbotthread;
extern HANDLE espoverlayinfothread;
extern HANDLE esprenderthread;
extern HANDLE commandmanagerthread;

#define VERSION "7.7.7.1 "
#define NOTES "\n CHANGELOG:\n~Minor fix for offset parser";