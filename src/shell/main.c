
// *****************************************************************************
// Anthony Tatowicz
//
// main.c - Entry point
// *****************************************************************************

#include "main.h"


int main(int argc, char* argv[]) {

    if(argc != 2) {
        shell_init(".");
    }
    else {
        shell_init(argv[1]);
    }

    return 0;
}
