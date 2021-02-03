#include "stdafx.h"
#include "Blocker.h"
#include <algorithm>

typedef struct Vector {
	float x, y, z;
}Vector;

int targetind = -1;

void NormalizeAngles(Vector& vec)
{
	if (!std::isfinite(vec.x))
	{
		vec.x = 0.f;
	}

	if (!std::isfinite(vec.y))
	{
		vec.y = 0.f;
	}

	vec.y = std::remainder(vec.y, 360.f);

	vec.x = std::clamp(vec.x, -89.f, 89.f);
	vec.y = std::clamp(vec.y, -180.f, 180.f);

	vec.z = 0.f;
}

Vector CalcAngle(Vector src, Vector dst)
{
	Vector result;
	float Delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	result.x = atan(Delta[2] / sqrt(Delta[0] * Delta[0] + Delta[1] * Delta[1])) * M_RADPI;
	result.y = atan(Delta[1] / Delta[0]) * M_RADPI;
	result.z = 0.0f;
	if (Delta[0] >= 0.0) result.y += 180.0f;
	return result;
}

bool Blocker(int targetindex)
{
	DWORD localplayer = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer);
	DWORD targetplayer = Globals::gamememory.Read<int>(Globals::dwClient + Offsets::dwEntityList + ((targetindex - 1) * 0x10));

	Vector localplayerorigin = Globals::gamememory.Read<Vector>(localplayer + Offsets::vecOrigin);
	Vector targetplayerorigin = Globals::gamememory.Read<Vector>(targetplayer + Offsets::vecOrigin);

	Vector blockerangle = CalcAngle(localplayerorigin, targetplayerorigin);
	NormalizeAngles(blockerangle);

	DWORD client_state = Globals::gamememory.Read<DWORD>(Globals::dwEngine + Offsets::clientstate);

	Vector localplayerviewangles = Globals::gamememory.Read<Vector>(client_state + Offsets::clientangle);

	if (blockerangle.y < localplayerviewangles.y)
	{
		return true;
	}
	else if (blockerangle.y > localplayerviewangles.y)
	{
		return false;
	}
}

DWORD WINAPI BlockerThread(LPVOID lpParam)
{
	bool direction;
	while (true)
	{
		if (GetAsyncKeyState(0x46) & 0x8000)
		{
			targetind = Globals::gamememory.Read<int>((Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer)) + Offsets::dwCrosshairId);
		}

		if (targetind > 1) // ignoring local player and world
		{
			direction = Blocker(targetind);

			if (direction)
			{
				keybd_event(0x44, MapVirtualKeyA(0x44, MAPVK_VK_TO_VSC), 0, 0);
				Sleep(1);
				keybd_event(0x44, MapVirtualKeyA(0x44, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
			}
			else
			{
				keybd_event(0x41, MapVirtualKeyA(0x41, MAPVK_VK_TO_VSC), 0, 0);
				Sleep(1);
				keybd_event(0x41, MapVirtualKeyA(0x41, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
			}
		}
		Sleep(1);
	}
}
