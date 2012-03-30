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

using namespace std;

int main(int argc, char ** argv) {

	Socket serverRegister;
	Communicator server;
	server.startListener(argv[1]);
	cout << "aspetto il client" << endl;
	server.waitForConnection(serverRegister);



	server.closeAllCommunications();
}
