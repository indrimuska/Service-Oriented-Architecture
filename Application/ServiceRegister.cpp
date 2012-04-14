//
//  Application/ServiceRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ServiceRegister.hpp"

ServiceRegister::ServiceRegister(string address, string port) {
	this->address = address;
	this->port = port;
}

bool ServiceRegister::sendAck(Socket * sk, string ack) {
	if (!sk->sendString(ack)) {
		cerr << "Errore nell'invio di un messaggio di ACK\n";
		return false;
	}
	return true;
}
bool ServiceRegister::splitServerName(string serverName, string &address, string &port) {
	address = serverName.substr(0, serverName.find_first_of(":"));
	port    = serverName.substr(serverName.find_first_of(":")+1);
	return address.length() + port.length() != 0;
}
bool ServiceRegister::splitServiceName(string serviceName, string &address, string &port, string &service) {
	address = serviceName.substr(0, serviceName.find_first_of(":"));
	port    = serviceName.substr(serviceName.find_first_of(":")+1, serviceName.find_first_of("/")-serviceName.find_first_of(":")-1);
	service = serviceName.substr(serviceName.find_first_of("/")+1);
	return address.length() + port.length() + service.length() != 0;
}
bool ServiceRegister::isServerRegistred(string serverName) {
	for (int i = 0; i < (int) servers.size(); i++)
		if (!serverName.compare(servers[i]))
			return true;
	return false;
}
bool ServiceRegister::ServiceRegister::isServiceRegistred(string serviceName) {
	return services.find(serviceName) != services.end();
}
void ServiceRegister::ServiceRegister::deleteServerFromServersVector(string service, string serverName) {
	vector<string> serversVector = services[service];
	for (int i = 0; i < (int) serversVector.size(); i++)
		if (!serversVector[i].compare(serverName)) {
			serversVector.erase(serversVector.begin() + i);
			break;
		}
	if (serversVector.empty()) services.erase(service);
	else services[service] = serversVector;
}

