//
//  SOA/Communication.hpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Communication_h
#define SOA_Communication_h

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Parameters.hpp"

using namespace std;

class Socket {
private:
	int sk;
public:
	Socket();
	Socket(int sk);
	bool sendInt(int number);
	bool sendString(string s_string);
	bool sendFile(string filename);
	bool sendBinary(void * binary, size_t length);
	bool sendParameter(parameter &p);
	bool receiveInt(int &number);
	bool receiveString(string &s_string);
	bool receiveFile(string where, string &filename);
	bool receiveBinary(void * binary, size_t length);
	bool receiveParameter(parameter &p);
	bool operator==(const Socket &operand);
	bool closeSocket();
};

class Communicator {
private:
	int listenSocket;
	vector<Socket> sockets;
public:
	bool startListener(string port, int backlog_queue = 10);
	bool waitForConnection(Socket &clientSocket);
	bool waitForConnection(Socket &clientSocket, string &clientAddress);
	bool stopListener();
	bool connectTo(string address, string port, Socket &S_socket);
	bool closeCommunication(Socket socket);
	bool closeAllCommunications();
	string getIP();
};

#endif
