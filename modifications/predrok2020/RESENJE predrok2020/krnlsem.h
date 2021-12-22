#ifndef _KRNLSEM_H_
#define _KRNLSEM_H_

typedef unsigned int Time;

class PCB;
class Queue;

class KernelSem{
public:
	KernelSem(int init);
	~KernelSem();

	void signal(int);
	int wait(Time);

	int val() const { return value; }
	void setSignalCalled();

	static Queue *allBlockedQueue;

	int prioritySignall;

protected:
	void block(Time);
	void deblock();

private:
	int value;
	int signalCalled;
	Queue *blockedQueue;

	friend class Queue;
};

#endif
