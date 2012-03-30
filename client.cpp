//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/Communicator.h"

using namespace std;

class Prova1 {
public:
	int value;
	Prova1(){value=1;};
};

int main(int argc, char ** argv) {
	
	Socket server;
	Communicator client;
	client.connectTo("127.0.0.1", argv[1], server);
	
	Prova1 * p1;
	server.receiveObject(p1, sizeof(Prova1));
	cout << "ricevuto: " << p1->value << endl;
	
	client.closeAllCommunications();
}