#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        printf("Wrong count of arguments!\n");
        exit(1);
    }

    int on, tick = 0;
    if (!strcmp(argv[2], "1")) {
        on = 1;
    }
    else if (!strcmp(argv[2], "0")){
        on = 0;
    }
    else {
        printf("Wrong input\n");
        exit(1);
    }
    if (argc == 4) {
        tick = atoi(argv[3]);
        if (tick < 0) {
            printf("Incorrect ticks\n");
            exit(1);
        }
    }

    prot(atoi(argv[1]), on, tick);
    exit(0);
}