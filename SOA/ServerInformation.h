/*
 * ServerInformation.h
 *
 *  Created on: 30/mar/2012
 *      Author: par
 */

#ifndef SERVERINFORMATION_H_
#define SERVERINFORMATION_H_

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


#endif /* SERVERINFORMATION_H_ */
