//
//  ServiceRegister.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "SOA/SOA.hpp"
#include "SOA/Communication.hpp"
#include "SOA/ServerInformation.hpp"
#include "SOA/ServiceInformation.hpp"

using namespace std;

class ServiceRegister {
private:
	Communicator comm;
	string SRaddress;
	string SRport;
	vector<ServerInformation> SRservers;
	vector<int> myvector;
	vector<ServerInformation>::iterator SIit;
	map<string, ServiceInformation> SRservices;
	map<string, ServiceInformation>::iterator servicesIt;
	vector<string> serviceProviders;

	bool sendAck(Socket * sk, string ack) {
		if (!sk->sendString(ack)) {
			cerr << "Errore nell'invio di un messaggio di ACK\n";
			return false;
		}
		return true;
	}
	bool isServerRegistred(string serverName) {
		for (int i = 0; i < (int) serviceProviders.size(); i++)
			if (!serverName.compare(serviceProviders[i]))
				return true;
		return false;
	}
	bool splitServerName(string serverName, string &address, string &port) {
		address = serverName.substr(0, serverName.find_first_of(":"));
		port = serverName.substr(serverName.find_first_of(":") + 1);
		return address.length() + port.length() != 0;
	}
	bool splitServiceName(string serviceName, string &address, string &port,
			string &service) {
		address = serviceName.substr(0, serviceName.find_first_of(":"));
		port = serviceName.substr(serviceName.find_first_of(":") + 1,
				serviceName.find_first_of("/") - serviceName.find_first_of(":")
						- 1);
		service = serviceName.substr(serviceName.find_first_of("/") + 1);
		return address.length() + port.length() + service.length() != 0;
	}

public:

