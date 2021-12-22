#ifndef _LOCK_H_
#define _LOCK_H_

#include "system.h"

//#define _lock asm cli;
//#define _unlock asm sti;

void lock();
void unlock();

#endif
