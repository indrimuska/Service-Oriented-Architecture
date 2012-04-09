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
#include "Application/ImageManipulation.h"

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
	
	// Inizializzaione del servizio
	RotateService rotate;
	rotate.setServer(SPaddress, SPport);
	
	HorizontalFlipService horizontalFlip;
	horizontalFlip.setServer(SPaddress, SPport);
	
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
		string add, service;
		comm.waitForConnection(sk, add);
		cout << "si è connesso " << add << endl;
		if (!sk.receiveString(service)) continue;
		if (!service.compare("rotate")) {
			if (rotate.serveRequests(&sk)) cout << "richiesta servita\n";
		} else {
			if (horizontalFlip.serveRequests(&sk)) cout << "richiesta servita\n";
		}
		sk.closeSocket();
		cout << endl;
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
	
}