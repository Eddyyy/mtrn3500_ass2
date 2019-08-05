
#include <iostream>
#include <cmath>

#include <unistd.h>
#include <stdio.h> /* for printf() */

#include <SDL2/SDL.h>

#include "SMFuncs.h"
#include "SMStructs.h"
#include "ass2Util.h"

#define MAX_XBOX_RETRYS 200

#define STEERING_MAX 40.0
#define SPEED_MAX 1.0

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

double mapVal(double input, double minSrc, double maxSrc, double minDest, double maxDest);

int main(int argc, char** argv) {
    std::cout << "Xbox: In xboxMain" << std::endl;
    
    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        return EXIT_FAILURE;
    }
    Remote * sharedRemote = (Remote*)accuireSHMem(REMOTE_KEY, sizeof(Remote));
    if (sharedRemote == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        releaseSHMem(sharedPM);
        return EXIT_FAILURE;
    }
   
    //--Initialize SDL--
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        releaseSHMem(sharedPM);
        return EXIT_FAILURE;
    }
    
    //Check for joysticks
    if(SDL_NumJoysticks() < 1) {
        std::cerr <<  "Warning: No joysticks connected!" << std::endl;
        releaseSHMem(sharedPM);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    else {
        //Load joystick
        gGameController = SDL_JoystickOpen(0);
        if(gGameController == NULL) {
            std::cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;
            releaseSHMem(sharedPM);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }

    std::cout << "Xbox: Starting up" << std::endl;

    //Main loop flag
    bool quit = false;
    int retrys = 0;

    //Event handler
    SDL_Event e;

    //Normalized direction
    int16_t xDir = 0;
    int16_t xDirOld = 0;
    int16_t yDir = 0;
    int16_t yDirOld = 0;
    usleep(100*1000);

    //--Wait for main loop--
    while (!sharedPM->Started.Flags.Xbox) {
        usleep(1000);
    }
    usleep(1000);
    sharedPM->Started.Flags.Xbox = 0;

    //--Main Loop--
    std::cout << "Xbox: Entering Main Loop" << std::endl;
    while (!sharedPM->Shutdown.Flags.Xbox and !quit) {
        usleep(10*1000);
        //--Check Heartbeat--
        std::cout << "Xbox: checking heartbeat " << (int)sharedPM->Heartbeats.Flags.Xbox << std::endl;
        if (sharedPM->Heartbeats.Flags.Xbox == PM_PROBE) {
            sharedPM->Heartbeats.Flags.Xbox = PM_RESPONSE;
            retrys = 0;
        } else {
            retrys += 1;
            if (retrys >= MAX_XBOX_RETRYS) {
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
            }   
        }

        //--Handle events on queue--
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
            } else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
                std::cout << "Something Disconnected" << std::endl;
                if (e.jdevice.which == 0) {
                    sharedRemote->SetSteering = 0;
                    sharedRemote->SetSpeed = 0;
                    quit = true;
                    sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                }
            } else if( e.type == SDL_JOYAXISMOTION ) {
                //Motion on controller 0
                if( e.jaxis.which == 0 ) {
                    if( e.jaxis.axis == 0 ) { //X axis motion
                        xDir = e.jaxis.value;
                    } else if( e.jaxis.axis == 1 ) { //Y axis motion
                        yDir = e.jaxis.value;
                    }
                }
            } else if (e.type == SDL_JOYBUTTONDOWN) {
                //Button on controller 0
                if( e.jbutton.which == 0 ) {
                    if (e.jbutton.button == 0) {
                        sharedPM->Shutdown.Status = SHUTDOWN_ALL;
                    }
                }
            }
        }

        if (pow(xDir, 2) + pow(yDir, 2) < pow(JOYSTICK_DEAD_ZONE, 2)) {
            xDir = 0;
            yDir = 0;
        }
        //Calculate angle
        double joystickAngle = atan2( (double)yDir, (double)xDir ) * ( 180.0 / M_PI );
        
        //Correct angle
        if( xDir == 0 && yDir == 0 ) {
            joystickAngle = 0;
        }
        if (xDir != xDirOld or yDir != yDirOld) {
            std::cout << "Angle " << joystickAngle << std::endl;
            std::cout << "X Axis " << xDir << " Y Axis " << yDir << std::endl;
            sharedRemote->SetSteering = mapVal((double)xDir, INT16_MIN, INT16_MAX, -STEERING_MAX, STEERING_MAX);
            sharedRemote->SetSpeed = mapVal((double)yDir, INT16_MIN, INT16_MAX, -SPEED_MAX, SPEED_MAX);
        }

        usleep(1000);
        xDirOld = xDir;
        yDirOld = yDir;
    }
    sharedRemote->SetSteering = 0;
    sharedRemote->SetSpeed = 0;

    SDL_JoystickClose(gGameController);
    gGameController = NULL;
    SDL_Quit();
    releaseSHMem(sharedPM);
    releaseSHMem(sharedRemote);
    std::cout << "Exiting Normally" << std::endl;
}

double mapVal(double input, double minSrc, double maxSrc, double minDest, double maxDest) {
        return ((input-minSrc)/(maxSrc-minSrc))*(maxDest-minDest)+minDest;
}
