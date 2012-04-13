//
//  Application/Threads.hpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 12/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef Application_Threads_h
#define Application_Threads_h

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>

#include "../SOA/Communication.hpp"

using namespace std;

class ThreadInfo {
private:
	bool busy;
	boost::mutex mutex;
	boost::condition_variable condition;
public:
	Socket client;
	
	ThreadInfo() {
		busy = false;
	}
	void setBusy() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		busy = true;
	}
	void setFree() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		busy = false;
	}
	bool isFree() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		return busy;
	}
	bool testAndSet() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		if (!busy) {
			busy = true;
			return true;
		}
		return false;
	}
	void waitStart() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		while (!busy) condition.wait(scoped_lock);
	}
	void startThread() {
		condition.notify_one();
	}
};

#endif
