
// *****************************************************************************
// Anthony Tatowicz
//
// shell.h - Shell header
// *****************************************************************************
#ifndef _SHELL_H_
#define  _SHELL_H_

#define MENU_LENGTH 8
#define DIR_OFFSET 8
#define FILE_OFFSET 18
#define CMD_OFFSET 2

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <ncurses.h>
#include <errno.h>

#include "list.h"

void shell_init(char *dir);

void free_item(void *data);

int read_dir(DIR *dir, list_t *hist);
int read_files(char *dir);

void main_loop_fn();

void print_list(list_t *head, char *title, int pos);
void print_ops();
void print_header();

int edit();
int run();
int change_dir();
void help();
node_t *next(list_t *head, node_t *current);
node_t *prev(list_t *head, node_t *current);

enum Commands {
    QUIT  = 'q',
    RUN   = 'r',
    EDIT  = 'e',
    CHDIR = 'c',
    NEXT  = 'n',
    PREV  = 'p',
    RESET = 's'
};

extern int errno;

#endif
