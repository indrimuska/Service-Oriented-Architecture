//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/Service.h"

using namespace std;

int main(int argc, char ** argv) {
	
	string SPaddress, SPport;
	
	//cout << "Insert service provider address: ";
	//cin >> SPaddress;
	//cout << "Insert service provider port:    ";
	//cin >> SPport;
	SPaddress = argv[1];
	SPport = argv[2];
	
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	Service rotate;
	rotate.setServer(SPaddress, SPport);
	rotate.setService("rotate", parameters);
	
	cout << "Richiesta del servizio ROTATE:\n";
	if (rotate.requestService()) cout << "richiesta accettata\n";
	else cout << "richiesta rifiutata\n";
	
	cout << endl;	
}