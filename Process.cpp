#include "Process.h"

std::map <DWORD, Process> ListProcesses(std::map<DWORD, Process>& pList)
{
    pList.clear();
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        print(_T("Could not get a Snapshot\n"));
        if (snapshot != NULL)
            CloseHandle(snapshot);
        return pList;
    }

    if (Process32First(snapshot, &entry)) { // Init Process here
        while (Process32Next(snapshot, &entry)) {
            pList.insert({ entry.th32ProcessID, { entry.th32ProcessID ,entry.szExeFile } });
        }
    }
    CloseHandle(snapshot);
    return pList;
}