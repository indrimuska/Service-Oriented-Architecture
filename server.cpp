//
//  server.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "Application/RotateService.cpp"

using namespace std;

int main(int argc, char ** argv) {
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
	Socket serviceRegister;
	if (!comm.connectTo(SRaddress, SRport, serviceRegister)) return 0;
	
	// Registrazione del servizio al Service Register
	rotate.serviceRegistration(serviceRegister, SPaddress, SPport);
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
}