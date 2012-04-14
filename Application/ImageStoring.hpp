//
//  Application/ImageStoring.hpp
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
#include <boost/thread/shared_mutex.hpp>

#include "../SOA/Service.hpp"

using namespace std;

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
	vector<string> getStringVectorFromParameter(parameter_direction direction, int parameter_number, string delimiter);
	parameter_value getParameterValue(parameter_direction direction, int parameter_number);
};

class StoreImageService : public ImageStoring {
private:
	boost::shared_mutex * mutex;
public:
	StoreImageService(boost::shared_mutex * mutex);
	bool execute();
};

class GetImageService : public ImageStoring {
private:
	boost::shared_mutex * mutex;
public:
	GetImageService(boost::shared_mutex * mutex);
	bool execute();
};

class GetListService : public ImageStoring {
private:
	boost::shared_mutex * mutex;
public:
	GetListService(boost::shared_mutex * mutex);
	bool execute();
};

#endif
