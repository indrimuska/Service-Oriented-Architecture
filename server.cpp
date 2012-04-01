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
#include "SOA/params.cpp"
#include "Application/RotateService.cpp"

using namespace std;

int main(int argc, char ** argv) {
	
	vector<param> received_params;
	
	Socket sk;
	Communicator comm;
	comm.startListener(argv[1]);
	comm.waitForConnection(sk);
	
	//sk.receiveObject(<#void *object#>, <#size_t length#>)
	
	comm.closeAllCommunications();
	
	/*
	
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	// Avvio del server
	//cout << "Set port number: ";
	//cin >> SPport;
	SPport = argv[1];
	Communicator comm;
	comm.startListener(SPport);
	SPaddress = comm.getIP();
	
	// Inizializzaione del servizio
	RotateService rotate;
	rotate.setServer(SPaddress, SPport);
	
	// Connessione al Service Register
	/*cout << "Insert server register address: ";
	cin >> SRaddress;
	cout << "Insert server register port:    ";
	cin >> SRport;*/
	/*
	SOA global;
	/*global.setServerRegister(SRaddress, SRport);
	if (!global.serverRegistration(SPaddress, SPport)) return 0;
	if (!global.serviceRegistration(rotate)) return 0;*/
	/*
	while (1) {
		// In attesa di connessione con i client...
		cout << "\nIn attesa di connessioni...\n";
		Socket * sk = new Socket();
		comm.waitForConnection(* sk);
		rotate.serveRequests(sk);
		sk->closeSocket();
		delete sk;
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();*/
}