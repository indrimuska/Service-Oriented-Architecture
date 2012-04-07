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
	
	string filename = argv[3];
	struct stat info;
	if (stat(filename.c_str(), &info) == -1) {
		cerr << "Impossibile determinare la dimensione del file specificato\n"
		"Controllare che il file esista e che si abbiano i permessi necessari\n";
		return false;
	}
	int dimension = static_cast<int>(info.st_size);
	FILE * file;
	if (!(file = fopen(filename.c_str(), "r"))) {
		cerr << "Impossibile aprire il file specificato\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	char * content = (char *) malloc(dimension);
	if ((int) fread(content, 1, dimension, file) < dimension) {
		cerr << "Impossibile leggere il contenuto del file\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	fclose(file);
	
	//int degrees = 90;
	vector<parameter> parameters;
	//parameters.push_back(parameter(IN, INT, degrees));
	parameters.push_back(parameter(IN, BUFFER, content, dimension));
	parameters.push_back(parameter(OUT, BUFFER));
	
	delete content;
	
	Service rotate;
	rotate.setServer(SPaddress, SPport);
	rotate.setService("rotate", parameters);
	
	cout << "Richiesta del servizio ROTATE:\n";
	if (rotate.requestService()) cout << "richiesta accettata\n";
	else cout << "richiesta rifiutata\n";
	
	cout << endl;	
}