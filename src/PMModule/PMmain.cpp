
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

using namespace std;


int main(int argc, char ** argv) {
    std::string consoleStarter = "xterm -e \'bin/";
    std::vector<std::string> programsToRun = {
        //"DummyModule"
        "LaserModule"
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

    //--Process Startup--
    // Set heartbeats and shutdown flags
    sharedPM->Shutdown.Status = 0x00;
    sharedPM->Heartbeats.Status = 0x00;
    sharedPM->Started.Status = 0x00;

    //new iteration in c++11
    for (std::string program : programsToRun) {
        //convert string to char * for exec
        char * programToRun = (char*)malloc(256);
        std::strcpy(programToRun, (consoleStarter + program + "\'").c_str());

        // create child process
        pid_t pid = fork(); 
		int consoleStatus = 1;

        switch (pid) {
        case -1: // error
            perror("fork");
            exit(1);

        case 0: // child process

            std::cout << "in process " << pid << std::endl;
            std::cout << "running " << programToRun << std::endl;

            //system blocks control from process running it,
            //so if we run system in a new process (hence fork) we can
            //run systems for each binary in the project
            consoleStatus = system(programToRun);
            free((void*)programToRun);
            exit(consoleStatus);

        default: // parent process
            std::cout << "in process " << pid << std::endl;
            free((void*)programToRun);

            break;
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
        usleep(1000*1000);

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
            std::cout << sharedLaser->XRange[0] << " " << sharedLaser->YRange[0] << std::endl;
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
