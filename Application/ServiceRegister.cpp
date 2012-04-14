//
//  Application/ServiceRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "../SOA/SOA.hpp"
#include "../SOA/Communication.hpp"

using namespace std;

class ServiceRegister {
private:
	string address;
	string port;
	vector<string> serviceProviders;
	
	bool sendAck(Socket * sk, string ack) {
		if (!sk->sendString(ack)) {
			cerr << "Errore nell'invio di un messaggio di ACK\n";
			return false;
		}
		return true;
	}
	bool isServerRegistred(string serverName) {
		for (int i = 0; i < (int) serviceProviders.size(); i++)
			if (!serverName.compare(serviceProviders[i]))
				return true;
		return false;
	}
	bool splitServerName(string serverName, string &address, string &port) {
		address = serverName.substr(0, serverName.find_first_of(":"));
		port    = serverName.substr(serverName.find_first_of(":")+1);
		return address.length() + port.length() != 0;
	}
	bool splitServiceName(string serviceName, string &address, string &port, string &service) {
		address = serviceName.substr(0, serviceName.find_first_of(":"));
		port    = serviceName.substr(serviceName.find_first_of(":")+1, serviceName.find_first_of("/")-serviceName.find_first_of(":")-1);
		service = serviceName.substr(serviceName.find_first_of("/")+1);
		return address.length() + port.length() + service.length() != 0;
	}
public:
	ServiceRegister(string address, string port) {
		this->address = address;
		this->port = port;
	}
	bool serveRequest(Socket * sk) {
		string request;
		if (!sk->receiveString(request)) return false;
		if (!request.compare(CONN_ACK_REQ)) return confirmConnection(sk);
		if (!request.compare(SRV_REG_REQ)) return serverRegistration(sk);
		if (!request.compare(SRC_REG_REQ)) return serviceRegistration(sk);
		if (!request.compare(SRV_UNREG_REQ)) return serverUnRegistration(sk);
		if (!request.compare(SRC_UNREG_REQ)) return serviceUnRegistration(sk);
		if (!request.compare(SRV_REG_DISP)) return serversDisplay(sk);
		if (!request.compare(SRC_REG_DISP)) return servicesDisplay(sk);
		if (!request.compare(SRV_REQ)) return serverRequest(sk);
		cerr << "\033[1;31mRichiesta sconosciuta (" << request << ")\033[0m\n\n";
		return false;
	}
	bool confirmConnection(Socket * sk) {
		if (!sendAck(sk, CONN_ACK_RESP)) return false;
		return true;
	}
	bool serverRegistration(Socket * sk) {
		string serverName;
		cout << "È stata richiesta la registrazione di un server\n";
		if (!sk->receiveString(serverName)) {
			cerr << "Errore nella ricezione del nome del server da registrare\n\n";
			return false;
		}
		if (isServerRegistred(serverName)) {
			cout << "\033[1;31mIl server richiedente è già registrato\033[0m\n";
			if (!sk->sendString("Server già registrato")) {
				cerr << "Errore nell'invio della notifica di server già registrato\n\n";
				return false;
			}
			cout << endl;
			return true;
		}
		serviceProviders.push_back(serverName);
		if (!sendAck(sk, SRV_REG_RESP)) return false;
		cout << "Il server \033[1;32m" << serverName << "\033[0m si è registrato\n\n";
		return true;
	}
	bool serviceRegistration(Socket * sk) {
		string serviceName;
		cout << "È stata richiesta la registrazione di un servizio\n";
		if (!sk->receiveString(serviceName)) {
			cerr << "Errore nella ricezione del nome del servizio\n\n";
			return false;
		}
		string address, port, service;
		if (!splitServiceName(serviceName, address, port, service)) {
			cerr << "Il nome del servizio è invalido\n";
			if (!sk->sendString("Nome del servizio invalido"))
				cerr << "Errore nell'invio della notifica di nome del servizio invalido\n";
			cerr << endl;
			return false;
		}
		if (!isServerRegistred(address + ':' + port)) {
			cerr << "Il server che vuole registrare il servizio non è registrato\n";
			if (!sk->sendString("Server non registrato"))
				cerr << "Errore nell'invio della notifica di server non registrato\n";
			cerr << endl;
			return false;
		}
		// Qui bisogna salvare il servizio
		// Prima però bisogna controllare che il servizio non sia già registrato
		if (!sendAck(sk, SRC_REG_RESP)) return false;
		cout << "Il servizio \033[1;32m" << serviceName << "\033[0m è stato registrato\n\n";
		return true;
	}
	bool serverUnRegistration(Socket * sk) { return true; }
	bool serviceUnRegistration(Socket * sk) { return true; }
	bool serversDisplay(Socket * sk) { return true; }
	bool servicesDisplay(Socket * sk) { return true; }
	bool serverRequest(Socket * sk) {
		string service;
		if (!sk->receiveString(service)) {
			cerr << "Errore nella ricezione del nome del servizio\n\n";
			return false;
		}
		cout << "È stato richiesto un server che fornisca il servizio \033[1;34m" << service << "\033[0m\n";
		// Qui bisogna cercare un server che supporta il servizio 'service'
		// e scegliero tra un insieme di server in base alla politica implementata (es. Round Robin)
		// Non mi ricordo come hai implementato il service register, per far prima io uso un vettore di stringhe
		vector<string> servers;
		servers.push_back("127.0.0.1:2222/rotate");
		servers.push_back("127.0.0.1:2222/horizontal flip");
		servers.push_back("127.0.0.1:22221/store image");
		servers.push_back("127.0.0.1:22221/get image");
		servers.push_back("127.0.0.1:22221/get list");
		string server[2], tempAddress, tempPort, tempService;
		for (int i = 0; i < (int) servers.size(); i++) {
			splitServiceName(servers[i], tempAddress, tempPort, tempService);
			if (!tempService.compare(service)) {
				server[0] = tempAddress;
				server[1] = tempPort;
				break;
			}
		}
		// Se non c'è nessun server che supporta quel servizio allora bisogna mandare questo messaggio:
		if (!server[0].size() || !server[1].size()) {
			cout << "\033[1;31mNon si è registrato alcune server che fornisca il servizio richiesto\033[0m\n";
			if (!sk->sendString("Non si è registrato alcun server che fornisca il servizio richiesto")) {
				cerr << "Errore nell'invio della notifica di assenza di server registrato\n\n";
				return false;
			}
			cout << endl;
			return true;
		}
		// Altrimenti bisogna mandare questo ACK:
		if (!sendAck(sk, SRV_RESP)) return false;
		cout << "Il server scelto è \033[1;32m" << server[0] << ":" << server[1] << "\033[0m\n";
		if (!sk->sendString(server[0]) || !sk->sendString(server[1])) {
			cerr << "Errore nell'invio dell'indirizzo o della porta del server\n\n";
			return false;
		}
		cout << endl;
		return true;
	}
};
