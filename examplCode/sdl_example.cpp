
#include <iostream>
#include <cmath>

#include <unistd.h>
#include <stdio.h> /* for printf() */

#include <SDL2/SDL.h>

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

int main(int argc, char** argv) {
    
    //Initialize SDL
    if(SDL_Init(SDL_INIT_JOYSTICK ) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    
    //Check for joysticks
    if(SDL_NumJoysticks() < 1) {
        std::cerr <<  "Warning: No joysticks connected!" << std::endl;
        return EXIT_FAILURE;
    }
    else {
        //Load joystick
        gGameController = SDL_JoystickOpen(0);
        if(gGameController == NULL) {
            std::cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }
    }

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Normalized direction
    int16_t xDir = 0;
    int16_t xDirOld = 0;
    int16_t yDir = 0;
    int16_t yDirOld = 0;

    while (!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
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
                        quit = true;
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
        }

        usleep(1000);
        xDirOld = xDir;
        yDirOld = yDir;
    }

    SDL_JoystickClose(gGameController);
    gGameController = NULL;
    SDL_Quit();
    std::cout << "Exiting Normally" << std::endl;
}
