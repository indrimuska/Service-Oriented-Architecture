//
//  Application/ImageManipulation.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 07/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ImageManipulation.h"

bool ImageManipulation::getImageFromBuffer(parameter &p, string filename) {
	char * buffer = new char[p.getValueDimension()];
	p.getValue(buffer);
	FILE * file;
	if (!(file = fopen(filename.c_str(), "w"))) {
		cerr << "Impossibile creare il file richiesto\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	int i = (int) fwrite(buffer, 1, p.getValueDimension(), file);
	fclose(file);
	delete buffer;
	if (i < (int) p.getValueDimension()) {
		cerr << "Impossibile salvare il contenuto del file richiesto\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	return true;
}
bool ImageManipulation::putImageInBuffer(parameter &p, string filename) {
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
	char * buffer = new char[dimension];
	if ((int) fread(buffer, 1, dimension, file) < dimension) {
		cerr << "Impossibile leggere il contenuto del file\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	fclose(file);
	p.setValue(buffer, dimension);
	delete buffer;
	return true;
}