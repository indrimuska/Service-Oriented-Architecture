//
//  SOA/Serialization.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Serialization_h
#define SOA_Serialization_h

#include "Parameters.h"

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

#endif
