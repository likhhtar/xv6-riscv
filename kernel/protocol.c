#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"
#include "file.h"
#include "protocol.h"

struct prot pr;

void
prot_init() {
    pr.end_ticks = 0;
    for (int i = 0; i < 4; i++) pr.events[i] = 1;
    initlock(&pr.lock, "protocol");
}

int
prot_check(int code) {
    acquire(&pr.lock);
    int result = 1;
    if (pr.end_ticks == 0 || (pr.end_ticks > 0 && ticks <= pr.end_ticks)) {
        // Если последнее время равно нулю, протоколирование выключено
        // Возвращаем результат в зависимости от установленного режима для данного типа события
        if (pr.events[code] == 1) result = 0;
        else result = -1;
    }
    else {
        if (ticks <= pr.end_ticks) {
            // Если текущее время меньше или равно последнему времени протоколирования, протоколирование выключено
            result = 0;
        }
    }

    release(&pr.lock);
    // Если текущее время больше последнего времени протоколирования, протоколирование включено
    return result;
}

uint64
sys_prot(void) {
    int code, on, ts;
    argint(0, &code);
    argint(1, &on);
    argint(2, &ts);

    acquire(&pr.lock);

    // Устанавливаем конечное время протоколирования
    if (ts != 0) pr.end_ticks = ticks + ts;  // Если количество тиков не равно 0, устанавливаем конечное время протоколирования как текущее время плюс количество тиков
    else pr.end_ticks = ts; // В противном случае, устанавливаем конечное время протоколирования равным 0, что означает отключение протоколирования

    pr.events[code - 1] = on;     // Устанавливаем флаг протоколирования для указанного события

    release(&pr.lock);

    return 0;
}