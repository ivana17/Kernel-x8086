#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "thread.h"
#include "pcb.h"
#include "idle.h"

typedef void (interrupt *InterruptRoutine)();

class System{
public:
	static void init();
	static void restore();
	static void dispatch();
	static volatile int lockCnt;
protected:
	friend class Thread;
	friend class PCB;
	friend class KernelSem;
	friend class KernelEv;
private:
	static Thread *starting;
	static Idle *idle;
	static volatile int context_switch_demand;
	static void interrupt (*oldRoutine)();
	static void interrupt timer();
	static InterruptRoutine replaceRoutine(int intN, InterruptRoutine newRoutine);
};
extern void tick();

#endif
