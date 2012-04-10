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
	
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	// Avvio del server
	//cout << "Set port number: ";
	//cin >> SPport;
	SPport = argv[1];
	
	Communicator comm;
	SPaddress = comm.getIP();
	if (!comm.startListener(SPport)) return 0;
	
	// Inizializzaione dei servizi
	StoreImageService storeImage;
	storeImage.setServer(SPaddress, SPport);
	
	GetImageService getImage;
	getImage.setServer(SPaddress, SPport);
	
	GetListService getList;
	getList.setServer(SPaddress, SPport);
	
	// Connessione al Service Register
	//cout << "Insert server register address: ";
	//cin >> SRaddress;
	//cout << "Insert server register port:    ";
	//cin >> SRport;
	
	//SOA global;
	//global.setServerRegister(SRaddress, SRport);
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	
	cout << "In attesa di connessioni (" << SPaddress << ":" << SPport << ")...\n\n";
	
	while (1) {
		// In attesa di connessione con i client...
		Socket sk;
		bool result;
		string add, service;
		comm.waitForConnection(sk, add);
		cout << "si è connesso " << add << endl;
		if (!sk.receiveString(service)) continue;
		if (!service.compare("store image")) result = storeImage.serveRequests(&sk); else
			if (!service.compare("get image")) result = getImage.serveRequests(&sk); else
												result = getList.serveRequests(&sk);
		if (result) cout << "richiesta servita\n";
		sk.closeSocket();
		cout << endl;
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
	
}