// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_
typedef unsigned int Time;
#include "thread.h"

class KernelSem;
class Queue;
class Semaphore {
public:
	Semaphore (int init = 1);
	virtual ~Semaphore ();

	virtual int wait (Time maxTimeToWait);
	virtual void signal();

	int val () const; // Returns the current value of the semaphore

	static void addOwner();
	void removeOwner(ID id);
	static Queue* owners;

private:
	KernelSem* myImpl;
};


#endif
