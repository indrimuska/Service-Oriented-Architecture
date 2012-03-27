//
//  Service.h
//  SOA
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Service_h
#define SOA_Service_h

#include <vector>
#include <iostream>

#include "types.h"
#include "Communicator.h"

using namespace std;

class Service {
private:
	string name;
	vector<param> inParameters;
	vector<param> outParameters;
	
public:
	void setService(string name, vector<param> parameters);
	bool serviceRegistration(Socket SRsocket, string SPaddres, string SPport);
	void execute();
};

#endif
