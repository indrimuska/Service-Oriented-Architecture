//
//  server.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <vector>
#include <iostream>

#include "SOA/SOA.h"
#include "SOA/Parameters.h"
#include "Application/RotateService.cpp"

using namespace std;

int main(int argc, char ** argv) {
	
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	Socket sk;
	Communicator comm;
	comm.startListener(argv[1]);
	cout << "Waiting for clients...\n";
	int sbagliati = 0;
	while (1) {
		int parameters_size;
		if (!comm.waitForConnection(sk)) return 0;
		if (!sk.receiveInt(parameters_size)) continue;
		for (int i = 0; i < parameters_size; i++) {
			parameter rp;
			sk.receiveParameter(rp);
			if (parameters[i] != rp) {
				cout << "sbagliato (" << ++sbagliati << ")\n";
				//cout << "Parameter " << i << " expected to be:\n" << parameters[i];
				//cout << "Parameter received is:\n" << d.getObject() << endl;
			}
		}
	}
	comm.closeAllCommunications();
	
	/*system("clear");
	cout << "\nwaiting for clients\n";
	
	vector<parameter> received_params;
	
	Socket sk;
	Communicator comm;
	comm.startListener(argv[1]);
	
	int i = 0;
	int perse = 0;
	
	while (1) {
		comm.waitForConnection(sk);
		system("clear");
		cout << "client connected [" << ++i << "|" << perse << "]\n\n";
		
		int parameters_size;
		cout << "Receiving size\n";
		if (!sk.receiveInt(parameters_size)) {
			perse++;
			break;
		}
		cout << "received\n\n";
		
		cout << "---parameters-------------\n\n";
		for (int i = 0; i < parameters_size; i++) {
			Deserializer d;
			if (!sk.receiveObject(d)) {
				perse++;
				break;
			}
			cout << d.getObject() << endl;
			//received_params.push_back(d.getObject());
			//cout << received_params[i] << endl;
		}
		cout << "--------------------------\n\n";
	}
	
	comm.closeAllCommunications();*/
	
	/*vector<parameter> parameters, received_parameters;
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	// Avvio del server
	//cout << "Set port number: ";
	//cin >> SPport;
	SPport = argv[1];
	
	Communicator comm;
	if (!comm.startListener(SPport)) return 0;
	SPaddress = comm.getIP();
	
	// Inizializzaione del servizio
	RotateService rotate;
	rotate.setServer(SPaddress, SPport);
	
	// Connessione al Service Register
	//cout << "Insert server register address: ";
	//cin >> SRaddress;
	//cout << "Insert server register port:    ";
	//cin >> SRport;
	
	//SOA global;
	//global.setServerRegister(SRaddress, SRport);
	//if (!global.serverRegistration(SPaddress, SPport)) return 0;
	//if (!global.serviceRegistration(rotate)) return 0;
	
	cout << "\nIn attesa di connessioni (" << SPaddress << ":" << SPport << ")...\n";
	while (1) {
		// In attesa di connessione con i client...
		//cout << "\nIn attesa di connessioni ("<<SPaddress<<":"<<SPport<<")...\n";
		Socket * sk = new Socket();
		comm.waitForConnection(* sk);
		if (rotate.receiveParameters(sk, received_parameters)) {
			if (parameters.size() != received_parameters.size()) {
				cout << "dimensione errata\n";
			} else
			for (int i = 0; i < (int) received_parameters.size(); i++) {
				if (parameters[i] != received_parameters[i]) {
					cout << "sbagliato\n";
					//cout << "Parameter " << i << " expected to be:\n" << parameters[i];
					//cout << "Parameter received is:\n" << d.getObject() << endl;
					break;
				}
			}
		}
		//rotate.serveRequests(sk);
		sk->closeSocket();
		delete sk;
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();*/
	
}