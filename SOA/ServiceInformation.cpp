/*
 * ServiceTable.cpp
 *
 *  Created on: 30/mar/2012
 *      Author: par
 */

#include "ServiceInformation.h"

ServiceInformation::ServiceInformation(string serviceName) {

	serviceRegisteredName = serviceName;
	myServices = vector<Service>();

}

ServiceInformation::~ServiceInformation() {
	// TODO Auto-generated destructor stub
}

