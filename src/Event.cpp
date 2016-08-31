/*
 * Event.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: OS1
 */


#include "Event.h"
#include "pcb.h"
#include "KernelEv.h"
#include "IVTEntry.h"


Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv((PCB*)PCB::running, ivtNo);
}

Event::~Event() {
	delete myImpl;
}

void Event::signal() {
	myImpl->signal();
}

void Event::wait() {
	myImpl->wait();
}

