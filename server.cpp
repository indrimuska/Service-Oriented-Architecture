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
	
	Prova1 * p1 = new Prova1();
	p1->value = 3;
	client.sendObject((void *) p1, sizeof(Prova1));
	cout << "inviato: " << p1->value << endl;
	
	//Prova1 ppp1;
	//client.sendObject((void *) &ppp1, sizeof(ppp1));
	
	server.closeAllCommunications();
}