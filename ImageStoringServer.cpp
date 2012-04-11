//
//  ImageStoringServer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 09/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "SOA/SOA.h"
#include "Application/ImageStoring.h"

using namespace std;

int main(int argc, char ** argv) {
	
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
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
	StoreImageService storeImage(&mutex);
	storeImage.setServer(SPaddress, SPport);
	
	GetImageService getImage(&mutex);
	getImage.setServer(SPaddress, SPport);
	
	GetListService getList(&mutex);
	getList.setServer(SPaddress, SPport);
	
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
	
	//SOA global;
	//global.setServerRegister(SRaddress, SRport);
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	
	cout << "Connesso all'indirizzo " << SPaddress << ":" << SPport << "\nIn attesa di connessioni...\n\n";
	
	while (1) {
		// In attesa di connessione con i client...
		Socket sk;
		bool result;
		string client, service;
		comm.waitForConnection(sk, client);
		if (!sk.receiveString(service)) continue;
		cout << "Il client " << client << " si è connesso e richiede il servizio \033[1;34m" << service << "\033[0m\n";
		if (!service.compare("store image")) result = storeImage.serveRequests(&sk); else
			if (!service.compare("get image")) result = getImage.serveRequests(&sk); else
												result = getList.serveRequests(&sk);
		if (result) cout << "Richiesta servita\n";
		sk.closeSocket();
		cout << endl;
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
	
}