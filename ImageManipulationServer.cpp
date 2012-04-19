/**
 * @file	ImageManipulationServer.cpp
 * @brief	Service Provider multi-threaded per la manipolazione delle immagini.
 *
 * @date	14/03/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#include <iostream>

#include "SOA/SOA.hpp"
#include "Application/Threads.hpp"
#include "Application/ImageManipulation.hpp"

/**
 * @define	NUM_THREADS
 * @brief	Numero di thread del service provider
 */
#define NUM_THREADS 10

/**
 * @brief	Corpo del thread che gestisce l'interazione
 * @param	server		Informazioni relative al server
 * @param	comm		Comunicatore del server
 * @param	global		Informazioni generali dell'applicazione S.O.A.
 * @param	storeImage	Informazioni relative al servizio store image
 * @param	getImage	Informazioni relative al servizio get image
 * @param	getList		Informazioni relative al servizio get list
 */
void controlThread(ThreadInfo * server, Communicator * comm, SOA * global, RotateService * rotate, HorizontalFlipService * horizontalFlip);

/**
 * @brief	Corpo di ogni thread del service provider
 * @param	thread			Informazioni relative al thread
 * @param	rotate			Informazioni relative al servizio rotate
 * @param	horizontalFlip	Informazioni relative al servizio horizontalFlip
 */
void executionThread(ThreadInfo * thread, RotateService * rotate, HorizontalFlipService * horizontalFlip);

/**
 * @brief	Corpo del processo principale del service register
 * @param	argc	Numero di parametri di ingresso
 * @param	argv	Array di parametri di ingresso
 */
int main(int argc, char ** argv) {
	
	string SPaddress, SPport;
	string SRaddress, SRport;
	
	if (argc < 2) {
		cout << "Numero di porta: ";
		cin >> SPport;
		cout << endl;
	} else SPport = argv[1];
	
	Communicator comm;
	SPaddress = comm.getIP();
	if (!comm.startListener(SPport)) return 0;
	
	if (argc != 4) {
		cout << "Indirizzo del Service Register : ";
		cin >> SRaddress;
		cout << "Porta del Service Register     : ";
		cin >> SRport;
		cout << endl;
	} else {
		SRaddress = argv[2];
		SRport = argv[3];
	}
	
	// Inizializzaione dei servizi
	RotateService rotate;
	HorizontalFlipService horizontalFlip;
	
	SOA global;
	global.setServiceProvider(SPaddress, SPport);
	if (!global.setServiceRegister(SRaddress, SRport)) return 0;
	if (!global.serverRegistration()) return 0;
	if (!global.serviceRegistration(rotate)) return 0;
	if (!global.serviceRegistration(horizontalFlip)) return 0;
	
	// Avvio dei thread
	boost::thread_group threads;
	ThreadInfo controlThreadInfo, threadsInfo[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) threads.add_thread(new boost::thread(executionThread, &threadsInfo[i], &rotate, &horizontalFlip));
	
	cout << "\033[4mIMAGE MANIPULATION SERVER                   " << SPaddress << ":" << SPport << "\033[0m\n\n";
	cout << "In attesa di connessioni...\n\n";
	
	// Avvio del thread per il controllo dei comandi
	boost::thread(controlThread, &controlThreadInfo, &comm, &global, &rotate, &horizontalFlip);
	
	while (controlThreadInfo.isActive()) {
		Socket sk;
		string client;
		comm.waitForConnection(sk, client);
		if (!controlThreadInfo.isActive()) break;
		printf("Il client %s si è connesso", client.c_str());
		for (int i = 0; i < NUM_THREADS; i++)
			if (threadsInfo[i].testAndSet()) {
				threadsInfo[i].client = sk;
				threadsInfo[i].startThread();
				break;
			}
	}
	
	// Terminazione di tutti i thread
	for (int i = 0; i < NUM_THREADS; i++) threadsInfo[i].exitThread();
	threads.join_all();
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
	
	cout << "Server chiuso\n\n";
}

void controlThread(ThreadInfo * server, Communicator * comm, SOA * global, RotateService * rotate, HorizontalFlipService * horizontalFlip) {
	bool serverRegistered = true;
	while (server->isActive()) {
		bool result = false;
		string command = "", operand = "";
		cout << "> " << flush;
		getline(cin, command);
		operand = command.substr(command.find(' ')+1);
		command = command.substr(0, command.find_first_of(' '));
		if (!command.compare("reg-server")) {
			result = global->serverRegistration();
			serverRegistered = true;
		} else
		if (!command.compare("reg-service")) {
			if (!operand.compare("rotate")) result = global->serviceRegistration(* rotate); else
			if (!operand.compare("horizontal flip")) result = global->serviceRegistration(* horizontalFlip); else
			printf("Servizio %s sconosciuto\n", operand.c_str());
		} else
		if (!command.compare("dereg-server")) {
			result = global->serverUnRegistration();
			serverRegistered = false;
		} else
		if (!command.compare("dereg-service")) {
			if (!operand.compare("rotate")) result = global->serviceUnRegistration(* rotate); else
			if (!operand.compare("horizontal flip")) result = global->serviceUnRegistration(* horizontalFlip); else
			printf("Servizio %s sconosciuto\n", operand.c_str());
		} else
		if (!command.compare("quit")) {
			if (serverRegistered) global->serverUnRegistration();
			server->exitThread();
			comm->stopListener();
		} else {
			cout << "Comando sconosciuto\n\n";
			continue;
		}
		if (result) cout << "Comando eseguito\n";
		cout << endl;
	}
}

void executionThread(ThreadInfo * thread, RotateService * rotate, HorizontalFlipService * horizontalFlip) {
	static int i = 0; i++;
	while (thread->isActive()) {
		bool result;
		string service;
		thread->waitStart();
		if (!thread->isActive()) break;
		if (!thread->client.receiveString(service)) continue;
		printf(" e richiede il servizio \033[1;34m%s\033[0m\n", service.c_str());
		if (!service.compare("rotate")) result = rotate->serveRequest(&thread->client); else
								result = horizontalFlip->serveRequest(&thread->client);
		if (result) cout << "Richiesta servita\n";
		cout << "\n> " << flush;
		thread->client.closeSocket();
		thread->setFree();
	}
}
