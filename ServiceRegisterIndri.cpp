//
//  ServiceRegisterIndri.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Application/Threads.hpp"
#include "Application/ServiceRegister.hpp"

#define NUM_THREADS 10

void threadMain(ThreadInfo * thread, ServiceRegister * SR);

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
		if (!SR->serveRequest(&thread->client)) continue;
		thread->client.closeSocket();
		thread->setFree();
	}
}