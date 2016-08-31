/*
 * KernelEv.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Timer.h"
#include "Schedule.h"
#include "IVTEntry.h"
#include "Event.h"
#include "pcb.h"
#include <dos.h>
#include <iostream.h>



KernelEv::KernelEv(PCB* owner, unsigned char num) {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		evOwner=owner;
		ivtnum=num;
		IVTEntry::events[num]=this;
		evWaitingList=0;
		numWaiting=0;
		override();
		asm popf
	#endif
}

KernelEv::~KernelEv() {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		IVTEntry::events[ivtnum]=0;
		restore();
		asm popf
	#endif
}

void KernelEv::signal() { //ovo se poziva samo iz prekidne rutine zato ne trebaju lock i unlock primitive
	#ifndef BCC_BLOCK_IGNORE
		while(evWaitingList!=0) {
			PCB* prev = (PCB*) evWaitingList;
			PCB* temp = (PCB*) evWaitingList->next;
			prev->next=0;
			Scheduler::put(prev);
			evWaitingList=temp;
		}
	#endif
}

void KernelEv::wait() {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		if(evOwner == PCB::running){
			if(numWaiting++ != 0) {
						PCB::running->next=evWaitingList;
			}
			evWaitingList = (PCB*)PCB::running;
			contextChFlag = 6;
			asm int 8h
		}
	asm popf
	#endif
}

void KernelEv::override() {
	#ifndef BCC_BLOCK_IGNORE
		oldHandler = getvect(ivtnum);
		setvect(ivtnum, IVTEntry::routines[ivtnum]);
	#endif
}

void KernelEv::restore() {
	#ifndef BCC_BLOCK_IGNORE
		setvect(ivtnum, oldHandler);
	#endif
}

