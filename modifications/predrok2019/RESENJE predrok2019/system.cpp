#include "system.h"
#include "schedule.h"
#include "lock.h"
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>
#include "krnlsem.h"

Thread* System::starting = 0;
Idle* System::idle = 0;

volatile int System::context_switch_demand = 0;
volatile int System::lockCnt = 0;

const unsigned maxStackSize = 65536;
volatile int callOld = 0;

void interrupt (*System::oldRoutine)() = 0;

void System::init(){
#ifndef BCC_BLOCK_IGNORE
		asm cli;
#endif
	oldRoutine = replaceRoutine(0x08, timer);
	starting = new Thread(maxStackSize, 1);
	starting->myPCB->myState = PCB::READY;
	PCB::running = starting->myPCB;
	idle = new Idle();
	idle->myPCB->myState = PCB::READY;
	idle->myPCB->createStack();
#ifndef BCC_BLOCK_IGNORE
		asm sti;
#endif
}

void System::restore(){
	if((PCB*)PCB::running != starting->myPCB) return;
#ifndef BCC_BLOCK_IGNORE
		asm cli;
#endif
	replaceRoutine(0x08, oldRoutine);
	delete PCB::allPCBQueue;
	delete starting;
#ifndef BCC_BLOCK_IGNORE
		asm sti;
#endif
}

void System::dispatch(){
	context_switch_demand = 1;
#ifndef BCC_BLOCK_IGNORE
		asm int 8h;
#endif
}

void interrupt System::timer() {
	static volatile unsigned int tsp, tss;
	static PCB *newThread;

	if (!context_switch_demand && PCB::running->myTimeSlice != 0)
		PCB::running->timeSliceCnt += 1;

	if(!context_switch_demand) {
		tick();
		KernelSem::allBlockedQueue->timeTick();
		callOld = 1;
	}

	if (context_switch_demand || (PCB::running->timeSliceCnt == PCB::running->myTimeSlice && PCB::running->myTimeSlice != 0)){
		if (lockCnt == 0) {
			context_switch_demand = 0;

			if(PCB::running->myState == PCB::READY && (PCB*)PCB::running != idle->myPCB && (PCB*)PCB::running != starting->myPCB)
				Scheduler::put((PCB*)PCB::running);
			while(1) {
				newThread = Scheduler::get();

				if (newThread == 0){
					if(PCB::allPCBQueue->getSize() == 0)
						newThread = (starting->myPCB->myState == PCB::READY) ? starting->myPCB : idle->myPCB;
					else
						newThread = idle->myPCB;
				}
				if (newThread->myState != PCB::READY) continue;
				asm {
					mov tsp, sp
					mov tss, ss
				}
				PCB::running->sp = tsp;
				PCB::running->ss = tss;

				PCB::running = newThread;
				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				asm {
					mov sp, tsp
					mov ss, tss
				}
				PCB::running->timeSliceCnt = 0;
				break;
			}
		} else {
			context_switch_demand = 1;
		} // if(lockCnt == 0)
	}

	if(callOld != 0){
		(*oldRoutine)();
		callOld = 0;
	}

}

InterruptRoutine System::replaceRoutine(int intN, InterruptRoutine newRoutine){
	InterruptRoutine old;
#ifndef BCC_BLOCK_IGNORE

	unsigned int tseg = FP_SEG(newRoutine);
	unsigned int toff = FP_OFF(newRoutine);
	unsigned int oldseg, oldoff;

	intN *= 4;
	asm {
		push es
		push ax
		push bx
		mov ax,0
		mov es,ax
		mov bx, word ptr intN

		//pamti staru
		mov ax, word ptr es:bx+2
		mov word ptr oldseg, ax
		mov ax, word ptr es:bx
		mov word ptr oldoff, ax

		//postavlja novu
		mov ax, word ptr tseg
		mov word ptr es:bx+2, ax
		mov ax, word ptr toff
		mov word ptr es:bx, ax

		pop bx
		pop ax
		pop es
	}
	old = (InterruptRoutine) MK_FP(oldseg,oldoff);

#endif
	return old;
}
