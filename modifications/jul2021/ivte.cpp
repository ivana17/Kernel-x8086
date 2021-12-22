#include "ivte.h"
#include "krnlev.h"
#include "queue.h"
#include "evqueue.h"
#include <dos.h>

IVTEntry* IVTEntry::IVTable[256] = { 0 };

IVTEntry::IVTEntry(IVTNo numEntry, pInterrupt interruptRoutine) {
	oldRoutine = 0;
	mIVTNo = numEntry;
	if (mIVTNo >= 0 && mIVTNo < 256) {
		if (IVTable[mIVTNo] == 0) {

#ifndef BCC_BLOCK_IGNORE
			asm cli; // asm pushf; asm cli;
			oldRoutine = getvect(mIVTNo);
			setvect(mIVTNo, interruptRoutine);
			asm sti; // asm popf;
#endif

			IVTable[mIVTNo] = this;

		}
	}
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(mIVTNo, oldRoutine);
	asm sti;
#endif
	IVTable[mIVTNo] = 0; // return the old routine to that IVT entry
}

void IVTEntry::signal() {
	EvQueue* evq = KernelEv::allEvents[mIVTNo];
	if (mIVTNo >= 0 && mIVTNo < 256 && evq){
		EvQueue::Elem* temp = evq->head;
		while(temp){
			temp->info->signal();
			temp = temp->next;
		}
	}
}

void IVTEntry::callOld() {
	oldRoutine();
}

IVTEntry* IVTEntry::getEntry(IVTNo num) {
	if (num >= 0 && num < 256) return IVTable[num];
	return 0;
}
