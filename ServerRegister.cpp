//
//  ServerRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include <map>
#include <stdio.h>
#include <string.h>
#include "SOA/SOA.h"
#include "SOA/Communicator.h"

using namespace std;

class ServerRegister {
private:
	Communicator comm;
	string SRaddress;
	string SRport;
	map<string, ServerInformation> SRservers;
	map<string, ServerInformation>::iterator serversIt;
	map<string, ServiceInformation> SRservices;
	map<string, ServiceInformation>::iterator servicesIt;


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
		string serverToReg; //serverToReg is a string that must have the format "address:port"
		sk->receiveString(serverToReg);
		serversIt = SRservers.find(serverToReg);
		if (serversIt == SRservers.end()){

			string address, port;
			sk -> receiveString(address);
			sk -> receiveString(port);
			ServerInformation serInf = ServerInformation(address, port);
			SRservers.insert(pair<string, ServerInformation> (serverToReg, serInf));
			return true;
		}

		cout << "Questo server è già registrato";
		return false;
	}
	bool registerService(Socket * sk) {
		string serviceToReg;
		sk->receiveString(serviceToReg);

		servicesIt = SRservices.find(serviceToReg);
		if (servicesIt == SRservices.end()){
			ServiceInformation si = ServiceInformation(serviceToReg); //qui creo una nuova ServiceInformation per un particolare tipo di servizio
		}
		return true;
	}
	bool unregisterService(Socket * sk){
		return true;
	}

    bool provideServiceInformation(Socket * sk){
    	string serviceName;
		cin >> serviceName;
		servicesIt = SRservices.find(serviceName);
		ServiceInformation si = servicesIt;
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
	
	serverRegister.~ServerRegister();
	
	return 1;
}
