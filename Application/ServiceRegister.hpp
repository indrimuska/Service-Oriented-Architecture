//
//  Application/ServiceRegister.hpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 15/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef Application_ServiceRegister_h
#define Application_ServiceRegister_h

#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "../SOA/SOA.hpp"
#include "../SOA/Communication.hpp"

using namespace std;

class ServiceRegister {
private:
	string address;
	string port;
	vector<string> servers;
	map<string, vector<string> > services;
	
	bool sendAck(Socket * sk, string ack);
	bool splitServerName(string serverName, string &address, string &port);
	bool splitServiceName(string serviceName, string &address, string &port, string &service);
	bool isServerRegistred(string serverName);
	bool isServiceRegistred(string serviceName);
	void deleteServerFromServersVector(string service, string serverName);
public:
	ServiceRegister(string address, string port);
	bool serveRequest(Socket * sk);
	bool confirmConnection(Socket * sk);
	bool serverRegistration(Socket * sk);
	bool serviceRegistration(Socket * sk);
	bool serverUnRegistration(Socket * sk);
	bool serviceUnRegistration(Socket * sk);
	bool serversDisplay(Socket * sk);
	bool servicesDisplay(Socket * sk);
	bool serverRequest(Socket * sk);
};

#endif
