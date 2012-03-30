/*
 * ServiceTable.h
 *
 *  Created on: 30/mar/2012
 *      Author: par
 */

#ifndef SERVICEINFORMATION_H_
#define SERVICEINFORMATION_H_

#include <vector>
#include <iostream>

#include "Service.h"
#include "Communicator.h"

class ServiceInformation {
private:
	string serviceRegisteredName;
	vector<Service> myServices;

public:
	ServiceInformation(string serviceName);
	bool addService();
	virtual ~ServiceInformation();
};

#endif /* SERVICEINFORMATION_H_ */
