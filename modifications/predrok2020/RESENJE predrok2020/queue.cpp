#include "queue.h"
#include "lock.h"
#include "pcb.h"
#include "schedule.h"
#include "krnlsem.h"

void Queue::put(PCB* pcb){
	lock();
	Elem *temp = head;
	while(temp){
		if(temp->info == pcb) {unlock(); return; }
		temp = temp->next;
	}
	if(head == 0) head = tail = new Elem();
	else{
		tail->next = new Elem();
		tail = tail->next;
	}
	tail->info = pcb;
	tail->next = 0;
	++size;
	unlock();
}

PCB* Queue::get(){
	if(head==0) return 0;
	lock();
	PCB *ret = head->info;
	head = head->next;
	--size;
	unlock();
	return ret;
}

Queue::~Queue(){
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

int Queue::removePCB(PCB* pcb){
	if(!head) return 0;
	Elem *tren = head, *pret = 0;
	int ret = 0;
	while(tren){
		if(tren->info == pcb){
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

PCB* Queue::find(ID id){
	if (head == 0)
		return 0;
	Elem *temp = head;
	while(temp){
		if(temp->info->myID == id) return temp->info;
		temp = temp->next;
	}
	return 0;
}


void Queue::enqueue(PCB* pcb){
	Elem* tmp = 0;
	for (tmp = head; tmp && tmp->info != pcb; tmp = tmp->next);
	if (tmp != 0) return;
	lock();
	if (head == 0) head = tail = new Elem();
	else tail = tail->next = new Elem();
	tail->info = pcb;
	tail->next = 0;
	size++;
	unlock();
}

PCB* Queue::removeFirst(){
	if(head==0) return 0;
	lock();
	Elem* temp = head;
	PCB *ret = head->info;
	head = head->next;
	delete temp;
	--size;
	unlock();
	return ret;
}

void Queue::timeTick(){
	if(head==0) return;
	Elem* temp = head;
	while(temp != 0){
		lock();
		temp->info->semWaitTime--;
		if(temp->info->semWaitTime <= 0){
			temp->info->myState = PCB::READY;
			Scheduler::put(temp->info);
			temp->info->mySem->blockedQueue->removePCB(temp->info);
			temp->info->mySem->setSignalCalled();
			removePCB(temp->info);
		}
		unlock();
	}
}

