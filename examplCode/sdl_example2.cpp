
#include <iostream>

#include <unistd.h>
#include <stdio.h> /* for printf() */

#include <SDL2/SDL.h>

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 4000;

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
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
            }
        }

        xDir = SDL_JoystickGetAxis(gGameController, 0);
        yDir = SDL_JoystickGetAxis(gGameController, 1);

        if (SDL_JoystickGetButton(gGameController, 0)) {
            quit = true;
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
