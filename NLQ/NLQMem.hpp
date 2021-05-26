//
//  NLQMem.hpp
//  NLQueueTest
//
//  Created by Eric Kampman on 5/24/21.
//

#ifndef NLQMem_h
#define NLQMem_h

#include "NLQItem.hpp"

template<typename T> class NLQMem {
public:
	NLQMem<T>() {
		head_ = 0;
	}
	
	void push(NLQItem<T> *item) {
		bool done = false;
		do {
			NLQItem<T> *head = head_;
			item->next = head;
			done = __sync_bool_compare_and_swap(&head_, head, item);
		} while (!done);
	}
	
	NLQItem<T> *pop() {
		bool done = false;
		NLQItem<T> *ret = 0;	// compiler complained incorrectly
		NLQItem<T> *oh;
		do {
			if (0 == (oh = head_)) {
				return new NLQItem<T>(0);
			} else {
				NLQItem<T> *next = oh->next;
				ret = head_;
				done = __sync_bool_compare_and_swap(&head_, oh, next);
			}
		} while (!done);
		return ret;
	}
	
private:
	NLQItem<T> *head_;
};

#endif /* NLQMem_h */
