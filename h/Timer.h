#ifndef H_TIMER_H
#define H_TIMER_H
#include "Thread.h"
#include "pcb.h"
#include "KernSem.h"
#include "KernelEv.h"
#include "Semaphor.h"

typedef void interrupt (*pInterrupt)(...);

extern volatile KernelSem* sys_sem;
extern volatile KernelEv* sys_ev;
extern volatile int contextChFlag;
extern volatile PCB* carrier;

void interrupt timer_routine();
void timer_routine_override();
void timer_routine_old();
static unsigned lockFlag;


#endif
