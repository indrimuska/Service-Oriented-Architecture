//
//  Socket.h
//  SOA
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Socket_h
#define SOA_Socket_h

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

class Socket {
private:
	int sk;
public:
	Socket();
	Socket(int sk);
	bool sendInt(int number);
	bool sendString(string s_string);
	bool sendBinary(char * binary, int length);
	bool receiveInt(int &number);
	bool receiveString(string &s_string);
	bool receiveBinary(char * binary, int &length);
	bool operator==(const Socket &operand);
	bool closeSocket();
};

#endif
