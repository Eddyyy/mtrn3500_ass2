
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "SMFuncs.h"
#include "SMStructs.h"
#include "ass2Util.h"

#include "GPSStruct.h"
#include "CRCGeneration.h"

#define MAX_GPS_RETRYS 40

#define GPS_PORT 24000
#define GPS_ADDR "192.168.1.200"

#define BUFFER_LEN MSG_SIZE

using namespace std;

int main(int argc, char ** argv) {
    std::cout << "GPS: In GPSMain" << std::endl;

    //--Shared Memory Acquisition--
    PM * sharedPM = (PM*)accuireSHMem(PM_KEY, sizeof(PM));
    if (sharedPM == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        exit(1);
    }

    GPS * sharedGPS = (GPS*)accuireSHMem(GPS_KEY, sizeof(GPS));
    if (sharedGPS == NULL) {
        std::cerr << "SHmem failed" << std::endl;
        releaseSHMem(sharedPM);
        exit(1);
    } 

    std::cout << "GPS: Starting up" << std::endl;
    GPSMsg GPSIn;
    int retrys = 0;

    //--Socket connection--
	int socket_fd = 0;
	char buffer[BUFFER_LEN] = {0};
    struct sockaddr_in serv_addr;
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        std::cerr << "Socket creation error" << std::endl;
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(GPS_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, GPS_ADDR, &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(1);
    } 

    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        std::cerr << "Connection Failed \n" << std::endl;
        exit(1);
    } 

    //--Wait for main loop--
    while (!sharedPM->Started.Flags.GPS) {
        usleep(1000);
    }
    usleep(1000);
    sharedPM->Started.Flags.GPS = 0;

    //--Main Loop--
    std::cout << "GPS: Entering Main Loop" << std::endl;
    while (!sharedPM->Shutdown.Flags.GPS and !kbhit()) {
        usleep(100*1000);
        //--Check Heartbeat--
        std::cout << "GPS: checking heartbeat " << (int)sharedPM->Heartbeats.Flags.GPS << std::endl;
        if (sharedPM->Heartbeats.Flags.GPS == PM_PROBE) {
            sharedPM->Heartbeats.Flags.GPS = PM_RESPONSE;
            retrys = 0;
        } else {
            retrys += 1;
            if (retrys >= MAX_GPS_RETRYS) {
                sharedPM->Shutdown.Status = SHUTDOWN_ALL;
            }
        }

        //--GPS Response--
        usleep(100*1000);
        std::cout << "Recieveing data" << std::endl;
        std::string recvStr = "";
        int numBytes = 0;
        numBytes = recv(socket_fd, buffer, BUFFER_LEN, 0);
        if (numBytes != BUFFER_LEN) {
            std::cout << "Bad Message" << std::endl;
            continue;
        }
        std::memcpy(&GPSIn.bytes, &buffer, sizeof(buffer));
        std::cout << "Server CRC value: " << std::hex << *(uint32_t*)GPSIn.bytes.fields.CRC << std::endl;
        std::cout << "Client CRC value: " << std::hex << CalculateBlockCRC32(DATA_SIZE, GPSIn.bytes.dataAndCRC.GPSData) << std::endl;
        std::cout << "Northing: " << *(double*)GPSIn.bytes.fields.northing << std::endl;
        std::cout << "Easting: " << *(double*)GPSIn.bytes.fields.easting << std::endl;
        std::cout << "Height: " << *(double*)GPSIn.bytes.fields.height << std::endl;
        sharedGPS->Northing = *(double*)GPSIn.bytes.fields.northing;
        sharedGPS->Easting = *(double*)GPSIn.bytes.fields.easting;
        sharedGPS->Height = *(double*)GPSIn.bytes.fields.height;
        std::cout << "----" << std::endl;
    }
    releaseSHMem(sharedPM);
    releaseSHMem(sharedGPS);
    //char * valIn = 0;
    //std::cout << "Press enter to exit" << std::endl;
    //std::cin >> valIn;
}
