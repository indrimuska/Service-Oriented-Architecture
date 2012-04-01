//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/params.cpp"
#include "SOA/Service.h"

using namespace std;

int main(int argc, char ** argv) {
	//parameter p(IN, BUFFER, parameter_value(pino));
	
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	Socket sk;
	Communicator comm;
	comm.connectTo(argv[1], argv[2], sk);
	
	cout << "---parameters-------------\n\n";
	for (int i = 0; i < (int) parameters.size(); i++) cout << parameters[i] << endl;
	cout << "--------------------------\n\n";
	
	cout << "Sending parameter no. 1\n";
	for (int i = 0; i < 1 + 0*(int) parameters.size(); i++) {
		void * parameter_serialized;
		size_t parameter_serialized_length;
		parameters[i].serialize(parameter_serialized, parameter_serialized_length);
		sk.sendObject(parameter_serialized, parameter_serialized_length);
		parameters[i].serializeEnd(parameter_serialized);
	}
	
	comm.closeAllCommunications();
	
	/************
	
	string SPaddress, SPport;
	
	//cout << "Insert service provider address: ";
	//cin >> SPaddress;
	//cout << "Insert service provider port:    ";
	//cin >> SPport;
	SPaddress = argv[1];
	SPport = argv[2];
	
	Service rotate;
	rotate.setServer(SPaddress, SPport);
	
	vector<param> parameters;
	parameters.push_back(param(IN, INT));
	parameters.push_back(param(IN, BUFFER));
	parameters.push_back(param(OUT, BUFFER));
	rotate.setService("rotate", parameters);
	
	rotate.requestService();*/
	
}