#include "krnlsem.h"
#include "schedule.h"
#include "lock.h"
#include <iostream.h>
#include <stdlib.h>
#include <limits.h>

Queue *KernelSem::allBlockedQueue = new Queue();

KernelSem::KernelSem(int init) :
		value(init), signalCalled(0) {
	blockedQueue = new Queue();
	if(value > 0){
		signalCalled = 1;
	}
	prioritySignall = 0;
}

KernelSem::~KernelSem(){
	while (blockedQueue->getSize() > 0) {
		deblock();
	}
	delete blockedQueue;
}

int KernelSem::wait(Time waitTime){

	if(--value < 0) {
		if(prioritySignall != 0){
			lock();
			cout << "Thread #" << PCB::running->myID << " called wait and blocked" << endl;
			unlock();
		}
		block(waitTime);
	} else {
		lock();
		cout << "Thread #" << PCB::running->myID << " called wait and did not block" << endl;
		unlock();
	}

	return signalCalled;
}

void KernelSem::signal(int prioritySignal){
	lock();
	this->prioritySignall = prioritySignal;

	cout << "Thread #" << PCB::running->myID << " called signal" << endl;

	if(value++ < 0) {
		if(prioritySignal == 0)
			deblock(); //standard deblock
		else {
			//deblock with priority
			PCB* pcb = blockedQueue->getMinId();
			blockedQueue->removePCB(pcb);
			if (pcb != 0) {
				allBlockedQueue->removePCB(pcb);
				pcb->myState = PCB::READY;
				pcb->mySem = 0;
				Scheduler::put((PCB*)pcb);
			}
		}
	}
	signalCalled = 1;
	unlock();
}

void KernelSem::setSignalCalled(){
	signalCalled = 0;
	value++;
}

void KernelSem::block(Time waitTime){
	lock();

	if(waitTime != 0){
		PCB::running->semWaitTime = waitTime;
		PCB::running->mySem = this;
		allBlockedQueue->enqueue((PCB*)PCB::running);
	}
	else{
		PCB::running->semWaitTime = INT_MAX;
		PCB::running->mySem = 0;
	}
	blockedQueue->put((PCB*)PCB::running);
	PCB::running->myState = PCB::BLOCKED;

	unlock();

	System::dispatch();
}

void KernelSem::deblock(){
	PCB* pcb = blockedQueue->removeFirst();
	if (pcb != 0) {
		allBlockedQueue->removePCB(pcb);
		pcb->myState = PCB::READY;
		pcb->mySem = 0;
		Scheduler::put((PCB*)pcb);
	}
}

