#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define SLEEP 100

int main() {
    int pid, id;
    pid = fork();
    id = getpid();

    if(pid == 0) {
        sleep(SLEEP);
        exit(1);
    }
    else if (pid > 0) {
        fprintf (1, "Parent process - %d Child process - %d\n", id, pid);

        kill(pid);
        int ex_code;
        wait(&ex_code);

        fprintf (1, "Child process - %d Return code - %d\n", pid, ex_code);
    }
    else
        fprintf(2, "Error\n");

    exit(0);
}

