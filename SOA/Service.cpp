//
//  SOA/Service.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Service.h"

void Service::setService(string name, vector<parameter> parameters) {
	this->name = name;
	for (int i = 0; i < (int) parameters.size(); i++) {
		if (parameters[i].getDirection() == IN) inParameters.push_back(parameters[i]);
		else outParameters.push_back(parameters[i]);
	}
}
void Service::setServer(string SPaddress, string SPport) {
	this->SPaddress = SPaddress;
	this->SPport = SPport;
}
bool Service::serviceRegistration(Socket SRsocket) {
	if (!SRsocket.sendString(SPaddress + ':' + SPport + '/' + name)) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
}
bool Service::serviceUnRegistration(Socket SRsocket) {
	if (!SRsocket.sendString(SPaddress + ':' + SPport + '/' + name)) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
}
bool Service::requestService() {
	Communicator comm;
	Socket serviceProvider;
	if (!comm.connectTo(SPaddress, SPport, serviceProvider)) return false;
	cout << "Invio la richiesta di servizio\n";
	if (!serviceProvider.sendString(SERVICE_REQ)) {
		cerr << "Errore nella richiesta di servizio\n";
		return false;
	}
	cout << "Invio il nome del servizio\n";
	if (!serviceProvider.sendString(name)) {
		cerr << "Errore nell'invio del nome del servizio richiesto\n";
		return false;
	}
	cout << "Invio del numero di parametri\n";
	if (!serviceProvider.sendInt((int) inParameters.capacity())) {
		cerr << "Errore nell'invio del numero di parametri del servizio\n";
		return false;
	}
	cout << "Ricevo la conferma parziale\n";
	string ack;
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "Errore nella conferma di accettazione parziale del servizio\n" << ack << endl;
		return false;
	}
	cout << "Invio i parametri del servizio\n";
	for (int i = 0; i < (int) inParameters.size(); i++) {
		Serializer s(inParameters[i]);
		if (!serviceProvider.sendObject(s)) {
			cerr << "Errore durante l'invio di uno dei parametri del servizio\n";
			return false;
		}
	}
	cout << "Ricevo la conferma\n";
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "La richiesta di servizio non è stata accettata\n" << ack << endl;
		return false;
	}
	serviceProvider.closeSocket();
	return true;
}
bool Service::serveRequests(Socket * sk) {
	int parameters_size;
	string request, name;
	vector<parameter> received_params;
	cout << "Ricevo una richiesta\n";
	if (!sk->receiveString(request)) return false;
	if (request.compare(SERVICE_REQ)) {
	}
	cout << "Ricevo nome del servizio e parametri\n";
	if (!sk->receiveString(name) || !sk->receiveInt(parameters_size)) {
		cerr << "Errore durante la ricezione di una richiesta di servizio\n";
		return false;
	}
	if (name.compare(this->name)) {
		cerr << "Richiesta con nome del servizio non corretto\n";
		sk->sendString("Nome del servizio non corretto");
		return false;
	}
	if (parameters_size == (int) inParameters.size()) {
		cerr << "Richiesta con numero di parametri di ingresso non corretto\n";
		sk->sendString("Il numero dei parametri di ingresso non è corretto");
		return false;
	}
	for (int i = 0; i < parameters_size; i++) {
		Deserializer d;
		if (!sk->receiveObject(d)) {
			cerr << "Errore durante la ricezione di un parametro\n";
			return false;
		}
		received_params.push_back(d.getObject());
	}
	for (int i = 0; i < parameters_size; i++) {
		if (inParameters[i] != received_params[i]) {
			cerr << "Richiesta con parametri di ingresso non corretti\n";
			sk->sendString("Parametri di ingresso non corretti");
			return false;
		}
	}
	if (!sk->sendString(SERVICE_RESP)) {
		cerr << "Errore durante l'invio della conferma di accettazione del servizio\n";
		return false;
	}
	return execute(sk);
}
bool Service::execute(Socket * sk) {
	cout << "Esecuzione del servizio\n";
	return true;
}