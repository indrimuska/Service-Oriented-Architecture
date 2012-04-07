//
//  SOA/param.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 31/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Parameters.h"

parameter_value::parameter_value() {
	dimension = 0;
}
parameter_value::parameter_value(int value) {
	dimension = 0;
	setValue(value);
}
parameter_value::parameter_value(double value) {
	dimension = 0;
	setValue(value);
}
parameter_value::parameter_value(std::string value) {
	dimension = 0;
	setValue(value);
}
parameter_value::parameter_value(void * value, size_t dimension) {
	this->dimension = 0;
	setValue(value, dimension);
}
parameter_value::parameter_value(const parameter_value &p) {
	dimension = 0;
	setValue(p.value, p.dimension);
}
void parameter_value::setValue(int value) {
	setValue((void *) &value, sizeof(int));
}
void parameter_value::setValue(double value) {
	setValue((void *) &value, sizeof(double));
}
void parameter_value::setValue(std::string value) {
	setValue((void *) value.c_str(), value.size());
}
void parameter_value::setValue(void * value, size_t dimension) {
	if (this->dimension > 0) free(value);
	this->value = malloc(dimension);
	memcpy(this->value, value, dimension);
	this->dimension = dimension;
}
void parameter_value::get(void * object) {
	memcpy(object, value, dimension);
}
size_t parameter_value::getDimension() {
	return dimension;
}
void parameter_value::operator=(const parameter_value &p) {
	setValue(p.value, p.dimension);
}
parameter_value::~parameter_value() {
	if (dimension > 0) free(value);
	//free(value);
}

parameter::parameter() {
}
parameter::parameter(parameter_direction direction, parameter_type type) {
	init(direction, type);
}
parameter::parameter(parameter_direction direction, parameter_type type, int value) {
	init(direction, type);
	this->value.setValue(value);
}
parameter::parameter(parameter_direction direction, parameter_type type, double value) {
	init(direction, type);
	this->value.setValue(value);
}
parameter::parameter(parameter_direction direction, parameter_type type, std::string value) {
	init(direction, type);
	this->value.setValue(value);
}
parameter::parameter(parameter_direction direction, parameter_type type, void * value, size_t dimension) {
	init(direction, type);
	setValue(value, dimension);
}
parameter::parameter(parameter_direction direction, parameter_type type, parameter_value value) {
	init(direction, type);
	setValue(value);
}
void parameter::init(parameter_direction direction, parameter_type type) {
	this->direction = direction;
	this->type = type;
}
void parameter::setValue(parameter_value &value) {
	this->value = value;
}
void parameter::setValue(void * value, size_t dimension) {
	this->value.setValue(value, dimension);
}
void parameter::getInfo(parameter_direction &direction, parameter_type &type) {
	direction = this->direction;
	type = this->type;
	
}
void parameter::getValue(int &int_value) {
	value.get((void *) &int_value);
}
void parameter::getValue(double &double_value) {
	value.get((void *) &double_value);
}
void parameter::getValue(std::string &string_value) {
	char c_string_value[value.getDimension()+1];
	value.get((void *) &c_string_value);
	c_string_value[value.getDimension()] = '\0';
	string_value = c_string_value;
}
void parameter::getValue(void * generic_value) {
	value.get(generic_value);
}
parameter_direction parameter::getDirection() {
	return direction;
}
parameter_type parameter::getType() {
	return type;
}
size_t parameter::getValueDimension() {
	return value.getDimension();
}
bool parameter::operator==(const parameter &p) {
	return p.direction == direction && p.type == type;
}
bool parameter::operator!=(const parameter &p) {
	return p.direction != direction || p.type != type;
}
std::ostream& operator<<(std::ostream &o, const parameter &p) {
	o << "direction: ";
	o << (p.direction == IN ? "IN" : "OUT") << "\ntype:      ";
	parameter_value * pv = new parameter_value(p.value);
	if (p.type == INT) {
		o << "INT";
		if (pv->getDimension()) {
			int int_value;
			pv->get((void *) &int_value);
			o << "\nvalue:     " << int_value;
		}
	}
	if (p.type == DOUBLE) {
		o << "DOUBLE";
		if (pv->getDimension()) {
			double double_value;
			pv->get((void *) &double_value);
			o << "\nvalue:     " << double_value;
		}
	}
	if (p.type == STRING) {
		o << "STRING";
		if (pv->getDimension()) {
			char string_value[pv->getDimension()+1];
			pv->get((void *) &string_value);
			string_value[pv->getDimension()] = '\0';
			o << "\nvalue:     " << string_value;
		}
	}
	if (p.type == BUFFER) {
		o << "BUFFER";
		if (pv->getDimension()) o << "\nvalue:     value not representable";
	}
	//delete pv;
	return o << std::endl;
}