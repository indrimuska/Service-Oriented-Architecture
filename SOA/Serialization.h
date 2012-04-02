//
//  SOA/Serialization.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Serialization_h
#define SOA_Serialization_h

#include <fstream>

#include "Parameters.h"

using namespace std;

class Serializer {
private:
	string filename;
	size_t length;
	
	string findName(string source);
public:
	Serializer(parameter p);
	string getSerialized();
	string getSerialized(size_t &length);
	size_t getLength();
	~Serializer();
};

class Deserializer {
private:
	string filename;
	size_t length;
	
	string findName(string source);
	void fileCopy(string filename);
public:
	Deserializer();
	Deserializer(const Deserializer &d);
	Deserializer(string filename, size_t length);
	parameter getObject();
	Deserializer& operator=(const Deserializer &d);
	~Deserializer();
};

class OLD_Serializer {
private:
	void * buffer;
	size_t length;
public:
	OLD_Serializer(parameter p);
	void * getSerialized();
	void * getSerialized(size_t &legth);
	size_t getLength();
	~OLD_Serializer();
};

class OLD_Deserializer {
private:
	void * buffer;
	size_t length;
public:
	OLD_Deserializer();
	OLD_Deserializer(const OLD_Deserializer &d);
	OLD_Deserializer(void * buffer, size_t length);
	parameter getObject();
	OLD_Deserializer& operator=(const OLD_Deserializer &d);
	~OLD_Deserializer();
};

#endif
