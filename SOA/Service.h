//
//  SOA/Service.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Service_h
#define SOA_Service_h

#include <vector>
#include <iostream>

#include "types.h"
#include "Communication.h"

#define SERVICE_REQ		"C'mon server, give me this service!"
#define SERVICE_RESP	"Et-voilà, here is your service"

using namespace std;

class Service {
private:
	string name;
	vector<param> inParameters;
	vector<param> outParameters;
	
	string SPaddress;
	string SPport;
	
public:
	void setService(string name, vector<param> parameters);
	// Il server lo usa per fare richiesta di registrazione di un servizio
	// Il client lo usa per fare richiesta di un servizio
	void setServer(string SPaddress, string SPport);
	
	// Usati dai Service Provider
	bool serviceRegistration(Socket SRsocket);
	bool serviceUnRegistration(Socket SRsocket);
	bool serveRequests(Socket * sk);
	bool execute(Socket * sk);
	
	// Usati dai client
	bool requestService();
};

#endif
