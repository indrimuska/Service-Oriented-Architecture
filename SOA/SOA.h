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
