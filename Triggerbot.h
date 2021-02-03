#pragma once
#include "windows.h"
int eGetTeam(int PlayerNumber, DWORD dwClient);
int eGetHealth(int PlayerNumber, DWORD dwClient);
void Click();
DWORD WINAPI Triggerbot(LPVOID lpParam);
