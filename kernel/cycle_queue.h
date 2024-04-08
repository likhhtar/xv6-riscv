#ifndef XV6_RISCV_CYCLE_QUEUE_H
#define XV6_RISCV_CYCLE_QUEUE_H

typedef struct cycle_queue {
    char data[BUFFER_SIZE];
    int head, tail;
    struct spinlock lock;
} queue_t;

#endif //XV6_RISCV_CYCLE_QUEUE_H
