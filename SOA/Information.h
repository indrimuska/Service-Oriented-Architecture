/*
 * ServerInformation.h
 *
 *  Created on: 30/mar/2012
 *      Author: par
 */

#ifndef INFORMATION_H_
#define INFORMATION_H_

#include <iostream>
#include <string.h>

using namespace std;

class ServerInformation {




public:
	string identification;
	string Saddress;
	string Sport;
	ServerInformation(string, string, string);
	virtual ~ServerInformation();
};


class ServiceInformation {
private:
	string serviceRegisteredName;
	vector<Service> myServices;

public:
	ServiceInformation(string serviceName);
	bool addService();
	virtual ~ServiceInformation();
};

#endif /* INFORMATION_H_ */
