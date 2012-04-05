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
bool Service::sendParameters(Socket &serviceProvider) {
	if (!serviceProvider.sendInt((int) inParameters.size())) return 0;
	for (int i = 0; i < (int) inParameters.size(); i++) {
		Serializer s(inParameters[i]);
		if (!serviceProvider.sendObject(s)) return 0;
	}
	/*if (!serviceProvider.sendInt((int) inParameters.size())) {
		cerr << "Errore nell'invio del numero di parametri del servizio\n";
		return false;
	}
	for (int i = 0; i < (int) inParameters.size(); i++) {
		Serializer s(inParameters[0]);
		if (!serviceProvider.sendObject(s)) {
			cerr << "Errore durante l'invio di uno dei parametri del servizio\n";
			return false;
		}
	}*/
	return true;
}
bool Service::receiveParameters(Socket * sk, vector<parameter> &parameters) {
	int parameters_size;
	if (!sk->receiveInt(parameters_size)) return false;
	for (int i = 0; i < parameters_size; i++) {
		Deserializer d;
		if (!sk->receiveObject(d)) return false;
		if (inParameters[i] != d.getObject()) {
			cout << "sbagliato\n";
			//cout << "Parameter " << i << " expected to be:\n" << parameters[i];
			//cout << "Parameter received is:\n" << d.getObject() << endl;
			return false;
		}
		parameters.push_back(d.getObject());
	}
	/*
	int parameters_size;
	if (!sk->receiveInt(parameters_size)) {
		cerr << "Errore durante la ricezione del numero di parametri\n";
		return false;
	}
	for (int i = 0; i < parameters_size; i++) {
		Deserializer d;
		if (!sk->receiveObject(d)) {
			cerr << "Errore durante la ricezione di un parametro\n";
			return false;
		}
		parameters.push_back(d.getObject());
	}*/
	return true;
}
bool Service::requestService() {
	Socket serviceProvider;
	Communicator c;
	string ack;
	if (!c.connectTo(SPaddress, SPport, serviceProvider)) return false;
	if (!sendParameters(serviceProvider)) return false;
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "La richiesta di servizio non è stata accettata\n" << ack << endl;
		return false;
	}
	c.closeAllCommunications();
	return true;
	/*********************
	Communicator comm;
	Socket serviceProvider;
	if (!comm.connectTo(SPaddress, SPport, serviceProvider)) return false;
	if (!serviceProvider.sendString(SERVICE_REQ)) {
		cerr << "Errore nella richiesta di servizio\n";
		return false;
	}
	if (!serviceProvider.sendString(name)) {
		cerr << "Errore nell'invio del nome del servizio richiesto\n";
		return false;
	}
	string ack;
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "Errore nella conferma di accettazione parziale del servizio\n" << ack << endl;
		return false;
	}
	if (!sendParameters(serviceProvider)) return false;
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "La richiesta di servizio non è stata accettata\n" << ack << endl;
		return false;
	}
	//serviceProvider.closeSocket();
	comm.closeAllCommunications();
	return true;*/
}
bool Service::serveRequests(Socket * sk) {
	vector<parameter> received_params;
	if (!receiveParameters(sk, received_params)) return false;
	for (int i = 0; i < (int) received_params.size(); i++) {
		if (inParameters[i] != received_params[i]) {
			stringstream error;
			error << "Parametri di ingresso non corretti (" << i << ":";
			error << (received_params[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (received_params[i].getType() == INT) error << "INT";
			if (received_params[i].getType() == DOUBLE) error << "DOUBLE";
			if (received_params[i].getType() == STRING) error << "STRING";
			if (received_params[i].getType() == BUFFER) error << "BUFFER";
			error << "|" << (inParameters[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (inParameters[i].getType() == INT) error << "INT";
			if (inParameters[i].getType() == DOUBLE) error << "DOUBLE";
			if (inParameters[i].getType() == STRING) error << "STRING";
			if (inParameters[i].getType() == BUFFER) error << "BUFFER";
			error << ")";
			cerr << "Richiesta con " << error.str() << endl;
			sk->sendString(error.str());
			return false;
		}
	}
	return true;
	/*********************
	string request, name;
	vector<parameter> received_params;
	if (!sk->receiveString(request)) return false;
	if (request.compare(SERVICE_REQ)) {
		cerr << "Richiesta sconosciuta\n";
		return true;
	}
	if (!sk->receiveString(name)) {
		cerr << "Errore durante la ricezione di una richiesta di servizio\n";
		return false;
	}
	if (name.compare(this->name)) {
		cerr << "Richiesta con nome del servizio non corretto\n";
		sk->sendString("Nome del servizio non corretto");
		return false;
	}
	if (!sk->sendString(SERVICE_RESP)) {
		cerr << "Errore durante l'invio della conferma di accettazione parziale del servizio\n";
		return false;
	}
	if (!receiveParameters(sk, received_params)) return false;
	if (received_params.size() != inParameters.size()) {
		cerr << "Richiesta con numero di parametri di ingresso non corretto\n";
		sk->sendString("Il numero dei parametri di ingresso non è corretto");
		return false;
	}
	for (int i = 0; i < (int) received_params.size(); i++) {
		if (inParameters[i] != received_params[i]) {
			stringstream error;
			error << "Parametri di ingresso non corretti (" << i << ":";
			error << (received_params[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (received_params[i].getType() == INT) error << "INT";
			if (received_params[i].getType() == DOUBLE) error << "DOUBLE";
			if (received_params[i].getType() == STRING) error << "STRING";
			if (received_params[i].getType() == BUFFER) error << "BUFFER";
			error << "|" << (inParameters[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (inParameters[i].getType() == INT) error << "INT";
			if (inParameters[i].getType() == DOUBLE) error << "DOUBLE";
			if (inParameters[i].getType() == STRING) error << "STRING";
			if (inParameters[i].getType() == BUFFER) error << "BUFFER";
			error << ")";
			cerr << "Richiesta con " << error.str() << endl;
			sk->sendString(error.str());
			return false;
		}
	}
	if (!sk->sendString(SERVICE_RESP)) {
		cerr << "Errore durante l'invio della conferma di accettazione del servizio\n";
		return false;
	}
	return execute(sk);*/
}
bool Service::execute(Socket * sk) {
	//cout << "Esecuzione del servizio\n\n";
	return true;
}