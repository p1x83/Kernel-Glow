#pragma once

#include <ntifs.h>
#include <ntddk.h>

#include "structure.h"

typedef PVOID(*fnPsGetProcessWow64Process)(PEPROCESS Process);

class Process
{
public:
	Process(CHAR* processName);
	uintptr_t GetModuleBaseAddress(WCHAR* moduleName);
	NTSTATUS GetProcess(PEPROCESS* pProcess);

private:
	PEPROCESS mProcess = NULL;

	NTSTATUS FindProcessByName(CHAR* processName);
};