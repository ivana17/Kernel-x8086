#include "pcb.h"
#include "lock.h"
#include "system.h"
#include "schedule.h"
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>

ID PCB::idCnt = -1;

Queue *PCB::allPCBQueue = new Queue();
volatile PCB* PCB::running = 0;

PCB::PCB(Thread *myThread, StackSize stackSize, Time myTimeSlice){
	lock();
	this->myThread = myThread;
	this->stackSize = stackSize;
	this->myTimeSlice = myTimeSlice;
	this->myState = NEW;

	myID = idCnt++;
	myStack = 0;
	sp = ss = 0;
	timeSliceCnt = 0;
	semWaitTime = 0;
	mySem = 0;

	if(myID > 0) allPCBQueue->put(this);
	myQueue = new Queue();

	unlock();
}

PCB::~PCB(){
	lock();
	delete myStack;
	delete myQueue;
	unlock();
}

void PCB::createStack(){
	lock();
	static volatile unsigned tss, tsp, oldss, oldsp, tip, tcs, tax, ths, tho;
	this->myStack = new unsigned char[this->stackSize];
#ifndef BCC_BLOCK_IGNORE
	tsp = this->sp = FP_OFF(myStack + stackSize);
	tss = this->ss = FP_SEG(myStack + stackSize);

	tip = FP_OFF(&(PCB::wrapper));
	tcs = FP_SEG(&(PCB::wrapper));

	tho = FP_OFF(&dispatch);
	ths = FP_SEG(&dispatch);

	asm{
		mov tax, ax
		mov oldss, ss
		mov oldsp, sp
		mov ss, tss
		mov sp, tsp

		mov ax, ths
		push ax //cs za posle wrappera
		mov ax, tho
		push ax //ip za posle wrappera

		pushf
		pop ax
		or ax, 1000000000b
		push ax

		mov ax, tcs
		push ax
		mov ax, tip
		push ax

		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp

		mov tsp, sp
		mov sp, oldsp
		mov ss, oldss
	}
#endif
	this->sp = tsp;
	unlock();
}

//static unsigned char* stack;
//stack = new unsigned char[this->stackSize];
//this->myStack = stack;

void PCB::wrapper(){
	PCB::running->myThread->run();
	lock();
	allPCBQueue->removePCB((PCB*)PCB::running);
	PCB* temp = 0;
	int qSize = PCB::running->myQueue->getSize();
	for(int i = 0; i < qSize; i++){
		temp = PCB::running->myQueue->get();
		temp->myState = PCB::READY;
		Scheduler::put((PCB*)temp);
	}
	PCB::running->myState = OVER;
	unlock();
}
