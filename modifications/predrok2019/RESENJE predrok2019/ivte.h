/*
 * ivtentry.h
 *
 *  Created on: Jan 20, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(i,flag)\
	void interrupt inter##i(...);\
	IVTEntry e##i = IVTEntry(i,inter##i);\
	void interrupt inter##i(...){\
		e##i.signal();\
		if (flag == 1)\
			e##i.callOld();\
	}

class IVTEntry{
public:
	IVTEntry(IVTNo no, pInterrupt interrupt);
	~IVTEntry();

	void signal();
	void callOld();

	static IVTEntry* IVTable[256];
	static IVTEntry* getEntry(IVTNo i);
private:
	pInterrupt oldRoutine;
	IVTNo mIVTNo;
};


#endif /* IVTENTRY_H_ */
