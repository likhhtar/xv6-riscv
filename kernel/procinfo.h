#pragma once

typedef struct procinfo {
    char name[16];           // Имя процесса
    int state;               // Состояние процесса
    int parent_pid;          // Идентификатор родительского процесса
} procinfo_t;

