//
//  ImageStoringServer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 09/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/SOA.hpp"
#include "Application/Threads.hpp"
#include "Application/ImageStoring.hpp"

#define NUM_THREADS 10

void threadMain(ThreadInfo * thread, StoreImageService * storeImage, GetImageService * getImage, GetListService * getList);

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
	
	// Mutex per l'accesso alla cartella di lavoro
	boost::shared_mutex mutex;
	
	// Inizializzazione dei servizi
	StoreImageService storeImage(&mutex);
	GetImageService getImage(&mutex);
	GetListService getList(&mutex);
	
	// Avvio dei thread (forks)
	//ThreadInfo threadsInfo[NUM_THREADS];
	//for (int i = 0; i < NUM_THREADS; i++) boost::thread(threadMain, &threadsInfo[i], &storeImage, &getImage, &getList);
	
	if (argc != 4) {
		cout << "Indirizzo del Service Register : ";
		cin >> SRaddress;
		cout << "Porta del Service Register     : ";
		cin >> SRport;
		cout << endl;
	} else {
		SRaddress = argv[2];
		SRport = argv[3];
	}
	
	SOA global;
	global.setServiceProvider(SPaddress, SPport);
	if (!global.setServiceRegister(SRaddress, SRport)) return 0;
	if (!global.serverRegistration()) return 0;

	string service;
	cin >> service;

	if (!global.serviceRegistration(storeImage)) return 0;
	//if (!global.serviceRegistration(getImage)) return 0;
	//if (!global.serviceRegistration(getList)) return 0;
	
	cout << "\033[4mIMAGE STORING SERVER                        " << SPaddress << ":" << SPport << "\033[0m\n\n";
	cout << "In attesa di connessioni...\n\n";
	
	/*while (1) {
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
	}*/
	
	//sleep(10);
	//if (!global.serverUnRegistration()) return 0;
	//if (!global.serviceUnRegistration(storeImage)) return 0;
	//if (!global.serviceUnRegistration(getImage)) return 0;
	//if (!global.serviceUnRegistration(getList)) return 0;
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
}

void threadMain(ThreadInfo * thread, StoreImageService * storeImage, GetImageService * getImage, GetListService * getList) {
	while (1) {
		bool result;
		string service;
		thread->waitStart();
		if (!thread->client.receiveString(service)) continue;
		cout << " e richiede il servizio \033[1;34m" << service << "\033[0m\n";
		if (!service.compare("store image")) result = storeImage->serveRequest(&thread->client); else
			if (!service.compare("get image")) result = getImage->serveRequest(&thread->client); else
												result = getList->serveRequest(&thread->client);
		if (result) cout << "Richiesta servita\n";
		cout << endl;
		thread->client.closeSocket();
		thread->setFree();
	}
}
