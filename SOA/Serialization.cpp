//
//  SOA/Serializer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Serialization.h"

Serializer::Serializer(parameter p) {
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
		file.write((char *) buffer, p.getValueDimension());
	}
	file.close();
}
string Serializer::findName(string source) {
	string filename = source;
	for (int i = 1; FILE * file = fopen(filename.c_str(), "r"); i++) {
		fclose(file);
		stringstream s;
		if (i == 1) s << filename << i;
		else s << filename.substr(0, filename.length()-2) << i;
		filename = s.str();
	}
	return filename;
}
string Serializer::getSerialized() {
	return filename;
}
string Serializer::getSerialized(size_t &length) {
	length = this->length;
	return filename;
}
size_t Serializer::getLength() {
	return length;
}
Serializer::~Serializer() {
	if (FILE * file = fopen(filename.c_str(), "r")) {
		fclose(file);
		if (remove(filename.c_str())) cerr << "Impossibile eliminare l'oggetto serializzato\n";
	}
}

Deserializer::Deserializer() {
	length = 0;
}
Deserializer::Deserializer(const Deserializer &d) {
	filename = findName(d.filename);
	length = d.length;
	fileCopy(d.filename);
}
Deserializer::Deserializer(string filename, size_t length) {
	this->filename = findName(filename);
	this->length = length;
	fileCopy(filename);
	return;
}
string Deserializer::findName(string source) {
	string filename = source;
	for (int i = 1; FILE * file = fopen(filename.c_str(), "r"); i++) {
		fclose(file);
		stringstream s;
		if (i == 1) s << filename << i;
		else s << filename.substr(0, filename.length()-2) << i;
		filename = s.str();
	}
	return filename;
}
void Deserializer::fileCopy(string filename) {
	ifstream source(filename.c_str(), std::ios::binary);
	ofstream destination(this->filename.c_str(), std::ios::binary);
	destination << source.rdbuf();
	if (FILE * file = fopen(this->filename.c_str(), "r")) fclose(file);
	else cerr << "Errore durante il recupero dell'oggetto da de-serializzare\n";
}
parameter Deserializer::getObject() {
	if (length == 0) {
		cerr << "Non ci sono oggetti da de-serializzare\n";
		return parameter();
	}
	parameter_direction direction;
	parameter_type type;
	parameter_value value;
	ifstream file;
	file.open(filename.c_str(), ios_base::binary);
	if (!file.good()) {
		cerr << "Errore durante la de-serializzazione dell'oggetto\n";
		return parameter();
	}
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
Deserializer& Deserializer::operator=(const Deserializer &d) {
	filename = findName(d.filename);
	length = d.length;
	fileCopy(d.filename);
	return * this;
}
Deserializer::~Deserializer() {
	if (FILE * file = fopen(filename.c_str(), "r")) {
		fclose(file);
		if (remove(filename.c_str())) cerr << "Impossibile eliminare l'oggetto serializzato\n";
	}
}

OLD_Serializer::OLD_Serializer(parameter p) {
	length = sizeof(parameter_direction) + sizeof(parameter_type) + p.getValueDimension();
	buffer = malloc(length);
	parameter_direction direction;
	parameter_type type;
	p.getInfo(direction, type);
	memcpy(buffer, &direction, sizeof(parameter_direction));
	memcpy((char *) buffer + sizeof(parameter_direction), &type, sizeof(type));
	if (p.getValueDimension()) p.getValue((char *) buffer + sizeof(parameter_direction) + sizeof(parameter_type));
}
void * OLD_Serializer::getSerialized() {
	return buffer;
}
void * OLD_Serializer::getSerialized(size_t &legth) {
	length = this->length;
	return buffer;
}
size_t OLD_Serializer::getLength() {
	return length;
}
OLD_Serializer::~OLD_Serializer() {
	if (length > 0) free(buffer);
}

OLD_Deserializer::OLD_Deserializer() {
	std::cout << "\nempty constructor\n\n";
	buffer = malloc(10);
	length = 0;
}
OLD_Deserializer::OLD_Deserializer(const OLD_Deserializer &d) {
	std::cout << "\ncopy constructor\n\n";
	if (length > 0) free(buffer);
	this->length = d.length;
	this->buffer = malloc(d.length);
	memcpy(this->buffer, d.buffer, d.length);
}
OLD_Deserializer::OLD_Deserializer(void * buffer, size_t length) {
	std::cout << "\nregular constructor\n\n";
	this->length = length;
	this->buffer = malloc(length);
	std::cout << "so the error is here\n";
	memcpy(this->buffer, buffer, length);
	std::cout << "or not?\n";
}
parameter OLD_Deserializer::getObject() {
	parameter_direction direction;
	parameter_type type;
	parameter_value value;
	memcpy(&direction, buffer, sizeof(parameter_direction));
	memcpy(&type, (char *) buffer + sizeof(parameter_direction), sizeof(parameter_type));
	size_t parameter_value_dimension = length - sizeof(parameter_direction) - sizeof(parameter_type);
	if (parameter_value_dimension) value.setValue((char *) buffer + sizeof(parameter_direction) + sizeof(parameter_type), parameter_value_dimension);
	return parameter(direction, type, value);
}
OLD_Deserializer& OLD_Deserializer::operator=(const OLD_Deserializer &d) {
	std::cout << "\noperator=\n\n";
	if (length > 0) free(buffer);
	this->length = d.length;
	this->buffer = malloc(d.length);
	memcpy(this->buffer, d.buffer, d.length);
	return * this;
}
OLD_Deserializer::~OLD_Deserializer() {
	if (length > 0) free(buffer);
}