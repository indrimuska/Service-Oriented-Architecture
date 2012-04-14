//
//  SOA/Service.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Service.hpp"

Response::Response() {
	result = true;
}
void Response::setResult(bool result) {
	this->result = result;
}
void Response::setError(string error) {
	result = false;
	setMessage("\033[1;31m" + error + "\033[0m");
}
void Response::setMessage(string message) {
	this->message = message;
}
void Response::setParameters(vector<parameter> &parameters) {
	this->parameters = parameters;
}
bool Response::getResult() {
	return result;
}
string Response::getMessage() {
	return message;
}
vector<parameter>& Response::getParameters() {
	return parameters;
}

void Service::setName(string name) {
	this->name = name;
}
void Service::setParameters(vector<parameter> &parameters) {
	inParameters.clear();
	outParameters.clear();
	for (int i = 0; i < (int) parameters.size(); i++) {
		if (parameters[i].getDirection() == IN) inParameters.push_back(parameters[i]);
		else outParameters.push_back(parameters[i]);
	}
}
string Service::getServiceName() {
	return name;
}
void Service::setService(string name, vector<parameter> &parameters) {
	this->name = name;
	setParameters(parameters);
}
bool Service::sendResponse(Socket &sk, Response &response) {
	if (!sk.sendInt(response.getResult())) return false;
	if (!sk.sendString(response.getMessage())) return false;
	if (!sendParameters(sk, response.getParameters())) return false;
	return true;
}
bool Service::receiveResponse(Socket &sk, Response &response) {
	int result;
	string message;
	vector<parameter> parameters;
	if (!sk.receiveInt(result)) return false;
	if (!sk.receiveString(message)) return false;
	if (!receiveParameters(&sk, parameters)) return false;
	response.setResult(result);
	response.setMessage(message);
	response.setParameters(parameters);
	return true;
}
bool Service::sendParameters(Socket &serviceProvider, vector<parameter> &parameters) {
	int parameters_size = (int) parameters.size();
	if (!serviceProvider.sendInt(parameters_size)) {
		cerr << "Errore nell'invio del numero di parametri del servizio\n";
		return false;
	}
	for (int i = 0; i < parameters_size; i++) {
		if (!serviceProvider.sendParameter(parameters[i])) {
			cerr << "Errore durante l'invio di uno dei parametri del servizio\n";
			return false;
		}
	}
	return true;
}
bool Service::receiveParameters(Socket * sk, vector<parameter> &parameters) {
	int parameters_size;
	if (!sk->receiveInt(parameters_size)) {
		cerr << "Errore durante la ricezione del numero di parametri\n";
		return false;
	}
	for (int i = 0; i < parameters_size; i++) {
		parameter p;
		if (!sk->receiveParameter(p)) {
			cerr << "Errore durante la ricezione di un parametro\n";
			return false;
		}
		parameters.push_back(p);
	}
	return true;
}
bool Service::requestService(string SPaddress, string SPport) {
	Communicator comm;
	Socket serviceProvider;
	if (!comm.connectTo(SPaddress, SPport, serviceProvider)) {
		cerr << "Impossibile connettersi al Service Provider\n";
		return false;
	}
	// Questo invio serve al Service Provider per sapere quale servizio viene richiesto
	if (!serviceProvider.sendString(name)) {
		cerr << "Errore nell'invio del nome del servizio richiesto\n";
		return false;
	}
	// Questo invio serve a controllare che il nome del servizio sia corretto
	if (!serviceProvider.sendString(name)) {
		cerr << "Errore nell'invio del nome del servizio richiesto\n";
		return false;
	}
	if (!sendParameters(serviceProvider, inParameters)) {
		cerr << "Errore nell'invio dei parametri di ingresso del servizio\n";
		return false;
	}
	if (!sendParameters(serviceProvider, outParameters)) {
		cerr << "Errore nell'invio dei parametri di uscita del servizio\n";
		return false;
	}
	Response response;
	if (!receiveResponse(serviceProvider, response)) {
		cerr << "Errore nella ricezione della risposta\n";
		return false;
	}
	if (!response.getResult()) {
		cerr << response.getMessage() << endl;
		return false;
	}
	outParameters = response.getParameters();
	comm.closeAllCommunications();
	return true;
}
bool Service::serveRequest(Socket * sk) {
	string name;
	Response response;
	vector<parameter> receivedInParams, receivedOutParams;
	if (!sk->receiveString(name)) {
		cerr << "Errore durante la ricezione di una richiesta di servizio\n";
		return false;
	}
	if (!receiveParameters(sk, receivedInParams)) {
		cerr << "Errore durante la ricezione dei parametri di ingresso del servizio\n";
		return false;
	}
	if (!receiveParameters(sk, receivedOutParams)) {
		cerr << "Errore durante la ricezione dei parametri di uscita del servizio\n";
		return false;
	}
	if (name.compare(this->name)) {
		cerr << "Richiesta con nome del servizio non corretto\n";
		response.setError("Il nome del servizio non è corretto");
	}
	if (receivedInParams.size() != inParameters.size()) {
		cerr << "Richiesta con numero di parametri di ingresso non corretto\n";
		response.setError("Il numero dei parametri di ingresso non è corretto");
	}
	for (int i = 0; i < (int) receivedInParams.size(); i++) {
		if (inParameters[i] != receivedInParams[i]) {
			stringstream error;
			error << "Parametri di ingresso non corretti\n";
			error << "Il parametro no. " << i << " dev'essere ";
			error << (inParameters[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (inParameters[i].getType() == INT) error << "INT";
			if (inParameters[i].getType() == DOUBLE) error << "DOUBLE";
			if (inParameters[i].getType() == STRING) error << "STRING";
			if (inParameters[i].getType() == BUFFER) error << "BUFFER";
			error << "\nInvece è stato ricevuto un parametro ";
			error << (receivedOutParams[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (receivedInParams[i].getType() == INT) error << "INT";
			if (receivedInParams[i].getType() == DOUBLE) error << "DOUBLE";
			if (receivedInParams[i].getType() == STRING) error << "STRING";
			if (receivedInParams[i].getType() == BUFFER) error << "BUFFER";
			cerr << "Richiesta con parametri di ingresso non corretti\n";
			response.setError(error.str());
		} else inParameters[i] = receivedInParams[i];
	}
	if (receivedOutParams.size() != outParameters.size()) {
		cerr << "Richiesta con numero di parametri di uscita non corretto\n";
		response.setError("Il numero dei parametri di uscita non è corretto");
	}
	for (int i = 0; i < (int) receivedOutParams.size(); i++) {
		if (outParameters[i] != receivedOutParams[i]) {
			stringstream error;
			error << "Parametri di uscita non corretti\n";
			error << "Il parametro no. " << i << " dev'essere ";
			error << (outParameters[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (outParameters[i].getType() == INT) error << "INT";
			if (outParameters[i].getType() == DOUBLE) error << "DOUBLE";
			if (outParameters[i].getType() == STRING) error << "STRING";
			if (outParameters[i].getType() == BUFFER) error << "BUFFER";
			error << "\nInvece è stato ricevuto un parametro ";
			error << (receivedOutParams[i].getDirection() == IN ? "IN" : "OUT") << ",";
			if (receivedOutParams[i].getType() == INT) error << "INT";
			if (receivedOutParams[i].getType() == DOUBLE) error << "DOUBLE";
			if (receivedOutParams[i].getType() == STRING) error << "STRING";
			if (receivedOutParams[i].getType() == BUFFER) error << "BUFFER";
			cerr << "Richiesta con parametri di uscita non corretti\n";
			response.setError(error.str());
		}
	}
	if (response.getResult()) {
		if (!execute()) response.setError("Errore durante l'esecuzione del servizio");
		else response.setParameters(outParameters);
	}
	if (!sendResponse(* sk, response)) {
		cerr << "Errore nell'invio della risposta\n";
		return false;
	}
	return response.getResult();
}
bool Service::execute() {
	cerr << "\033[1;31mServizio non implementato\033[0m\n";
	return false;
}