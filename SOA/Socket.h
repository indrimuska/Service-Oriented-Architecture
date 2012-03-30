//
//  SOA/Socket.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Socket_h
#define SOA_Socket_h

#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
	bool sendObject(void * object, size_t length);
	bool receiveInt(int &number);
	bool receiveString(string &s_string);
	bool receiveFile(string where, string &filename);
	bool receiveObject(void * object, size_t length);
	bool operator==(const Socket &operand);
	bool closeSocket();
};

#endif
