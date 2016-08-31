#include "Timer.h"
#include "Schedule.h"
#include "KernSem.h"
#include "pcb.h"
#include <iostream.h>

extern PCB* initial;

KernelSem::~KernelSem() {
	while(headBlocked->next) {
		volatile PCB* prev=headBlocked;
		headBlocked=headBlocked->next;
		prev->next=0;
		Scheduler::put((PCB*)prev);
	}
	if(headBlocked) Scheduler::put((PCB*)headBlocked);
	headBlocked=0;
}


void KernelSem::wait() {
	--value;
	if(value < 0) {
		contextChFlag = 4;
		sys_sem=this;
		asm int 8h
	}
}


void KernelSem::signal() {
	++value;
	if( (headBlocked!=0) && (value<=0)){
		if(headBlocked->next == 0) {
			if(headBlocked!=initial) {
			Scheduler::put((PCB*)headBlocked);
			headBlocked=0;
			}
			else {
				headBlocked==0;
				contextChFlag = 1;
				asm int 8h
			}
		}
		else {
			PCB* temp=(PCB*)headBlocked->next;
			PCB* prev=(PCB*)headBlocked;
			while(temp->next) { prev=(PCB*)temp; temp=(PCB*)temp->next; }
			prev->next=0;
			if(temp!=initial) {
				Scheduler::put((PCB*)temp);
				temp=0;
			}
			else{
				contextChFlag = 1;
				asm int 8h
			}
		}
	}
}



