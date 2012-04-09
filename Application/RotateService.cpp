//
//  Application/RotateService.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 27/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ImageManipulation.h"

class RotateService : public ImageManipulation {
public:
	RotateService() {
		vector<parameter> parameters;
		parameters.push_back(parameter(IN, INT));
		parameters.push_back(parameter(IN, BUFFER));
		parameters.push_back(parameter(OUT, BUFFER));
		setService("rotate", parameters);
	}
	bool execute(Socket * sk) {
		string inFile = workDirectory + "/source.gif";
		string outFile = workDirectory + "/rotated.gif";
		getImageFromBuffer(inParameters[1], inFile, true);
		int degrees;
		inParameters[0].getValue(degrees);
		CImg<unsigned char> image(inFile.c_str());
		image.rotate(degrees).save(outFile.c_str());
		putImageInBuffer(outParameters[0], outFile);
		if (remove(inFile.c_str()) || remove(outFile.c_str())) {
			cerr << "Impossibile rimuovere i file temporanei\n";
			return false;
		}
		return true;
	}
};