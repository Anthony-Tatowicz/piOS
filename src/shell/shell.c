// *****************************************************************************
// Anthony Tatowicz
//
// shell.c - Shell implementation
// *****************************************************************************

#include "shell.h"


void shell_init(char* dir) {

    if(chdir(dir)) {
        perror("Error");
        help();
        exit(1);
    }

    while(true) {
        char s[1024];

        if(getcwd(s, 1024) == NULL) {
            perror("Error");
            exit(1);
        }

        print_menu(s);

        char cmd = getchar(); getchar();

        if(process_command(cmd)) {
            printf("%s\n", "No command found\n\n");
        }
    }
    exit(0);
}


void print_menu(char* cwd) {
    time_t t = time(NULL);
    printf("\n******************************\n \
            %s******************************\n", ctime(&t));

    printf("\nCurrent Directory: %s\n", cwd);
    printf("%s\n\n", "---------------------------------------------------");

    read_dirs(".");
    read_files(".");

    printf("\n%s\n\n", "Operations: ");
    printf("%s\n",      "e  Edit");
    printf("%s\n",      "r  Run");
    printf("%s\n",      "c  Change Directory");
    printf("%s\n\n\n",  "q  Quit");
}

int read_dirs(char* dir) {
    struct dirent* de;
    DIR* d = opendir(dir);
    size_t count = 0;

    printf("%s\n", "Directorys");
    while(de = readdir(d)) {
        if((de->d_type) & DT_DIR) {
            printf("%d. %s\n", count++, de->d_name);
        }
    }

    closedir(d);

    return count;
}

int read_files(char* dir) {
    struct dirent* de;
    DIR* d = opendir(dir);
    size_t count = 0;

    printf("%s\n", "Files:");
    while(de = readdir(d)) {
        if((de->d_type) & DT_REG) {
            printf("%d. %s\n", count++, de->d_name);

            if(count % 8 == 0 && count > 0) {
                printf("\n%s\n", "Hit n for next");
                getchar();
                getchar();
            }
        }
    }

    closedir(d);

    return count;
}

int process_command(char cmd) {
    switch (cmd) {
        case QUIT: exit(0);
        case EDIT: edit();
            break;
        case RUN: run();
            break;
        case CHDIR: change_dir();
            break;
        default: return -1;
    }

    return 0;
}

int edit(char* file_list) {
    char cmd[NAME_MAX + 5];
    char buff[NAME_MAX + 1];
    int status = 0;

    printf("%s", "File to edit: ");
    if(scanf("%2048s", buff) < 0) {
        perror("\nError");
        status = -1;
    }

    strcpy(cmd, "vi ");
    strcat(cmd, buff);
    printf("%s\n", cmd);

    if(system(cmd) < 0) {
        perror("Error");
        status = -1;
    }

    return status;
}

int run() {
    char cmd[NAME_MAX];
    int status = 0;

    printf("%s", "Enter command: ");

    if(scanf("%2048s", cmd) < 0) {
        perror("\nError");
        status = -1;
    }

    if(system(cmd) < 0) {
        perror("Error");
        status = -1;
    }

    return status;
}

int change_dir(char* dir_list) {
    char path[NAME_MAX];
    int status = 0;

    printf("\n%s", "Enter Dir: ");

    if(scanf("%2048s", path) < 0) {
        perror("Error");
        status = -1;
    }

    else if (chdir(path)) {
        printf("%s\n", "Directory not found!");
        status = -1;
    }


    return status;
}

void help() {
    printf("%s\n", "Usage: shell <workdir>\n ex. ./shell $HOME");
}
