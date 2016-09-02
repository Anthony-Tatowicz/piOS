
// *****************************************************************************
// Anthony Tatowicz
//
// shell.c - Shell implementation
// *****************************************************************************

#include "shell.h"

int main() {

  while(true) {

    print_menu();

    char cmd = get_command();
    char err = '!';

    if(strcmp(&cmd, &err)) {
      printf("%s\n", "Invalid command!\n\n");
      continue;
    }

    if(process_command(cmd)) {
      printf("%s\n", "Error processing command");
      return 1;
    }

    put_output();
  }

  return 0;
}

void print_menu() {
  char s[256];

  time_t t = time(NULL);
  printf("%s\n", ctime(&t));

  getcwd(s, 200);
  printf("\n%s\n", s);

}

char get_command() {
  char choice;
  char cmd;

  if(sscanf(&cmd, "%c\n", &choice)) {
      return choice;
  }
  else {
      return '!';
  }
}

bool process_command(char cmd) {
  //OKAY do stuff with the command and return res.
}

void put_output() {
  //print out the res...
}
