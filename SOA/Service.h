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
	vector<parameter> parameters;
public:
	Response();
	void setResult(bool result);
	void setError(string error);
	void setMessage(string message);
	bool getResult();
	string getMessage();
	vector<parameter>& getParameters();
	void setParameters(vector<parameter> &parameters);
};

class Service {
protected:
	string SPaddress;
	string SPport;
	string name;
	vector<parameter> inParameters;
	vector<parameter> outParameters;
	
	bool sendResponse(Socket &sk, Response &response);
	bool receiveResponse(Socket &sk, Response &response);
	bool sendParameters(Socket &serviceProvider, vector<parameter> &parameters);
	bool receiveParameters(Socket * sk, vector<parameter> &parameters);
	
	// Da implmentare nell'applicazione
	virtual bool execute(Socket * sk);
public:
	void setName(string name);
	void setParameters(vector<parameter> &parameters);
	void setService(string name, vector<parameter> &parameters);
	string getServiceName();
	
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
