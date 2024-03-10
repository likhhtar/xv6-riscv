#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(2, "Wrong args count\n");
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(2, "Error creating pipe\n");
        exit(1);
    }

    int pid;
    pid = fork();

    if(pid == 0) {
        close(pipefd[1]);

        close(0);
        dup(pipefd[0]);

        close(pipefd[0]);

        char *argv[] = {"/wc", 0};
        exec("/wc", argv);
        fprintf(2, "Error exec\n");
    }
    else if (pid > 0) {
        close(pipefd[0]);

        for (int i = 1; i < argc; i++) {
            if (write(pipefd[1], argv[i], strlen(argv[i])) == -1) {
                fprintf(2, "Error writing to pipe\n");
                exit(1);
            }
            if (write(pipefd[1], "\n", 1) == -1) {
                fprintf(2, "Error writing to pipe\n");
                exit(1);
            }
        }

        close(pipefd[1]);

        int ex_code;
        wait(&ex_code);
    }
    else {
        fprintf(2, "Error\n");
        exit(1);
    }

    exit(0);
}

