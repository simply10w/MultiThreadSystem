// File: thread.h
#ifndef _Thread_h_
#define _Thread_h_
#define lock asm cli
#define unlock asm sti



typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 20; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	static void sleep(Time timeToSleep);
	PCB* myPCB;//vrati ga u protected!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
				defaultTimeSlice);
	virtual void run() {}
};

void dispatch ();


#endif
