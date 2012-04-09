//
//  Application/ImageStoring.h
//  Service Oriented Architecture
//
//  Created by Indri Muska on 09/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#ifndef Application_ImageStoring_h
#define Application_ImageStoring_h

#include <string>
#include <iostream>
#include <dirent.h>

#include "../SOA/Service.h"

class ImageStoring : public Service {
protected:
	string workDirectory;
	
	bool getImageFromBuffer(parameter &p, string filename, bool hideWarning = false);
	bool putImageInBuffer(parameter &p, string filename);
public:
	ImageStoring();
	ImageStoring(string workDirectory);
	bool setImageAsParameter(parameter_direction direction, int parameter_number, string filename);
	bool getImageFromParameter(parameter_direction direction, int parameter_number, string filename);
	string getStringFromParameter(parameter_direction direction, int parameter_number);
};

class StoreImageService : public ImageStoring {
public:
	StoreImageService();
	bool execute(Socket * sk);
};

class GetImageService : public ImageStoring {
public:
	GetImageService();
	bool execute(Socket * sk);
};

class GetListService : public ImageStoring {
public:
	GetListService();
	bool execute(Socket * sk);
};

#endif
