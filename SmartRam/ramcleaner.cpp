#include "ramcleaner.h"
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

RamCleaner::RamCleaner(QObject *parent) : QObject(parent) {}

void trimProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess) {
        EmptyWorkingSet(hProcess);
        CloseHandle(hProcess);
    }
}

void trimAllProcesses() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &pe)) {
        do {
            trimProcess(pe.th32ProcessID);
        } while (Process32Next(snapshot, &pe));
    }
    CloseHandle(snapshot);
}

void memoryPressureSafe() {
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);

    std::vector<char*> blocks;

    while (true) {
        GlobalMemoryStatusEx(&mem);

        // Dừng khi còn < 500MB RAM trống
        if (mem.ullAvailPhys < 500ull * 1024ull * 1024ull)
            break;

        char* b = new char[200ull * 1024ull * 1024ull];
        memset(b, 0, 200ull * 1024ull * 1024ull);
        blocks.push_back(b);
    }

    for (auto b : blocks) delete[] b;
}

void RamCleaner::cleanRam() {
    trimAllProcesses();
    //memoryPressureSafe();
}

qint64 RamCleaner::getUsedRamMB() {
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    return (mem.ullTotalPhys - mem.ullAvailPhys) / (1024 * 1024);
}
