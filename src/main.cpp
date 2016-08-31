/*
 * main.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: OS1
 */
#include "Timer.h"
#include "pcb.h"
#include "Thread.h"
#include "Schedule.h"

#include <iostream.h>

extern PCB* initial;

extern int userMain(int argc, char* argv[]);

int retvalue;

int main(int argc, char* argv[]){
	cout << endl << "Petar!" << endl << endl;
	PCB::running = initial;
	timer_routine_override();
	retvalue = userMain(argc, argv);
	timer_routine_old();
	cout << "Function userMain returned " << retvalue;
	return retvalue;
}




