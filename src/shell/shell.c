// *****************************************************************************
// Anthony Tatowicz
//
// shell.c - Shell implementation
// *****************************************************************************

#include "shell.h"

int row, col;
char cwd[2048];

void shell_init(char* dir) {

    initscr();
    raw();
    noecho();

    getmaxyx(stdscr, row, col);

    /* For color, this is for later */

    // if(has_colors() == FALSE) {
    //     endwin();
	// 	printf("Your terminal does not support color\n");
	// 	exit(1);
	// }
    // start_color();
    // init_pair(1, COLOR_BLUE, BLACK);
    // init_pair(2, COLOR_GREEN, BLACK);

    if(chdir(dir)) {
        printw("%s", strerror(errno));
        help();
        exit(1);
    }

    while(true) {

        if(getcwd(cwd, 2048) == NULL) {
            printw("%s", strerror(errno));
            refresh();
            exit(1);
        }

        print_header();

        DIR* d = opendir(".");

        list_t hist;
        node_t current;
        size_t index = 1;

        new_list(&hist, sizeof(struct dirent), free_item);

        int ct = read_dir(d, &hist);

        // print_list(&dir_hist, "Directories", 8);
        // print_list(&file_hist, "Files", 17);

        print_header();

        mvprintw(8, 0, "%s:", "Directories");
        mvprintw(17, 0, "%s:", "Files");
        next(&hist, &current, &index);

        print_ops();
        char in = getch();
        int res = process_command(in, &hist, &current, &index);

        closedir(d);
        delete_list(&hist);
        clear();
    }

    exit(0);
}

void free_item(void* data) {
    free((struct dirrent *)data);
}

void print_header() {
    time_t t = time(NULL);
    char* msg = ctime(&t);
    char accent[] = { "*****************************" };

    mvprintw(0, (col-strlen(accent))/2, accent);
    mvprintw(1, (col-strlen(msg))/2, msg);
    mvprintw(2, (col-strlen(accent))/2, accent);

    mvprintw(5, 0, "Current Directory: %s", cwd);
    refresh();
}

void print_ops() {
    mvprintw(30, 0, "%s", "Operations: ");
    mvprintw(30, 15, "%s",      "e  Edit");
    mvprintw(31, 15, "%s",      "r  Run");
    mvprintw(32, 15, "%s",      "c  Change Directory");
    mvprintw(33, 15, "%s",      "n  Next");
    mvprintw(34, 15, "%s",      "p  Previous");
    mvprintw(35, 15, "%s",      "q  Quit");
    move(0,0);
    refresh();
}

// void print_list(list_t* list, char* title, int s_pos) {
//     mvprintw(s_pos, 0, "%s:", title);
//
//     node_t* current = list->head;
//     size_t pos = 0;
//
//
//     for(size_t i = 0; i < list_size(list); i++) {
//
//         if(i % MENU_LENGTH == 0 && i > 0) {
//
//             mvprintw(s_pos, 0, "%s:", title);
//             move(MENU_LENGTH + s_pos + 1, 0);
//             printw("%d. %s", i, current->data);
//             print_ops();
//
//             int res = -1;
//
//             while(res < 0) {
//                 char in = getch();
//                 res = process_command(in);
//             }
//
//             if(res == CHDIR) {
//                 break;
//             }
//
//             print_header();
//         }
//         else {
//             move(i % MENU_LENGTH + s_pos + 1, 0);
//             printw("%d. %s", i, current->data);
//         }
//
//         current = current->next;
//     }
//
//     refresh();
// }

int read_dir(DIR *dir, list_t *hist) {
    struct dirent *de = NULL;
    size_t count = 0;

    de = readdir(dir);

    while(de) {
        append(hist, de);
        count++;
        de = readdir(dir);


        // if((de->d_type) & DT_DIR) {
        //     append(hist, de->d_name);
        //     count++;
        // }
        // else if((de->d_type) & DT_REG) {
        //     append(files, de->d_name);
        //     count++;
        // }
    }

    return count;
}

int process_command(char cmd, list_t *list, node_t *current, size_t *index) {
    switch (cmd) {
        case QUIT: clear(); endwin(); exit(0);
        case EDIT: return edit();
        case RUN: return run();
        case CHDIR: return change_dir();
        case NEXT: return next(list, current, index);
        case PREV: return prev(list, current, index);
        default: return -1;
    }

    return 0;
}

int next(list_t *list, node_t *current, size_t *index) {

    while(current->next != NULL && (*index) % MENU_LENGTH != 0) {
        struct dirent *de = (struct dirent*)current->data;

        assert(de != NULL);

        if(de->d_type & DT_DIR) {
            move(MENU_LENGTH + DIR_OFFSET + 1, 0);
            printw("%d. %s", (*index)++, de->d_name);
        }
        else {
            move(MENU_LENGTH + FILE_OFFSET + 1, 0);
            printw("%d. %s", (*index)++, de->d_name);
        }

        current = current->next;
    }

    return 1;
}


int prev(list_t *list, node_t *current, size_t *idx) {
    return 1;
}

int edit() {
    char cmd[NAME_MAX + 5];
    char buff[NAME_MAX + 1];
    int status = 0;

    char msg[] = {"File to edit: "};
    move(row - 2, (col - strlen(msg)) / 2);
    clrtoeol();
    printw(msg);

    refresh();
    echo();

    if(scanw("%2048s", buff) < 0) {
        char *msg = strerror(errno);
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);
        status = -1;
    }
    else {
        strcpy(cmd, "vi ");
        strcat(cmd, buff);
        printw("%s\n", cmd);

        if(system(cmd) < 0) {
            char *msg = strerror(errno);
            mvprintw(row - 2, (col - strlen(msg)) / 2, msg);
            status = -1;
        }
    }

    noecho();
    refresh();

    return status;
}

int run() {
    char cmd[NAME_MAX];
    int status = 0;

    char msg[] = {"Enter command: "};
    move(row - 2, (col - strlen(msg)) / 2);
    clrtoeol();
    printw(msg);

    echo();
    refresh();

    if(scanw("%2048s", cmd) < 0) {
        char *msg = strerror(errno);
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);
        status = -1;
    }
    else if(system(cmd) < 0) {
        char *msg = strerror(errno);
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);
        status = -1;
    }

    noecho();
    refresh();

    return status;
}

int change_dir() {
    char path[NAME_MAX];
    int status = 0;

    char msg[] = {"Enter dir: "};
    move(row - 2, (col - strlen(msg)) / 2);
    clrtoeol();
    printw(msg);

    refresh();
    echo();

    if(scanw("%2048s", path) < 0) {
        char *msg = strerror(errno);
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);

        status = -1;
    }

    else if (chdir(path)) {
        char *msg = strerror(errno);
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);

        status = -1;
    }

    noecho();
    refresh();

    return status;
}

void help() {
    printw("%s\n", "Usage: shell <workdir>\n ex. ./shell $HOME");
    refresh();
}
