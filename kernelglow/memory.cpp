#include "memory.h"
#include <cstdint>
#include <minwindef.h>

Memory::Memory(PEPROCESS process)
{
	mProcess = process;
}

uintptr_t Memory::ReadMemoryPtr(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return NULL;
	}

	uintptr_t value = NULL;

	NTSTATUS status = KeReadProcessMemory((PVOID)address, &value, sizeof(uintptr_t));

	if (NT_SUCCESS(status))
	{
		return value;
	}

	return NULL;
}

uintptr_t Memory::ReadMemoryPtr32(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return NULL;
	}

	uintptr_t value = NULL;
	NTSTATUS status = KeReadProcessMemory((PVOID)address, &value, sizeof(uintptr_t));

	if (NT_SUCCESS(status))
	{
		return (uintptr_t)PtrToPtr32((PVOID)value);
	}
	return NULL;
}

INT Memory::ReadMemoryInt(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return NULL;
	}

	INT value = NULL;
	NTSTATUS status = KeReadProcessMemory((PVOID)address, &value, sizeof(INT));

	if (NT_SUCCESS(status))
	{
		return value;
	}
	return NULL;
}


float Memory::ReadMemoryFloat(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return NULL;
	}

	float value = NULL;
	NTSTATUS status = KeReadProcessMemory((PVOID)address, &value, sizeof(float));

	if (NT_SUCCESS(status))
	{
		return value;
	}
	return NULL;
}

BOOLEAN Memory::ReadMemoryBool(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return NULL;
	}

	uintptr_t value = NULL;
	NTSTATUS status = KeReadProcessMemory((PVOID)address, &value, sizeof(BOOLEAN));

	if (NT_SUCCESS(status))
	{
		return (BOOLEAN)value;
	}
	return NULL;
}

NTSTATUS Memory::WriteMemoryPtr(uintptr_t address, uintptr_t value)
{
	if (mProcess == NULL)
	{
		return STATUS_NOT_FOUND;
	}

	return KeWriteProcessMemory(&value, (PVOID)address, sizeof(uintptr_t));
}

NTSTATUS Memory::WriteMemoryInt(uintptr_t address, INT value)
{
	if (mProcess == NULL)
	{
		return STATUS_NOT_FOUND;
	}

	return KeWriteProcessMemory(&value, (PVOID)address, sizeof(INT));
}

NTSTATUS Memory::WriteMemoryFloat(uintptr_t address, float value)
{
	if (mProcess == NULL)
	{
		return STATUS_NOT_FOUND;
	}

	return KeWriteProcessMemory(&value, (PVOID)address, sizeof(float));
}

NTSTATUS Memory::WriteMemoryGlow(uintptr_t address)
{
	if (mProcess == NULL)
	{
		return STATUS_NOT_FOUND;
	}

	struct glowStruct {
		float red = 1.f;
		float green = 0.f;
		float blue = 0.f;
		float alpha = 0.7f;
		uint8_t padding[8];
		float unknown = 1.f;
		uint8_t padding2[4];
		BYTE renderOccluded = true;
		BYTE renderUnoccluded = false;
		BYTE fullBloom = false;
	} glow;

	return KeWriteProcessMemory(&glow, (PVOID)address, sizeof(glow));
}

NTSTATUS Memory::WriteMemoryBool(uintptr_t address, BOOLEAN value)
{
	if (mProcess == NULL)
	{
		return STATUS_NOT_FOUND;
	}

	return KeWriteProcessMemory(&value, (PVOID)address, sizeof(BOOLEAN));
}

NTSTATUS Memory::KeReadProcessMemory(PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
	PEPROCESS sourceProcess = mProcess;
	PEPROCESS targetProcess = IoGetCurrentProcess();

	SIZE_T result;

	if (NT_SUCCESS(MmCopyVirtualMemory(sourceProcess, sourceAddress, targetProcess, targetAddress, size, KernelMode, &result)))
	{
		return STATUS_SUCCESS;
	}

	return STATUS_ACCESS_DENIED;
}

NTSTATUS Memory::KeWriteProcessMemory(PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
	PEPROCESS sourceProcess = IoGetCurrentProcess();
	PEPROCESS targetProcess = mProcess;

	SIZE_T result;

	if (NT_SUCCESS(MmCopyVirtualMemory(sourceProcess, sourceAddress, targetProcess, targetAddress, size, KernelMode, &result)))
	{
		return STATUS_SUCCESS;
	}

	return STATUS_ACCESS_DENIED;
}