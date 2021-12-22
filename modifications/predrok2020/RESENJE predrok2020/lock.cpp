#include "lock.h"

void lock() {
	System::lockCnt++;
}

void unlock() {
	if(System::lockCnt > 0)
		System::lockCnt--;
}
