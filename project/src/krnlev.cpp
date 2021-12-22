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

KernelEv* KernelEv::allEvents[256] = { 0 };

KernelEv::KernelEv(IVTNo ivtNo){
	value = 0;
	mIVTNo = ivtNo;
	mHolder = (PCB*)PCB::running;
	mBlocked = 0;

	if (mIVTNo >= 0 && mIVTNo < 256) allEvents[mIVTNo] = this;
}

KernelEv::~KernelEv(){
	mHolder = 0;
	if (mIVTNo >= 0 && mIVTNo < 256) allEvents[mIVTNo] = 0;
}

void KernelEv::wait(){
	if ((PCB*)PCB::running == mHolder) {
		if (value == 0) block();
		else value = 0;
	}
}

void KernelEv::signal(){
	if (mBlocked == 0) value = 1;
	else deblock();
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
