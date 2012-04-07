//
//  ImageManipulation.h
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
public:
	bool getImageFromBuffer(parameter &p, string filename);
	bool putImageInBuffer(parameter &p, string filename);
};

#endif
