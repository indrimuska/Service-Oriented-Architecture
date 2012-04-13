//
//  SOA/SOA.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "SOA.hpp"

bool SOA::sendRequest(string request, Socket &SRsocket) {
	if (!comm.connectTo(SRaddress, SRport, SRsocket)) {
		cerr << "Impossibile connettersi al Server Register\n";
		return false;
	}
	if (!SRsocket.sendString(request)) {
		cerr << "Errore nell'invio di una richiesta al Service Register\n";
		return false;
	}
	return true;
}
bool SOA::setServerRegister(string SRaddress, string SRport) {
	this->SRaddress = SRaddress;
	this->SRport = SRport;
	string ack;
	Socket SRsocket;
	if (!sendRequest(CONN_ACK_REQ, SRsocket)) return false;
	if (!SRsocket.receiveString(ack) || ack.compare(CONN_ACK_RESP)) {
		cerr << "Impossibile confermare la connesione al Server Register\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
void SOA::setServiceProvider(string SPaddress, string SPport) {
	this->SPaddress = SPaddress;
	this->SPport = SPport;
}
bool SOA::serverRegistration() {
	string ack;
	Socket SRsocket;
	if (!sendRequest(SRV_REG_REQ, SRsocket)) return false;
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
bool SOA::serviceRegistration(Service &s) {
	string ack;
	Socket SRsocket;
	if (!sendRequest(SRC_REG_REQ, SRsocket)) return false;
	if (!SRsocket.sendString(SPaddress + ':' + SPport + '/' + s.getServiceName())) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
	if (!SRsocket.receiveString(ack) || ack.compare(SRC_REG_RESP)) {
		cerr << "Errore nella conferma di registrazione del servizio\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::serverUnRegistration() {
	string ack;
	Socket SRsocket;
	if (!sendRequest(SRV_UNREG_REQ, SRsocket)) return false;
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
bool SOA::serviceUnRegistration(Service &s) {
	string ack;
	Socket SRsocket;
	if (!sendRequest(SRC_UNREG_REQ, SRsocket)) return false;
	if (!SRsocket.sendString(SPaddress + ':' + SPport + '/' + s.getServiceName())) {
		cerr << "Errore nella richiesta di registrazione al Server Register\n";
		return false;
	}
	return true;
	if (!SRsocket.receiveString(ack) || ack.compare(SRC_UNREG_RESP)) {
		cerr << "Errore nella conferma di de-registrazione del servizio\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}
bool SOA::getServerAddress(Service &s, string &address, string &port) {
	string ack;
	Socket SRsocket;
	if (!sendRequest(SRV_REQ, SRsocket)) return false;
	if (!SRsocket.sendString(s.getServiceName())) {
		cerr << "Errore nell'invio del nome del servizio\n";
		return false;
	}
	if (!SRsocket.receiveString(ack) || ack.compare(SRV_RESP)) {
		cerr << "Errore nella ricezione dell'indirizzo del server\n" << ack << endl;
		return false;
	}
	if (!SRsocket.receiveString(address) || !SRsocket.receiveString(port)) {
		cerr << "Errore nella ricezione dell'indirizzo o della porta del server\n" << ack << endl;
		return false;
	}
	SRsocket.closeSocket();
	return true;
}