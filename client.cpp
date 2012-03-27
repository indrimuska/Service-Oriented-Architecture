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

int main(int argc, char ** argv) {
	Communicator * client = new Communicator();
	
	string s_string;
	Socket server_socket;
	cout << "connessione a 127.0.0.1:12345\n";
	client->connectTo("127.0.0.1", "12345", server_socket);
	cout << "invio dell'intero 326\n";
	server_socket.sendInt(326);
	cout << "ricezione di una stringa\n";
	server_socket.receiveString(s_string);
	cout << "stringa ricevuta \"" << s_string << "\"\n";
	cout << "chiusura di tutte le comunicazioni\n";
	client->closeAllCommunications();
	cout << "uscita\n";
	return 1;
}