#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

DWORD FindProcessId(const char* processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (strcmp(pe32.szExeFile, processName) == 0) {
            CloseHandle(hSnapshot);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return 0;
}

int main() {
    const char* exeName = "LEGOBatman.exe";
    const char* dllPath = "C:\\Users\\Shadow\\Desktop\\legobatman\\testlib.dll";

    DWORD procId = FindProcessId(exeName);
    if (procId == 0) {
        cout << exeName << " process not found." << endl;
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (hProcess == NULL) {
        cout << "Failed to open " << exeName << " process. Error code: " << GetLastError() << endl;
        return 1;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pDllPath == NULL) {
        cout << "Failed to allocate memory in " << exeName << " process. Error code: " << GetLastError() << endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, dllPath, strlen(dllPath) + 1, NULL)) {
        cout << "Failed to write to " << exeName << " process memory. Error code: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, NULL);
    if (hRemoteThread == NULL) {
        cout << "Failed to create remote thread in " << exeName << " process. Error code: " << GetLastError() << endl;
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    cout << "Successfully injected into " << exeName << "!" << endl;

    WaitForSingleObject(hRemoteThread, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeThread(hRemoteThread, &exitCode);
    if (exitCode == 0) {
        cout << "DLL failed to load in " << exeName << endl;
    }

    VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
