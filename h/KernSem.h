
#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

#include "pcb.h"
#include "Semaphor.h"

class KernelSem {
public:
	volatile PCB* headBlocked;
	volatile int attached_pcb_num;
	KernelSem(int init) {
		value=init;
		headBlocked=0;
	}
	~KernelSem();
	void wait();
	void signal();
	int val() const { return value; };

protected:
	friend class Semaphore;
	friend class PCB;
private:
	int value;
};

#endif /* KERNELSEM_H_ */
