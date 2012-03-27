//
//  RotateService.cpp
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
		vector<param> parameters;
		parameters.push_back(param(IN, INT));
		parameters.push_back(param(IN, BUFFER));
		parameters.push_back(param(OUT, BUFFER));
		this->setService("rotate", parameters);
	};
};