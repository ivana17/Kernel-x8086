#include <iostream.h>
#include "event.h"
#include "krnlev.h"
#include "system.h"
#include "lock.h"

Event::Event(IVTNo ivtNo, int p) {
	lock();
	myImpl = new KernelEv(ivtNo, p);
	unlock();
}

Event::~Event() {
	lock();
	if(myImpl)
		delete myImpl;
	myImpl = 0;
	unlock();
}

void Event::wait() {
	if(myImpl)
		myImpl->wait();
}

void Event::signal() {
	if(myImpl)
		myImpl->signal();
}
