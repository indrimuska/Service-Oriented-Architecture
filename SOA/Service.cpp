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
bool Service::serviceRegistration(Socket SRsocket, string SPaddres, string SPport) {
	if (!SRsocket.sendString(SPaddres + ':' + SPport + '/' + name)) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
}
bool Service::serviceUnRegistration(Socket SRsocket, string SPaddres, string SPport) {
	if (!SRsocket.sendString(SPaddres + ':' + SPport + '/' + name)) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
}