#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

class Thread;

class Idle : public Thread {
public:
	Idle(): Thread(256, 1) {}
//protected:
	virtual void run();
};

#endif
//poslednja verzija