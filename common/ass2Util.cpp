
#include "ass2Util.h"

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int forkAndSystem(std::string progString) {
        //convert string to char * for exec
        char * programToRun = (char*)malloc(256);
        std::strcpy(programToRun, progString.c_str());

        // create child process
        pid_t pid = fork(); 
		int consoleStatus = 1;

        switch (pid) {
        case -1: // error
            throw badForkException();

        case 0: // child process

            std::cout << "Child Process" << std::endl;
            std::cout << "running " << programToRun << std::endl;

            //system blocks control from process running it,
            //so if we run system in a new process (hence fork) we can
            //run systems for each binary in the project
            consoleStatus = system(programToRun);
            free((void*)programToRun);
            throw childFinishException(std::to_string(consoleStatus));

        default: // parent process
            std::cout << "forked process " << pid << std::endl;
            free((void*)programToRun);

            return 0;
        }
}
