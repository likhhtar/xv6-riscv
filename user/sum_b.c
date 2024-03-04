#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    const int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    int pointer_b = 0, pointer_x = 0, pointer_y = 0;

    gets(buffer, BUFFER_SIZE);

    while (pointer_b < BUFFER_SIZE && buffer[pointer_b] >= '0' && buffer[pointer_b] <= '9'){
        pointer_x++;
        pointer_b++;
    }
    if (pointer_x == 0) {
        fprintf(2, "First number is missing or extra spaces\n");
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
    while (pointer_b < BUFFER_SIZE && buffer[pointer_b] >= '0' && buffer[pointer_b] <= '9') {
        pointer_b++;
        pointer_y++;
    }
    if (pointer_y == 0) {
        fprintf(2, "Second number is missing or extra spaces\n");
        exit(1);
    }
    if (buffer[pointer_b] != '\0' && buffer[pointer_b] != '\n' && pointer_b != BUFFER_SIZE) {
        fprintf(2, "Wrong string type\n");
        exit(1);
    }

    printf("%d\n", atoi(&buffer[0]) + atoi(&buffer[pointer_x + 1]));
    exit(0);
}
