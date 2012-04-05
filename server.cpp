//
//  server.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "SOA/SOA.h"
#include "SOA/Parameters.h"
#include "Application/RotateService.cpp"

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
	
	// Connessione al Service Register
	//cout << "Insert server register address: ";
	//cin >> SRaddress;
	//cout << "Insert server register port:    ";
	//cin >> SRport;
	
	//SOA global;
	//global.setServerRegister(SRaddress, SRport);
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	
	while (1) {
		// In attesa di connessione con i client...
		cout << "In attesa di connessioni ("<<SPaddress<<":"<<SPport<<")...\n";
		Socket sk;
		comm.waitForConnection(sk);
		rotate.serveRequests(&sk);
		sk.closeSocket();
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
	
}