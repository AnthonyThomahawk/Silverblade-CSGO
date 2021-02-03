#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

typedef unsigned char byte;

class BladeMemory
{
public:
	BladeMemory();
	BladeMemory(wchar_t * procname);
	~BladeMemory();
	HANDLE pHandle;
	template <typename datatype>
	datatype Read(DWORD address)
	{
		datatype buffer;
		ReadProcessMemory(pHandle, (LPCVOID)address, &buffer, sizeof(datatype), NULL);
		return buffer;
	}

	template <typename datatype>
	void Write(DWORD address, datatype value)
	{
		WriteProcessMemory(pHandle, (LPVOID)address, &value, sizeof(value), NULL);
	}

	template < typename T >
	void Read_ex(T* out_object_ptr, std::uintptr_t address, std::size_t object_count) {
		ReadProcessMemory(pHandle, reinterpret_cast<LPCVOID>(address), out_object_ptr, sizeof T * object_count, nullptr);
	}

	template < typename T >
	bool Write_ex(T* object_ptr, std::uintptr_t address, std::size_t object_count) {
		SIZE_T bytes_written;
		WriteProcessMemory(pHandle, reinterpret_cast<LPCVOID>(address), object_ptr, sizeof T * object_count, &bytes_written);

		return bytes_written == sizeof T * object_count;
	}

	bool CompareBytes(byte* data, byte* mask, char *_mask)
	{
		for (; *_mask; ++_mask, ++data, ++mask)
			if (*_mask == 'x' && *data != *mask)
				return 0;

		return (*_mask == 0);
	}

	uintptr_t FindPattern(uintptr_t start, size_t size, char* sig, char* mask)
	{
		byte* Data = new byte[size];

		if (!ReadProcessMemory(pHandle, (void*)start, Data, size, 0))
		{
			delete[] Data;
			return 0;
		}
		for (uintptr_t i = 0; i < size; i++)
		{
			if (CompareBytes((byte*)(Data + i), (byte*)sig, mask))
			{
				delete[] Data;
				return start + i;
			}
		}
		delete[] Data;
		return 0;
	}

	bool ReadBlock(void* Dest, byte* Out, size_t Size)
	{
		return ReadProcessMemory(pHandle, Dest, Out, Size, 0);
	}

	bool WriteBlock(void* Dest, byte* Data, size_t Size)
	{
		return WriteProcessMemory(pHandle, Dest, Data, Size, 0);
	}

	void* Allocate(size_t Size)
	{
		return VirtualAllocEx(pHandle, 0, Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}

	void* AllocateWrite(std::string str)
	{
		void* block = Allocate(str.length() + 1);

		WriteBlock(block, (byte*)str.c_str(), str.length());

		return block;
	}

	void* AllocateWrite(byte* Data, size_t Size)
	{
		void* block = Allocate(Size + 1);

		WriteBlock(block, Data, Size);

		return block;
	}

	bool FreeBlock(void* block, size_t size = 0)
	{
		return VirtualFreeEx(pHandle, block, size, MEM_RELEASE);
	}


	HANDLE MakeThreadInProcess(void* At, void* parameter = nullptr)
	{
		LPDWORD tid = NULL;
		if (CreateRemoteThread(pHandle, 0, 0, (LPTHREAD_START_ROUTINE)At, parameter, 0, tid) == INVALID_HANDLE_VALUE)
			return NULL;

		return tid;
	}


	HANDLE GetProcessHandleByName(wchar_t * processname);
	DWORD GetModuleHandleByName(wchar_t * ModuleName);
	MODULEENTRY32 BladeMemory::GetModule(wchar_t * ModuleName);
	void SetProcess(wchar_t * proc);
};

