
#include <unistd.h>
#include <iostream>

#include "SMStructs.h"
#include "SMFuncs.h"

#define MAX_LASER_RETRYS 100

int main() {
    std::cout << "Laser: In LaserMain" << std::endl;
    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        exit(1);
    } 
    Laser * sharedLaser = (Laser*)accuireSHMem(LASER_KEY, sizeof(Laser));
    if (sharedLaser == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        releaseSHMem(sharedPM);
        exit(1);
    } 
    int retrys = 0;
    std::cout << "Laser: Starting up" << std::endl;
    sharedLaser->XRange[0] = 0;
    sharedLaser->YRange[0] = 100;

	while (!sharedPM->Shutdown.Flags.Laser) {
        usleep(100*1000);
        //--CHeck Heartbeat--
        std::cout << "Laser: checking heartbeat " << (int)sharedPM->Heartbeats.Flags.Laser << std::endl;
		if (sharedPM->Heartbeats.Flags.Laser == PM_PROBE) {
			sharedPM->Heartbeats.Flags.Laser = PM_RESPONSE;
            retrys = 0;
		} else {
			retrys += 1;
            if (retrys >= MAX_LASER_RETRYS) {
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
            }
        }

        if (sharedLaser->XRange[0] >= 100) {
            sharedLaser->XRange[0] = 0;
        } else {
            sharedLaser->XRange[0] += 1;
        }
        if (sharedLaser->YRange[0] <= 0) {
            sharedLaser->YRange[0] = 100;
        } else {
            sharedLaser->YRange[0] -= 1;
        }
        std::cout << sharedLaser->XRange[0] << " " << sharedLaser->YRange[0] << std::endl;
	}
    releaseSHMem(sharedPM);
    releaseSHMem(sharedLaser);
}
