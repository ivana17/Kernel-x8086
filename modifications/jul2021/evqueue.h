#ifndef _EVQUEUE_H_
#define _EVQUEUE_H_

class KernelEv;
class IVTEntry;

class EvQueue{
public:
	EvQueue(): size(0), head(0), tail(0){}
	~EvQueue();
	int getSize() const { return size; };
	void put(KernelEv*);
	KernelEv* find(int);
	int removeEvent(KernelEv*);

	struct Elem{
		KernelEv* info;
		Elem* next;
	};

	Elem *head, *tail;
	int size;
protected:
	friend class KernelEv;
	friend class IVTEntry;
};

#endif
