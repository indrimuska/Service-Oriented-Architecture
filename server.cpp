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
#include "Application/RotateService.cpp"

using namespace std;

class Prova1 {
	public:
	int value;
	Prova1(){value=1;};
};

class Prova2 {
	int value;
	Prova2(){value=2;};
};

int main(int argc, char ** argv) {
	
	Socket client;
	Communicator server;
	server.startListener(argv[1]);
	cout << "aspetto il client" << endl;
	server.waitForConnection(client);
	
	client.sendFile(argv[2]);
	cout << "file inviato\n";
	
	/*
	vector<packet_field> packet;
	Prova1 p1;
	client.sendBinary((void *) &p1, sizeof(Prova1));
	cout << "inviato\n";
	*/
	
	server.closeAllCommunications();
	
	/*
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	// Avvio del server
	cout << "Set port number: ";
	cin >> SPport;
	Communicator comm;
	comm.startListener(SPport);
	SPaddress = comm.getIP();
	
	// Inizializzaione del servizio
	RotateService rotate;
	
	// Connessione al Service Register
	cout << "Insert server register address: ";
	cin >> SRaddress;
	cout << "Insert server register port:    ";
	cin >> SRport;
	
	SOA global;
	global.setServerRegister(SRaddress, SRport);
	if (!global.serviceRegistration(rotate)) return 0;
	
	while(1);
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();*/
}