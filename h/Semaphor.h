#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_

#include "KernSem.h"
class KernelSem;

class Semaphore {
public:
Semaphore (int init=1);
virtual ~Semaphore ();
int val() const;
virtual void wait ();
virtual void signal();

private:
KernelSem* myImpl;
};

#endif /* SEMAPHORE_H_ */
