//
//  NLQTestConsumer.h
//  NLQ
//
//  Created by Eric Kampman on 5/25/21.
//

#ifndef NLQTestConsumer_h
#define NLQTestConsumer_h

#include <thread>
#include <vector>
#include <ctime>
#include <algorithm>

#include "NLQ.hpp"

class NLQTestConsumer {
public:
	void operator()(int base) {
		time_t timeout = std::time(NULL) + 5;
		int err = 0;
		while (!err) {
			int val;
			err = gNLQ->pop(val);
			if (0 == err) {
				vals.push_back(val);
			} else {
				std::cout << "X";
				// don't quit!
				err = 0;
			}
			
			if (timeout <= std::time(NULL)) {
				err = 1;	// not really an error
			}
		}
		std::cout << std::endl << "Vals count: " << vals.size() << std::endl;
		sort(vals.begin(), vals.end());
		int prev = -1;
		for (int i : vals) {
			if (i != prev + 1) {
				std::cout << "!!! prev: " << prev << " i " << i << std::endl;
			} else {
				if (0 == i % 1000) {
					std::cout << ">>> i " << i << std::endl;
				}
			}
			prev = i;
		}
	}
	
	const int loopCount = 30000;
	std::vector<int> vals;
};


#endif /* NLQTestConsumer_h */
