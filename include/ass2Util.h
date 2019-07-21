
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/select.h>

#include <termios.h>

/*
 * Code pulled from 
 * https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
 * Answer by user: Alnitak
 */

void reset_terminal_mode();
void set_conio_terminal_mode();

int kbhit();
int getch();
