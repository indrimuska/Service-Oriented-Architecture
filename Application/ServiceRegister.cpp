/**
 * @file	Application/ServiceRegister.cpp
 * @brief	Definizione dei metodi dichiarati in Application/ServiceRegister.hpp
 *
 * @date	11/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#include "ServiceRegister.hpp"

ServiceRegister::ServerInfo::ServerInfo(string serverName) {
	address = serverName.substr(0, serverName.find_first_of(":"));
	port    = serverName.substr(serverName.find_first_of(":")+1);
}
bool ServiceRegister::ServerInfo::isValid() {
	return address.length() + port.length() != 0;
}
string ServiceRegister::ServerInfo::getAddress() {
	return address;
}
string ServiceRegister::ServerInfo::getPort() {
	return port;
}
string ServiceRegister::ServerInfo::getServerName() {
	return address + ':' + port;
}

ServiceRegister::ServiceInfo::ServiceInfo(string serviceName) {
	address = serviceName.substr(0, serviceName.find_first_of(":"));
	port    = serviceName.substr(serviceName.find_first_of(":")+1, serviceName.find_first_of("/")-serviceName.find_first_of(":")-1);
	service = serviceName.substr(serviceName.find_first_of("/")+1);
}
bool ServiceRegister::ServiceInfo::isValid() {
	return address.length() + port.length() + service.length() != 0;
}
string ServiceRegister::ServiceInfo::getAddress() {
	return address;
}
string ServiceRegister::ServiceInfo::getPort() {
	return port;
}
string ServiceRegister::ServiceInfo::getService() {
	return service;
}
string ServiceRegister::ServiceInfo::getServerName() {
	return address + ':' + port;
}
string ServiceRegister::ServiceInfo::getServiceName() {
	return address + ':' + port + '/' + service;
}

ServiceRegister::ServiceRegister(string address, string port) {
	this->address = address;
	this->port = port;
}
bool ServiceRegister::serveRequest(Socket * sk) {
	string request;
	if (!sk->receiveString(request))     return false;
	if (!request.compare(CONN_ACK_REQ))  return confirmConnection(sk);
	if (!request.compare(SRV_REG_REQ))   return serverRegistration(sk);
	if (!request.compare(SRC_REG_REQ))   return serviceRegistration(sk);
	if (!request.compare(SRV_UNREG_REQ)) return serverUnRegistration(sk);
	if (!request.compare(SRC_UNREG_REQ)) return serviceUnRegistration(sk);
	if (!request.compare(SRV_DISP_REQ))  return serversDisplay(sk);
	if (!request.compare(SRC_DISP_REQ))  return servicesDisplay(sk);
	if (!request.compare(SRV_REQ))       return serverRequest(sk);
	cerr << "\033[1;31mRichiesta sconosciuta (" << request << ")\033[0m\n\n";
	return false;
}

bool ServiceRegister::sendAck(Socket * sk, string ack) {
	if (!sk->sendString(ack)) {
		cerr << "Errore nell'invio di un messaggio di ACK\n";
		return false;
	}
	return true;
}
bool ServiceRegister::isServerRegistered(string serverName) {
	for (int i = 0; i < (int) servers.size(); i++)
		if (!serverName.compare(servers[i].getServerName()))
			return true;
	return false;
}
bool ServiceRegister::isServiceRegistered(string serviceName) {
	return services.find(serviceName) != services.end();
}
void ServiceRegister::deleteServerFromServersList(string service, string serverName) {
	list<ServiceInfo> serversList = services[service];
	for (list<ServiceInfo>::iterator i = serversList.begin(); i != serversList.end(); i++)
		if (!i->getServerName().compare(serverName)) {
			serversList.erase(i);
			break;
		}
	if (serversList.empty()) services.erase(service);
	else services[service] = serversList;
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
	ServerInfo server(serverName);
	if (!server.isValid()) {
		cerr << "\033[1;31mIl nome del server è invalido\033[0m\n";
		if (!sk->sendString("Nome del server invalido"))
			cerr << "Errore nell'invio della notifica di nome del server invalido\n";
		cerr << endl;
		return false;
	}
	if (isServerRegistered(serverName)) {
		cerr << "\033[1;31mIl server richiedente è già registrato\033[0m\n";
		if (!sk->sendString("Server già registrato"))
			cerr << "Errore nell'invio della notifica di server già registrato\n\n";
		cerr << endl;
		return false;
	}
	servers.push_back(serverName);
	if (!sendAck(sk, SRV_REG_RESP)) return false;
	printf("Il server \033[1;32m%s\033[0m si è registrato\n\n", serverName.c_str());
	return true;
}
bool ServiceRegister::serviceRegistration(Socket * sk) {
	string serviceName;
	cout << "È stata richiesta la registrazione di un servizio\n";
	if (!sk->receiveString(serviceName)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	ServiceInfo service(serviceName);
	if (!service.isValid()) {
		cerr << "\033[1;31mIl nome del servizio è invalido\033[0m\n";
		if (!sk->sendString("Nome del servizio invalido"))
			cerr << "Errore nell'invio della notifica di nome del servizio invalido\n";
		cerr << endl;
		return false;
	}
	if (!isServerRegistered(service.getServerName())) {
		cerr << "\033[1;31mIl server che vuole registrare il servizio non è registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n";
		cerr << endl;
		return false;
	}
	if (isServiceRegistered(service.getService())) {
		cerr << "\033[1;31mIl servizio che si vuole registrare è già registrato\033[0m\n";
		if (!sk->sendString("Servizio già registrato"))
			cerr << "Errore nell'invio della notifica di servizio già registrato\n";
		cerr << endl;
		return false;
	}
	services[service.getService()].push_back(service);
	if (!sendAck(sk, SRC_REG_RESP)) return false;
	printf("Il servizio \033[1;32m%s\033[0m è stato registrato\n\n", serviceName.c_str());
	return true;
}
bool ServiceRegister::serverUnRegistration(Socket * sk) {
	string serverName;
	cout << "È stata richiesta la de-registrazione di un server\n";
	if (!sk->receiveString(serverName)) {
		cerr << "Errore nella ricezione del nome del server da de-registrare\n\n";
		return false;
	}
	if (!isServerRegistered(serverName)) {
		cerr << "\033[1;31mIl server richiedente non è già registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n\n";
		cerr << endl;
		return false;
	}
	// Eliminazione dal vettore dei server
	for (int i = 0; i < (int) servers.size(); i++)
		if (!servers[i].getServerName().compare(serverName)) {
			servers.erase(servers.begin() + i);
			break;
		}
	// Eliminazione dalla mappa dei servizi
	map<string, list<ServiceInfo> > servicesCopy = services;
	for (map<string, list<ServiceInfo> >::iterator i = servicesCopy.begin(); i != servicesCopy.end(); i++)
		deleteServerFromServersList(i->first, serverName);
	if (!sendAck(sk, SRV_UNREG_RESP)) return false;
	printf("Il server \033[1;32m%s\033[0m si è de-registrato\n\n", serverName.c_str());
	return true;
}
bool ServiceRegister::serviceUnRegistration(Socket * sk) {
	string serviceName;
	cout << "È stata richiesta la de-registrazione di un servizio\n";
	if (!sk->receiveString(serviceName)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	ServiceInfo service(serviceName);
	if (!service.isValid()) {
		cerr << "\033[1;31mIl nome del servizio è invalido\033[0m\n";
		if (!sk->sendString("Nome del servizio invalido"))
			cerr << "Errore nell'invio della notifica di nome del servizio invalido\n";
		cerr << endl;
		return false;
	}
	if (!isServerRegistered(service.getServerName())) {
		cerr << "\033[1;31mIl server che vuole de-registrare il servizio non è registrato\033[0m\n";
		if (!sk->sendString("Server non registrato"))
			cerr << "Errore nell'invio della notifica di server non registrato\n";
		cerr << endl;
		return false;
	}
	if (!isServiceRegistered(service.getService())) {
		cerr << "\033[1;31mIl servizio che si vuole de-registrare non è registrato\033[0m\n";
		if (!sk->sendString("Servizio non registrato"))
			cerr << "Errore nell'invio della notifica di servizio non registrato\n";
		cerr << endl;
		return false;
	}
	// Eliminazione dalla mappa dei servizi del server
	deleteServerFromServersList(service.getService(), service.getServerName());
	if (!sendAck(sk, SRC_UNREG_RESP)) return false;
	cout << "Il servizio \033[1;32m" << serviceName << "\033[0m è stato de-registrato\n\n";
	return true;
}
bool ServiceRegister::serverRequest(Socket * sk) {
	string service;
	if (!sk->receiveString(service)) {
		cerr << "Errore nella ricezione del nome del servizio\n\n";
		return false;
	}
	printf("È stato richiesto un server che fornisca il servizio \033[1;34m%s\033[0m\n", service.c_str());
	if (!isServiceRegistered(service)) {
		cerr << "\033[1;31mIl servizio non è registrato\033[0m\n";
		if (!sk->sendString("Servizio non registrato"))
			cerr << "Errore nell'invio della notifica di servizio non registrato\n";
		cerr << endl;
		return true;
	}
	// Estrazione del primo elemento della lista e reinserimento in coda
	list<ServiceInfo> serversList = services[service];
	ServiceInfo server(serversList.begin()->getServerName());
	serversList.pop_front();
	serversList.push_back(server);
	services[service] = serversList;
	// Invio dell'ACK e dell'indirizzo e porta del server scelto
	if (!sendAck(sk, SRV_RESP)) return false;
	printf("Il server scelto è \033[1;32m%s\033[0m\n", server.getServerName().c_str());
	if (!sk->sendString(server.getAddress()) || !sk->sendString(server.getPort())) {
		cerr << "Errore nell'invio dell'indirizzo o della porta del server\n\n";
		return false;
	}
	cout << endl;
	return true;
}