//
//  Application/RotateService.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "../SOA/Service.h"

class RotateService : public Service {
public:
	RotateService() {
		vector<parameter> parameters;
		parameters.push_back(parameter(IN, INT));
		parameters.push_back(parameter(IN, BUFFER));
		parameters.push_back(parameter(OUT, BUFFER));
		this->setService("rotate", parameters);
	};
};