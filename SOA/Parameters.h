//
//  SOA/Parameters.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_Parameters_h
#define SOA_Parameters_h

#include <cstdlib>
#include <iostream>
#include <string.h>

enum parameter_direction { IN, OUT };

enum parameter_type { INT, DOUBLE, STRING, BUFFER };

class parameter_value {
private:
	void * value;
	size_t dimension;
public:
	parameter_value();
	parameter_value(int value);
	parameter_value(double value);
	parameter_value(std::string value);
	parameter_value(void * value, size_t dimension);
	parameter_value(const parameter_value &p);
	void setValue(int value);
	void setValue(double value);
	void setValue(std::string value);
	void setValue(void * value, size_t dimension);
	void get(void * object);
	size_t getDimension();
	void operator=(const parameter_value &p);
	~parameter_value();
};

class parameter {
private:
	parameter_direction direction;
	parameter_type type;
	parameter_value value;
public:
	parameter();
	parameter(parameter_direction direction, parameter_type type);
	parameter(parameter_direction direction, parameter_type type, int value);
	parameter(parameter_direction direction, parameter_type type, double value);
	parameter(parameter_direction direction, parameter_type type, std::string value);
	parameter(parameter_direction direction, parameter_type type, void * value, size_t dimension);
	parameter(parameter_direction direction, parameter_type type, parameter_value value);
	void init(parameter_direction direction, parameter_type type);
	void setValue(int value);
	void setValue(double value);
	void setValue(std::string value);
	void setValue(void * value, size_t dimension);
	void setValue(parameter_value &value);
	void getInfo(parameter_direction &direction, parameter_type &type);
	void getValue(int &int_value);
	void getValue(double &double_value);
	void getValue(std::string &string_value);
	void getValue(void * generic_value);
	parameter_direction getDirection();
	parameter_type getType();
	parameter_value getParameterValue();
	size_t getValueDimension();
	bool operator==(const parameter &p);
	bool operator!=(const parameter &p);
	friend std::ostream& operator<<(std::ostream &o, const parameter &p);
};

#endif
