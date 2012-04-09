//
//  Application/ImageStoring.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 09/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ImageStoring.h"

ImageStoring::ImageStoring() {
	workDirectory = "Servers/ImageStoringServer/";
}
ImageStoring::ImageStoring(string workDirectory) {
	this->workDirectory = workDirectory;
}
bool ImageStoring::getImageFromBuffer(parameter &p, string filename, bool hideWarning) {
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
bool ImageStoring::putImageInBuffer(parameter &p, string filename) {
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
bool ImageStoring::setImageAsParameter(parameter_direction direction, int parameter_number, string filename) {
	parameter * p;
	if (direction == IN) p = &inParameters[parameter_number];
	else p = &outParameters[parameter_number];
	return putImageInBuffer(* p, filename);
}
bool ImageStoring::getImageFromParameter(parameter_direction direction, int parameter_number, string filename) {
	parameter * p;
	if (direction == IN) p = &inParameters[parameter_number];
	else p = &outParameters[parameter_number];
	return getImageFromBuffer(* p, filename);
}
string ImageStoring::getStringFromParameter(parameter_direction direction, int parameter_number) {
	parameter * p;
	if (direction == IN) p = &inParameters[parameter_number];
	else p = &outParameters[parameter_number];
	string string_value;
	p->getValue(string_value);
	return string_value;
}

StoreImageService::StoreImageService() {
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, STRING));
	parameters.push_back(parameter(IN, BUFFER));
	setService("store image", parameters);
}
bool StoreImageService::execute(Socket * sk) {
	string filename;
	inParameters[0].getValue(filename);
	return getImageFromBuffer(inParameters[1], workDirectory + filename, true);
}

GetImageService::GetImageService() {
	vector<parameter> parameters;
	parameters.push_back(parameter(IN, STRING));
	parameters.push_back(parameter(OUT, BUFFER));
	setService("get image", parameters);
}
bool GetImageService::execute(Socket * sk) {
	string filename;
	inParameters[0].getValue(filename);
	return putImageInBuffer(outParameters[0], filename);
}

GetListService::GetListService() {
	vector<parameter> parameters;
	parameters.push_back(parameter(OUT, STRING));
	setService("get list", parameters);
}
bool GetListService::execute(Socket * sk) {
	DIR * directory;
	string directory_list;
	struct stat file_info;
	struct dirent * dir_info;
	if (!(directory = opendir(workDirectory.c_str()))) {
		cerr << "Errore durante la lettura della directory\n"
		"Controllare di avere i permessi necessari\n";
		return false;
    }
	while ((dir_info = readdir(directory))) {
		if (dir_info->d_name[0] == '.') continue;
		if (stat((workDirectory + dir_info->d_name).c_str(), &file_info)) continue;
		if (S_ISDIR(file_info.st_mode)) continue;
		directory_list += string("\n") + dir_info->d_name;
	}
	outParameters[0].setValue(directory_list.substr(1, directory_list.length()));
	closedir(directory);
	return true;
}