
#include <iostream>

#include "SMFuncs.h"
#include "SMStructs.h"
#include "ass2Util.h"

#define MAX_DUMMY_RETRYS 20

using namespace std;

int main(int argc, char ** argv) {
    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        exit(1);
    }

    //--Wait for main loop--
    while (!sharedPM->Started.Flags.Dummy) {
        usleep(1000);
    }
    usleep(1000);
    sharedPM->Started.Flags.Dummy = 0;

    //--Main Loop--
    std::cout << "Dummy: Entering Main Loop" << std::endl;
    int retrys = 0;
    while (!sharedPM->Shutdown.Flags.Dummy and !kbhit()) {
        usleep(100*1000);
        //--CHeck Heartbeat--
        std::cout << "Dummy: checking heartbeat " << (int)sharedPM->Heartbeats.Flags.Dummy << std::endl;
        if (sharedPM->Heartbeats.Flags.Dummy == PM_PROBE) {
            sharedPM->Heartbeats.Flags.Dummy = PM_RESPONSE;
            retrys = 0;
        } else {
            retrys += 1;
            if (retrys >= MAX_DUMMY_RETRYS) {
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
            }   
        }
    }
    releaseSHMem(sharedPM);
}