	ServiceRegister(string SRport) {
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
		cout << "La richiesta è: " << request << endl;
		if (!request.compare(CONN_ACK_REQ)) {
			cout << "Mi manda una conn_ack_req" << endl;
			return confirmConnection(sk);
		}
		if (!request.compare(SRV_REG_REQ))
			return registerServer(sk);
		if (!request.compare(SRC_REG_REQ))
			return registerService(sk);
		if (!request.compare(SRV_REG_DISP)) {
			cout << "è stata richiesta una display " << endl;
			return displayRegisteredServers(sk);
		}
		if (!request.compare(SRC_REG_DISP)) {
			cout << "è stata richiesta la display dei servizi " << endl;
			return displayRegisteredServices(sk);
		}
		if (!request.compare(SRV_UNREG_REQ))
			return unregisterServer(sk);
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
		cout << "Dimensione del registro = " << dim << endl;
		string serverToReg; //serverToReg is a string that must have the format "address:port"
		sk->receiveString(serverToReg);
		cout << "Server da Registrare = " << serverToReg << endl;
		string address, port;
		int risultato = serverToReg.find_first_of(":");
		cout << "Il carattere : si trova al carattere numero " << risultato
				<< endl;
		address = serverToReg.substr(0, risultato);
		int dimensione = serverToReg.size();
		port = serverToReg.substr(risultato + 1, dimensione);
		cout << "Indirizzo ottenuto tramite la funzione substring: " << address
				<< endl;
		cout << "Porta ottenuta tramite la funzione substring: " << port
				<< endl;
		//if (SRservers.size() == 0) {
		bool salta = false;
		for (int i = 0; i < (int) SRservers.size(); i++) {
			if (serverToReg == SRservers[i].identification) {
				cout << "Gia registrato" << endl;
				salta = true;
			}
		}
		if (salta == true) {
			string inviaErrore = "Questo server è già registrato\n";
			sk->sendString(inviaErrore);
			return true;
		} else {
			//address = serverToReg.
			ServerInformation serInf = ServerInformation(serverToReg, address,
					port);
			SIit = SRservers.begin();
			SRservers.insert(SIit, serInf);
			cout << "Il server " + serInf.identification << endl;
			cout << "indirizzo = " + serInf.Saddress << endl;
			cout << "porta = " + serInf.Sport << endl;
			cout << "È stato inserito con successo\n";
			int nuovaDim = (int) SRservers.size();
			cout << "Nuova dimensione del registro = " << nuovaDim << endl;
			sk->sendString(SRV_REG_RESP);
			return true;
		}
		//}
		/*for (int i = 0; i < (int) SRservers.size(); i++) {
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
		 *
		 */
		return true;

	}
	bool displayRegisteredServers(Socket * sk) {
		cout << "Eseguo displayRegisteredServers\n";
		string ritornoDisplay;
		ritornoDisplay +=
				"-----------------------------------------------------------------\n";
		if (SRservers.size() == 0) {
			ritornoDisplay +=
					"| Non ci sono al momento server disponibili, riprova in seguito |\n";
		} else {
			ritornoDisplay +=
					"|   Nome del Server\t|\tIndirizzo\t|\tPorta\t|\n";
		}
		ritornoDisplay +=
				"-----------------------------------------------------------------\n";
		for (int i = 0; i < (int) SRservers.size(); i++) {
			ServerInformation siShow = SRservers[i];
			ritornoDisplay += "|   " + siShow.identification;
			ritornoDisplay += "\t|\t" + siShow.Saddress + "\t|\t" + siShow.Sport
					+ "\t|\n";
			ritornoDisplay +=
					"-----------------------------------------------------------------\n";

			//cout<< "Indirizzo server = " + siShow.Saddress
			//				+ ", porta Server = " + siShow.Sport << endl;
		}
		cout << ritornoDisplay;
		sk->sendString(ritornoDisplay);
		return true;
	}
	bool registerService(Socket * sk) {
		cout << "Operazioni di register service" << endl;
		string serviceName;
		cout << "È stata richiesta la registrazione di un servizio\n";
		if (!sk->receiveString(serviceName)) {
			cerr << "Errore nella ricezione del nome del servizio\n\n";
			return false;
		}
		cout << "serviceName = " << serviceName << endl;

		string address, port, service;
		if (!splitServiceName(serviceName, address, port, service)) {
			cerr << "Il nome del servizio è invalido\n";
			if (!sk->sendString("Nome del servizio invalido"))
				cerr
						<< "Errore nell'invio della notifica di nome del servizio invalido\n";
			cerr << endl;
			return false;
		}
		cout << "address: " << address << endl;
		cout << "port: " << port << endl;
		cout << "service: " << service << endl;

		cout << "In questo momento dovrei aver già ricevuto serverInfo" << endl;
		for (int i = 0; i < (int) SRservers.size(); i++) {
		cout << "serviceName: " << serviceName << endl;
		cout << "SRservers[i].identification: " << SRservers[i].identification << endl;
		 if (serviceName == SRservers[i].identification) {
		 cout << "***** faccio continue ed esco dal ciclo ***** " << endl;
		 continue;
		 }
		 if (i == (int) SRservers.size() - 1){
		 cout << "Server non registrato!" << endl;
		 cout << "Prima di registrare questo servizio è necessario registrare il server " << endl;
		 //sk->sendString("non_registrato");
		 return true;
		 }
		 }
		//cout << "2: invio info sulla registrazione del server" << endl;
		//sk->sendString("non_registrato");

		//string serverParameters;
		 cout << "***********" << endl;
		 // da qui in poi lo devo fare diversamente perché ho già ricevuto tutto quello che mi serve
		 sk->receiveString(serviceName);
		 cout << "Eseguo registerService" << endl;
		 //string serviceToReg;
		 //sk->receiveString(serviceToReg);
		 cout << "Il servizio che si vuole registrare è: " << endl;
		 cout << serviceName << endl;
		 servicesIt = SRservices.find(serviceName);
		 if (servicesIt == SRservices.end()) {
		 cout << "Voglio registrare: " + serviceName << endl;
		 ServiceInformation si = ServiceInformation(serviceName); //qui creo una nuova ServiceInformation per un particolare tipo di servizio
		 }
		return true;
	}
	bool unregisterServer(Socket * sk) {
		cout << "Entro in unregisterServer\n";
		string serverToUnReg;
		sk->receiveString(serverToUnReg);
		cout << serverToUnReg << endl;
		string address, port;
		int risultato = serverToUnReg.find_first_of(":");
		cout << "Il carattere : si trova al carattere numero " << risultato
				<< endl;
		address = serverToUnReg.substr(0, risultato);
		int dimensione = serverToUnReg.size();
		port = serverToUnReg.substr(risultato + 1, dimensione);
		cout << "Indirizzo ottenuto tramite la funzione substring: " << address
				<< endl;
		cout << "Porta ottenuta tramite la funzione substring: " << port
				<< endl;
		ServerInformation serInf = ServerInformation(serverToUnReg, address,
				port);
		for (int i = 0; i < (int) SRservers.size(); i++) {
			if (serverToUnReg == SRservers[i].identification) {
				SRservers.erase(SRservers.begin() + i);
				break;
			}
		}
		//SIit = SRservers[indice];
		//qui va tolto serverToUnReg da SRservers
		//SRservers.erase(SIit);
		sk->sendString(SRV_UNREG_RESP);
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
	~ServiceRegister() {
		// Chiusura del socket di ascolto
		comm.stopListener();
		// Chiusura di tutte le connessioni aperte
		comm.closeAllCommunications();
	}
};

int main(int argc, char ** argv) {
	string SRport;

	cout << " * SERVICE REGISTER * \n\n";
	cout << "Set port number: ";
	cin >> SRport;

	ServiceRegister serviceRegister(SRport);

	while (1) {
		// Attesa di connessioni in corso
		cout << "\nWaiting for connection...\n";
		if (!serviceRegister.waitForConnection())
			continue;

		cout << "Connection closed\n";
	}

	//serviceRegister.~ServiceRegister();

	return 1;
}
