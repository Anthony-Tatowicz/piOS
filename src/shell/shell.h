
// *****************************************************************************
// Anthony Tatowicz
//
// shell.h - Shell header
// *****************************************************************************
#ifndef _SHELL_H_
#define  _SHELL_H_

#define MENU_LENGTH 8
#define DIR_OFFSET 8
#define FILE_OFFSET 17

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

int process_command(char cmd, list_t *hist, node_t *current, size_t *index);

void print_list(list_t *head, char *title, int pos);
void print_ops();
void print_header();

int edit();
int run();
int change_dir();
void help();
int next(list_t *head, node_t *current, size_t *idx);
int prev(list_t *head, node_t *current, size_t *idx);

enum Commands {
    QUIT  = 'q',
    RUN   = 'r',
    EDIT  = 'e',
    CHDIR = 'c',
    NEXT  = 'n',
    PREV  = 'p'
};

extern int errno;

#endif
