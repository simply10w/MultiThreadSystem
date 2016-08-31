// File: thread.cpp
#include "Thread.h"
#include "Schedule.h"
#include "Timer.h"
#include "pcb.h"
#include <iostream.h>




Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = new PCB(this,stackSize, timeSlice);
}

Thread::~Thread() {
	delete myPCB;
} 

void Thread::waitToComplete() {
	myPCB->waitToComplete();
} 

void Thread::sleep(Time timeToSleep) {
	PCB::sleep((unsigned long) timeToSleep);
} 

void Thread::start() {
	myPCB->start();
}

void dispatch() {
    #ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli
	contextChFlag=1;
	asm int 8h;
	asm popf
	#endif
}



