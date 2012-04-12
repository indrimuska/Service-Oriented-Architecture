//
//  Application/Thread.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 12/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef Application_Thread_h
#define Application_Thread_h

#include <semaphore.h>

#include "../SOA/Service.h"
#include "../SOA/Communication.h"
#include "UniversalSemaphore.h"

using namespace std;

#define NUM_THREADS 10

class ThreadInfo {
private:
	bool busy;
	pthread_t thread;
	pthread_mutex_t mutex;
	universal_semaphore semaphore;
public:
	Socket client;
	void * otherInfos;
	
	ThreadInfo() {
		busy = false;
		if (pthread_mutex_init(&mutex, NULL)) {
			cerr << "Errore nell'inizializzazione del mutex\n";
			exit(0);
		}
		if (universal_sem_create(&semaphore, 0)) {
			cerr << "Errore nell'inizializzazione del semaforo\n";
			exit(0);
		}
	}
	void startThread(void * (* start_routine)(void *)) {
		static long i = 0;
		if (pthread_create(&thread, NULL, start_routine, (void *) i++)) {
			cerr << "Errore nell'inizializzazione del thread\n";
			exit(0);
		}
	}
	bool P() { return universal_sem_wait(&semaphore); }
	bool V() { return universal_sem_signal(&semaphore); }
	void setBusy() {
		pthread_mutex_lock(&mutex);
		busy = true;
		pthread_mutex_unlock(&mutex);
	}
	void setFree() {
		pthread_mutex_lock(&mutex);
		busy = false;
		pthread_mutex_unlock(&mutex);
	}
	bool isFree() {
		pthread_mutex_lock(&mutex);
		bool result = busy;
		pthread_mutex_unlock(&mutex);
		return result;
	}
	bool testAndSet() {
		pthread_mutex_lock(&mutex);
		if (!busy) {
			busy = false;
			pthread_mutex_unlock(&mutex);
			return true;
		}
		pthread_mutex_unlock(&mutex);
		return false;
	}
	~ThreadInfo() {
		universal_sem_destroy(semaphore);
		pthread_exit(NULL);
	}
};

#endif
