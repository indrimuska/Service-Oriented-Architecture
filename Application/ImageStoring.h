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
#include <pthread.h>

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
	vector<string> getStringVectorFromParameter(parameter_direction direction, int parameter_number, string delimiter);
	parameter_value getParameterValue(parameter_direction direction, int parameter_number);
};

class StoreImageService : public ImageStoring {
private:
	pthread_mutex_t * mutex;
public:
	StoreImageService(pthread_mutex_t * mutex);
	bool execute(Socket * sk);
};

class GetImageService : public ImageStoring {
private:
	pthread_mutex_t * mutex;
public:
	GetImageService(pthread_mutex_t * mutex);
	bool execute(Socket * sk);
};

class GetListService : public ImageStoring {
private:
	pthread_mutex_t * mutex;
public:
	GetListService(pthread_mutex_t * mutex);
	bool execute(Socket * sk);
};

#endif
