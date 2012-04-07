//
//  Application/RotateService.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <fstream>
#include <iostream>

#include "../SOA/Service.h"
#include "CImg.h"

using namespace cimg_library;

class RotateService : public Service {
public:
	RotateService() {
		vector<parameter> parameters;
		parameters.push_back(parameter(IN, INT));
		parameters.push_back(parameter(IN, BUFFER));
		parameters.push_back(parameter(OUT, BUFFER));
		this->setService("rotate", parameters);
	};
	bool execute(Socket * sk) {
		string filename = "temp.gif";
		string outfile = "rotated.gif";
		char * buffer = new char[inParameters[1].getValueDimension() + 1];
		memset(buffer, '\0', inParameters[1].getValueDimension() + 1);
		inParameters[1].getValue(buffer);
		buffer[inParameters[1].getValueDimension()] = '\0';
		FILE * file;
		if (!(file = fopen(filename.c_str(), "w"))) {
			cerr << "Impossibile creare il file richiesto\n"
			"Controllare di avere i permessi necessari\n";
			return false;
		}
		int i = (int) fwrite(buffer, 1, inParameters[1].getValueDimension(), file);
		fclose(file);
		delete buffer;
		if (i < (int) inParameters[1].getValueDimension()) {
			cerr << "Impossibile salvare il contenuto del file richiesto\n"
			"Controllare di avere i permessi necessari\n";
			return false;
		}
		int degrees;
		inParameters[0].getValue(degrees);
		CImg<unsigned char> image(filename.c_str());
		image.rotate(degrees).save(outfile.c_str());
		if (remove(filename.c_str())) {
			cerr << "Impossibile rimuovere l'immagine temporanea\n";
			return false;
		}
		
		// TODO: send image trought socket using class RESPONSE (must be created)
		
		// TODO: cambiare tutte le setValue/getValue usando l'operatore '='
		
		return true;
	}
};