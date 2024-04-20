#ifndef XV6_RISCV_PROTOCOL_H
#define XV6_RISCV_PROTOCOL_H

struct prot {
    struct spinlock lock;
    int end_ticks;
    int events[4];
};

#endif //XV6_RISCV_PROTOCOL_H
