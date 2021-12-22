#include "queue.h"
#include "lock.h"
#include "pcb.h"
#include "schedule.h"
#include "krnlev.h"
#include "evqueue.h"

void EvQueue::put(KernelEv* ev){
	lock();
	Elem *temp = head;
	while(temp){
		if(temp->info == ev) {unlock(); return; }
		temp = temp->next;
	}

	Elem* newElem = new Elem();
	newElem->info = ev;
	newElem->next = 0;

	if(head == 0) head = tail = newElem;
	else{
		Elem *tElem = head, *prev = 0;
		while(tElem && tElem->info->priority > ev->priority){
			prev = tElem; tElem =  tElem->next;
		}
		if(prev){
			newElem->next = prev->next;
			prev->next = newElem;
			if(prev == tail) tail = tail->next;
		} else{
			newElem->next = head;
			head = newElem;
		}
	}
	++size;
	unlock();
}

EvQueue::~EvQueue(){
	lock();
	Elem* temp;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
	size = 0;
	head = tail = 0;
	unlock();
}

int EvQueue::removeEvent(KernelEv* ev){
	if(!head) return 0;
	Elem *tren = head, *pret = 0;
	int ret = 0;
	while(tren){
		if(tren->info == ev){
			if(tren == head) head = head->next;
			else pret->next = tren->next;
			--size;
			ret = 1;
			break;
		}
		pret = tren;
		tren = tren->next;
	}
	if(ret){
		if(!head) tail = 0;
		else if(tren == tail) tail = pret;
	}
	return ret;
}

KernelEv* EvQueue::find(int pr){
	if (head == 0)
		return 0;
	Elem *temp = head;
	while(temp){
		if(temp->info->priority == pr) return temp->info;
		temp = temp->next;
	}
	return 0;
}


