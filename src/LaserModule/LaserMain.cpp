
#include <iostream>
#include <cmath>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <boost/algorithm/string.hpp>

#include "SMStructs.h"
#include "SMFuncs.h"
#include "ass2Util.h"

#define PI 3.14159265

#define MAX_LASER_RETRYS 15

#define LASER_PORT 23000
#define LASER_ADDR "192.168.1.200"

#define LASER_AUTH_MSG "5117714\n"
#define DATA_REQ_MSG "sRN LMDscandata"
#define BUFFER_LEN 4096
#define MSG_END_ZEROS 5

#define MEASRUE_TYPE_INDEX 19
#define STARTANGLE_INDEX 22
#define RESOLUTION_INDEX 23
#define NUMPOINTS_INDEX 24
#define START_DATA_INDEX 25

int parseLaserData(std::string recvStr, std::vector<std::string> &parsedString);

int main(int argc, char ** argv) {
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

    //--Socket connection--
	int socket_fd = 0;
	char buffer[BUFFER_LEN] = {0};
    struct sockaddr_in serv_addr;
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        std::cerr << "Socket creation error" << std::endl;
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(LASER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, LASER_ADDR, &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(1);
    } 
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        std::cerr << "Connection Failed \n" << std::endl;
        exit(1);
    } 

    std::cout << "Authenticating For Laser" << std::endl;
    if (send(socket_fd, LASER_AUTH_MSG, strlen(LASER_AUTH_MSG), 0) < 0) {
        std::cerr << "Send Failed" << std::endl;
        exit(1);
    }
    if (recv(socket_fd, buffer, BUFFER_LEN, 0) < 0) {
        std::cerr << "Auth Failed" << std::endl;
        exit(1);
    }
    std::cout << buffer;
    memset(buffer, 0, BUFFER_LEN * sizeof(buffer[0]));

    //--Wait for main loop--
    while (!sharedPM->Started.Flags.Laser) {
        usleep(1000);
    }
    usleep(1000);
    sharedPM->Started.Flags.Laser = 0;

    //--Main Loop--
	while (!sharedPM->Shutdown.Flags.Laser and !kbhit()) {
        usleep(100*1000);
        //--Check Heartbeat--
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

        //--Data request--
        std::cout << "Sending data request command" << std::endl;
        char msgStart[1] = {(char)0x02};
        if (send(socket_fd, msgStart, 1, 0) < 0) {
            std::cerr << "Send Failed" << std::endl;
            exit(1);
        }
        usleep(10*1000);
        if (send(socket_fd, DATA_REQ_MSG, strlen(DATA_REQ_MSG), 0) < 0) {
            std::cerr << "Send Failed" << std::endl;
            exit(1);
        }
        usleep(10*1000);
        char msgEnd[1] = {(char)0x03};
        if (send(socket_fd, msgEnd, 1, 0) < 0) {
            std::cerr << "Send Failed" << std::endl;
            exit(1);
        }

        //--Laser Response--
        usleep(100*1000);
        std::cout << "Recieveing data" << std::endl;
        std::string recvStr = "";
        int numBytes = 0;
        while ((numBytes = recv(socket_fd, buffer, BUFFER_LEN, MSG_DONTWAIT)) > 0) {
            std::cout << "Bytes recieved " << numBytes << std::endl;
            recvStr += buffer;
            memset(buffer, 0, BUFFER_LEN * sizeof(buffer[0]));
            usleep(10*1000);
        }

        //--Pasreing Response--
        if (recvStr != "") {
            std::cout << "Parsing data" << std::endl;
            std::vector<std::string> laserScanData;
            if (parseLaserData(recvStr, laserScanData)) {
                std::cerr << "String Parsing Error" << std::endl;
            } else {
                //--Found Points--
                uint16_t numPoints = std::stoi(laserScanData.at(NUMPOINTS_INDEX), 0, 16);
                float resolution = ((float)std::stoi(laserScanData.at(RESOLUTION_INDEX), 0, 16))/10000;
                float startAngle = ((float)std::stoi(laserScanData.at(STARTANGLE_INDEX), 0, 16))/10000;

                std::cout << "NumPoints " << numPoints << std::endl;
                std::cout << "Resolution " << resolution << std::endl;
                std::cout << "StartAngle " << startAngle << std::endl;

                for (int pointInd = 0; pointInd <= numPoints; pointInd++) {
                    float x = (float)(std::stoi(laserScanData[START_DATA_INDEX+pointInd], 0, 16))*sin(pointInd*resolution*PI/180);
                    float y = (float)(std::stoi(laserScanData[START_DATA_INDEX+pointInd], 0, 16))*cos(pointInd*resolution*PI/180);
                    std::cout << "X[" << pointInd << "] = " << x << "    ";
                    std::cout << "Y[" << pointInd << "] = " << y << std::endl;
                }
            }
        }
	}
    releaseSHMem(sharedPM);
    releaseSHMem(sharedLaser);
    char * valIn = 0;
    std::cout << "Press enter to exit" << std::endl;
    std::cin >> valIn;
}

int parseLaserData(std::string recvStr, std::vector<std::string> &parsedString) {
    std::vector<std::string> result;
    std::string prev = "";
    parsedString.clear();

    //find each "packet" of data
    boost::split(result, recvStr, boost::is_any_of(" "));

    bool message_started = false;
    bool message_ended = false;
    int end_zeros = 0;

    //--Packet Parsing--
    for (std::string curr : result) {
        if (!message_started) {
            if (curr == "LMDscandata") {
                /*
                if (prev != "sRA") {
                    std::cerr << "Incorrect message response '" << prev << "'" << std::endl;
                    return -1;
                }
                */
                message_started = true;
                parsedString.push_back(curr);
            }
        } else if (!message_ended) {
            parsedString.push_back(curr);
            if (curr == "0") {
                end_zeros += 1;
                if (end_zeros >= MSG_END_ZEROS) {
                    message_ended = true;
                } 
            }
            if (end_zeros > 0 and prev == "0" and curr != "0") {
                end_zeros = 0;
            }
        }
        prev = curr;
    }
    
    if (parsedString[MEASRUE_TYPE_INDEX] != "DIST1") {
        std::cerr << "Incorrect measurement type" << std::endl;
        return -1;
    }

    return 0;
}
