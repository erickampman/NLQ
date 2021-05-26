//
//  NLQ.hpp
//  NLQueueTest
//
//  Created by Eric Kampman on 5/23/21.
//


/*
 https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf
 
 This algorithm includes a count field for head and tail, but I currently:
 - don't understand why it's needed
 - it seems to need to be set via CAS but there's no CAS for a struct...
 */

#include <cassert>

#include "NLQItem.hpp"
#include "NLQMem.hpp"

#ifndef NLQ_h
#define NLQ_h

template<typename T> struct NLQCtxt {
	NLQItem<T> *head_;
	NLQItem<T> *tail_;
	
	NLQCtxt() { head_ = tail_ = 0; }
};

template<typename T> NLQItem<T> *newQItem(T val)
{
	// is new thread-safe? I suppose it had better be...
	// FIXME - need to save off used items instead of new/delete
	// this requires another queue...
	return new NLQItem<T>(val);
}

template<typename T> void delQItem(NLQItem<T> **item)
{
	// FIXME add to queue instead of delete
	if (item && *item) {
		delete *item;
		*item = 0;
	}
}


template<typename T> class NLQ {
public:
	NLQ() :
		qctxt()
	{
		// Note that T needs to be able to be 0.
		// Not enough of a C++ guru to know how to
		// mandate that. We don't care about the
		// value in the dummy, however.
		NLQItem<T> *dummy = new NLQItem<T>(0, true);
		qctxt.head_ = qctxt.tail_ = dummy;
	}
	
#if 1
	// this works, but I'm dubious about "ot->next = node", atomic-wise
	void push(T value) {
		NLQItem<T> *node = qmem.pop(); //newQItem(value);
		node->item = value;
		NLQItem<T> *ot = __sync_lock_test_and_set(&qctxt.tail_, node);
		ot->next = node;	// not too sure about this...
	}
#else
	// not sure why the below has issues.
	void push(T value) {
		bool done = false;
		NLQItem<T> *node = qmem.pop(); 
		node->item = value;

		do {
			NLQItem<T> *ot = qctxt.tail_;
			ot->next = node;	// the problem has to be here.
			done = __sync_bool_compare_and_swap(&qctxt.tail_, ot, node);
		} while (!done);
	}
#endif
	int pop(T& value) {
		bool done = false;
		NLQItem<T> *oh;
		NLQItem<T> *nh;
		do {
			if (0 == (oh = qctxt.head_)) {
				// should never happen!
				assert(0);
			}

			if (0 == (nh = oh->next)) {
				return -1;
			}

			done = __sync_bool_compare_and_swap(&qctxt.head_, oh, nh);
		} while (!done);
		
		value = nh->item;
		qmem.push(oh);
		
		return 0;
	}
	
private:
	NLQCtxt<T> qctxt;
	NLQMem<T> qmem;
};

#endif /* NLQ_h */
