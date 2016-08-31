#ifndef H_PCB_H_
#define H_PCB_H_

#include "Thread.h"
#include "KernSem.h"
#include <iostream.h>

class Thread;
class KernelSem;



#define MAX_SIZE 16384
class PCB {
public:

	//konstruktori i destruktor
	PCB(Thread* t,StackSize stackSize, Time timeSlice);

	PCB(void(*body)()); //for idle
	PCB(); 				//FOR INITiAL PCB
	~PCB();

	//esencijalni atributi
	volatile unsigned sp;
	volatile unsigned bp;
	volatile unsigned ss;
	unsigned* st;
	volatile StackSize st_size;
	volatile Time time_left;
	int id;
	Thread* myThread;

	//pomocni atributi
	volatile unsigned long sleep_left;
	volatile PCB* next;
	volatile PCB* toSignal;
	unsigned unlimited;

	//staticki atributi
	static int posId;
	volatile static PCB* running;
	static int active_threads;

	//metode
	void createContext(void (*code) ());
	void waitToComplete();
	void start();

	//staticke metode
	static void sleep(unsigned long timeToSleep);
	static void wrapper();
	static void idle();
	static void exitThread();
};

#endif
