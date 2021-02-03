#include "GlowESP.h"
#include "stdafx.h"
#include "BladeMemory.h"
#include "Globals.h"
#include "Offsets.h"
DWORD WINAPI GlowESP(LPVOID lpParam)
{
	while (true)
	{
		if ((getgamestate() == 6))
		{
			for (int i = 0; i < 64; i = i + 1)
			{
				DWORD glow_LocalBase = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer); //localplayer was localplayerindex
				DWORD glow_Pointer = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwGlowObject);
				int MyTeamID = Globals::gamememory.Read<int>(glow_LocalBase + Offsets::dwTeam);
				int glow_currentPlayer = Globals::gamememory.Read<int>(Globals::dwClient + Offsets::dwEntityList + i * 0x10);
				bool glow_currentPlayerDormant = Globals::gamememory.Read<bool>(glow_currentPlayer + Offsets::bDormant);
				int glow_currentPlayerGlowIndex = Globals::gamememory.Read<int>(glow_currentPlayer + Offsets::iGlowIndex);
				int EntityBaseTeamID = Globals::gamememory.Read<int>(glow_currentPlayer + Offsets::dwTeam);
				if (glow_currentPlayerDormant == 1 || EntityBaseTeamID == 0)
					continue;
				else
					//if (MyTeamID != EntityBaseTeamID || GlowTeamCheck == false)
					switch (EntityBaseTeamID)	// 1 GoTV; 2 T; 3 CT
					{
					case 2:
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), Globals::GlowTerroristRed);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), Globals::GlowTerroristGreen);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), Globals::GlowTerroristBlue);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), Globals::GlowTerroristAlpha);
						Globals::gamememory.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), true);
						Globals::gamememory.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), false);
						break;
					case 3:
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)), Globals::GlowCounterTerroristRed);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)), Globals::GlowCounterTerroristGreen);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)), Globals::GlowCounterTerroristBlue);
						Globals::gamememory.Write<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)), Globals::GlowCounterTerroristAlpha);
						Globals::gamememory.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)), true);
						Globals::gamememory.Write<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)), false);
						break;
					}
			}
		}
		Sleep(5);
	}
	return 0;
}