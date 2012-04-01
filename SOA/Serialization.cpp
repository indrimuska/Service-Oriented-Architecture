//
//  SOA/Serializer.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Serialization.h"

Serializer::Serializer(parameter p) {
	length = sizeof(parameter_direction) + sizeof(parameter_type) + p.getValueDimension();
	buffer = malloc(length);
	parameter_direction direction;
	parameter_type type;
	p.getInfo(direction, type);
	memcpy(buffer, &direction, sizeof(parameter_direction));
	memcpy((char *) buffer + sizeof(parameter_direction), &type, sizeof(type));
	if (p.getValueDimension()) p.getValue((char *) buffer + sizeof(parameter_direction) + sizeof(parameter_type));
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

Deserializer::Deserializer() {
	buffer = malloc(10);
	length = 0;
}
Deserializer::Deserializer(const Deserializer &d) {
	if (length > 0) free(buffer);
	this->length = d.length;
	this->buffer = malloc(d.length);
	memcpy(this->buffer, d.buffer, d.length);
}
Deserializer::Deserializer(void * buffer, size_t length) {
	this->length = length;
	this->buffer = malloc(length);
	memcpy(this->buffer, buffer, length);
}
parameter Deserializer::getObject() {
	parameter_direction direction;
	parameter_type type;
	parameter_value value;
	memcpy(&direction, buffer, sizeof(parameter_direction));
	memcpy(&type, (char *) buffer + sizeof(parameter_direction), sizeof(parameter_type));
	size_t parameter_value_dimension = length - sizeof(parameter_direction) - sizeof(parameter_type);
	if (parameter_value_dimension) value.setValue((char *) buffer + sizeof(parameter_direction) + sizeof(parameter_type), parameter_value_dimension);
	return parameter(direction, type, value);
}
Deserializer& Deserializer::operator=(const Deserializer &d) {
	if (length > 0) free(buffer);
	this->length = d.length;
	this->buffer = malloc(d.length);
	memcpy(this->buffer, d.buffer, d.length);
	return * this;
}
Deserializer::~Deserializer() {
	if (length > 0) free(buffer);
}