//
//  SOA/types.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef SOA_types_h
#define SOA_types_h

struct buffer {
	void * b;
	int dim;
};

enum param_direction { IN, OUT };
enum param_type { INT, DOUBLE, STRING, BUFFER };
union param_value {
	int i;
	double d;
	char * s;
	buffer b;
};

struct param {
	param_direction direction;
	param_type		type;
	param_value		value;
	
	param(param_direction d, param_type t) {
		this->direction = d;
		this->type = t;
	}
	param(param_direction d, param_type t, param_value v) {
		this->direction = d;
		this->type = t;
		this->value = v;
	}
	friend std::ostream& operator<< (std::ostream &o, const param &p) {
		return o
		<< "direction: " << (p.direction == IN ? "IN" : "OUT") << '\n'
		<< "type:      " << (p.type == INT ? "INT" :
							(p.type == DOUBLE ? "DOUBLE" :
							(p.type == STRING ? "STRING" :
							(p.type == BUFFER ? "BUFFER" : "UNKNOWN")))) << '\n';
	}
};

struct packet_field {
	void * value;
	int length;
	
	packet_field(void * v, int l) {
		this->value = v;
		this->length = l;
	}
};

#endif
