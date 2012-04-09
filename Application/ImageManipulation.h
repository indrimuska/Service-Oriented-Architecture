//
//  Application/ImageManipulation.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 07/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef Application_ImageManipulation_h
#define Application_ImageManipulation_h

#include <fstream>
#include <iostream>

#include "../SOA/Service.h"

#include "CImg.h"

using namespace cimg_library;

class ImageManipulation : public Service {
protected:
	string workDirectory;
	
	bool getImageFromBuffer(parameter &p, string filename, bool hideWarning = false);
	bool putImageInBuffer(parameter &p, string filename);
public:
	ImageManipulation();
	ImageManipulation(string workDirectory);
	bool setImageAsParameter(parameter_direction direction, int parameter_number, string filename);
	bool getImageFromParameter(parameter_direction direction, int parameter_number, string filename);
};

class RotateService : public ImageManipulation {
public:
	RotateService();
	bool execute(Socket * sk);
};

class HorizontalFlipService : public ImageManipulation {
public:
	HorizontalFlipService();
	bool execute(Socket * sk);
};

#endif
