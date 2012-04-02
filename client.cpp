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
	//parameter p(IN, BUFFER, parameter_value(pino));
	
	/*string pippo = "io so' pippo, tu?";
	parameter p(IN, STRING, parameter_value(pippo));
	cout << p << endl;
	
	Serializer2 s(p);
	Deserializer2 d(s.getSerialized(), s.getLength());
	cout << d.getObject() << endl;
	
	Deserializer2 d1 = d;
	cout << d1.getObject() << endl;*/
	
	string pippo = "io so' pippo, tu?";
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, STRING, parameter_value(pippo)));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	for (int i = 0; i < 10; i++) {
		Serializer2 s(parameters[1]);
		Deserializer2 d(s.getSerialized(), s.getLength());
		if (parameters[1] != d.getObject()) cout << "NON SONO UGUALI (" << i << ")\n";
	}
	
	/*string gigi = "Io mi chiamo gigi";
	parameter p(IN, STRING, parameter_value(gigi));
	cout << endl << p << endl;
	
	Serializer2 s(p);
	
	Deserializer2 d(s.getSerialized(), s.getLength());
	cout << d.getObject() << endl;
	
	//Socket sk;
	//Communicator comm;
	//comm.connectTo(argv[1], argv[2], sk);
	//sk.sendObject(s);
	
	
	
	/*string gigi = "Io mi chiamo gigi";
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, STRING, parameter_value(gigi)));
	//parameters.push_back(parameter(IN, BUFFER));
	//parameters.push_back(parameter(OUT, BUFFER));
	
	Socket sk;
	Communicator comm;
	comm.connectTo(argv[1], argv[2], sk);
	
	cout << "Sending size\n";
	sk.sendInt((int) parameters.size());
	cout << "sent\n\n";
	
	cout << "---parameters-------------\n\n";
	for (int i = 0; i < (int) parameters.size(); i++) {
		cout << parameters[i] << endl;
		Serializer2 s(parameters[i]);
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