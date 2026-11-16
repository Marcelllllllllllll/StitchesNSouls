/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#pragma once

#if !UE_BUILD_SHIPPING

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#endif

#if PLATFORM_LINUX
#include <sys/mman.h>
#include <unistd.h>
#endif

#if PLATFORM_WINDOWS || PLATFORM_LINUX

struct FHotPatch
{
	template <typename FunctionType>
	static bool Hook(FunctionType* From, FunctionType* To)
	{
		uint64* FromAddress = reinterpret_cast<uint64*>(From);
		uint64* ToAddress = reinterpret_cast<uint64*>(To);

		uint8 Patch[] =
		{
			0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x41, 0xFF, 0xE2
		};

		FMemory::Memcpy(&Patch[2], &ToAddress, sizeof(ToAddress));
		
#if PLATFORM_WINDOWS
		DWORD BaseProtection;
		const DWORD NewProtection = PAGE_EXECUTE_READWRITE;
		if (!VirtualProtect(FromAddress, sizeof(Patch), NewProtection, &BaseProtection))
		{
			return false;
		}

		FMemory::Memcpy(FromAddress, Patch, sizeof(Patch));
		VirtualProtect(FromAddress, sizeof(Patch), BaseProtection, &BaseProtection);
		FlushInstructionCache(GetCurrentProcess(), nullptr, 0);

		return true;
		
#elif PLATFORM_LINUX
		const size_t PageSize = sysconf(_SC_PAGESIZE);
		const uintptr_t PatchStart = reinterpret_cast<uintptr_t>(FromAddress);
		const uintptr_t PageStartAddress = PatchStart & ~(PageSize - 1);
		void* PageStart = reinterpret_cast<void*>(PageStartAddress);
		const size_t ProtectLength = (PatchStart + sizeof(Patch)) - PageStartAddress;

		if (mprotect(PageStart, ProtectLength, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
		{
			return false;
		}

		FMemory::Memcpy(FromAddress, Patch, sizeof(Patch));
		mprotect(PageStart, ProtectLength, PROT_READ | PROT_EXEC);

		__builtin___clear_cache(
			reinterpret_cast<char*>(FromAddress),
			reinterpret_cast<char*>(FromAddress) + sizeof(Patch)
		);

		return true;
#endif
		
	}
};

#endif


#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#endif