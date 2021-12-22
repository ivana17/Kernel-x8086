#include "thread.h"
#include "pcb.h"
#include "lock.h"
#include "schedule.h"
#include "system.h"
#include <iostream.h>

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock();
}

Thread::~Thread(){
	lock();
	//waitToComplete();
	delete myPCB;
	unlock();
}

void Thread::waitToComplete(){
	lock();

	if((myPCB->myID <= 0) || (myPCB == (PCB*)PCB::running)) {
		unlock(); return;
	}
	if(myPCB->myState == PCB::OVER || this->myPCB->myState == PCB::NEW) {
		unlock(); return;
	}

	PCB::running->myState = PCB::BLOCKED;
	myPCB->myQueue->put((PCB*)PCB::running);
	unlock();

	dispatch();
}

ID Thread::getId(){
	return this->myPCB->threadID();
}

void Thread::start(){
	lock();
	if (myPCB->myID > 0 && myPCB->myState == PCB::NEW) {
		this->myPCB->myState = PCB::READY;
		this->myPCB->createStack();
		Scheduler::put(this->myPCB);
	}
	unlock();
}

ID Thread::getRunningId(){
	return ((PCB*)PCB::running)->threadID();
}

Thread *Thread::getThreadById(ID id){
	if (id == -1) return System::starting;
	else if (id == 0) return System::idle;
	PCB* temp = PCB::allPCBQueue->find(id);
	if(temp) return temp->myThread;
	else return 0;
}

void dispatch(){
	System::dispatch();
}

