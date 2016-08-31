/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "Timer.h"

routinepointer IVTEntry::routines[256];
KernelEv* IVTEntry::events[256];


IVTEntry::IVTEntry(routinepointer function, IVTNo num) {
	entrynum=num;
	routines[num]=function;
}

IVTEntry::~IVTEntry() {
	routines[entrynum]=0;
}

void IVTEntry::getold(IVTNo num, routinepointer oldroutine) {
	oldroutine = events[num]->oldHandler;
}

void IVTEntry::signal(IVTNo num) {
	events[num]->signal();
}


