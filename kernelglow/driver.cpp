#include <ntifs.h>
#include <ntddk.h>
#include <ntifs.h>
#include <ntddk.h>

#include "process.h"
#include "memory.h"
#include "offsets.h"

typedef float FLOAT;
EXTERN_C INT _fltused = 0x9875;


VOID Cheat(PVOID pVoid)
{
	UNREFERENCED_PARAMETER(pVoid);
	Process ProcessManager("csgo.exe");
	PEPROCESS GameProcess;
	NTSTATUS status = ProcessManager.GetProcess(&GameProcess);
	if (NT_SUCCESS(status))
	{
		Memory MemoryManager(GameProcess);

		uintptr_t client = ProcessManager.GetModuleBaseAddress(L"client.dll");
		while (TRUE)
		{
			if (client != NULL)
			{
				uintptr_t glowManager = MemoryManager.ReadMemoryPtr32(client + offset::dwGlowObjectManager);
				uintptr_t localplayer = MemoryManager.ReadMemoryPtr32(client + offset::dwLocalPlayer);
				INT team = MemoryManager.ReadMemoryInt(localplayer + offset::m_iTeamNum);
				for (INT i = 1; i <= 32; i++)
				{
					uintptr_t entity = MemoryManager.ReadMemoryPtr32(client + offset::dwEntityList + i * 0x10);
					INT iGlowIndex = MemoryManager.ReadMemoryInt(entity + offset::m_iGlowIndex);
					INT enemyHealth = MemoryManager.ReadMemoryInt(entity + offset::m_iHealth);

					if (enemyHealth < 1 || enemyHealth > 100)
					{
						continue;
					}

					INT dormant = MemoryManager.ReadMemoryInt(entity + offset::m_bDormant);
					if (dormant)
					{
						continue;
					}

					INT entityteam = MemoryManager.ReadMemoryInt(entity + offset::m_iTeamNum);
					if (team == entityteam)
					{
					}
					else if (team != entityteam)
					{
						MemoryManager.WriteMemoryGlow(glowManager + (iGlowIndex * 0x38) + 0x8);
					}
				}

				for (INT i = 1; i <= 32; i++)
				{
					uintptr_t entity = MemoryManager.ReadMemoryPtr32(client + offset::dwEntityList + i * 0x10);
					if (entity == NULL)
					{
						continue;
					}
					MemoryManager.WriteMemoryBool(entity + offset::m_bSpotted, TRUE);
				}
			}
		}
	}
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	// Unreference parameters since KDMapper cant map those to iqvw64e.sys
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	HANDLE hThread;
	PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, Cheat, NULL);
	return STATUS_SUCCESS;
}