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

// глобальная таблица мьютексов
struct mutex mutexes[NMUTEX];

void
mutex_init(void) {
    struct mutex *mt;
    for (mt = mutexes; mt < &mutexes[NMUTEX]; mt++) {           // Проход по всем элементам массива mutexes
        initlock(&mt->spin, "spinlock"); // Инициализация спин-блокировки для каждого мьютекса
        initsleeplock(&mt->sleep, "sleeplock"); // Инициализация блокировки сон для каждого мьютекса
        mt->count = 0;                   // Установка начального значения счетчика использования мьютекса
    }
}

int
mutex_lock(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1; // Проверка на корректность описателя мьютекса
    if (mutexes[mutex_desc].count < 0 || mutexes[mutex_desc].count >= NMUTEX)
        return -2; // Проверяем, что процесс владеет мьютексом с данным дескриптором
    acquiresleep(&mutexes[mutex_desc].sleep);           // Захват блокировки сон для безопасного доступа к мьютексу
    return 0;                                              // Возвращаем 0 в случае успешного захвата мьютекса
}

int
mutex_unlock(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1; // Проверка на корректность описателя мьютекса
    if (mutexes[mutex_desc].count < 0 || mutexes[mutex_desc].count >= NMUTEX)
        return -2; // Проверяем, что процесс владеет мьютексом с данным дескриптором
    releasesleep(&mutexes[mutex_desc].sleep);           // Освобождение блокировки сон после завершения операций с мьютексом
    return 0;                                              // Возвращаем 0 в случае успешного освобождения мьютекса
}


int
mutex_create(void) {
    for (int i = 0; i < NMUTEX; i++) {  // Проход по массиву mutexes
        acquire(&mutexes[i].spin);   // Захватываем блокировку для безопасного доступа к текущему мьютексу
        if (mutexes[i].count == 0) {   // Проверяем, свободен ли текущий мьютекс
            mutexes[i].count++;        // Увеличиваем счетчик использования текущего мьютекса
            release(&mutexes[i].spin); // Освобождаем блокировку после завершения операций с текущим мьютексом
            return i;                     // Возвращаем индекс свободного мьютекса
        }
        release(&mutexes[i].spin);     // Освобождаем блокировку, если текущий мьютекс занят, перед переходом к следующему
    }
    return -1;                             // Если все мьютексы заняты, возвращаем -1, чтобы указать, что не удалось создать новый мьютекс
}

int
mutex_hold(int mutex_desc) {
    if (mutex_desc < 0 || mutex_desc >= NMUTEX) return -1;
    acquire(&mutexes[mutex_desc].spin);
    int d = holdingsleep(&mutexes[mutex_desc].sleep); // Получение текущего количества удержаний блокировки
    release(&mutexes[mutex_desc].spin);
    return d;
}

int
mutex_destroy(int mutex_desc) {
    int h = mutex_hold(mutex_desc);  // Получение текущего количества удержаний мьютекса
    if (h < 0) return -1;             // Если получение количества удержаний вызвало ошибку, возвращаем -1

    if (mutex_unlock(mutex_desc) < 0 && h == 1) return -2;  // Попытка разблокировать мьютекс, если не удалось, возвращаем -2

    acquire(&mutexes[mutex_desc].spin);        // Захват спин-блокировки для безопасного доступа к мьютексу

    if (mutexes[mutex_desc].count == 0) {      // Если счетчик использования мьютекса равен 0,
        release(&mutexes[mutex_desc].spin);    // освобождаем спин-блокировку и возвращаем -3, указывая на ошибку
        return -3;
    }

    mutexes[mutex_desc].count--;               // Уменьшаем счетчик использования мьютекса
    release(&mutexes[mutex_desc].spin);        // Освобождаем спин-блокировку после завершения операций с мьютексом

    return 0;                                      // Возвращаем 0 в случае успешного разрушения мьютекса
}


int
mutex_use(int t) {
    if (t < 0 || t >= NMUTEX) return -1;  // Проверка на корректность индекса мьютекса

    if (mutexes[t].count < 0 || mutexes[t].count >= NMUTEX)
        return -2; // Проверяем, что процесс владеет мьютексом с данным дескриптором

    acquire(&mutexes[t].spin);         // Захват блокировки для безопасного доступа к мьютексу
    mutexes[t].count++;                 // Увеличение счетчика использования мьютекса
    release(&mutexes[t].spin);          // Освобождение блокировки после операций с мьютексом

    return 0;                              // Возвращаем 0 в случае успешного использования мьютекса
}