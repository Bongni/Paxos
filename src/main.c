#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool DEBUG_FLAG = false;

void set_flags(char *argv[], int argc) {
    if ( strcmp(argv[1], "-d") == 0 ) {
        DEBUG_FLAG = true;
        printf("Debugging enabled.\n");
    }
}

int main (int argc, char *argv[]) {

    printf("Executing Paxos ... \n");

    if (argc >= 2) {
        set_flags(argv, argc);
    }

    return 0;
}
