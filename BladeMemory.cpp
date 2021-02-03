#include "stdafx.h"
#include "BladeMemory.h"

BladeMemory::BladeMemory()
{

}

BladeMemory::BladeMemory(wchar_t * procname)
{
	pHandle = GetProcessHandleByName(procname);
}

BladeMemory::~BladeMemory()
{
}

void BladeMemory::SetProcess(wchar_t * proc)
{
	pHandle = GetProcessHandleByName(proc);
}

HANDLE BladeMemory::GetProcessHandleByName(wchar_t * processname)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			//wprintf(L"%ls \n", (entry.szExeFile));
			if (!wcscmp(entry.szExeFile, processname))
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				return hProcess;
				//printf("Process found");
				//CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(snapshot);

	return 0;
}

DWORD BladeMemory::GetModuleHandleByName(wchar_t * ModuleName) {
	MODULEENTRY32 Entry;
	DWORD ProcID = GetProcessId(pHandle);
	Entry.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcID);
	if (Module32First(hSnap, &Entry) == TRUE) {
		if (!wcscmp(Entry.szModule, ModuleName)) {
			DWORD hModule = (DWORD)Entry.modBaseAddr;
			return hModule;
		}
		else {
			while (Module32Next(hSnap, &Entry) == TRUE) {
				if (!wcscmp(Entry.szModule, ModuleName)) {
					DWORD hModule = (DWORD)Entry.modBaseAddr;
					return hModule;
				}
			}
			return 0;
		}
	}
	CloseHandle(hSnap);
}

MODULEENTRY32 BladeMemory::GetModule(wchar_t * ModuleName) {
	MODULEENTRY32 Entry;
	DWORD ProcID = GetProcessId(pHandle);
	Entry.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcID);
	if (Module32First(hSnap, &Entry) == TRUE) {
		if (!wcscmp(Entry.szModule, ModuleName)) {
			return Entry;
		}
		else {
			while (Module32Next(hSnap, &Entry) == TRUE) {
				if (!wcscmp(Entry.szModule, ModuleName)) {
					return Entry;
				}
			}
		}
	}
	CloseHandle(hSnap);
}