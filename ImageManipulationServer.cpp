//
//  ImageManipulationServer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "SOA/SOA.h"
#include "Application/Threads.h"
#include "Application/ImageManipulation.h"

using namespace std;

void * threadMain(void * threadID);

ThreadInfo threadsInfo[NUM_THREADS];

struct Infos {
	RotateService * rotate;
	HorizontalFlipService * horizontalFlip;
	Infos(RotateService * rotate, HorizontalFlipService * horizontalFlip) {
		this->rotate = rotate;
		this->horizontalFlip = horizontalFlip;
	}
};

int main(int argc, char ** argv) {
	
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	if (argc < 2) {
		cout << "Numero di porta: ";
		cin >> SPport;
		cout << endl;
	} else SPport = argv[1];
	
	Communicator comm;
	SPaddress = comm.getIP();
	if (!comm.startListener(SPport)) return 0;
	
	// Inizializzaione del servizio
	RotateService rotate;
	rotate.setServer(SPaddress, SPport);
	
	HorizontalFlipService horizontalFlip;
	horizontalFlip.setServer(SPaddress, SPport);
	
	Infos * otherInfos = new Infos(&rotate, &horizontalFlip);
	
	// Avvio dei thread (forks)
	for (int i = 0; i < NUM_THREADS; i++) {
		threadsInfo[i].setInfos((void *) otherInfos);
		threadsInfo[i].startThread(threadMain);
	}
	
	if (argc != 4) {
		cout << "Indirizzo del Server Register : ";
		cin >> SRaddress;
		cout << "Porta del Server Register     : ";
		cin >> SRport;
		cout << endl;
	} else {
		SRaddress = argv[2];
		SRport = argv[3];
	}
	
	SOA global;
	global.setServerRegister(SRaddress, SRport);
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	//if (!global.serviceRegistration(horizontalFlip)) return 0;
	
	cout << "Connesso all'indirizzo " << SPaddress << ":" << SPport << "\nIn attesa di connessioni...\n\n";
	
	while (1) {
		// In attesa di connessione con i client...
		Socket sk;
		string client;
		comm.waitForConnection(sk, client);
		cout << "Il client " << client << " si è connesso" << flush;
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < NUM_THREADS; i++)
			if (!threadsInfo[i].busy) {
				threadsInfo[i].busy = true;
				threadsInfo[i].client = sk;
				threadsInfo[i].V();
				break;
			}
		pthread_mutex_unlock(&mutex);
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
}

void * threadMain(void * threadID) {
	long tid;
	tid = (long) threadID;
	ThreadInfo * thread = &threadsInfo[tid];
	Infos * otherInfos = (Infos *) thread->otherInfos;
	while (1) {
		bool result;
		string service;
		thread->P();
		if (!thread->client.receiveString(service)) continue;
		cout << " e richiede il servizio \033[1;34m" << service << "\033[0m\n";
		if (!service.compare("rotate")) result = otherInfos->rotate->serveRequests(&thread->client); else
								result = otherInfos->horizontalFlip->serveRequests(&thread->client);
		if (result) cout << "Richiesta servita\n";
		thread->client.closeSocket();
		cout << endl;
		pthread_mutex_lock(&mutex);
		thread->busy = false;
		pthread_mutex_unlock(&mutex);
	}
	thread->~ThreadInfo();
	return NULL;
}