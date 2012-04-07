//
//  Application/HorizontalFlipService.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 07/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "ImageManipulation.h"

class HorizontalFlipService : public ImageManipulation {
public:
	HorizontalFlipService() {
		vector<parameter> parameters;
		parameters.push_back(parameter(IN, BUFFER));
		parameters.push_back(parameter(OUT, BUFFER));
		this->setService("rotate", parameters);
	}
	bool execute(Socket * sk) {
		string inFile = "temp.gif";
		string outFile = "flipped.gif";
		getImageFromBuffer(inParameters[0], inFile);
		CImg<unsigned char> image(inFile.c_str());
		image.mirror('x').save(outFile.c_str());
		putImageInBuffer(outParameters[0], outFile);
		if (remove(inFile.c_str()) || remove(outFile.c_str())) {
			cerr << "Impossibile rimuovere i file temporanei\n";
			return false;
		}
		return true;
	}
};