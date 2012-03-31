//
//  param.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 31/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <string.h>

#include "types.h"

class parameter_value {
private:
	void * value;
	size_t dimension;
public:
	parameter_value() {
		value = NULL;
		dimension = 0;
	}
	parameter_value(void * value, size_t dimension) {
		this->dimension = 0;
		setValue(value, dimension);
	}
	parameter_value(const parameter_value &p) {
		dimension = 0;
		setValue(p.value, p.dimension);
	}
	void setValue(void * value, size_t dimension) {
		if (this->dimension > 0) free(value);
		this->value = malloc(dimension);
		memcpy(this->value, value, dimension);
		this->dimension = dimension;
	}
	void get(void * object) {
		memcpy(object, value, dimension);
	}
	size_t getDimension() {
		return dimension;
	}
	void operator=(const parameter_value &p) {
		setValue(p.value, p.dimension);
	}
	~parameter_value() {
		free(value);
	}
};

class parameter {
private:
	param_direction direction;
	param_type type;
	parameter_value value;
public:
	parameter() {
	}
	parameter(param_direction direction, param_type type) {
		init(direction, type);
	}
	parameter(param_direction direction, param_type type, parameter_value value) {
		init(direction, type);
		setValue(value);
	}
	void init(param_direction direction, param_type type) {
		this->direction = direction;
		this->type = type;
	}
	void setValue(parameter_value value) {
		this->value = value;
	}
	void * getValue() {
		void * generic_value;
		value.get(generic_value);
		return generic_value;
	}
	friend std::ostream& operator<<(std::ostream &o, const parameter &p) {
		o << "direction: ";
		o << (p.direction == IN ? "IN" : "OUT") << "\ntype:      ";
		parameter_value pv = p.value;
		if (p.type == INT) {
			o << "INT";
			if (pv.getDimension() != 0) {
				int int_value;
				pv.get((void *) &int_value);
				o << "\nvalue:     " << int_value;
			}
		}
		if (p.type == DOUBLE) {
			o << "DOUBLE";
			if (pv.getDimension() != 0) {
				double double_value;
				pv.get((void *) &double_value);
				o << "\nvalue:     " << double_value;
			}
		}
		if (p.type == STRING) o << "STRING\nvalue:     value not representable";
		if (p.type == BUFFER) o << "BUFFER\nvalue:     value not representable";
		return o << std::endl;
	}
};