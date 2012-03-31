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
	
	
	int value = 6453;
	
	//parameter() {
	//parameter(param_direction direction, param_type type) {
	//parameter(param_direction direction, param_type type, parameter_value value) {
	//void init(param_direction direction, param_type type) {
	//void setValue(parameter_value value) {
	
	parameter p(IN, INT);
	parameter_value pv((void *) &value, sizeof(int));
	p.setValue(pv);
	
	cout << p << endl;
	
	
	/*
	
	vector<param> parameters;
	parameters.push_back(param(IN, INT));
	parameters.push_back(param(IN, BUFFER));
	parameters.push_back(param(OUT, BUFFER));
	
	Socket sk;
	Communicator comm;
	comm.connectTo(argv[1], argv[2], sk);
	
	cout << "\nparameters:\n";
	cout << " - size:   " << parameters.size() << endl;
	cout << " - values: " << endl;
	
	cout << parameters[0] << endl;
	sk.sendInt((int) sizeof(parameters[0]));
	sk.sendObject((void *) &parameters[0], sizeof(parameters[0]));
	
	for (int i = 0; i < 0*(int) parameters.size(); i++) {
		cout << i+1 << ")\n" << parameters[i] << endl;
		sk.sendInt((int) sizeof(parameters[i]));
		sk.sendObject((void *) &parameters[i], sizeof(parameters[i]));
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