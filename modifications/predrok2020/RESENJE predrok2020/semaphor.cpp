#include "semaphor.h"
#include "krnlsem.h"
#include "system.h"
#include "lock.h"

Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	prioritySignal = 0;
	unlock();
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

void Semaphore::turnOnPriorities(){
	prioritySignal = 1;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	myImpl->signal(prioritySignal);
}

int Semaphore::val() const {
	return myImpl->val();
}
