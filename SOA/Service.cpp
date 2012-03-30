//
//  SOA/Service.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Service.h"

void Service::setService(string name, vector<param> parameters) {
	this->name = name;
	for (int i = 0; i < (int) parameters.size(); i++) {
		if (parameters[i].direction == IN) inParameters.push_back(parameters[i]);
		else outParameters.push_back(parameters[i]);
	}
}
bool Service::setServer(string SPaddress, string SPport) {
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
	if (!serviceProvider.sendString(SRC_REQ)) {
		cerr << "Errore nella richiesta di servizio\n";
		return false;
	}
	string ack;
	if (!serviceProvider.receiveString(ack) || ack.compare(SRC_RESP)) {
		// Perché la richiesta non è andata a buon fine?
		// Forse il servizio non esisteva?
		// Oppure il server non c'ha più voglia? (boh)
		cerr << "Errore nella conferma di accettazione del servizio\n";
		return false;
	}
	return true;
}