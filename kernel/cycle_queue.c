#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include "cycle_queue.h"
#include <stddef.h>
#include <assert.h>


int queue_add(struct cycle_queue *q, char a) {

    // Проверка на нулевой указатель
    if (q == NULL) {
        release(&q->lock);
        return -1;
    }

    // Добавление элемента в конец очереди
    q->data[q->tail++] = a;

    // Проверка на переполнение
    if (q->tail == BUFFER_SIZE) {
        q->head = 1; // Установка флага переполнения
        q->tail = 0; // Обнуление индекса конца очереди
    }

    return 0;
}