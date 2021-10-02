#pragma once

#include <ntifs.h>
#include <ntddk.h>
#include <ntifs.h>
#include <ntddk.h>

#include "Ntapi.h"


class Memory
{
public:
	Memory(PEPROCESS process);
	uintptr_t ReadMemoryPtr(uintptr_t address);
	uintptr_t ReadMemoryPtr32(uintptr_t address);
	INT ReadMemoryInt(uintptr_t address);
	float ReadMemoryFloat(uintptr_t address);
	BOOLEAN ReadMemoryBool(uintptr_t address);
	NTSTATUS WriteMemoryPtr(uintptr_t address, uintptr_t value);
	NTSTATUS WriteMemoryInt(uintptr_t address, INT value);
	NTSTATUS WriteMemoryFloat(uintptr_t address, float value);
	NTSTATUS WriteMemoryGlow(uintptr_t address);
	NTSTATUS WriteMemoryBool(uintptr_t address, BOOLEAN value);

private:

	PEPROCESS mProcess = NULL;

	NTSTATUS KeReadProcessMemory(PVOID source, PVOID target, SIZE_T size);
	NTSTATUS KeWriteProcessMemory(PVOID source, PVOID target, SIZE_T size);
};

