#include "Triggerbot.h"
#include "stdafx.h"
#include "BladeMemory.h"
#include "Globals.h"
#include "Offsets.h"
int eGetTeam(int PlayerNumber, DWORD dwClient) {
	return Globals::gamememory.Read<int>((Globals::gamememory.Read<DWORD>(dwClient + Offsets::dwEntityList + (0x10 * PlayerNumber))) + Offsets::dwTeam);

}
int eGetHealth(int PlayerNumber, DWORD dwClient) {
	return Globals::gamememory.Read<int>((Globals::gamememory.Read<DWORD>(dwClient + Offsets::dwEntityList + (0x10 * PlayerNumber))) + Offsets::hpoffset);
}
void Click() {
	mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
	Sleep(5);
	mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
}

DWORD WINAPI Triggerbot(LPVOID lpParam)
{
	
	while (true)
	{
		bool flag;
		if (Globals::tbotmode == true)
		{
			if (GetAsyncKeyState(Globals::triggerbotkey) & 0x8000)
				flag = true;
			else
				flag = false;
		}
		else
			flag = true;
		if (flag==true)
		{
			int cId = Globals::gamememory.Read<int>((Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer)) + Offsets::dwCrosshairId);
			int myTeam = Globals::gamememory.Read<int>((Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer)) + Offsets::dwTeam);
			if (cId > 0 && cId <= 64)
			{
				int eTeam = eGetTeam(cId - 1, Globals::dwClient);
				int eHealth = eGetHealth(cId - 1, Globals::dwClient);
				if (eTeam != 0 && eTeam != myTeam && (eHealth > 0))
				{
						Sleep(Globals::triggerdelay);
						Click();
				}
			}
		}
	}
	return 0;
}