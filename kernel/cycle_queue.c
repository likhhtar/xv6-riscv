#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include "cycle_queue.h"

void queue_add(struct cycle_queue *q, char a) {
    acquire(&q->lock);

    if ((q->tail + 1) % BUFFER_SIZE == q->head) q->head = (q->head + 1) % BUFFER_SIZE;

    q->data[q->tail] = a;
    q->tail = (q->tail + 1) % BUFFER_SIZE;

    release(&q->lock);
}