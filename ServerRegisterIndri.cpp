//
//  ServerRegisterIndri.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Application/ServerRegister.cpp"

int main(int argc, char ** argv) {
	
	string port;
	
	if (argc != 2) {
		cout << "Porta del Server Register: ";
		cin >> port;
		cout << endl;
	} else port =  argv[1];
	
	Communicator comm;
	ServerRegister SR(port);
	
	cout << "\033[4mSERVER REGISTER                             " << comm.getIP() << ":" << port << "\033[0m\n\n";
	cout << "In attesa di connessioni...\n\n";
	while (1) SR.serveRequests();
	
}