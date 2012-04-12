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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class ThreadInfo {
private:
	pthread_t thread;
	universal_semaphore semaphore;
public:
	bool busy;
	Socket client;
	void * otherInfos;
	
	ThreadInfo() {
		busy = false;
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
	void setInfos(void * infos) { otherInfos = infos; }
	bool P() { return universal_sem_wait(&semaphore); }
	bool V() { return universal_sem_signal(&semaphore); }
	~ThreadInfo() {
		universal_sem_destroy(semaphore);
		pthread_exit(NULL);
	}
};

#endif
