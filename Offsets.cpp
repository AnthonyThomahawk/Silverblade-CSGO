#include "Offsets.h"
#include "stdafx.h"
//#include <Windows.h>
namespace Offsets
{
	DWORD m_vecVelocity = 0x114;
	DWORD vecViewOffset = 0x108;
	DWORD SpottedByMask = 0x980;
	DWORD boneMatrix = 0x26A8;
	DWORD vecOrigin = 0x138;
	DWORD clientbase;
	DWORD clientstate = 0x58ADD4; // dwClientState
	DWORD clientangle = 0x4D88; // dwClientState_ViewAngles
	DWORD aimPunchoffset = 0x302C;
	DWORD GetLocalPlayer = 0x180;
	DWORD dwEntityList = 0x4D5022C;
	DWORD dwLocalPlayer = 0xD3BBEC;
	DWORD dwCrosshairId = 0xB3E4;
	DWORD dwViewMatrix = 0x4D41B74;
	DWORD dwTeam = 0xF4;
	DWORD hpoffset = 0x100;
	DWORD dwGlowObject = 0x5298070;
	DWORD bDormant = 0xED;
	DWORD iGlowIndex = 0xA438;
	DWORD Attack = 0x318179C;
	DWORD dwPlayerResource = 0x317FB10;
	DWORD dwClientState_PlayerInfo = 0x52B8;
	DWORD m_iCompetitiveRanking = 0x1A84;
	DWORD m_iCompetitiveWins = 0x1B88;
	DWORD dwGameRulesProxy = 0x526D1BC;
	DWORD m_SurvivalGameRuleDecisionTypes = 0x1320;
	DWORD m_nSurvivalTeam = 0xA3C0;
}