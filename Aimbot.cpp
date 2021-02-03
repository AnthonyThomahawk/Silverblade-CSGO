#include "Aimbot.h"
#include "stdafx.h"
#include "BladeMemory.h"
#include "Globals.h"
#include "Offsets.h"
#include <cstdlib>


#define PLAYER_ON_FLOOR (1 << 0)
#define PLAYER_CROUCHED 0x6
#define M_RADPI 57.295779513082
#define PI 3.14159265359
int Ind = 0;
int lasttarget = -2;
typedef struct Vector {
	float x, y, z;
}Vector;
struct BoneBase {
	byte padding[12];
	float x;
	byte padding2[12];
	float y;
	byte padding3[12];
	float z;

};

struct PlayerStruct {
	int Team;
	bool Ignore;
	bool Dormant;
	bool Spotted;
	int	Health;
	int	GlowIndex;
	int	Base;
	Vector Pos;
	float Angle[3];
	float FOV;
};
//Stucts End

PlayerStruct Players[33];

DWORD localPlayer;
int localHealth, enemyteam;
DWORD dwPID, dwProtection, dwCaveAddress;
BOOL bPOn, bIOn, bProt;
float degreesToRadians(float angle_in_degrees) {
	return angle_in_degrees * (PI / 180.0);
}

bool EntIsVisible(DWORD ent, DWORD local)
{
	int mask = Globals::gamememory.Read<int>(ent + Offsets::SpottedByMask);
	int PBASE = Globals::gamememory.Read<int>(local + 0x64) - 1;
	return (mask & (1 << PBASE)) > 0;
}

float getFOV(float *src, Vector flLocalAngles)
{
	float back[2];
	float smoothdiff[2];
	float srcangle[2];
	srcangle[0] = src[0];
	srcangle[1] = src[1];
	srcangle[2] = src[2];

	srcangle[0] -= flLocalAngles.x;
	srcangle[1] -= flLocalAngles.y;
	if (srcangle[0] > 180)  srcangle[0] -= 360;
	if (srcangle[1] > 180)  srcangle[1] -= 360;
	if (srcangle[0] < -180) srcangle[0] += 360;
	if (srcangle[1] < -180) srcangle[1] += 360;
	smoothdiff[0] = (srcangle[0]);
	smoothdiff[1] = (srcangle[1]);
	back[0] = smoothdiff[0];
	back[1] = smoothdiff[1];
	if (back[0] > 180)  back[0] -= 360;
	if (back[1] > 180)  back[1] -= 360;
	if (back[0] < -180) back[0] += 360;
	if (back[1] < -180) back[1] += 360;
	if (back[0] > 89.0f) back[0] = 89.0f;
	else if (back[0] < -89.0f) back[0] = -89.0f;
	if (back[1] > 180.0f) back[1] = 180.0f;
	else if (back[1] < -180.0f) back[1] = -180.0f;
	back[2] = 0.f;

	return back[1];
}

