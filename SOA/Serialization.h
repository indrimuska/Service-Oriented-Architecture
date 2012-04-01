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
public:
	Serializer2(parameter p) {
		length = sizeof(parameter_direction) + sizeof(parameter_type) + p.getValueDimension();
		filename = "parameter_serialized";
		for (int i = 0; FILE * file = fopen(filename.c_str(), "r"); i++) {
			fclose(file);
			stringstream s;
			if (i == 0) s << filename << i;
			else s << filename.substr(0, filename.length()-1) << i;
			filename = s.str();
		}
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
		struct stat info;
		stat(filename.c_str(), &info);
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
public:
	Deserializer2() {
	}
	Deserializer2(const Deserializer2 &d) {
		filename = d.filename;
		length = d.length;
		for (int i = 0; FILE * file = fopen(filename.c_str(), "r"); i++) {
			fclose(file);
			stringstream s;
			if (i == 0) s << filename << i;
			else s << filename.substr(0, filename.length()-2) << i;
			filename = s.str();
		}
		ifstream source(d.filename.c_str(), std::ios::binary);
		std::ofstream destination(filename.c_str(), std::ios::binary);
		destination << source.rdbuf();
	}
	Deserializer2(string filename, size_t length) {
		struct stat info;
		stat(filename.c_str(), &info);
		this->filename = filename;
		this->length = length;
	}
	parameter getObject() {
		parameter_direction direction;
		parameter_type type;
		parameter_value value;
		ifstream file;
		file.open(filename.c_str(), ios_base::binary);
		if (!file.good()) cerr << "Errore durante la de-serializzazione dell'oggetto\n";
		file.read((char *) &direction, sizeof(parameter_direction));
		file.read((char *) &type, sizeof(parameter_type));
		size_t parameter_value_dimension = length - sizeof(parameter_direction) - sizeof(parameter_type);
		if (parameter_value_dimension) {
			void * buffer = malloc(parameter_value_dimension);
			file.read((char *) buffer, parameter_value_dimension);
			value.setValue(buffer, parameter_value_dimension);
		}
		file.close();
		return parameter(direction, type, value);
	}
	Deserializer2& operator=(const Deserializer2 &d) {
		filename = d.filename;
		length = d.length;
		for (int i = 0; FILE * file = fopen(filename.c_str(), "r"); i++) {
			fclose(file);
			stringstream s;
			if (i == 0) s << filename << i;
			else s << filename.substr(0, filename.length()-1) << i;
			filename = s.str();
		}
		ifstream source(d.filename.c_str(), std::ios::binary);
		std::ofstream destination(filename.c_str(), std::ios::binary);
		destination << source.rdbuf();
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
