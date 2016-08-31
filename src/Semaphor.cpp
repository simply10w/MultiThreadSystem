
#include "Timer.h"
#include "KernSem.h"
#include "Semaphor.h"
#include "pcb.h"

int  Semaphore:: val() const { return myImpl->val(); }

Semaphore:: ~Semaphore() {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		if(myImpl) delete myImpl;
		asm popf
	#endif
}

Semaphore::Semaphore(int init) {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
		myImpl = new KernelSem(init);
		asm popf
	#endif
}

void Semaphore::wait() {
	#ifndef BCC_BLOCK_IGNORE
		lock
		myImpl->wait();
		unlock
	#endif
}

void Semaphore::signal() {
	#ifndef BCC_BLOCK_IGNORE
		lock
		myImpl->signal();
		unlock
	#endif
}








