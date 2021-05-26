//
//  NLQTestProducer.hpp
//  NLQ
//
//  Created by Eric Kampman on 5/25/21.
//

#ifndef NLQTestProducer_h
#define NLQTestProducer_h

#include <thread>

#include "NLQ.hpp"

extern NLQ<int> *gNLQ;

class NLQTestProducer {
public:
	void operator()(int base) {
		for (int i = base; i < base + loopCount; ++i) {
			gNLQ->push(i);
		}
	}
	
	const int loopCount = 10000;
};

#endif /* NLQTestProducer_h */