DWORD WINAPI entRefresher(LPVOID lpParam) {
	while (true) {
		localPlayer = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwLocalPlayer);
		DWORD localTeam = Globals::gamememory.Read<DWORD>(localPlayer + Offsets::dwTeam);
		localHealth = Globals::gamememory.Read<int>(localPlayer + Offsets::hpoffset);
		enemyteam = localTeam == 0x3 ? 0x2 : 0x3;
			Players[0].Pos = Globals::gamememory.Read<Vector>(localPlayer + Offsets::vecOrigin);
			Vector VecView = Globals::gamememory.Read<Vector>(localPlayer + Offsets::vecViewOffset);
			Players[0].Pos.x += VecView.x;
			Players[0].Pos.y += VecView.y;
			Players[0].Pos.z += VecView.z;
		if (!localPlayer && !localHealth)
			continue;

		DWORD gamerules = Globals::gamememory.Read<DWORD>(Globals::dwClient + Offsets::dwGameRulesProxy);
		DWORD SurvivalDecisionTypes = Globals::gamememory.Read<DWORD>(gamerules + Offsets::m_SurvivalGameRuleDecisionTypes);
		DWORD dzteam = Globals::gamememory.Read<DWORD>(localPlayer + Offsets::m_nSurvivalTeam);

		bool isdangerzone;

		if (SurvivalDecisionTypes != 0)
			isdangerzone = true;
		else
			isdangerzone = false;


		for (int i = 1; i < 33; i++) {
			DWORD player = Globals::gamememory.Read<int>(Globals::dwClient + Offsets::dwEntityList + ((i - 1) * 0x10));
			if (player == 0 && player == localPlayer) {
				Players[i].Ignore = true;
				continue;
			}
			else {
				Players[i].Ignore = false;
			}

			int one = Globals::gamememory.Read<int>(player + 0x8); //IClientNetworkable vtable
			int two = Globals::gamememory.Read<int>(one + 2 * 0x4); //3rd function in the vtable (GetClientClass)
			int three = Globals::gamememory.Read<int>(two + 0x1); //pointer to the ClientClass struct out of the mov eax
			int classid = Globals::gamememory.Read<int>(three + 0x14); //classid

			if (classid != 40) // if its not a player
			{
				Players[i].Ignore = true;
				continue;
			}

			Players[i].Dormant = Globals::gamememory.Read<bool>(player + Offsets::bDormant);

			if (Players[i].Dormant) {
				Players[i].Ignore = true;
				continue;
			}
			else {
				Players[i].Ignore = false;
			}

			DWORD playerteam = Globals::gamememory.Read<DWORD>(player + Offsets::dwTeam);

			Players[i].Team = playerteam;


			if (Globals::teamcheck)
			{
				if (isdangerzone) // doesnt teamcheck correctly, but fixes jittery aim in dangerzone problem
				{
					DWORD playerdzteam = Globals::gamememory.Read<DWORD>(player + Offsets::m_nSurvivalTeam);
					if (dzteam != -1) //if ur not alone
					{
						if (playerdzteam != dzteam)
						{
							Players[i].Ignore = false;
						}
						else
						{
							Players[i].Ignore = true;
							continue;
						}
					}
					else
						Players[i].Ignore = false;
				}
				else
				{
					if (playerteam == localTeam) {
						Players[i].Ignore = true;
						continue;
					}
				}
			}
			else {
				Players[i].Ignore = false;
			}

			Players[i].Ignore = false;
			Players[i].Base = player;
				DWORD playerbonemtrix = Globals::gamememory.Read<DWORD>(player + Offsets::boneMatrix);
				BoneBase temp = Globals::gamememory.Read<BoneBase>((playerbonemtrix + (0x30 * Globals::aimbone))); // 0x30 * bone
				Players[i].Pos.x = temp.x;
				Players[i].Pos.y = temp.y;
				Players[i].Pos.z = temp.z;
				if (Globals::vischeckswitch)
				{
					Players[i].Spotted = EntIsVisible(player, localPlayer);
				}
				else
				{
					Players[i].Spotted = true;
				}
			Players[i].Health = Globals::gamememory.Read<int>(player + Offsets::hpoffset);
			Players[i].GlowIndex = Globals::gamememory.Read<int>(player + Offsets::iGlowIndex);
		}
		Sleep(1);
	}
}
float scrToWorld(float X, float Y, float Z, float eX, float eY, float eZ)
{
	return(sqrtf((eX - X) * (eX - X) + (eY - Y) * (eY - Y) + (eZ - Z) * (eZ - Z)));
}

void CalcAngle(Vector src, Vector dst, float *angles)
{
	float Delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	angles[0] = atan(Delta[2] / sqrt(Delta[0] * Delta[0] + Delta[1] * Delta[1])) * M_RADPI;
	angles[1] = atan(Delta[1] / Delta[0]) * M_RADPI;
	angles[2] = 0.0f;
	if (Delta[0] >= 0.0) angles[1] += 180.0f;
}
void Smooth(float x, float y, float *src, float *back, Vector flLocalAngles, float smooth)
{
	float smoothdiff[2];
	src[0] -= flLocalAngles.x;
	src[1] -= flLocalAngles.y;
	if (src[0] > 180)  src[0] -= 360;
	if (src[1] > 180)  src[1] -= 360;
	if (src[0] < -180) src[0] += 360;
	if (src[1] < -180) src[1] += 360;
	smoothdiff[0] = (src[0] - x) * smooth;
	smoothdiff[1] = (src[1] - y) * smooth;
	back[0] = flLocalAngles.x + smoothdiff[0];
	back[1] = flLocalAngles.y + smoothdiff[1];
	back[2] = flLocalAngles.z;
	if (back[0] > 180)  back[0] -= 360;
	if (back[1] > 180)  back[1] -= 360;
	if (back[0] < -180) back[0] += 360;
	if (back[1] < -180) back[1] += 360;
	if (back[0] > 89.0f) back[0] = 89.0f;
	else if (back[0] < -89.0f) back[0] = -89.0f;
	if (back[1] > 180.0f) back[1] = 180.0f;
	else if (back[1] < -180.0f) back[1] = -180.0f;
	back[2] = 0.f;

}

