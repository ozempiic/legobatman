# LEGO Batman: The Video Game DLL Injection Method

This repository demonstrates a DLL injection method into LEGO Batman: The Video Game using C++ and Windows API.

## Purpose

The purpose of this code is to showcase how to inject a custom DLL (`testlib.dll`) into the LEGO Batman: The Video Game process (`LEGOBatman.exe`). DLL injection allows executing custom code within the game's address space.

## Files

### `main.cpp`

This file contains the main logic for injecting the DLL into the LEGO Batman: The Video Game process:

1. **Find Process ID**: Finds the process ID of `LEGOBatman.exe` using `CreateToolhelp32Snapshot` and related functions.

2. **Open Process**: Opens `LEGOBatman.exe` with `OpenProcess` using `PROCESS_ALL_ACCESS`.

3. **Allocate Memory**: Allocates memory within the game's process using `VirtualAllocEx`.

4. **Write DLL Path**: Writes the path of `testlib.dll` to the allocated memory space in the game's process using `WriteProcessMemory`.

5. **Create Remote Thread**: Creates a remote thread within the game's process to execute `LoadLibraryA` with the path to `testlib.dll`, thereby injecting the DLL.

6. **Handle Thread Execution**: Waits for the remote thread to complete using `WaitForSingleObject` and checks the exit code using `GetExitCodeThread`.

### `testlib.dll - dllmain.cpp`

This DLL is a placeholder for demonstration purposes. In a real scenario, it would contain functionality to interact with or modify the game's behavior.

## Usage

To use this method:

1. Compile `inject_dll.cpp` into an executable.
2. Ensure `LEGOBatman.exe` and `testlib.dll` are located at the specified paths (`C:\Users\Shadow\Desktop\legobatman\`).
3. Run the compiled executable. It will attempt to inject `testlib.dll` into the running instance of LEGO Batman: The Video Game.

## Notes

- Ensure you have appropriate permissions to inject into `LEGOBatman.exe`.
- This method is for educational purposes and may require adjustments for different versions of the game or operating system.
- DLL injection can trigger antivirus alerts and is often flagged as suspicious behavior, so exercise caution and use responsibly.

