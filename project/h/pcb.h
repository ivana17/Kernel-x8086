#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "queue.h"

class System;
class KernelSem;
typedef unsigned int Time;
typedef int State;

class PCB{
public:
	PCB(Thread *myThr, StackSize stacksz, Time timeSlice);
	~PCB();
	enum State { READY, BLOCKED, OVER, NEW };
	State myState;
	void createStack();
	StackSize stackSize;
	ID threadID() { return myID; }
	Queue *myQueue;
	static Queue* allPCBQueue;
	static void wrapper();

private:
	static volatile PCB* running;

	static ID idCnt;
	ID myID;
	Thread *myThread;
	Time myTimeSlice;
	volatile Time timeSliceCnt;
	unsigned int sp, ss;
	unsigned char *myStack;
	KernelSem* mySem;
	Time semWaitTime;

	friend class Thread;
	friend class Queue;
	friend class System;
	friend class KernelSem;
	friend class KernelEv;
};

#endif
