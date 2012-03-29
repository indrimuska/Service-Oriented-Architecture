//
//  ServerRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/SOA.h"
#include "SOA/Communicator.h"

using namespace std;

class ServerRegister {
private:
	Communicator comm;
	string SRaddress;
	string SRport;
	vector<Service> services;
	
public:
	ServerRegister(string SRport) {
		this->SRaddress = comm.getIP();
		this->SRport = SRport;
		comm.startListener(SRport);
	}
	bool waitForConnection() {
		Socket * sk = new Socket();
		string request;
		comm.waitForConnection(* sk);
		sk->receiveString(request);
		if (!request.compare(CONN_ACK_REQ)) return confirmConnection(sk);
		if (!request.compare(SRV_REG_REQ)) return registerServer(sk);
		if (!request.compare(SRC_REG_REQ)) return registerService(sk);
		return true;
	}
	bool confirmConnection(Socket * sk) {
		if (!sk->sendString(CONN_ACK_RESP)) {
			cerr << "Impossibile confermare la connessione\n";
			return false;
		}
		delete sk;
		return true;
	}
	bool registerServer(Socket * sk) {
		return true;
	}
	bool registerService(Socket * sk) {
		return true;
	}
	bool unregisterService(Socket * sk){
		return true;
	}
    Service provideServiceInformation(Socket * sk){
    	return true;
    }


	~ServerRegister() {
		// Chiusura del socket di ascolto
		comm.stopListener();
		// Chiusura di tutte le connessioni aperte
		comm.closeAllCommunications();
	}
};

int main(int argc, char ** argv) {
	string SRport;
	
	cout << " * SERVER REGISTER * \n\n";
	cout << "Set port number: ";
	cin >> SRport;
	
	ServerRegister serverRegister(SRport);
	
	while (1) {
		// Attesa di connessioni in corso
		cout << "\nWaiting for connection...\n";
		if (!serverRegister.waitForConnection()) return 0;
		
		cout << "Connection closed\n";
	}
	
	~serverRegister();
	
	return 1;
}
