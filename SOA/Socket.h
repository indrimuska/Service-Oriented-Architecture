//
//  SOA/Socket.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Socket_h
#define SOA_Socket_h

#include <netdb.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

class Socket {
private:
	int sk;
	bool serializeObject(void * object, size_t length, string &filename);
public:
	Socket();
	Socket(int sk);
	bool sendInt(int number);
	bool sendString(string s_string);
	bool sendBinary(char * binary, int length);
	bool sendObject(void * object, size_t length);
	bool sendFile(string filename);
	bool receiveInt(int &number);
	bool receiveString(string &s_string);
	bool receiveBinary(char * binary, int &length);
	bool receiveObject(void * object, int &length);
	bool operator==(const Socket &operand);
	bool closeSocket();
};

#endif
