//
//  SOA/SOA.hpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_SOA_h
#define SOA_SOA_h

#include "Service.hpp"
#include "Communication.hpp"

#define CONN_ACK_REQ	"Could you give an ACK for the connection, please?"
#define CONN_ACK_RESP	"This is a simple ACK packet"
#define SRV_REG_REQ		"I'd like me to register"
#define SRV_REG_RESP	"Ok, now you're registred"
#define SRV_REG_DISP    "I want to know who is registered"
#define SRC_REG_REQ		"Hey, I have a new service for any client!"
#define SRC_REG_RESP	"Well, your service is now registred"
#define SRC_REG_DISP    "I want to know the available services"
#define SRV_UNREG_REQ	"I don't want to serv clients anymore"
#define SRV_UNREG_RESP	"Starting from now, you will not contacted"
#define SRC_UNREG_REQ	"Sorry, I can't give a service"
#define SRC_UNREG_RESP	"Don't worry, you'll not asked for that service again"
#define SRV_REQ			"I need the address of one Service Provider that support this service"
#define SRV_RESP		"This is the address of the SP you've requested"

using namespace std;

class SOA {
private:


	bool sendRequest(string request, Socket &SRsocket);
public:
	string SRaddress;
	string SRport;
	string SPaddress;
	string SPport;
	Communicator comm;
	bool setServiceRegister(string SRaddres, string SRport);
	void setServiceProvider(string SPaddres, string SPport);
	bool serverRegistration();
	bool serviceRegistration(Service &s);
	bool serverUnRegistration();
	bool serviceUnRegistration(Service &s);
	bool getServerAddress(Service &s, string &address, string &port);
};

#endif
