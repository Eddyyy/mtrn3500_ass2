
#include <iostream>
#include <cmath>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "SMStructs.h"
#include "SMFuncs.h"
#include "ass2Util.h"

#define MAX_VEHICLE_RETRYS 30

#define VEHICLE_PORT 25000
#define VEHICLE_ADDR "192.168.1.200"

#define VEHICLE_AUTH_MSG "5117714\n"
#define BUFFER_LEN 4096

int main(int argc, char ** argv) {
    std::cout << "Vehicle: In VehicleMain" << std::endl;
    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        exit(1);
    } 
    Remote * sharedRemote = (Remote*)accuireSHMem(REMOTE_KEY, sizeof(Remote));
    if (sharedRemote == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        releaseSHMem(sharedPM);
        exit(1);
    } 

    std::cout << "Vehicle: Starting up" << std::endl;

    //--Socket connection--
	int socket_fd = 0;
	char buffer[BUFFER_LEN] = {0};
    struct sockaddr_in serv_addr;
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        std::cerr << "Socket creation error" << std::endl;
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(VEHICLE_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, VEHICLE_ADDR, &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(1);
    } 
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        std::cerr << "Connection Failed \n" << std::endl;
        exit(1);
    } 

    std::cout << "Authenticating For Vehicle" << std::endl;
    if (send(socket_fd, VEHICLE_AUTH_MSG, strlen(VEHICLE_AUTH_MSG), 0) < 0) {
        std::cerr << "Send Failed" << std::endl;
        exit(1);
    }
    if (recv(socket_fd, buffer, BUFFER_LEN, 0) > 0) {
        std::cout << buffer;
    }

    memset(buffer, 0, BUFFER_LEN * sizeof(buffer[0]));
    int retrys = 0;
    double steering = 0;
    double speed = 0;
    int flag = 0;

    //--Wait for main loop--
    while (!sharedPM->Started.Flags.Vehicle) {
        usleep(1000);
    }
    usleep(1000);
    sharedPM->Started.Flags.Vehicle = 0;

    //--Main Loop--
	while (!sharedPM->Shutdown.Flags.Vehicle and !kbhit()) {
        usleep(100*1000);
        //--Check Heartbeat--
        std::cout << "Vehicle: checking heartbeat " << (int)sharedPM->Heartbeats.Flags.Vehicle << std::endl;
		if (sharedPM->Heartbeats.Flags.Vehicle == PM_PROBE) {
			sharedPM->Heartbeats.Flags.Vehicle = PM_RESPONSE;
            retrys = 0;
		} else {
			retrys += 1;
            if (retrys >= MAX_VEHICLE_RETRYS) {
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
            }
        }

        steering = sharedRemote->SetSteering;
        speed = sharedRemote->SetSpeed;

        //--Control Message--
        std::string controlMessage = "";
        controlMessage += "# " + std::to_string(steering);
        controlMessage += " " + std::to_string(speed);
        controlMessage += " " + std::to_string(flag) + " #";
        std::cout << controlMessage << std::endl;
        if (send(socket_fd, controlMessage.c_str(), controlMessage.length(), 0) < 0) {
            std::cerr << "Send Failed" << std::endl;
            exit(1);
        }
        flag = ~flag;

    }
}
