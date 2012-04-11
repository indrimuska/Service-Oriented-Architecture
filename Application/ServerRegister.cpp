//
//  ServerRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "../SOA/SOA.h"
#include "../SOA/Communication.h"

using namespace std;

class ServerRegister {
private:
	string address;
	string port;
	Communicator comm;
	
	bool sendAck(Socket * sk, string ack) {
		if (!sk->sendString(ack)) {
			cerr << "Errore nell'invio di un messaggio di ACK\n";
			return false;
		}
		return true;
	}
public:
	ServerRegister(string port) {
		this->address = comm.getIP();
		this->port = port;
		comm.startListener(port);
	}
	bool serveRequests() {
		string request;
		Socket * sk = new Socket();
		if (!comm.waitForConnection(* sk)) return false;
		if (!sk->receiveString(request)) return false;
		if (!request.compare(CONN_ACK_REQ)) return confirmConnection(sk);
		if (!request.compare(SRV_REG_REQ)) return serverRegistration(sk);
		if (!request.compare(SRC_REG_REQ)) return serviceRegistration(sk);
		if (!request.compare(SRV_UNREG_REQ)) return serverUnRegistration(sk);
		if (!request.compare(SRC_UNREG_REQ)) return serviceUnRegistration(sk);
		if (!request.compare(SRV_REG_DISP)) return serversDisplay(sk);
		if (!request.compare(SRC_REG_DISP)) return servicesDisplay(sk);
		if (!request.compare(SRV_REQ)) return serverRequest(sk);
		cerr << "Richiesta sconosciuta\n\n";
		return false;
	}
	bool confirmConnection(Socket * sk) {
		if (!sendAck(sk, CONN_ACK_RESP)) return false;
		delete sk;
		return true;
	}
	bool serverRegistration(Socket * sk) { return true; }
	bool serviceRegistration(Socket * sk) { return true; }
	bool serverUnRegistration(Socket * sk) { return true; }
	bool serviceUnRegistration(Socket * sk) { return true; }
	bool serversDisplay(Socket * sk) { return true; }
	bool servicesDisplay(Socket * sk) { return true; }
	bool serverRequest(Socket * sk) {
		string service;
		if (!sk->receiveString(service)) {
			cerr << "Errore nella ricezione del nome del servizio\n";
			return false;
		}
		cout << "È stato richiesto un server che fornisca il servizio \033[1;34m" << service << "\033[0m\n";
		// Qui bisogna cercare un server che supporta il servizio 'service'
		// e scegliero in base alla politica implementata (es. Round Robin)
		// Non mi ricordo come hai implementato il server register, per far prima io uso un vettore di stringhe
		vector<string> servers;
		servers.push_back("127.0.0.1:2222/rotate");
		servers.push_back("127.0.0.1:2222/horizontal flip");
		servers.push_back("127.0.0.1:22221/store image");
		servers.push_back("127.0.0.1:22221/get image");
		servers.push_back("127.0.0.1:22221/get list");
		string server[2];
		for (int i = 0; i < (int) servers.size(); i++)
			if (!servers[i].substr(servers[i].find_first_of("/")+1).compare(service)) {
				server[0] = servers[i].substr(0, servers[i].find_first_of(":"));
				server[1] = servers[i].substr(servers[i].find_first_of(":")+1, servers[i].find_first_of("/")-servers[i].find_first_of(":")-1);
				break;
			}
		// Se non c'è nessun server che supporta quel servizio allora bisogna mandare questo messaggio:
		if (!server[0].size() || !server[1].size()) {
			cout << "\033[1;31mNon si è registrato alcune server che fornisca il servizio richiesto\033[0m\n\n";
			if (!sk->sendString("Non si è registrato alcun server che fornisca il servizio richiesto")) {
				cerr << "Errore nell'invio della notifica di assenza di server registrato\n";
				return false;
			}
			return true;
		}
		// Altrimenti bisogna mandare questo ACK:
		if (!sendAck(sk, SRV_RESP)) return false;
		cout << "Il server scelto è \033[1;32m" << server[0] << ":" << server[1] << "\033[0m\n\n";
		if (!sk->sendString(server[0]) || !sk->sendString(server[1])) {
			cerr << "Errore nell'invio dell'indirizzo o della porta del server\n";
			return false;
		}
		delete sk;
		return true;
	}
	~ServerRegister() {
		comm.closeAllCommunications();
	}
};