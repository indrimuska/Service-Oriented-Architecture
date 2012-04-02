//
//  SOA/SOA.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "SOA.h"

bool SOA::setServerRegister(string SRaddress, string SRport) {
	this->SRaddress = SRaddress;
	this->SRport = SRport;
	Socket SRsocket;
	if (!comm.connectTo(SRaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	string ack;

	if (!SRsocket.sendString(CONN_ACK_REQ) ||
		!SRsocket.receiveString(ack) || ack.compare(CONN_ACK_RESP)) {
		cerr << "Impossibile confermare la connesione al Server Register\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serverRegistration(string SPaddress, string SPport) {
	Socket SRsocket;
	if (!comm.connectTo(SRaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	string ack;
	if (!SRsocket.sendString(SRV_REG_REQ)) {
		cerr << "Errore nella richiesta di registrazione del server\n";
		return false;
	}
	if (!SRsocket.sendString(SPaddress + ':' + SPport)) {
		cerr << "Errore nella registrazione del server\n";
		return false;
	}
	if (!SRsocket.receiveString(ack) || ack.compare(SRV_REG_RESP)) {
		cerr << "Errore nella conferma di registrazione del server\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serviceRegistration(Service s) {
	Socket SRsocket;
	if (!comm.connectTo(SRaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	if (!SRsocket.sendString(SRC_REG_REQ)) {
		cerr << "Errore nella richiesta di registrazione del servizio\n";
		return false;
	}
	string ack;
	s.serviceRegistration(SRsocket);
	if (!SRsocket.receiveString(ack) || ack.compare(SRC_REG_RESP)) {
		cerr << "Errore nella conferma di registrazione del servizio\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serverUnRegistration(string SPaddress, string SPport) {
	Socket SRsocket;
	if (!comm.connectTo(SPaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	string ack;
	if (!SRsocket.sendString(SRV_UNREG_REQ)) {
		cerr << "Errore nella richiesta di de-registrazione del server\n";
		return false;
	}
	if (!SRsocket.sendString(SPaddress + ':' + SPport)) {
		cerr << "Errore nella registrazione del server\n";
		return false;
	}
	if (!SRsocket.receiveString(ack) || ack.compare(SRV_UNREG_RESP)) {
		cerr << "Errore nella conferma di de-registrazione del server\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serviceUnRegistration(Service s) {
	Socket SRsocket;
	if (!comm.connectTo(SRaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	string ack;
	if (!SRsocket.sendString(SRV_UNREG_REQ)) {
		cerr << "Errore nella richiesta di de-registrazione del servizio\n";
		return false;
	}
	s.serviceUnRegistration(SRsocket);
	if (!SRsocket.receiveString(ack) || ack.compare(SRV_UNREG_RESP)) {
		cerr << "Errore nella conferma di de-registrazione del servizio\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
