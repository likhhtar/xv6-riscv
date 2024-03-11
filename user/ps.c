#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "kernel/param.h"
#include "user/user.h"

int main(void) {
    procinfo_t buf[NPROC];

    int cnt_proc = ps_listinfo(buf, NPROC);
    if (cnt_proc < 0 || cnt_proc > NPROC) {
        fprintf(2, "Error: ps_listinfo failed with error code %d\n", cnt_proc);
        exit(1);
    }

    char *states[] = {
            [0]    "unused ",
            [1]    "used   ",
            [2]    "sleep  ",
            [3]    "runable",
            [4]    "running",
            [5]    "zombie "
    };

    for (int i = 0; i < cnt_proc; ++i) {
        printf("%s %d %s\n", states[buf[i].state], buf[i].parent_pid, buf[i].name);
    }

    exit(0);
}
