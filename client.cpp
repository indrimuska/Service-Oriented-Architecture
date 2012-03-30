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
	
	string file;
	server.receiveFile("file_prova", file);
	cout << "ricevuto file: " << file << endl;
	
	/*
	int i;
	Prova1 * p1;
	server.receiveBinary(p1, i);
	cout << "ricevuto " << p1->value << "\n";
	*/
	
	client.closeAllCommunications();
	
	/*
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
	return 1;*/
}