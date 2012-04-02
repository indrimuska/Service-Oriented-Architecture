//
//  ServerRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include <map>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "SOA/SOA.h"
#include "SOA/Communication.h"
#include "SOA/ServerInformation.h"
#include "SOA/ServiceInformation.h"

using namespace std;

class ServerRegister {
private:
	Communicator comm;
	string SRaddress;
	string SRport;
	vector<ServerInformation> SRservers;
	vector<int> myvector;
	vector<ServerInformation>::iterator SIit;
	map<string, ServiceInformation> SRservices;
	map<string, ServiceInformation>::iterator servicesIt;

public:

	ServerRegister(string SRport) {
		this->SRaddress = comm.getIP();
		this->SRport = SRport;
		comm.startListener(SRport);
		//SRservers = vector<ServerInformation>(10);
	}
	bool waitForConnection() {
		Socket * sk = new Socket();
		string request;
		comm.waitForConnection(*sk);
		if (!sk->receiveString(request))
			return false;
		cout << "La richiesta è: "<< request << endl;
		if (!request.compare(CONN_ACK_REQ)){
			cout << "Mi manda una conn_ack_req" << endl;
			return confirmConnection(sk);
		}
		if (!request.compare(SRV_REG_REQ)){
			return registerServer(sk);
		}
		if (!request.compare(SRC_REG_REQ)){
			return registerService(sk);
		}
		if (!request.compare(SRV_REG_DISP)) {
			cout << "è stata richiesta una display " << endl;
			return displayRegisteredServers(sk);
		}
		if (!request.compare(SRC_REG_DISP)) {
			cout << "è stata richiesta la display dei servizi " << endl;
			return displayRegisteredServices(sk);
		}
		return true;
	}
	bool confirmConnection(Socket * sk) {
		cout << "entro in confirmConnection" << endl;
		if (!sk->sendString(CONN_ACK_RESP)) {
			cerr << "Impossibile confermare la connessione\n";
			return false;
		}
		delete sk;
		return true;
	}
	bool registerServer(Socket * sk) {
		cout << "Entro in registerServer\n";
		int dim = (int) SRservers.size();
		cout << "Dimensione del registro = " << endl;
		cout << dim << endl;
		cout << "forse" << endl;
		string serverToReg; //serverToReg is a string that must have the format "address:port"
		sk->receiveString(serverToReg);
		if (SRservers.size() == 0) {
			cout << "faccio l'inserimento in seguito di size = 0" << endl;
			string address, port;
			sk->receiveString(address);
			sk->receiveString(port);
			ServerInformation serInf = ServerInformation(serverToReg, address,
					port);
			SIit = SRservers.begin();
			SRservers.insert(SIit, serInf);
			cout << "Il server " + serInf.identification << endl;
			cout << "indirizzo = " + serInf.Saddress << endl;
			cout << "porta = " + serInf.Sport << endl;
			cout << "È stato inserito con successo\n";
			int nuovaDim = (int) SRservers.size();
			cout << "Nuova dimensione del registro = " << endl;
			cout << nuovaDim << endl;
			return true;
		}
		for (int i = 0; i < (int) SRservers.size(); i++) {
			if (serverToReg == SRservers[i].identification) {
				cout << "Questo server è già registrato\n";
				return true;
			}
		}
		cout << "faccio l'inserimento normale" << endl;
		string address, port;
		sk->receiveString(address);
		sk->receiveString(port);
		ServerInformation serInf = ServerInformation(serverToReg, address,
				port);
		SIit = SRservers.begin();
		SRservers.insert(SIit, serInf);
		cout << "Il server " + serInf.identification << endl;
		cout << "indirizzo = " + serInf.Saddress << endl;
		cout << "porta = " + serInf.Sport << endl;
		cout << "È stato inserito con successo\n";
		int nuovaDim = SRservers.size();
		cout << "Nuova dimensione del registro (paolo check) = " << endl;
		cout << nuovaDim << endl;
		//da qui solo per fare debug, va tolta perché questa è la display
		cout << "------------------------------------" << endl;
				cout << "|  Indirizzo Server | Porta Server |" << endl;
				cout << "------------------------------------" << endl;
				for (int i = 0; i < (int) SRservers.size(); i++) {
					ServerInformation siShow = SRservers[i];
					cout
							<< "|   " + siShow.Saddress + "       |   " + siShow.Sport
									+ "\t   |" << endl;
					cout << "------------------------------------" << endl;
					//cout<< "Indirizzo server = " + siShow.Saddress
					//				+ ", porta Server = " + siShow.Sport << endl;
				}

		//fino a qui da togliere, serve solo per debug
		return true;

	}
	bool displayRegisteredServers(Socket * sk) {
		cout << "Eseguo displayRegisteredServers\n";
		cout << "------------------------------------" << endl;
		cout << "|  Indirizzo Server | Porta Server |" << endl;
		cout << "------------------------------------" << endl;
		for (int i = 0; i < (int) SRservers.size(); i++) {
			ServerInformation siShow = SRservers[i];
			cout
					<< "|   " + siShow.Saddress + "       |   " + siShow.Sport
							+ "\t   |" << endl;
			cout << "------------------------------------" << endl;
			//cout<< "Indirizzo server = " + siShow.Saddress
			//				+ ", porta Server = " + siShow.Sport << endl;
		}
		return true;
	}
	bool registerService(Socket * sk) {
		cout << "Operazioni di register service" << endl;
		string serverInfo;
		cout << "1: ricezione stringa Info server " << endl;
		sk->receiveString(serverInfo); //per come sto facendo ora più che serverInfo dovrei passare separatamente indirizzo e porta
		cout << "1 OK: ricevuta stringa Info Server = " << endl;
		cout << serverInfo << endl;
		//cout << "In questo momento dovrei aver già ricevuto serverInfo" << endl;
		/*for (int i = 0; i < (int) SRservers.size(); i++) {
			if (serverInfo != SRservers[i].identification) {
				cout << "***** faccio continue ed esco dal ciclo ***** " << endl;
				continue;
			}
			if (i == (int) SRservers.size() - 1){
				cout << "Server non registrato!" << endl;
				cout << "Prima di registrare questo servizio è necessario registrare il server " << endl;
				//sk->sendString("non_registrato");
				return true;
			}
		}*/
		cout << "2: invio info sulla registrazione del server" << endl;
		sk->sendString("non_registrato");


		/*string serverParameters;
		cout << "***********" << endl;
		sk->receiveString(serverParameters);
		cout << "Eseguo registerService" << endl;
		string serviceToReg;
		sk->receiveString(serviceToReg);
		cout << "Il servizio che si vuole registrare è: " << endl;
		cout << serviceToReg << endl;
		servicesIt = SRservices.find(serviceToReg);
		if (servicesIt == SRservices.end()) {
			cout << "Voglio registrare: " + serviceToReg << endl;
			ServiceInformation si = ServiceInformation(serviceToReg); //qui creo una nuova ServiceInformation per un particolare tipo di servizio
		}*/
		return true;
	}
	bool unregisterService(Socket * sk) {
		return true;
	}
	bool displayRegisteredServices(Socket * sk) {
		cout << "Eseguo displayRegisteredServices\n";
		cout << "-----------------------------------------------" << endl;
		cout << "| Servizio |  Indirizzo Server | Porta Server |" << endl;
		cout << "-----------------------------------------------" << endl;
		for (int i = 0; i < (int) SRservers.size(); i++) {
			ServerInformation siShow = SRservers[i];
			cout
					<< "|   " + siShow.Saddress + "       |   " + siShow.Sport
							+ "\t   |" << endl;
			cout << "-----------------------------------------------" << endl;
			//cout<< "Indirizzo server = " + siShow.Saddress
			//				+ ", porta Server = " + siShow.Sport << endl;
		}
		return true;
	}

	bool provideServiceInformation(Socket * sk) {
		string serviceName;
		cin >> serviceName;
		servicesIt = SRservices.find(serviceName);
		//ServiceInformation si = servicesIt;
		return true;
	}
	~ServerRegister() {
		// Chiusura del socket di ascolto
		comm.stopListener();
		// Chiusura di tutte le connessioni aperte
		comm.closeAllCommunications();
	}
};

int main(int argc, char ** argv) {
	string SRport;

	cout << " * SERVER REGISTER * \n\n";
	cout << "Set port number: ";
	cin >> SRport;

	ServerRegister serverRegister(SRport);

	while (1) {
		// Attesa di connessioni in corso
		cout << "\nWaiting for connection...\n";
		if (!serverRegister.waitForConnection())
			continue;

		cout << "Connection closed\n";
	}

	serverRegister.~ServerRegister();

	return 1;
}
