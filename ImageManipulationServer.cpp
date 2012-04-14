//
//  ImageManipulationServer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/SOA.hpp"
#include "Application/Threads.hpp"
#include "Application/ImageManipulation.hpp"

#define NUM_THREADS 10

void threadMain(ThreadInfo * thread, RotateService * rotate, HorizontalFlipService * horizontalFlip);

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
	
	// Inizializzaione dei servizi
	RotateService rotate;
	HorizontalFlipService horizontalFlip;
	
	// Avvio dei thread (forks)
	ThreadInfo threadsInfo[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) boost::thread(threadMain, &threadsInfo[i], &rotate, &horizontalFlip);
	
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
	global.setServiceProvider(SPaddress, SPport);
	if (!global.setServerRegister(SRaddress, SRport)) return 0;
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	//if (!global.serviceRegistration(horizontalFlip)) return 0;
	
	cout << "\033[4mIMAGE MANIPULATION SERVER                   " << SPaddress << ":" << SPport << "\033[0m\n\n";
	cout << "In attesa di connessioni...\n\n";
	
	while (1) {
		Socket sk;
		string client;
		comm.waitForConnection(sk, client);
		cout << "Il client " << client << " si è connesso" << flush;
		for (int i = 0; i < NUM_THREADS; i++)
			if (threadsInfo[i].testAndSet()) {
				threadsInfo[i].client = sk;
				threadsInfo[i].startThread();
				break;
			}
	}
	
	//if (!global.serverUnRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	//if (!global.serviceRegistration(horizontalFlip)) return 0;
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
}

void threadMain(ThreadInfo * thread, RotateService * rotate, HorizontalFlipService * horizontalFlip) {
	while (1) {
		bool result;
		string service;
		thread->waitStart();
		if (!thread->client.receiveString(service)) continue;
		cout << " e richiede il servizio \033[1;34m" << service << "\033[0m\n";
		if (!service.compare("rotate")) result = rotate->serveRequest(&thread->client); else
								result = horizontalFlip->serveRequest(&thread->client);
		if (result) cout << "Richiesta servita\n";
		cout << endl;
		thread->client.closeSocket();
		thread->setFree();
	}
}