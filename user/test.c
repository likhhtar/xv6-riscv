#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MUL 1103515245
#define ADD 12345
#define MOD 2147483648

#define BUF_SIZE 1024
#define MAXSIZE (11 + 256 + 256 * 256) * 1024 / sizeof(uint)

#define filename "large_file"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Wrong number of args \n");
        exit(1);
    }

    int size = atoi(argv[1]) / sizeof(uint);
    int seed = atoi(argv[2]);
    uint cur_num = seed;
    if (size <= 0 || size > MAXSIZE) {
        fprintf(2, "Wrong file size (is out of range)\n");
        exit(1);
    }
    if (seed < 0) {
        fprintf(2, "Seed must be >= 0\n");
        exit(1);
    }
    uint* buf = malloc(BUF_SIZE * sizeof(uint));
    int fd = open(filename, O_WRONLY | O_CREATE);
    if (fd < 0) {
        fprintf(2, "Cannot create file\n");
        exit(1);
    }
    for (int i = 0; i < size; ++i) {
        buf[i % BUF_SIZE] = cur_num;

        if (i == size - 1 || (i + 1) % BUF_SIZE == 0) {
            int cur = size % BUF_SIZE;
            if ((i + 1) % BUF_SIZE == 0) {
                cur = BUF_SIZE;
            }
            int fw = write(fd, buf, cur * sizeof(uint));
            if (fw != cur * sizeof(uint)) {
                close(fd);
                fprintf(2, "Error in write\n");
                exit(1);
            }
        }
        cur_num = ((uint64) MUL * cur_num + ADD) % MOD;
    }
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(2, "Cannot open file\n");
        exit(1);
    }
    cur_num = seed;
    for (int i = 0; i < size / BUF_SIZE; i++) {
        int fw = read(fd, buf, BUF_SIZE * sizeof(uint));
        if (fw != BUF_SIZE * sizeof(uint)) {
            close(fd);
            fprintf(2, "Error in reading\n");
            exit(1);
        }
        for (int j = 0; j < BUF_SIZE; j++) {
            if (cur_num != buf[j]) {
                close(fd);
                printf("Error in file data\n");
                exit(1);
            }
            cur_num = ((uint64) MUL * cur_num + ADD) % MOD;
        }
    }

    if (size % BUF_SIZE != 0) {
        int fw = read(fd, buf, (size % BUF_SIZE) * sizeof(uint));
        if (fw != (size % BUF_SIZE) * sizeof(uint)) {
            close(fd);
            fprintf(2, "Error in reading\n");
            exit(1);
        }
        for (int j = 0; j < size % BUF_SIZE; j++) {
            if (cur_num != buf[j]) {
                close(fd);
                printf("Error in file data\n");
                exit(1);
            }
            cur_num = ((uint64) MUL * cur_num + ADD) % MOD;
        }
    }
    printf("Test passed!\n");
    close(fd);
    exit(0);
}