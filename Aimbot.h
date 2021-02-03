#pragma once

typedef struct Vector {
	float x, y, z;
}Vector;

float degreesToRadians(float angle_in_degrees);
bool EntIsVisible(DWORD ent, DWORD local);
float getFOV(float *src, Vector flLocalAngles);
DWORD WINAPI entRefresher(LPVOID lpParam);
float scrToWorld(float X, float Y, float Z, float eX, float eY, float eZ);
void CalcAngle(Vector src, Vector dst, float *angles);
void Smooth(float x, float y, float *src, float *back, Vector flLocalAngles, float smooth);
float CloseEnt();
float AngleDifference(float* ViewAngles, float* TargetAngles, float Distance);
DWORD WINAPI Aimbot(LPVOID lpParam);


