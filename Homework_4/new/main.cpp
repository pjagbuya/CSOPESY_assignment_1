#include "Lib.h"
#include "OperatingSystem.cpp"

int main() {
    OperatingSystem os;
    
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