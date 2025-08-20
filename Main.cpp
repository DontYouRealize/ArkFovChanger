#include <Windows.h>
#include <iostream>

int main() {
    // gets the window handle
    HWND hWnd = FindWindowW(0, L"ARK: Survival Evolved");

    // check if the window was found
    if (hWnd == NULL) {
        std::cerr << "Game window not found. Please make sure the game is running." << std::endl;
        // Pause and exit if the window isnt found
        return 1;
    }

    // Process ID (PID)
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    // checks if couldnt open process
    if (pHandle == NULL) {
        std::cerr << "Could not open process." << std::endl;
        system("pause");
        return 1;
    }

    // The address to read from, a placeholder for the FOV address
    uintptr_t fovAddress = 0x151EE85BA50;
    float fovValue = 0.0f;

    // Read the fov value from the placeholder address
    if (ReadProcessMemory(pHandle, (LPCVOID)fovAddress, &fovValue, sizeof(fovValue), NULL)) {
        std::cout << "Current FOV value: " << fovValue << std::endl;
    }
    else {
        std::cerr << "Failed to read memory." << std::endl;
        CloseHandle(pHandle);
        system("pause");
        return 1;
    }

    // user input type shi
    while (true) {
        float newFovValue = 0.0f;
        std::cout << "\nEnter a new FOV value to set (e.g., 90.0): ";
        std::cin >> newFovValue;

        // Write the new fov value to the games memory
        if (WriteProcessMemory(pHandle, (LPVOID)fovAddress, &newFovValue, sizeof(newFovValue), NULL)) {
            std::cout << "Successfully wrote new FOV value." << std::endl;

            // read the fov again to confirm
            ReadProcessMemory(pHandle, (LPCVOID)fovAddress, &fovValue, sizeof(fovValue), NULL);
            // if it fovaddress changes were right then yay
            std::cout << "Confirmed FOV value: " << fovValue << std::endl;

        }
        else {
            std::cerr << "failed to write to the memory. error code: " << GetLastError() << std::endl;
        }
    }
}
