//
//  Application/ImageManipulation.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 07/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ImageManipulation.h"

ImageManipulation::ImageManipulation() {
	workDirectory = ".";
}
ImageManipulation::ImageManipulation(string workDirectory) {
	this->workDirectory = workDirectory;
}
bool ImageManipulation::getImageFromBuffer(parameter &p, string filename, bool hideWarning) {
	FILE * file;
	if (!hideWarning && (file = fopen(filename.c_str(), "r")) != 0) {
		fclose(file);
		char response;
		cout << "Il file '" << filename << "' esiste già. Sovrascriverlo [Y/N]? ";
		while (response != 'Y' && response != 'N' && response != 'S') {
			cin >> response;
			response = toupper(response);
			if (response == 'N') return false;
		}
	}
	if (!(file = fopen(filename.c_str(), "w"))) {
		cerr << "Impossibile creare il file richiesto\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	char * buffer = new char[p.getValueDimension()];
	p.getValue(buffer);
	size_t i = fwrite(buffer, 1, p.getValueDimension(), file);
	fclose(file);
	delete buffer;
	if (i < p.getValueDimension()) {
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
bool ImageManipulation::setImageAsParameter(parameter_direction direction, int parameter_number, string filename) {
	parameter * p;
	if (direction == IN) p = &inParameters[parameter_number];
	else p = &outParameters[parameter_number];
	return putImageInBuffer(* p, filename);
}
bool ImageManipulation::getImageFromParameter(parameter_direction direction, int parameter_number, string filename) {
	parameter * p;
	if (direction == IN) p = &inParameters[parameter_number];
	else p = &outParameters[parameter_number];
	return getImageFromBuffer(* p, filename);
}

RotateService::RotateService() {
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, INT));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	setService("rotate", parameters);
}
bool RotateService::execute(Socket * sk) {
	string inFile = workDirectory + "/source.gif";
	string outFile = workDirectory + "/rotated.gif";
	getImageFromBuffer(inParameters[1], inFile, true);
	int degrees;
	inParameters[0].getValue(degrees);
	CImg<unsigned char> image(inFile.c_str());
	image.rotate(degrees).save(outFile.c_str());
	putImageInBuffer(outParameters[0], outFile);
	if (remove(inFile.c_str()) || remove(outFile.c_str())) {
		cerr << "Impossibile rimuovere i file temporanei\n";
		return false;
	}
	return true;
}

HorizontalFlipService::HorizontalFlipService() {
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	setService("horizontal flip", parameters);
}
bool HorizontalFlipService::execute(Socket * sk) {
	Response response;
	string inFile = workDirectory + "/source.gif";
	string outFile = workDirectory + "/flipped.gif";
	getImageFromBuffer(inParameters[0], inFile, true);
	CImg<unsigned char> image(inFile.c_str());
	image.mirror('x').save(outFile.c_str());
	putImageInBuffer(outParameters[0], outFile);
	if (remove(inFile.c_str()) || remove(outFile.c_str())) {
		cerr << "Impossibile rimuovere i file temporanei\n";
		return false;
	}
	return true;
}