//
//  server.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "SOA/Service.h"



#include <sys/unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

int main(int argc, char ** argv) {
	
	// Parametri della funzione ROTATE
	vector<param> parameters;
	parameters.push_back(param(IN, BUFFER));
	parameters.push_back(param(OUT, BUFFER));
	
	// Inizializzaione della funzione
	Service rotate("rotate", parameters);
	
	char hostname[128];
	gethostname(hostname, sizeof hostname);
	
	return 1;
	
	// Connessione al Service Register
	Socket serviceRegister;
	Communicator comm;
	comm.connectTo("127.0.0.1", "12345", serviceRegister);
	
	// Registrazione del servizio al Service Register
	rotate.serviceRegistration(serviceRegister, "myAddress", "myPort");
}