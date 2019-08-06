
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include "SMFuncs.h"
#include "SMStructs.h"
#include "ass2Util.h"

#define DUMMY_BINARY "DummyModule"
#define DISPLAY_BINARY "DisplayModule"
#define XBOX_BINARY "XboxModule"
#define VEHICLE_BINARY "VehicleModule"
#define LASER_BINARY "LaserModule"
#define GPS_BINARY "GPSModule"


using namespace std;


int main(int argc, char ** argv) {
    std::string consoleStarter = "xterm -e \'bin/";
    std::vector<std::string> programsToRun = {
        //DUMMY_BINARY,
        DISPLAY_BINARY,
        LASER_BINARY,
        GPS_BINARY,
        VEHICLE_BINARY,
        XBOX_BINARY,
    };

    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem PM failed" << std::endl;
        exit(1);
    }

    Laser * sharedLaser = (Laser*)accuireSHMem(LASER_KEY, sizeof(Laser));
    if (sharedLaser == NULL) {
        std::cerr << "SHmem Laser failed" << std::endl;
        releaseSHMem(sharedPM);
        exit(1);
    }

    GPS * sharedGPS = (GPS*)accuireSHMem(GPS_KEY, sizeof(GPS));
    if (sharedGPS == NULL) {
        std::cerr << "SHmem GPS failed" << std::endl;
        releaseSHMem(sharedPM);
        releaseSHMem(sharedLaser);
        exit(1);
    }

    Remote * sharedRemote = (Remote*)accuireSHMem(REMOTE_KEY, sizeof(Remote));
    if (sharedRemote == NULL) {
        std::cerr << "SHmem Remote failed" << std::endl;
        releaseSHMem(sharedPM);
        releaseSHMem(sharedLaser);
        releaseSHMem(sharedGPS);
        exit(1);
    }

    //--Process Startup--
    // Set heartbeats and shutdown flags
    sharedPM->Shutdown.Status = 0x00;
    sharedPM->Heartbeats.Status = 0x00;
    sharedPM->Started.Status = 0x00;

    //new iteration in c++11
    for (std::string program : programsToRun) {
        std::string progToRun = consoleStarter + program + "\'";
        try {
            forkAndSystem(progToRun);
        } catch (badForkException& err) {
            std::cerr << err.what() << std::endl;
            return EXIT_FAILURE;
        } catch (childFinishException& err) {
            return std::stoi(err.what());
        }
    }

    usleep(1000*1000);
    // Reset heartbeats and shutdown flags
    // Signal main loop start
    sharedPM->Shutdown.Status = 0x00;
    sharedPM->Heartbeats.Status = 0x00;
    sharedPM->Started.Status = 0xFF;

    //--Main Loop--
    std::cout << "PM: Entering Main Loop" << std::endl;
    while (!sharedPM->Shutdown.Flags.PM and !kbhit()) {
        usleep(2000*1000);

        if (sharedPM->Started.Flags.Dummy == STARTUP_RESET) {
            //--Check Dummy Heartbeats--
            std::cout << "Dummy Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Dummy << std::endl;
            if (sharedPM->Heartbeats.Flags.Dummy == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.Dummy = PM_PROBE;
                std::cout << "Changed Dummy Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Dummy << std::endl;
            } else {
                //Assuming Dummy device is super important
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                std::cout << "Dummy seems dead" << std::endl;
            }
        }
        if (sharedPM->Started.Flags.Vehicle == STARTUP_RESET) {
            //--Check Vehicle Heartbeat--
            std::cout << "Vehicle Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Vehicle << std::endl;
            if (sharedPM->Heartbeats.Flags.Vehicle == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.Vehicle = PM_PROBE;
                std::cout << "Changed Vehicle Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Vehicle << std::endl;
            } else {
                //Assuming Vehicle device is super important
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                std::cout << "Vehicle seems dead" << std::endl;
            }
        }
        if (sharedPM->Started.Flags.Xbox == STARTUP_RESET) {
            //--Check Xbox Heartbeat--
            std::cout << "Xbox Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Xbox << std::endl;
            if (sharedPM->Heartbeats.Flags.Xbox == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.Xbox = PM_PROBE;
                std::cout << "Changed Xbox Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Xbox << std::endl;
            } else {
                //Assuming Xbox device is super important
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                std::cout << "Xbox seems dead" << std::endl;
            }
        }
        if (sharedPM->Started.Flags.Laser == STARTUP_RESET) {
            //--Check Laser Heartbeat--
            std::cout << "Laser Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Laser << std::endl;
            if (sharedPM->Heartbeats.Flags.Laser == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.Laser = PM_PROBE;
                std::cout << "Changed Laser Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Laser << std::endl;
            } else {
                //Assuming Laser device is super important
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                std::cout << "Laser seems dead" << std::endl;
            }
        }
        if (sharedPM->Started.Flags.GPS == STARTUP_RESET) {
            //--Check GPS Heartbeat--
            std::cout << "GPS Heartbeat: " << (int)sharedPM->Heartbeats.Flags.GPS << std::endl;
            if (sharedPM->Heartbeats.Flags.GPS == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.GPS = PM_PROBE;
                std::cout << "Changed GPS Heartbeat: " << (int)sharedPM->Heartbeats.Flags.GPS << std::endl;
            } else {
                //Assuming GPS device is not important
                std::cout << "GPS seems dead" << std::endl;
                sharedPM->Started.Flags.GPS = 1;
                std::string progToRun = consoleStarter + GPS_BINARY + "\'";
                try {
                    forkAndSystem(progToRun);
                } catch (badForkException& err) {
                    std::cerr << err.what() << std::endl;
                    return EXIT_FAILURE;
                } catch (childFinishException& err) {
                    return std::stoi(err.what());
                }
            }
        }
        if (sharedPM->Started.Flags.Display == STARTUP_RESET) {
            //--Check Display Heartbeat--
            std::cout << "Display Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Display << std::endl;
            if (sharedPM->Heartbeats.Flags.Display == PM_RESPONSE) {
                sharedPM->Heartbeats.Flags.Display = PM_PROBE;
                std::cout << "Changed Display Heartbeat: " << (int)sharedPM->Heartbeats.Flags.Display << std::endl;
            } else {
                //Assuming Display device is not important
                std::cout << "Display seems dead" << std::endl;
                sharedPM->Started.Flags.Display = 1;
                std::string progToRun = consoleStarter + DISPLAY_BINARY + "\'";
                try {
                    forkAndSystem(progToRun);
                } catch (badForkException& err) {
                    std::cerr << err.what() << std::endl;
                    return EXIT_FAILURE;
                } catch (childFinishException& err) {
                    return std::stoi(err.what());
                }
            }
        }

        if (sharedPM->Started.Status == 0xFF) {
            sharedPM->Shutdown.Status = SHUTDOWN_ALL;
        }
    }

    std::cout << "Shutting down Flags " << (int)sharedPM->Shutdown.Status << std::endl;
    //--Shutdown Sequence--
    //sharedPM->Shutdown.Status = SHUTDOWN_ALL;
    releaseSHMem(sharedPM);
    releaseSHMem(sharedLaser);
    releaseSHMem(sharedGPS);

    int errnum = 0;
    while (errnum != ECHILD) {
        int status;
	    wait(&status); 
	    errnum = errno;
        if (WIFEXITED(status)) {
            printf("Exit status: %d\n", WEXITSTATUS(status)); 
        } else if (WIFSIGNALED(status)) {
            psignal(WTERMSIG(status), "Exit signal");
        }
    }
    std::cout << "Ending PMmain" << std::endl;
    return 0;
}
