
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <cstring>

using namespace std;


int main()
{
    std::vector<std::string> programsToRun = {
        "sleep 5"
    };
    std::string consoleStarter = "xterm -e \'";

    //new iteration in c++11
    for (std::string program : programsToRun) {
        //convert string to char * for exec
        char * programToRun = (char*)malloc(256);
        std::strcpy(programToRun, (consoleStarter + program + "\'").c_str());

        // create child process
        pid_t pid = fork(); 
        int status;
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
            exit(consoleStatus);

        default: // parent process
            std::cout << "in process " << pid << std::endl;

            //initialize shared memory
			//and check heartbeats
			

			wait(&status); 
    		if (WIFEXITED(status)) 
    		    printf("Exit status: %d\n", WEXITSTATUS(status)); 
    		else if (WIFSIGNALED(status)) 
    		    psignal(WTERMSIG(status), "Exit signal");

            break;
        }
    }
    return 0;
}
