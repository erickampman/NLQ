//
//  main.cpp
//  NLQ
//
//  Created by Eric Kampman on 5/25/21.
//

#include <iostream>
#include <thread>

#include "NLQ.hpp"
#include "NLQTestProducer.hpp"
#include "NLQTestConsumer.hpp"


NLQ<int> *gNLQ = 0;


int main(int argc, const char * argv[]) {
	gNLQ = new NLQ<int>();

	NLQTestProducer p1, p2, p3, p4;
	NLQTestConsumer c1;
	
	std::thread t0(c1, 0);

	std::thread t1(p1, 0);
	std::thread t2(p2, 10000);
	std::thread t3(p3, 20000);
	std::thread t4(p4, 30000);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	t0.join();
	return 0;
}
