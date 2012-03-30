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
private:
	string Saddress;
	string Sport;

public:
	ServerInformation(string, string);
	virtual ~ServerInformation();
};


#endif /* SERVERINFORMATION_H_ */
