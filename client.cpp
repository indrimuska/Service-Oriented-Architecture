//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "Application/ImageManipulation.h"
#include "Application/ImageStoring.h"

using namespace std;

int main(int argc, char ** argv) {
	
	string SPaddress, SPport;
	
	//cout << "Insert service provider address: ";
	//cin >> SPaddress;
	//cout << "Insert service provider port:    ";
	//cin >> SPport;
	SPaddress = argv[1];
	SPport = argv[2];
	
	cout << endl;
	vector<parameter> parameters;
	
	/*// Inizializzazione del serivzio ROTATE
	parameters.clear();
	parameters.push_back(parameter(IN, INT, 15));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation rotate;
	rotate.setServer(SPaddress, SPport);
	rotate.setService("rotate", parameters);
	rotate.setImageAsParameter(IN, 1, argv[3]);
	
	cout << "Richiesta del servizio ROTATE:\n";
	if (rotate.requestService()) {
		cout << "richiesta accettata\n";
		if (!rotate.getImageFromParameter(OUT, 0, argv[4])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[4] << "'\n";
	} else cout << "richiesta rifiutata\n";
	
	cout << endl;
	
	// Inizializzazione del servizio HORIZONTAL FLIP
	parameters.clear();
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation horizontalFlip;
	horizontalFlip.setServer(SPaddress, SPport);
	horizontalFlip.setService("horizontal flip", parameters);
	horizontalFlip.setImageAsParameter(IN, 0, argv[3]);
	
	cout << "Richiesta del servizio HORIZONTAL FLIP:\n";
	if (horizontalFlip.requestService()) {
		cout << "richiesta accettata\n";
		if (!horizontalFlip.getImageFromParameter(OUT, 0, argv[5])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[5] << "'\n";
	} else cout << "richiesta rifiutata\n";
	
	cout << endl;*/
	
	// Inizializzazione del servizio STORE IMAGE
	parameters.clear();
	parameters.push_back(parameter(IN, STRING, argv[3]));
	parameters.push_back(parameter(IN, BUFFER));
	
	ImageStoring storeImage;
	storeImage.setServer(SPaddress, SPport);
	storeImage.setService("store image", parameters);
	storeImage.setImageAsParameter(IN, 1, argv[3]);
	
	cout << "Richiesta del servizio STORE IMAGE:\n";
	if (storeImage.requestService()) cout << "richiesta accettata\n";
	else cout << "richiesta rifiutata\n";
	
	cout << endl;
	
	// Inizializzazione del servizio GET IMAGE
	parameters.clear();
	parameters.push_back(parameter(IN, STRING, argv[3]));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageStoring getImage("Clients");
	getImage.setServer(SPaddress, SPport);
	getImage.setService("get image", parameters);
	
	cout << "Richiesta del servizio GET IMAGE:\n";
	if (getImage.requestService()) {
		cout << "richiesta accettata\n";
		if (!getImage.getImageFromParameter(OUT, 0, argv[4])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[4] << "'\n";
	} else cout << "richiesta rifiutata\n";
	
	cout << endl;
	
	// Inizializzazione del servizio GET LIST
	parameters.clear();
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageStoring getList;
	getList.setServer(SPaddress, SPport);
	getList.setService("get list", parameters);
	
	cout << "Richiesta del servizio GET LIST:\n";
	if (getList.requestService()) cout << "richiesta accettata:\n" << getList.getStringFromParameter(OUT, 0) << endl;
	else cout << "richiesta rifiutata\n";
	
	cout << endl;
}