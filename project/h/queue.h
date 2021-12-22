#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "thread.h"

class PCB;

class Queue{
public:
	Queue(): size(0), head(0), tail(0){}
	~Queue();
	int getSize() const { return size; };
	void put(PCB* pcb);
	PCB* get();
	void timeTick();
	void enqueue(PCB*);
	PCB* removeFirst();
	PCB* find(ID);
	int removePCB(PCB*);
	struct Elem{
		PCB *info;
		Elem* next;
	};
private:
	Elem *head, *tail;
	int size;
protected:
	friend class Thread;
};

#endif