float CloseEnt()
{
	//Variables
	float fLowest = 1000000, TMP;
	int iIndex = -1;
	Vector localAngles = Globals::gamememory.Read<Vector>(Offsets::clientbase + Offsets::clientangle);

	for (int i = 1; i < 33; i++)
	{
		//Store Distances In Array
		TMP = scrToWorld(Players[0].Pos.x, Players[0].Pos.y, Players[0].Pos.z, Players[i].Pos.x, Players[i].Pos.y, Players[i].Pos.z);
		CalcAngle(Players[0].Pos, Players[i].Pos, Players[i].Angle);
		Players[i].FOV = abs(getFOV(Players[i].Angle, localAngles));
		//If Enemy Has Lower Distance The Player 1, Replace (var)Lowest With Current Enemy Distance
		bool vis;
		if (Globals::vischeckswitch)
		{
			vis = Players[i].Spotted;
		}
		else
		{
			vis = true;
		}
		if (TMP < fLowest && !Players[i].Ignore && Players[i].Base != localPlayer && Players[i].Health > 0 && vis && Players[i].FOV < Globals::aimfov)
		{
			fLowest = TMP;
			iIndex = i;
		}
	}
	return iIndex;
}

float AngleDifference(float* ViewAngles, float* TargetAngles, float Distance)
{
	float pitch = sin(degreesToRadians(ViewAngles[0] - TargetAngles[0]));
	float yaw = sin(degreesToRadians(ViewAngles[1] - TargetAngles[1]));
	return sqrt(powf(pitch, 2.0) + powf(yaw, 2.0));
}

DWORD WINAPI Aimbot(LPVOID lpParam) {
	while (true) {
		if (localPlayer && localHealth && (getgamestate() == 6)) {
			Sleep(2);
			static Vector oldAimPunch;
			static Vector fixedAngle;
			if (GetAsyncKeyState(Globals::aimkey) & 0x8000) {
				//printf("targeting\n"); // looks ok
				Ind = CloseEnt();
				bool localteamcheck;
				if (Globals::teamcheck)
				{
					localteamcheck = (Players[lasttarget].Team == enemyteam);
				}
				else
				{
					localteamcheck = true;
				}
				if (Players[lasttarget].Spotted && localteamcheck && !Players[lasttarget].Dormant && Players[lasttarget].Health > 0) {
					Ind = lasttarget;
					//printf("last target : %d\n", lasttarget); // looks ok
				}
				if (Ind != -1) {
					float smoothed[2];
					Vector aimpunch = Globals::gamememory.Read<Vector>(localPlayer + Offsets::aimPunchoffset);
					aimpunch.x = aimpunch.x * 2.f;
					aimpunch.y = aimpunch.y * 2.f;
					//printf("aimpunch x = %f", aimpunch.x); // 0
					//printf("aimpunch y = %f", aimpunch.y); // 0
					Vector localAngles = Globals::gamememory.Read<Vector>(Offsets::clientbase + Offsets::clientangle);
					CalcAngle(Players[0].Pos, Players[Ind].Pos, Players[Ind].Angle);
					//printf("start pos : x=%f ,y=%f, z = %f", Players[0].Pos.x, Players[0].Pos.y, Players[0].Pos.z);
					//printf("dest pos : x=%f ,y=%f, z = %f", Players[Ind].Pos.x, Players[Ind].Pos.y, Players[Ind].Pos.z);
					//printf("angle %f , %f , %f", Players[Ind].Angle[0], Players[Ind].Angle[1], Players[Ind].Angle[2]); // returns 0? **fixed , aimsmooth value must be greater than 1.0 or smooth func returns 0
					Smooth(aimpunch.x, aimpunch.y, Players[Ind].Angle, smoothed, localAngles, Globals::aimsmooth);
					Globals::gamememory.Write<float>(Offsets::clientbase + Offsets::clientangle, smoothed[0]);
					Globals::gamememory.Write<float>(Offsets::clientbase + Offsets::clientangle + 0x4, smoothed[1]);

					//nosmooth
					//Globals::gamememory.Write<float>(Offsets::clientbase + Offsets::clientangle, Players[Ind].Angle[0]);
					//Globals::gamememory.Write<float>(Offsets::clientbase + Offsets::clientangle + 0x4, Players[Ind].Angle[1]);
					//lasttarget = Ind; mistake that made aimbot miscalculate closest target fixed by the line below.
					Ind = CloseEnt();
				}
			}
		}
		Sleep(5);
	}
}