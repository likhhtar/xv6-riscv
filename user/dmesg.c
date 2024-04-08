#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int
main(int argc, char *argv[]) {
    // Check if the program was invoked with the argument "go"
    if (argc == 2 && strcmp(argv[1], "go") == 0) {
        // Test buffer overflow by forking multiple processes
        for (int i = 0; i < 100; ++i) {
            int pid = fork();
            if (pid == 0) {
                // Child process executes the "vm" program
                exec("vm", argv);
                exit(0);
            } else {
                // Parent waits for child process to finish
                wait(&pid);
            }
        }
    }

    // Buffer to store kernel messages
    char message[MEMPAGESIZE];

    // Retrieve kernel messages using the dmesg system call
    if (dmesg(message, MEMPAGESIZE) < 0) {
        printf("Error: Unable to retrieve kernel messages\n");
        exit(1);
    }

    // Print the retrieved kernel messages
    printf("%s", message);

    exit(0);
}