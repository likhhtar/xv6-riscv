#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    const int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE], x[BUFFER_SIZE], y[BUFFER_SIZE];
    int pointer_b = 0, pointer_x = 0, pointer_y = 0;

    while (pointer_b < BUFFER_SIZE - 1) {
        char point;
        int has_read = read(0, &point, 1);

        if (has_read == 0) break;
        else if (has_read < 0) {
            fprintf(2, "Reading error\n");
            exit(1);
        }

        if (point == '\n' || point == '\0') {
            buffer[pointer_b++] = '\0';
            break;
        }
        else buffer[pointer_b++] = point;
    }
    pointer_b = 0;

    while (pointer_b < BUFFER_SIZE && buffer[pointer_b] >= '0' && buffer[pointer_b] <= '9')
        x[pointer_x++] = buffer[pointer_b++];
    if (pointer_x == 0) {
        fprintf(2, "First number is missing\n");
        exit(1);
    }
    if (buffer[pointer_b] != ' ') {
        fprintf(2, "Wrong char\n");
        exit(1);
    }
    if (pointer_b == BUFFER_SIZE) {
        fprintf(2, "No memory for the second number\n");
        exit(1);
    }

    pointer_b++;
    while (pointer_b < BUFFER_SIZE && buffer[pointer_b] >= '0' && buffer[pointer_b] <= '9')
        y[pointer_y++] = buffer[pointer_b++];
    if (pointer_y == 0) {
        fprintf(2, "Second number is missing\n");
        exit(1);
    }
    if (buffer[pointer_b] != '\0' && buffer[pointer_b] != '\n' && pointer_b != BUFFER_SIZE) {
        fprintf(2, "Wrong string type\n");
        exit(1);
    }

    printf("%d\n", atoi(x) + atoi(y));
    exit(0);
}
