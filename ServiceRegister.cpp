/**
 * @file	ServiceRegister.cpp
 * @brief	Registro dei servizi.
 *
 * @date	11/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#include "Application/Threads.hpp"
#include "Application/ServiceRegister.hpp"

/**
 * @define	NUM_THREADS
 * @brief	Numero di thread del service provider
 */
#define NUM_THREADS 10

/**
 * @brief	Corpo di ogni thread del service register
 * @param	thread	Informazioni relative al thread
 * @param	SR		Identificativo del service register
 */
void threadMain(ThreadInfo * thread, ServiceRegister * SR);

/**
 * @brief	Corpo del processo principale del service register
 * @param	argc	Numero di parametri di ingresso
 * @param	argv	Array di parametri di ingresso
 */
int main(int argc, char ** argv) {
	
	string address, port;
	
	if (argc != 2) {
		cout << "Porta del Service Register: ";
		cin >> port;
		cout << endl;
	} else port =  argv[1];
	
	Communicator comm;
	address = comm.getIP();
	ServiceRegister SR(address, port);
	if (!comm.startListener(port)) return 0;
	
	// Avvio dei thread (forks)
	ThreadInfo threadsInfo[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) boost::thread(threadMain, &threadsInfo[i], &SR);
	
	cout << "\033[4mSERVICE REGISTER                             " << address << ":" << port << "\033[0m\n\n";
	cout << "In attesa di connessioni...\n\n";
	
	while (1) {
		Socket sk;
		if (!comm.waitForConnection(sk)) continue;
		for (int i = 0; i < NUM_THREADS; i++)
			if (threadsInfo[i].testAndSet()) {
				threadsInfo[i].client = sk;
				threadsInfo[i].startThread();
				break;
			}
	}
	
	// Chiusura di tutte le connessioni
	comm.closeAllCommunications();
}

void threadMain(ThreadInfo * thread, ServiceRegister * SR) {
	while (1) {
		thread->waitStart();
		SR->serveRequest(&thread->client);
		thread->client.closeSocket();
		thread->setFree();
	}
}
