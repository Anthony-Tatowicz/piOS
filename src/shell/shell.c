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

    main_loop_fn();
}

// Main progrm loop
void main_loop_fn() {

    while(true) {

        list_t *hist = (list_t *)malloc(sizeof(list_t));
        if(hist == NULL) exit(1);
        size_t index = 1;
        node_t *current;
        if(current == NULL) exit(1);


        if(getcwd(cwd, 2048) == NULL) {
            printw("%s", strerror(errno));
            refresh();
            exit(1);
        }

        DIR* d = opendir(".");

        new_list(hist, sizeof(struct dirent), free_item);

        int ct = read_dir(d, hist);
        closedir(d);

        current = hist->head;
        current = next(hist, current);

        // List Loop
        while(true) {
            print_header(cwd);
            mvprintw(DIR_OFFSET, 0, "%s:", "Directories");
            mvprintw(FILE_OFFSET, 0, "%s:", "Files");

            print_ops();
            char in = getch();

            if(in == QUIT) {
                clear();
                endwin();
                exit(0);
            }
            else if(in == EDIT) {
                edit();
            }
            else if(in == RUN) {
                run();
            }
            else if(in == CHDIR) {
                change_dir();
                break;
            }
            else if(in == NEXT) {
                current = next(hist, current);
            }
            else if(in == PREV) {
                current = prev(hist, current);
            }
            else if(in == RESET) {
                clear();
                print_header(cwd);
                current = hist->head;
                current = next(hist, current);
            }
        }

        delete_list(hist);
        free(hist);

        clear();
    }
}

void free_item(void* data) {
    free((struct dirrent *)data);
}

void print_header(char *cwd) {
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
    mvprintw(30, 0,  "%s", "Operations: ");
    mvprintw(30, 15, "%s",      "e  Edit");
    mvprintw(31, 15, "%s",      "r  Run");
    mvprintw(32, 15, "%s",      "c  Change Directory");
    mvprintw(33, 15, "%s",      "n  Next");
    mvprintw(34, 15, "%s",      "p  Previous");
    mvprintw(35, 15, "%s",      "s  Reset");
    mvprintw(36, 15, "%s",      "q  Quit");
    move(0,0);
    refresh();
}

int read_dir(DIR *dir, list_t *hist) {
    struct dirent *de = (struct dirent*)malloc(sizeof(struct dirent));
    size_t count = 0;

    while((de = readdir(dir))) {
        append(hist, de);
        count++;
    }

    free(de);

    return count;
}

node_t *next(list_t *list, node_t *current) {

    assert(current != NULL);
    assert(list != NULL);

    if(current->next == NULL) {
        return current;
    }

    clear();

    int file_p = 0;
    int dir_p = 0;

    while(current->next != NULL) {
        struct dirent *de = (struct dirent*)current->data;

        assert(de != NULL);

        if(de->d_type & DT_DIR && dir_p < MENU_LENGTH) {
            int y_pos = dir_p + DIR_OFFSET + 1;
            move(y_pos, 0);
            clrtoeol();
            printw("%d. %s", dir_p, de->d_name);
            dir_p++;
        }
        else if(de->d_type & DT_REG && file_p < MENU_LENGTH) {
            int y_pos = file_p + FILE_OFFSET + 1;
            move(y_pos, 0);
            clrtoeol();
            printw("%d. %s", file_p, de->d_name);
            file_p++;
        }
        else {
            break;
        }

        current = current->next;
    }

    refresh();
    return current;
}


node_t *prev(list_t *list, node_t *current) {

    assert(current != NULL);
    assert(list != NULL);

    if(current->prev == NULL) {
        return current;
    }

    clear();

    int file_p = 0;
    int dir_p = 0;

    while(current->prev != NULL) {
        struct dirent *de = (struct dirent*)current->data;

        assert(de != NULL);

        if(de->d_type & DT_DIR && dir_p < MENU_LENGTH) {
            int y_pos = dir_p + DIR_OFFSET + 1;
            move(y_pos, 0);
            clrtoeol();
            printw("%d. %s", dir_p, de->d_name);
            dir_p++;
        }
        else if(de->d_type & DT_REG && file_p < MENU_LENGTH) {
            int y_pos = file_p + FILE_OFFSET + 1;
            move(y_pos, 0);
            clrtoeol();
            printw("%d. %s", file_p, de->d_name);
            file_p++;
        }
        else {
            break;
        }

        current = current->prev;
    }

    refresh();
    return current;
}

int edit() {
    char cmd[NAME_MAX + 5];
    char buff[NAME_MAX + 1];
    int status = 0;

    char msg[] = {"File to edit: "};
    move(row - CMD_OFFSET, 0);
    clrtoeol();
    move(row - CMD_OFFSET, (col - strlen(msg)) / 2);
    printw(msg);

    refresh();
    echo();

    if(scanw("%2048s", buff) < 0) {
        char *msg = strerror(errno);
        move(row - CMD_OFFSET, 0);
        clrtoeol();
        mvprintw(row - CMD_OFFSET, (col - strlen(msg)) / 2, msg);
        status = -1;
    }
    else {
        strcpy(cmd, "vi ");
        strcat(cmd, buff);
        printw("%s\n", cmd);

        if(system(cmd) < 0) {
            char *msg = strerror(errno);
            move(row - CMD_OFFSET, 0);
            clrtoeol();
            mvprintw(row - CMD_OFFSET, (col - strlen(msg)) / 2, msg);
            status = -1;
        }
    }

    printw("DONE!");
    noecho();
    refresh();

    return status;
}

int run() {
    char cmd[NAME_MAX];
    int status = 0;

    char msg[] = {"Enter command: "};
    move(row - CMD_OFFSET, 0);
    clrtoeol();
    move(row - 2, (col - strlen(msg)) / 2);
    printw(msg);

    echo();
    refresh();

    if(scanw("%2048s", cmd) < 0) {
        char *msg = strerror(errno);
        move(row - CMD_OFFSET, 0);
        clrtoeol();
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);
        status = -1;
    }
    else if(system(cmd) < 0) {
        char *msg = strerror(errno);
        move(row - CMD_OFFSET, 0);
        clrtoeol();
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
    move(row - CMD_OFFSET, 0);
    clrtoeol();
    move(row - 2, (col - strlen(msg)) / 2);
    printw(msg);

    refresh();
    echo();

    if(scanw("%2048s", path) < 0) {
        char *msg = strerror(errno);
        move(row - CMD_OFFSET, 0);
        clrtoeol();
        mvprintw(row - 2, (col - strlen(msg)) / 2, msg);

        status = -1;
    }

    else if (chdir(path)) {
        char *msg = strerror(errno);
        move(row - CMD_OFFSET, 0);
        clrtoeol();
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
