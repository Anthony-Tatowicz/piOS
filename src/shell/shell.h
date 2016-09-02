
// *****************************************************************************
// Anthony Tatowicz
//
// shell.h - Shell header
// *****************************************************************************

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main();
char get_command();
bool process_command(char cmd);
void put_output();
void print_menu();
