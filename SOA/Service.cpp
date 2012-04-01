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
		//if (parameters[i].direction == IN) inParameters.push_back(parameters[i]);
		//else outParameters.push_back(parameters[i]);
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
		// TODO: inviare i parametri
	}
	if (!serviceProvider.sendObject((void *) &inParameters, inParameters.capacity())) {
		cerr << "Errore nell'invio dei parametri del servizio richiesto\n";
		return false;
	}
	cout << "Ricevo la conferma\n";
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "Errore nella conferma di accettazione del servizio\n" << ack << endl;
		return false;
	}
	serviceProvider.closeSocket();
	return true;
}
bool Service::serveRequests(Socket * sk) {
	int params_capacity;
	string request, name;
	vector<parameter> received_params;
	cout << "Ricevo una richiesta\n";
	if (!sk->receiveString(request)) return false;
	if (!request.compare(SERVICE_REQ)) {
		cout << "Ricevo nome del servizio e parametri\n";
		if (sk->receiveString(name) &&// !name.compare(this->name) &&
			sk->receiveInt(params_capacity) &&// params_capacity == (int) inParameters.capacity() &&
			sk->receiveObject((void *) &received_params, params_capacity)) {
			if (sk->sendString(SERVICE_RESP)) return execute(sk);
		} else {
			if (name.compare(this->name)) {
				cout << "Richiesta con nome del servizio non corretto\n";
				sk->sendString("Nome del servizio non corretto");
			}
			if (received_params.size() == inParameters.size()) {
				cout << "Richiesta con parametri di ingresso non corretti\n";
				sk->sendString("Parametri di ingresso non corretti");
			}
		}
	}
	return false;
}
bool Service::execute(Socket * sk) {
	cout << "Esecuzione del servizio\n";
	return true;
}