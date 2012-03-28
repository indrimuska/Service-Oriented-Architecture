//
//  SOA/SOA.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 28/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_SOA_h
#define SOA_SOA_h

#include "Communicator.h"
#include "Service.h"

#define CONN_ACK_REQ	"Could you give an ACK for the connection, please?"
#define CONN_ACK_RESP	"This is a simple ACK packet"
#define SRV_REG_REQ		"I'd like me to register"
#define SRV_REG_RESP	"Ok, now you're registred"
#define SRC_REG_REQ		"Hey, I have a new service for any client!"
#define SRC_REG_RESP	"Well, your service is now registred"
#define SRV_DEREG_REQ	"I don't want to serv clients anymore"
#define SRV_DEREG_RESP	"Starting from now, you will not contacted"
#define SRC_DEREG_REQ	"Sorry, I can't give a service"
#define SRC_DEREG_RESP	"Don't worry, you'll not asked for that service"

//#define SOA_MESSAGES enum { CONN_ACK_REQ, CONN_ACK_RESP, SRV_REG_REQ, SRV_REG_RESP, SRC_REG_REQ, SRC_REG_RESP, SRV_DEREG_REQ, SRV_DEREG_RESP, SRC_DEREG_REQ, SRC_DEREG_RESP };

using namespace std;

class SOA {
private:
	string SRaddres;
	string SRport;
	Communicator comm;
	
public:
	bool setServerRegister(string SPaddres, string SPport);
	bool serviceRegistration(Service s);
};

#endif
