//
//  SOA/Serialization.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Serialization_h
#define SOA_Serialization_h

#include <time.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "Parameters.h"

using namespace std;

class Serializer {
private:
	void * buffer;
	size_t length;
public:
	Serializer(parameter p);
	void * getSerialized();
	void * getSerialized(size_t &legth);
	size_t getLength();
	~Serializer();
};

class Deserializer {
private:
	void * buffer;
	size_t length;
public:
	Deserializer();
	Deserializer(const Deserializer &d);
	Deserializer(void * buffer, size_t length);
	parameter getObject();
	Deserializer& operator=(const Deserializer &d);
	~Deserializer();
};

class Serializer2 {
private:
	string filename;
	size_t length;
	
	string findName(string source) {
		string filename = source;
		for (int i = 1; FILE * file = fopen(filename.c_str(), "r"); i++) {
			fclose(file);
			stringstream s;
			if (i == 0) s << filename << i;
			else s << filename.substr(0, filename.length()-2) << i;
			filename = s.str();
		}
		return filename;
	}
	void fileCopy(string filename) {
		ifstream source(filename.c_str(), std::ios::binary);
		ofstream destination(this->filename.c_str(), std::ios::binary);
		destination << source.rdbuf();
		if (FILE * file = fopen(this->filename.c_str(), "r")) fclose(file);
		else cerr << "Errore durante il recupero dell'oggetto da de-serializzare\n";
	}
public:
	Serializer2(parameter p) {
		filename = findName("parameter_serialized");
		length = sizeof(parameter_direction) + sizeof(parameter_type) + p.getValueDimension();
		parameter_direction direction;
		parameter_type type;
		ofstream file;
		file.open(filename.c_str(), ios_base::binary);
		if (!file.good()) cerr << "Errore durante la serializzazione dell'oggetto\n";
		p.getInfo(direction, type);
		file.write((char *) &direction, sizeof(parameter_direction));
		file.write((char *) &type, sizeof(parameter_type));
		if (p.getValueDimension()) {
			void * buffer = malloc(p.getValueDimension());
			p.getValue(buffer);
			file.write((char *) buffer, sizeof(type));
		}
		file.close();
	}
	string getSerialized() {
		return filename;
	}
	string getSerialized(size_t &length) {
		length = this->length;
		return filename;
	}
	size_t getLength() {
		return length;
	}
	~Serializer2() {
		if (FILE * file = fopen(filename.c_str(), "r")) {
			fclose(file);
			if (remove(filename.c_str())) cerr << "Impossibile eliminare l'oggetto serializzato\n";
		}
	}
};

class Deserializer2 {
private:
	string filename;
	size_t length;
	
	string findName(string source) {
		string filename = source;
		for (int i = 1; FILE * file = fopen(filename.c_str(), "r"); i++) {
			fclose(file);
			stringstream s;
			if (i == 0) s << filename << i;
			else s << filename.substr(0, filename.length()-2) << i;
			filename = s.str();
		}
		return filename;
	}
	void fileCopy(string filename) {
		ifstream source(filename.c_str(), std::ios::binary);
		ofstream destination(this->filename.c_str(), std::ios::binary);
		destination << source.rdbuf();
		if (FILE * file = fopen(this->filename.c_str(), "r")) fclose(file);
		else cerr << "Errore durante il recupero dell'oggetto da de-serializzare\n";
	}
public:
	Deserializer2() {
		length = 0;
	}
	Deserializer2(const Deserializer2 &d) {
		filename = findName(d.filename);
		length = d.length;
		fileCopy(d.filename);
	}
	Deserializer2(string filename, size_t length) {
		this->filename = findName(filename);
		this->length = length;
		fileCopy(filename);
	}
	parameter getObject() {
		if (length == 0) {
			cerr << "Non ci sono oggetti da de-serializzare\n";
			return parameter();
		}
		parameter_direction direction;
		parameter_type type;
		parameter_value value;
		ifstream file;
		file.open(filename.c_str(), ios_base::binary);
		if (!file.good()) cerr << "Errore durante la de-serializzazione dell'oggetto\n";
		file.read((char *) &direction, sizeof(parameter_direction));
		file.read((char *) &type, sizeof(parameter_type));
		size_t parameter_value_dimension = length - sizeof(parameter_direction) - sizeof(parameter_type);
		if (parameter_value_dimension > 0) {
			void * buffer = malloc(parameter_value_dimension);
			file.read((char *) buffer, parameter_value_dimension);
			value.setValue(buffer, parameter_value_dimension);
		}
		file.close();
		return parameter(direction, type, value);
	}
	Deserializer2& operator=(const Deserializer2 &d) {
		filename = findName(d.filename);
		length = d.length;
		fileCopy(d.filename);
		return * this;
	}
	~Deserializer2() {
		if (FILE * file = fopen(filename.c_str(), "r")) {
			fclose(file);
			if (remove(filename.c_str())) cerr << "Impossibile eliminare l'oggetto serializzato\n";
		}
	}
};

#endif
