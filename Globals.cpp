#include "Globals.h"
#include "stdafx.h"
#include "BladeMemory.h"
#include "Offsets.h"
#include "dxoverlay.h"
#include "RankReveal.h"
namespace Globals
{
	bool rendernames = false;
	bool renderboxes = false;
	bool renderhps = false;
	bool renderonlyenemy = false;

	bool botmode = false;
	bool teamcheck = true;
	bool vischeckswitch = true;
	bool walkbotswitch = false;
	bool GlowTeamCheck = true;
	float GlowTerroristRed = 255.f;
	float GlowTerroristGreen = 0.f;
	float GlowTerroristBlue = 0.f;
	float GlowTerroristAlpha = 255.f;

	float GlowCounterTerroristRed = 0.f;
	float GlowCounterTerroristGreen = 0.f;
	float GlowCounterTerroristBlue = 255.f;
	float GlowCounterTerroristAlpha = 255.f;
	int triggerdelay = 5;
	float aimsmooth = 0.5;
	int aimfov = 10;
	int aimbone = 8;
	int aimkey = VK_MENU;

	int triggerbotkey= VK_LSHIFT;
	bool tbotmode = 0;

	int glowesptogglekey = VK_NUMPAD2;
	int triggerbottogglekey = VK_NUMPAD1;
	int aimbottogglekey = VK_NUMPAD3;

	int menukey = VK_INSERT;

	int oldglowsel = 5;
	int oldtsel = 4;
	int oldaimsel = 6;

	int spambind = 0x4F;
	int blockertargetkey = 0x46;
	bool enablespambind = false;

	std::string command = "";

	std::vector<PlayerRanking>playerranklist;

	BladeMemory gamememory;
	dx_overlay overlay;
	DWORD dwEngine;
	DWORD dwClient;
	MODULEENTRY32 clientmodule;
}

int getgamestate()
{
	auto EnginePointer = Globals::gamememory.Read<DWORD>(Globals::dwEngine + Offsets::clientstate);
	auto GameState = Globals::gamememory.Read<int>(EnginePointer + 0x108);
	return GameState;
}

HANDLE StartThread(LPTHREAD_START_ROUTINE func)
{
	HANDLE h;
	h =CreateThread(NULL, 0, func, 0, 0, NULL);
	return h;
}

HANDLE tbotthread;
HANDLE glowthread;
HANDLE bhopthread;
HANDLE entrefreshthread;
HANDLE aimthread;
HANDLE walkbotthread;
HANDLE espoverlayinfothread=NULL;
HANDLE esprenderthread;
HANDLE commandmanagerthread;
