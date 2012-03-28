//
//  SOA/SOA.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "SOA.h"

bool SOA::setServerRegister(string SRaddres, string SRport) {
	this->SRaddres = SRaddres;
	this->SRport = SRport;
	Socket SRsocket;
	if (!comm.connectTo(SRaddres, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	if (!SRsocket.sendString(ACK_PACKET)) {
		cerr << "Impossibile confermare la connesione al Server Register\n";
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serviceRegistration(Service s) {
	Socket SRsocket;
	if (!comm.connectTo(SRaddres, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	s.serviceRegistration(SRsocket, SRaddres, SRport);
	SRsocket.closeSocket();
	return true;
}