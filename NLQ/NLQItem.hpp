//
//  NLQItem.hpp
//  NLQueueTest
//
//  Created by Eric Kampman on 5/24/21.
//

#ifndef NLQItem_h
#define NLQItem_h

template<typename T> struct NLQItem {
	NLQItem *next;
	T item;
	bool dummy;
	
	NLQItem(T item, bool dummy = false) {
		this->item = item;
		this->next = 0;
		this->dummy = dummy;
	}
};



#endif /* NLQItem_h */
