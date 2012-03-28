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
		return true;
	}
	bool registerServer(Socket * sk) {
		return true;
	}
	bool registerService(Socket * sk) {
		return true;
	}
	~ServerRegister() {
		comm.stopListener();
		comm.closeAllCommunications();
	}
};

int main(int argc, char ** argv) {
	string SRport;
	
	cout << " * SERVER REGISTER * \n\n";
	cout << "Set port number: ";
	cin >> SRport;
	
	ServerRegister serverRegister(SRport);
	
	Communicator comm;
	
	Socket * sk;
	while (1) {
		// Attesa di connessioni in corso
		cout << "\nWaiting for connection...\n";
		sk = new Socket();
		if (!comm.waitForConnection(* sk)) return 0;
		
		sk->sendString(ACK_PACKET);
		cout << "Ack send\n";
		
		comm.closeCommunication(* sk);
		cout << "Connection closed\n";
	}
	
	// Chiusura del socket di ascolto
	if (!comm.stopListener()) return 0;
	
	// Chiusura di tutte le connessioni aperte
	if (!comm.closeAllCommunications()) return 0;
	
	return 1;
}