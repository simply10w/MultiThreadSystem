#include "pcb.h"
#include <dos.h>
#include "Timer.h"
#include "Thread.h"
#include "Schedule.h"
#include <iostream.h>
#include "KernSem.h"

PCB* initial= new PCB();
PCB* idlethread = new PCB(PCB::idle);
volatile PCB* PCB::running;
int PCB::active_threads=0;
int PCB::posId=0;
volatile PCB* carrier;



PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		myThread=t;
		toSignal=0;
		unlimited=0;
		id=++posId;
		if((time_left=timeSlice) == 0) unlimited=1;
		sleep_left=0;
		if(stackSize>MAX_SIZE) st_size=MAX_SIZE;
		else st_size=stackSize;
		st=0;
		createContext(PCB::wrapper);
		next=0;
		asm popf
	#endif
}

PCB::PCB(void(*body)()){ //za idle nit
	#ifndef BCC_BLOCK_IGNORE
		myThread=0;
		toSignal=0;
		unlimited=0;
		id=0;
		time_left=1;
		st_size=1024;
		st=0;
		createContext(body);

	#endif
}

PCB::PCB() {
	#ifndef BCC_BLOCK_IGNORE
		time_left=55;
		toSignal=0;
		sp=0;
		bp=0;
		ss=0;
		id=-1;
		unlimited=0;
		next=0;
		myThread=0;
		st=0;//mozda i ne treba ovo

	#endif
}

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock
	delete [] st;
	unlock
#endif
}

void PCB::createContext(void (*code)()) { //na vrh steka ide PSW, CS, PC
	st = new unsigned[st_size];
	st[st_size - 1] = 0x200; //PSW SA SETOVANIM I NA VRH STEKA
	#ifndef BCC_BLOCK_IGNORE
		st[st_size - 2] = FP_SEG(code);
		st[st_size - 3] = FP_OFF(code); //proveriti ovo
		sp=FP_OFF(st + st_size - 12);
		ss=FP_SEG(st + st_size - 12);
		bp=FP_OFF(st + st_size - 12);
	#endif
}

void PCB::exitThread(){
	#ifndef BCC_BLOCK_IGNORE
		lock
		contextChFlag = 2;
		asm int 8h
		unlock
	#endif
}

void PCB::sleep(unsigned long timeToSleep) { //u timeru cu da vrsim budjenje i azuriranje vremena
	#ifndef BCC_BLOCK_IGNORE
		lock
		contextChFlag=3;
		running->sleep_left=timeToSleep;
		asm int 8h
		unlock
	#endif
}


void PCB::waitToComplete() {
	#ifndef BCC_BLOCK_IGNORE
		if( (PCB::running == this) || (this==initial) || (this == idlethread)) return;
		else {
			lock
			carrier=this;
			contextChFlag=5;
			asm int 8h
			unlock
		}
	#endif
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	--PCB::active_threads;
	PCB::running->exitThread();
}

void PCB::idle() {
	while(PCB::active_threads);
}


void PCB::start(){
#ifndef BCC_BLOCK_IGNORE
	lock
	++PCB::active_threads;
	Scheduler::put(this);
	unlock
#endif
}

 





