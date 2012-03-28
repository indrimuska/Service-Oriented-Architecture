//
//  SOA/Communicator.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Communicator_h
#define SOA_Communicator_h

#include <vector>

#include "Socket.h"

using namespace std;

class Communicator {
private:
	int listenSocket;
	vector<Socket> sockets;
public:
	bool startListener(string port, int backlog_queue = 10);
	bool waitForClients(Socket &clientSocket);
	bool stopListener();
	bool connectTo(string address, string port, Socket &S_socket);
	bool closeCommunication(Socket socket);
	bool closeAllCommunications();
	string getIP();
};

#endif
