/*
 * krnlev.h
 *
 *  Created on: Jan 20, 2020
 *      Author: OS1
 */

#ifndef _KRNLEV_H_
#define _KRNLEV_H_

#include "event.h"

class PCB;
class EvQueue;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo, int);
	~KernelEv();

	void wait();
	void signal();

	static int allEventsSize[256];
	static EvQueue* allEvents[256];
	int priority;

private:
	void deblock();
	void block();

	int value;
	int mBlocked;

	IVTNo mIVTNo;

	PCB* mHolder;

};

#endif /* KRNLEV_H_ */
