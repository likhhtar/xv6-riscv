#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Функция для проверки доступа к страницам памяти и очистки флага доступа
uint64
sys_pgaccess(void) {
    uint64 page_addr;
    int size;
    int num_pages;
    uint64 result_addr;

    argaddr(0, &page_addr);
    argint(1, &size);
    argaddr(2, &result_addr);

    num_pages = (size + PGSIZE - 1) / PGSIZE;

    pagetable_t pagetable = myproc()->pagetable;

    int res = 0;
    for (int i = 0; i < num_pages; ++i) {
        // Получение указателя на запись таблицы страниц для текущей страницы
        pte_t *pte = walk(pagetable, page_addr, 0);

        if (*pte & PTE_A) {
            // Сброс флага доступа
            (*pte) &= ~PTE_A;
            res = 1;
        }

        page_addr += PGSIZE;
    }

    if (copyout(pagetable, result_addr, (char *)&res, sizeof(res)) < 0) return -1;
    return 0;
}