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

#include "Parameters.h"
#include "Communication.h"

#define SERVICE_REQ		"C'mon server, give me this service!"
#define SERVICE_RESP	"Et-voilà, here is your service"

using namespace std;

class Response {
private:
	bool result;
	string message;
	string successfulMessage;
public:
	Response(string successfulMessage);
	void setResult(bool result);
	void setMessage(string message);
	void setError(string error);
	bool getResult();
	string getMessage();
};

class Service {
private:
	Response * response;
protected:
	string SPaddress;
	string SPport;
	string name;
	vector<parameter> inParameters;
	vector<parameter> outParameters;
	
	bool sendResponse(Socket &sk);
	bool receiveResponse(Socket &sk);
	bool sendParameters(Socket &serviceProvider);
	bool receiveParameters(Socket * sk, vector<parameter> &parameters);
	
	// Da implmentare nell'applicazione
	virtual bool execute(Socket * sk);
public:
	
	Service();
	
	void setService(string name, vector<parameter> &parameters);
	// Il server lo usa per fare richiesta di registrazione di un servizio
	// Il client lo usa per fare richiesta di un servizio
	void setServer(string SPaddress, string SPport);
	
	// Usati dai Service Provider
	bool serviceRegistration(Socket SRsocket);
	bool serviceUnRegistration(Socket SRsocket);
	bool serveRequests(Socket * sk);
	
	// Usati dai client
	bool requestService();
};

#endif
