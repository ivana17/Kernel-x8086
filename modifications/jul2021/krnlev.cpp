/*
 * krnlev.cpp
 *
 *  Created on: Jan 20, 2020
 *      Author: OS1
 */

#include "krnlev.h"
#include "pcb.h"
#include "schedule.h"
#include "lock.h"
#include "system.h"
#include "evqueue.h"
#include <iostream.h>

int KernelEv::allEventsSize[256] = { 0 };
EvQueue* KernelEv::allEvents[256];

KernelEv::KernelEv(IVTNo ivtNo, int p){
	value = 0;
	mIVTNo = ivtNo;
	mHolder = (PCB*)PCB::running;
	mBlocked = 0;

	priority = p;
	if (allEventsSize[mIVTNo] == 0)
		allEvents[mIVTNo] = new EvQueue();
	allEventsSize[mIVTNo]++;
	allEvents[mIVTNo]->put(this);
}

KernelEv::~KernelEv(){
	mHolder = 0;
	if (mIVTNo >= 0 && mIVTNo < 256) {
		allEventsSize[mIVTNo]--;
		allEvents[mIVTNo]->removeEvent(this);
	}
}

void KernelEv::wait(){
	if ((PCB*)PCB::running == mHolder) {

		if (value == 0) {
			lock();
			cout<<"BLOCKED - THREAD ID = "<<Thread::getRunningId()<<endl;
			unlock();

			block();
		}
		else value = 0;
	}
}

void KernelEv::signal(){
	if (mBlocked == 0) value = 1;
	else deblock();

	lock();
	cout<<"SIGNALED - THREAD ID = "<<priority<<endl;
	unlock();
}

void KernelEv::block() {
	lock();
	mHolder->myState = PCB::BLOCKED;
	mBlocked = 1;
	unlock();
	dispatch();
}

void KernelEv::deblock() {
	lock();
	mHolder->myState = PCB::READY;
	Scheduler::put(mHolder);
	mBlocked = 0;
	unlock();
}
