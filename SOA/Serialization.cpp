//
//  SOA/Serializer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Serialization.h"

Serializer::Serializer(parameter p) {
	length = sizeof(param_direction) + sizeof(param_type) + p.getValueDimension();
	buffer = malloc(length);
	param_direction direction;
	param_type type;
	p.getInfo(direction, type);
	memcpy(buffer, &direction, sizeof(param_direction));
	memcpy((char *) buffer + sizeof(param_direction), &type, sizeof(type));
	p.getValue((char *) buffer + sizeof(param_direction) + sizeof(param_type));
}
void * Serializer::getSerialized() {
	return buffer;
}
void * Serializer::getSerialized(size_t &legth) {
	length = this->length;
	return buffer;
}
size_t Serializer::getLength() {
	return length;
}
Serializer::~Serializer() {
	if (length > 0) free(buffer);
}

Deserializer::Deserializer(void * buffer, size_t length) {
	this->length = length;
	this->buffer = malloc(length);
	memcpy(this->buffer, buffer, length);
}
parameter Deserializer::getObject() {
	param_direction direction;
	param_type type;
	parameter_value value;
	memcpy(&direction, buffer, sizeof(param_direction));
	memcpy(&type, (char *) buffer + sizeof(param_direction), sizeof(param_type));
	value.setValue((char *) buffer + sizeof(param_direction) + sizeof(param_type), length - sizeof(param_direction) - sizeof(param_type));
	return parameter(direction, type, value);
}
Deserializer::~Deserializer() {
	if (length > 0) free(buffer);
}