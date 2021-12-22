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

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

	static KernelEv *allEvents[256];
private:
	void deblock();
	void block();

	int value;
	int mBlocked;

	IVTNo mIVTNo;

	PCB* mHolder;
};

#endif /* KRNLEV_H_ */
