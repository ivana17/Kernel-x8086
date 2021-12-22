#include <iostream.h>
#include "event.h"
#include "krnlev.h"
#include "system.h"
#include "lock.h"

Event::Event(IVTNo ivtNo) {
	lock();
	myImpl = new KernelEv(ivtNo);
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
