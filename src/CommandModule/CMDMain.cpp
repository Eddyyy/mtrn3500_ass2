
#include <iostream>

#include "SMFuncs.h"
#include "SMStructs.h"

using namespace std;

int main() {
    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        exit(1);
    } 
    while (!sharedPM->Shutdown.Flags.PM) {
        std::string consoleInput;
        std::cout << ">>> ";
        std::cin >> consoleInput;
        if (consoleInput == "s") {
            sharedPM->Shutdown.Status = SHUTDOWN_ALL;
        }
    }
    releaseSHMem(sharedPM);
}
