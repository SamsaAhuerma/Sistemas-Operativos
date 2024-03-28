#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "semaphore.h"

#define NMAX 11

struct semaphore {
  int counter;
  int init_value;
  struct spinlock lock;
};

struct semaphore* sema[NMAX];

int sem_down(int sem){
    if (sema[sem]->counter < 0 || sema[sem]->counter > 1){
        return 0;
    }
    acquire(&sema[sem]->lock);
    while (sema[sem]->counter == 0){
        sleep(&sema[sem]->lock.name, &sema[sem]->lock);
    }
    sema[sem]->counter--;
    release(&sema[sem]->lock);
    return 1;
}


int sem_close(int sem){
    //cerrar el semaforo.
    kfree((char *)sema[sem]);
    sema[sem]=0;
    return 1;
}

int sem_open(int sem, int value){

    if (sem <0 || sem >= NMAX){
        return 0;
    }
    if (sema[sem] != 0){
        return 0;
    }

    sema[sem] = (struct semaphore *)kalloc();
    initlock(&sema[sem]->lock,"semaforro");
    acquire(&sema[sem]->lock);

    sema[sem]->counter = value;
    sema[sem]->init_value = value;

    release(&sema[sem]->lock);

    return sem;
}

int sem_up(int sem){

    acquire(&sema[sem]->lock);

    sema[sem]->counter++;

    wakeup(&sema[sem]->lock.name);
    release(&sema[sem]->lock);
    return 1;
}