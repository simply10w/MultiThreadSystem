/*
 * KernelEv.h
 *
 *  Created on: Aug 31, 2015
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"
#include "pcb.h"
#include "IVTEntry.h"
//unsigned char IVTNo
typedef void interrupt (*routinepointer)(...);

class KernelEv {
public:
	KernelEv(PCB* owner, unsigned char num);
	~KernelEv();
	unsigned int numWaiting;
	PCB* evOwner;
	PCB* evWaitingList;
	unsigned char ivtnum;
	routinepointer oldHandler;

	void wait();
	void signal();
	void override();
	void restore();

};



#endif /* KERNELEV_H_ */
