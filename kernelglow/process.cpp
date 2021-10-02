#include "process.h"

Process::Process(CHAR* processName)
{
    FindProcessByName(processName);
}

uintptr_t Process::GetModuleBaseAddress(WCHAR* moduleName)
{
    fnPsGetProcessWow64Process PsGetProcessWow64Process;
    UNICODE_STRING routineName;
    RtlInitUnicodeString(&routineName, L"PsGetProcessWow64Process");
    PsGetProcessWow64Process = (fnPsGetProcessWow64Process)MmGetSystemRoutineAddress(&routineName);
    KAPC_STATE AttachState;
    KeStackAttachProcess(mProcess, &AttachState);
    PPEB32 pPeb32 = (PPEB32)PsGetProcessWow64Process(mProcess);
    if (!pPeb32 || !pPeb32->Ldr)
    {
        KeUnstackDetachProcess(&AttachState);
        return 0;
    }

    for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)
        ->InLoadOrderModuleList.Flink;
        pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
        pListEntry = (PLIST_ENTRY32)pListEntry->Flink) {
        PLDR_DATA_TABLE_ENTRY32 pEntry =
            CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

        if (wcscmp((PWCH)pEntry->BaseDllName.Buffer, moduleName) == 0)
        {
            uintptr_t ModuleAddress = pEntry->DllBase;
            KeUnstackDetachProcess(&AttachState);
            return ModuleAddress;
        }
    }

    KeUnstackDetachProcess(&AttachState);
    return 0;
}

NTSTATUS Process::GetProcess(PEPROCESS* pProcess)
{
    if (mProcess != NULL)
    {
        *pProcess = mProcess;
        return STATUS_SUCCESS;
    }

    return STATUS_NOT_FOUND;
}

// Read the README if this function gives you a system exception (bluescreen)
NTSTATUS Process::FindProcessByName(CHAR* processName)
{
    PEPROCESS SystemProcess = PsInitialSystemProcess;
    PEPROCESS CurrentEntry = SystemProcess;

    CHAR ImageName[15];

    do
    {
        RtlCopyMemory((PVOID)(&ImageName), (PVOID)((uintptr_t)CurrentEntry + 0x5a8), sizeof(ImageName));

        if (strstr(ImageName, processName))
        {
            DWORD32 ActiveThreads;
            RtlCopyMemory((PVOID)&ActiveThreads, (PVOID)((uintptr_t)CurrentEntry + 0x5f0), sizeof(ActiveThreads));
            if (ActiveThreads)
            {
                mProcess = CurrentEntry;
                return STATUS_SUCCESS;
            }
        }

        PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(CurrentEntry) + 0x448);
        CurrentEntry = (PEPROCESS)((uintptr_t)list->Flink - 0x448);

    } while (CurrentEntry != SystemProcess);

    return STATUS_NOT_FOUND;
}