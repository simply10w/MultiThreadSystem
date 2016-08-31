/*
 * IVTEntry.h
 *
 *  Created on: Aug 31, 2015
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"
#include <iostream.h>
#include "Timer.h"



typedef void interrupt (*routinepointer)(...);


class IVTEntry{
public:

		static KernelEv* events[256];
		static routinepointer routines[256];
		~IVTEntry();
		IVTEntry(routinepointer function, unsigned char num);
		static void getold(unsigned char num, routinepointer oldroutine);
		static void signal(unsigned char num);
		unsigned char entrynum;
};



#define PREPAREENTRY(ivtno, old)\
KernelEv IVTEntry::*events[256];\
void interrupt intr##ivtno () {\
	IVTEntry::signal(ivtno);\
}\
IVTEntry Instance##ivtno((routinepointer) intr##ivtno, (unsigned char) ivtno);


#endif /* IVTENTRY_H_ */
