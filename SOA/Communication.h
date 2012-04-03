//
//  SOA/Communication.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Communication_h
#define SOA_Communication_h

#include <time.h>
#include <vector>
#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Serialization.h"

using namespace std;

class Socket {
private:
	int sk;
	
	bool serializeObject(void * object, size_t length, string &filename);
	bool deserializeObject(void * object, size_t length, string filename);
public:
	Socket();
	Socket(int sk);
	bool sendInt(int number);
	bool sendString(string s_string);
	bool sendFile(string filename);
	bool sendBinary(void * binary, size_t length);
	bool sendObject(Serializer &s);
	bool receiveInt(int &number);
	bool receiveString(string &s_string);
	bool receiveFile(string where, string &filename);
	bool receiveBinary(void * binary, size_t &length);
	bool receiveObject(Deserializer &d);
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
	bool stopListener();
	bool connectTo(string address, string port, Socket &S_socket);
	bool closeCommunication(Socket socket);
	bool closeAllCommunications();
	string getIP();
};

#endif
