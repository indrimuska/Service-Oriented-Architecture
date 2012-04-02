//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "SOA/Serialization.h"
#include "SOA/Service.h"

using namespace std;

int main(int argc, char ** argv) {
	
	double pippo = 0987654324567890.98765435678;
	string gigi = "Io mi chiamo gigi";
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, STRING, parameter_value(gigi)));
	parameters.push_back(parameter(IN, STRING));
	parameters.push_back(parameter(OUT, STRING));
	parameters.push_back(parameter(OUT, DOUBLE, parameter_value(pippo)));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(OUT, INT));
	
	Socket sk;
	Communicator comm;
	comm.connectTo(argv[1], argv[2], sk);
	
	cout << "Sending size\n";
	sk.sendInt((int) parameters.size());
	cout << "sent\n\n";
	
	cout << "---parameters-------------\n\n";
	for (int i = 0; i < (int) parameters.size(); i++) {
		cout << parameters[i] << endl;
		Serializer s(parameters[i]);
		sk.sendObject(s);
	}
	cout << "--------------------------\n\n";
	
	cout << "closing all communications\n";
	comm.closeAllCommunications();
	cout << "done\n\n";
	
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