//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "Application/ImageManipulation.h"

using namespace std;

int main(int argc, char ** argv) {
	
	string SPaddress, SPport;
	
	//cout << "Insert service provider address: ";
	//cin >> SPaddress;
	//cout << "Insert service provider port:    ";
	//cin >> SPport;
	SPaddress = argv[1];
	SPport = argv[2];
	
	string filename = argv[3];
	
	vector<parameter> parameters;
	//parameters.push_back(parameter(IN, INT, 90));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation horizontalFlip;
	horizontalFlip.setServer(SPaddress, SPport);
	horizontalFlip.setService("horizontal fli", parameters);
	horizontalFlip.setImageAsParameter(IN, 0, filename);
	
	cout << "Richiesta del servizio:\n";
	if (horizontalFlip.requestService()) {
		cout << "richiesta accettata\n";
		//horizontalFlip.getImageFromParameter(OUT, 0, argv[4]);
	} else cout << "richiesta rifiutata\n";
	
	cout << endl;	
}