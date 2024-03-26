#pragma once

typedef struct mutex {
    int count;
    struct sleeplock sleep;
    struct spinlock spin;
} mutex_t;