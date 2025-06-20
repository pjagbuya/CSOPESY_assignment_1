#include "Lib.h"
#include "OperatingSystem.cpp"

int main() {
    OperatingSystem os;

    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE); 
    
    while (!os.IsQuit()) {
        if (!os.IsInitialized()) {
            os.PreBoot();
        } else {
            os.Run();
        }
    }
    os.Shutdown();

    return 0;
}