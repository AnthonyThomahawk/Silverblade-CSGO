#include "Globals.h"
#include "Offsets.h"

#define M_RADPI 57.295779513082
#define PI 3.14159265359

DWORD WINAPI ChangeBlockerTargetThread(LPVOID lpParam);
DWORD WINAPI BlockerThread(LPVOID lpParam);