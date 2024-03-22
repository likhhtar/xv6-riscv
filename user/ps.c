#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "kernel/param.h"
#include "user/user.h"

int main(void) {
    int size = 1;
    int cnt_proc;
    
     char *states[] = {
            [0]    "unused ",
            [1]    "used   ",
            [2]    "sleep  ",
            [3]    "runable",
            [4]    "running",
            [5]    "zombie "
    };

    while (1) {
        procinfo_t buf[size];
        cnt_proc = procinfo((uint64)buf, size);

        if (cnt_proc < 0) {
            fprintf(2, "Error: ps_listinfo failed with error code %d\n", cnt_proc);
            exit(1);
        }

        if (size < cnt_proc) {
            size *= 2;
        }
        else {
            for (int i = 0; i < cnt_proc; ++i) {
                printf("%s %d %s\n", states[buf[i].state], buf[i].parent_pid, buf[i].name);
            }
            exit(0);
        }
    }
    exit(1);
}
