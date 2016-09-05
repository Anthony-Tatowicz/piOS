
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
#include <limits.h>

void shell_init(char* dir);
int read_dirs(char* dir);
int read_files(char* dir);
int process_command(char cmd);
void print_menu(char* cwd);
int edit();
int run();
int change_dir();
void help();

enum Commands {
    QUIT = 'q',
    RUN  = 'r',
    EDIT = 'e',
    CHDIR = 'c'
};
