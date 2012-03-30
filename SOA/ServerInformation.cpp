/*
 * ServerInformation.cpp
 *
 *  Created on: 30/mar/2012
 *      Author: par
 */

#include "ServerInformation.h"

using namespace std;

ServerInformation::ServerInformation(string identif, string address, string port) {
	identification = identif;
	Saddress = address;
	Sport = port;

}

ServerInformation::~ServerInformation() {
	// TODO Auto-generated destructor stub
}