bool ServiceRegister::serveRequest(Socket * sk) {
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
bool ServiceRegister::confirmConnection(Socket * sk) {
	if (!sendAck(sk, CONN_ACK_RESP)) return false;
	return true;
}
bool ServiceRegister::serverRegistration(Socket * sk) {
	string serverName;
	cout << "È stata richiesta la registrazione di un server\n";
	if (!sk->receiveString(serverName)) {
		cerr << "Errore nella ricezione del nome del server da registrare\n\n";
		return false;
	}
	if (isServerRegistred(serverName)) {
		cerr << "\033[1;31mIl server richiedente è già registrato\033[0m\n";
		if (!sk->sendString("Server già registrato"))
			cerr << "Errore nell'invio della notifica di server già registrato\n\n";
		cerr << endl;
		return false;
	}
	servers.push_back(serverName);
	if (!sendAck(sk, SRV_REG_RESP)) return false;
	cout << "Il server \033[1;32m" << serverName << "\033[0m si è registrato\n\n";
	return true;
}
bool ServiceRegister::serviceRegistration(Socket * sk) {
	string serviceName;
	cout << "È stata richiesta la registrazione di un servizio\n";
	if (!sk->receiveString(serviceName)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	string address, port, service;
	if (!splitServiceName(serviceName, address, port, service)) {
		cerr << "\033[1;31mIl nome del servizio è invalido\033[0m\n";
		if (!sk->sendString("Nome del servizio invalido"))
			cerr << "Errore nell'invio della notifica di nome del servizio invalido\n";
		cerr << endl;
		return false;
	}
	if (!isServerRegistred(address + ':' + port)) {
		cerr << "\033[1;31mIl server che vuole registrare il servizio non è registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n";
		cerr << endl;
		return false;
	}
	if (isServiceRegistred(service)) {
		cerr << "\033[1;31mIl servizio che si vuole registrare è già registrato\033[0m\n";
		if (!sk->sendString("Servizio già registrato"))
			cerr << "Errore nell'invio della notifica di servizio già registrato\n";
		cerr << endl;
		return false;
	}
	services[service].push_back(address + ':' + port);
	if (!sendAck(sk, SRC_REG_RESP)) return false;
	cout << "Il servizio \033[1;32m" << serviceName << "\033[0m è stato registrato\n\n";
	return true;
}
bool ServiceRegister::serverUnRegistration(Socket * sk) {
	string serverName;
	cout << "È stata richiesta la de-registrazione di un server\n";
	if (!sk->receiveString(serverName)) {
		cerr << "Errore nella ricezione del nome del server da de-registrare\n\n";
		return false;
	}
	if (!isServerRegistred(serverName)) {
		cerr << "\033[1;31mIl server richiedente non è già registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n\n";
		cerr << endl;
		return false;
	}
	// Eliminazione dal vettore dei server
	for (int i = 0; i < (int) servers.size(); i++)
		if (!servers[i].compare(serverName)) {
			servers.erase(servers.begin() + i);
			break;
		}
	// Eliminazione dalla mappa dei servizi
	map<string, vector<string> >::iterator serversVector;
	for (serversVector = services.begin(); serversVector != services.end(); serversVector++)
		deleteServerFromServersVector(serversVector->first, serverName);
	if (!sendAck(sk, SRV_UNREG_RESP)) return false;
	cout << "Il server \033[1;32m" << serverName << "\033[0m si è de-registrato\n\n";
	return true;
}
bool ServiceRegister::serviceUnRegistration(Socket * sk) {
	string serviceName;
	cout << "È stata richiesta la de-registrazione di un servizio\n";
	if (!sk->receiveString(serviceName)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	string address, port, service;
	if (!splitServiceName(serviceName, address, port, service)) {
		cerr << "\033[1;31mIl nome del servizio è invalido\033[0m\n";
		if (!sk->sendString("Nome del servizio invalido"))
			cerr << "Errore nell'invio della notifica di nome del servizio invalido\n";
		cerr << endl;
		return false;
	}
	if (!isServerRegistred(address + ':' + port)) {
		cerr << "\033[1;31mIl server che vuole de-registrare il servizio non è registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n";
		cerr << endl;
		return false;
	}
	if (!isServiceRegistred(service)) {
		cerr << "\033[1;31mIl servizio che si vuole de-registrare non è registrato\033[0m\n";
		if (!sk->sendString("Servizio non registrato"))
			cerr << "Errore nell'invio della notifica di servizio non registrato\n";
		cerr << endl;
		return false;
	}
	// Eliminazione dalla mappa dei servizi del server
	deleteServerFromServersVector(service, address + ':' + port);
	if (!sendAck(sk, SRC_UNREG_RESP)) return false;
	cout << "Il servizio \033[1;32m" << serviceName << "\033[0m è stato de-registrato\n\n";
	return true;
}
bool ServiceRegister::serversDisplay(Socket * sk) {
	return true;
}
bool ServiceRegister::servicesDisplay(Socket * sk) {
	return true;
}
bool ServiceRegister::serverRequest(Socket * sk) {
	string service;
	if (!sk->receiveString(service)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	map<string, vector<string> >::iterator serversVector = services.find(service);
	cout << "È stato richiesto un server che fornisca il servizio \033[1;34m" << service << "\033[0m\n";
	if (serversVector == services.end()) {
		cout << "\033[1;31mNon si è registrato alcune server che fornisca il servizio richiesto\033[0m\n";
		if (!sk->sendString("Non si è registrato alcun server che fornisca il servizio richiesto")) {
			cerr << "Errore nell'invio della notifica di assenza di server registrato\n\n";
			return false;
		}
		cout << endl;
		return true;
	}
	string server[2];
	// Qui bisogna cercare un server che supporta il servizio 'service'
	// e scegliero tra un insieme di server in base alla politica implementata (es. Round Robin)
	// Io per semplicità scelgo sempre il primo (di fatto escludendo gli altri, infatti non va bene)
	splitServerName(serversVector->second[0], server[0], server[1]);
	
	if (!sendAck(sk, SRV_RESP)) return false;
	cout << "Il server scelto è \033[1;32m" << server[0] << ":" << server[1] << "\033[0m\n";
	if (!sk->sendString(server[0]) || !sk->sendString(server[1])) {
		cerr << "Errore nell'invio dell'indirizzo o della porta del server\n\n";
		return false;
	}
	cout << endl;
	return true;
}