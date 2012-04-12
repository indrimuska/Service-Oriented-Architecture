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

class ImageManager {
private:
    bool busy;
    int readersActive;
    int writersIdle;
    int readersIdle;
	pthread_mutex_t mutex;
	pthread_cond_t ok_read;
	pthread_cond_t ok_write;
public:
	ImageManager();
	void readRequest();
    void readRelease();
    void writeRequest();
    void writeRelease();
};

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
	ImageManager * manager;
public:
	StoreImageService(ImageManager * manager);
	bool execute(Socket * sk);
};

class GetImageService : public ImageStoring {
private:
	ImageManager * manager;
public:
	GetImageService(ImageManager * manager);
	bool execute(Socket * sk);
};

class GetListService : public ImageStoring {
private:
	ImageManager * manager;
public:
	GetListService(ImageManager * manager);
	bool execute(Socket * sk);
};

#endif
