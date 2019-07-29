
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/select.h>

#include <termios.h>
#include <fcntl.h>

/*
 * Code pulled from 
 * https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html?fbclid=IwAR13Eodm2iQMHqJjxEbkv6luojea7NdxB0TKfMY_SVbS1DlSND7W9yTQd8w
 * Answer by user: itsme86
 */

//void reset_terminal_mode();
//void set_conio_terminal_mode();

int kbhit();
//int getch();
