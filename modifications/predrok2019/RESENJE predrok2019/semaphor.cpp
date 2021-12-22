#include "semaphor.h"
#include "krnlsem.h"
#include "system.h"
#include "lock.h"
#include "queue.h"

Queue* Semaphore::owners = new Queue();

Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

void Semaphore::addOwner(){
	owners->enqueue((PCB*)PCB::running);
}

void Semaphore::removeOwner(ID id){
	PCB* old = owners->find(id);
	if(old != 0)
		owners->removePCB(old);
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

int Semaphore::wait(Time maxTimeToWait) {
	if(owners->find(Thread::getRunningId()) != 0)
		return myImpl->wait(maxTimeToWait);
	return -1;
}

void Semaphore::signal() {
	if(owners->find(Thread::getRunningId()) != 0)
		myImpl->signal();
}

int Semaphore::val() const {
	return myImpl->val();
}
