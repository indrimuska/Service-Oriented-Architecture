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
	cout << "Invio i parametri del servizio\n";
	for (int i = 0; i < (int) inParameters.size(); i++) {
		Serializer s(inParameters[0]);
		if (!serviceProvider.sendObject(s)) {
			cerr << "Errore durante l'invio di uno dei parametri del servizio\n";
			return false;
		}
	}
	//for (int i = 0; i < (int) inParameters.size(); i++) {
		/*
		cout << inParameters[i] << endl;
		string filename = "serializzato";
		ofstream file(filename.c_str(), ios_base::binary);
		if (!file.is_open()) {
			cerr << "Errore durante la serializzazione dei parametri\n";
			return false;
		}
		parameter_direction direction = inParameters[i].getDirection();
		parameter_type type = inParameters[i].getType();
		file.write((char *) &direction, sizeof(parameter_direction));
		file.write((char *) &type, sizeof(parameter_type));
		file.close();
		serviceProvider.sendFile(filename);
		if (FILE * f = fopen(filename.c_str(), "r")) fclose(f);
		else {
			cerr << "Il file non esiste\n";
			return false;
		}
		if (remove(filename.c_str())) {
			cerr << "Impossibile rimuovere il file\n";
			return false;
		}
		*/
		/*
		int pd = inParameters[i].getDirection() == IN ? 0 : 1, pt;
		if (inParameters[i].getType() == INT) pt = 0;
		if (inParameters[i].getType() == DOUBLE) pt = 1;
		if (inParameters[i].getType() == STRING) pt = 2;
		if (inParameters[i].getType() == BUFFER) pt = 3;
		serviceProvider.sendInt(pd);
		serviceProvider.sendInt(pt);
		*/
	//	Serializer s(inParameters[i]);
	//	if (!serviceProvider.sendObject(s)) {
	//		cerr << "Errore durante l'invio di uno dei parametri del servizio\n";
	//		return false;
	//	}
	//}
	return true;
}
bool Service::receiveParameters(Socket * sk, int parameters_size, vector<parameter> &parameters) {
	parameter p(IN, INT);
	for (int i = 0; i < parameters_size; i++) {
		Deserializer d;
		if (!sk->receiveObject(d)) {
			cerr << "Errore durante la ricezione di un parametro\n";
			return false;
		}
		if (p != d.getObject()) cout << "Sbagliato\n";
		parameters.push_back(d.getObject());
	}
	//for (int i = 0; i < parameters_size; i++) {
		/*
		string filename;
		sk->receiveFile(".", filename);
		ifstream file(filename.c_str(), ios_base::binary);
		if (!file.is_open()) {
			cerr << "Impossibile aprire il file serializzato\n";
			return false;
		}
		parameter_direction direction;
		parameter_type type;
		file.read((char *) &direction, sizeof(parameter_direction));
		file.read((char *) &type, sizeof(parameter_type));
		if (remove(filename.c_str())) {
			cerr << "Impossibile rimuovere il file\n";
			return false;
		}
		parameter p(direction, type);
		cout << p << endl;
		parameters.push_back(p);
		*/
		/*
		int direction, type;
		sk->receiveInt(direction);
		sk->receiveInt(type);
		parameter_direction pd;
		parameter_type pt;
		pd = direction == 0 ? IN : OUT;
		switch (type) {
			case 0: pt = INT; break;
			case 1: pt = DOUBLE; break;
			case 2: pt = STRING; break;
			case 3: pt = BUFFER; break;
		}
		parameters.push_back(parameter(pd, pt));
		*/
	//	Deserializer d;
	//	bool res = sk->receiveObject(d);
	//	cout << i << ") " << d.getObject() << endl;
	//	if (!res) {
	//		cerr << "Errore durante la ricezione di un parametro\n";
	//		return false;
	//	}
	//	parameters.push_back(d.getObject());
	//}
	return true;
}
bool Service::requestService() {
	Socket sp;
	Communicator c;
	Serializer s(inParameters[0]);
	if (!c.connectTo(SPaddress, SPport, sp)) return false;
	sp.sendInt((int) inParameters.size());
	sendParameters(sp);
	c.closeAllCommunications();
	return true;
	
	/*Communicator comm;
	Socket serviceProvider;
	cout << "Provo a connettermi al service provider ("<<SPaddress<<":"<<SPport<<")\n";
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
	if (!serviceProvider.sendInt((int) inParameters.size())) {
		cerr << "Errore nell'invio del numero di parametri del servizio\n";
		return false;
	}
	cout << "Ricevo la conferma parziale\n";
	string ack;
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "Errore nella conferma di accettazione parziale del servizio\n" << ack << endl;
		return false;
	}
	if (!sendParameters(serviceProvider)) return false;
	cout << "Ricevo la conferma\n";
	if (!serviceProvider.receiveString(ack) || ack.compare(SERVICE_RESP)) {
		cerr << "La richiesta di servizio non è stata accettata\n" << ack << endl;
		return false;
	}
	//serviceProvider.closeSocket();
	comm.closeAllCommunications();
	return true;*/
}
bool Service::serveRequests(Socket * sk) {
	int ps;
	sk->receiveInt(ps);
	vector<parameter> rp;
	if (!receiveParameters(sk, ps, rp)) return false;
	return true;
	
	/*int parameters_size;
	string request, name;
	vector<parameter> received_params;
	cout << "Ricevo una richiesta\n";
	if (!sk->receiveString(request)) return false;
	if (request.compare(SERVICE_REQ)) {
		cerr << "Richiesta sconosciuta\n";
		return true;
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
	if (parameters_size != (int) inParameters.size()) {
		cerr << "Richiesta con numero di parametri di ingresso non corretto\n";
		sk->sendString("Il numero dei parametri di ingresso non è corretto");
		return false;
	}
	if (!sk->sendString(SERVICE_RESP)) {
		cerr << "Errore durante l'invio della conferma di accettazione parziale del servizio\n";
		return false;
	}
	if (!receiveParameters(sk, parameters_size, received_params)) return false;
	for (int i = 0; i < parameters_size; i++) {
		if (inParameters[i] != received_params[i]) {
			cerr << "Richiesta con parametri di ingresso non corretti\n";
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
	cout << "Esecuzione del servizio\n";
	return true;
}