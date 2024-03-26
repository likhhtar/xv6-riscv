#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "mutex.h"
#include "stat.h"
#include "proc.h"

typedef struct mutex_table{
    struct spinlock spin;
    struct sleeplock sleep;
    mutex_t mutexes[NMUTEX];
} mtable;

mtable mt;

void
mutex_init(void) {
    for (int i = 0 ; i < NMUTEX; i++) {           // Проход по всем элементам массива mutexes
        initlock(&mt.mutexes[i].spin, "spinlock"); // Инициализация спин-блокировки для каждого мьютекса
        initsleeplock(&mt.mutexes[i].sleep, "sleeplock"); // Инициализация блокировки сон для каждого мьютекса
        mt.mutexes[i].count = 0;                   // Установка начального значения счетчика использования мьютекса
    }
}

int
mutex_lock(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1; // Проверка на корректность описателя мьютекса
    acquiresleep(&mt.mutexes[mutex_desc].sleep);           // Захват блокировки сон для безопасного доступа к мьютексу
    return 0;                                              // Возвращаем 0 в случае успешного захвата мьютекса
}

int
mutex_unlock(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1; // Проверка на корректность описателя мьютекса
    releasesleep(&mt.mutexes[mutex_desc].sleep);           // Освобождение блокировки сон после завершения операций с мьютексом
    return 0;                                              // Возвращаем 0 в случае успешного освобождения мьютекса
}


int
mutex_create(void) {
    for (int i = 0; i < NMUTEX; i++) {  // Проход по массиву mutexes
        acquire(&mt.mutexes[i].spin);   // Захватываем блокировку для безопасного доступа к текущему мьютексу
        if (mt.mutexes[i].count == 0) {   // Проверяем, свободен ли текущий мьютекс
            mt.mutexes[i].count++;        // Увеличиваем счетчик использования текущего мьютекса
            release(&mt.mutexes[i].spin); // Освобождаем блокировку после завершения операций с текущим мьютексом
            return i;                     // Возвращаем индекс свободного мьютекса
        }
        release(&mt.mutexes[i].spin);     // Освобождаем блокировку, если текущий мьютекс занят, перед переходом к следующему
    }
    return -1;                             // Если все мьютексы заняты, возвращаем -1, чтобы указать, что не удалось создать новый мьютекс
}

int
mutex_hold(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1;
    acquire(&mt.mutexes[mutex_desc].spin);
    int d = holdingsleep(&mt.mutexes[mutex_desc].sleep); // Получение текущего количества удержаний блокировки
    release(&mt.mutexes[mutex_desc].spin);
    return d;
}

int
mutex_destroy(int mutex_desc) {
    int h = mutex_hold(mutex_desc);  // Получение текущего количества удержаний мьютекса
    if (h < 0) return -1;             // Если получение количества удержаний вызвало ошибку, возвращаем -1

    if (mutex_unlock(mutex_desc) < 0) return -2;  // Попытка разблокировать мьютекс, если не удалось, возвращаем -2

    acquire(&mt.mutexes[mutex_desc].spin);        // Захват спин-блокировки для безопасного доступа к мьютексу

    if (mt.mutexes[mutex_desc].count == 0) {      // Если счетчик использования мьютекса равен 0,
        release(&mt.mutexes[mutex_desc].spin);    // освобождаем спин-блокировку и возвращаем -3, указывая на ошибку
        return -3;
    }

    mt.mutexes[mutex_desc].count--;               // Уменьшаем счетчик использования мьютекса
    release(&mt.mutexes[mutex_desc].spin);        // Освобождаем спин-блокировку после завершения операций с мьютексом

    return 0;                                      // Возвращаем 0 в случае успешного разрушения мьютекса
}


int
mutex_use(int t) {
    if (t < 0 || t >= NMUTEX) return -1;  // Проверка на корректность индекса мьютекса

    acquire(&mt.mutexes[t].spin);         // Захват блокировки для безопасного доступа к мьютексу
    mt.mutexes[t].count++;                 // Увеличение счетчика использования мьютекса
    release(&mt.mutexes[t].spin);          // Освобождение блокировки после операций с мьютексом

    return 0;                              // Возвращаем 0 в случае успешного использования мьютекса
}