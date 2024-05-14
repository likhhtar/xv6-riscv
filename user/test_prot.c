#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Wrong count of arguments!\n");
        exit(1);
    }

    int fd1[2];
    int fd2[2];
    if (pipe(fd1)) {
        printf("Creation of pipes failed\n");
        exit(1);
    }

    if (pipe(fd2)) {
        close(fd1[0]);
        close(fd2[0]);
        printf("Creation of pipes failed\n");
        exit(1);
    }

    int cid = fork();
    if (cid < 0) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        printf("Creation of fork failed\n");
        exit(1);
    }

    if (cid > 0) {
        int read_result;
        char buf;

        if (close(fd1[0]) < 0) {
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            printf("Closing of fork failed fd1[0] (parent)\n");
            exit(1);
        }
        if (close(fd2[1]) < 0) {
            close(fd1[1]);
            close(fd2[0]);
            printf("Closing of fork failed fd2[0] (parent)\n");
            exit(1);
        }

        if (write(fd1[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
            close(fd1[1]);
            close(fd2[0]);
            printf("Writing to pipe failed!\n");
            exit(1);
        }
        if (close(fd1[1]) < 0) {
            close(fd2[0]);
            printf("Closing of fork failed fd1[1] (parent)\n");
            exit(1);
        }

        while (1) {
            read_result = read(fd2[0], &buf, 1);
            if (read_result < 0) {
                close(fd2[0]);
                printf("Reading from pipe failed!\n");
                exit(1);
            }
            if (read_result == 0) break;

            printf("%d: received '%c'\n", getpid(), buf);
        }
        if (close(fd2[0]) < 0) {
            printf("Closing of fork failed fd2[1] (parent)\n");
            exit(1);
        }

    }
    else {
        int read_result;
        char buf;

        if (close(fd1[1]) < 0) {
            close(fd1[0]);
            close(fd2[0]);
            close(fd2[1]);
            printf("Closing of fork failed fd1[0] (parent)\n");
            exit(1);
        }
        if (close(fd2[0]) < 0) {
            close(fd1[0]);
            close(fd2[1]);
            printf("Closing of fork failed fd2[0] (parent)\n");
            exit(1);
        }

        while (1) {
            read_result = read(fd1[0], &buf, 1);
            if (read_result < 0) {
                close(fd1[0]);
                close(fd2[1]);
                printf("Reading from pipe failed!\n");
                exit(1);
            }

            if (read_result == 0) break;

            printf("%d: received '%c'\n", getpid(), buf);

            if (write(fd2[1], &buf, 1) != 1) {
                close(fd1[0]);
                close(fd2[1]);
                printf("Writing to pipe failed!\n");
                exit(1);
            }
        }

        if (close(fd1[0]) < 0) {
            close(fd2[1]);
            printf("Closing of fork failed fd1[0] (parent)\n");
            exit(1);
        }
        if (close(fd2[1]) < 0){
            printf("Closing of fork failed fd2[1] (parent)\n");
            exit(1);
        }
    }
    exit(0);
}