//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <time.h>
#include <iostream>

#include "SOA/SOA.hpp"
#include "Application/ImageManipulation.hpp"
#include "Application/ImageStoring.hpp"
#include "Application/Alerts.cpp"

using namespace std;

#define NUM_ITERATIONS 1
#define IMAGES_DIRECTORY "Images/"

int main(int argc, char ** argv) {
	
	Alerts info;
	
	string SRaddress, SRport;
	
	string RotateServer[2];
	string HorizontalFlipServer[2];
	string StoreImageServer[2];
	string GetImageServer[2];
	string GetListServer[2];
	
	if (argc != 3) {
		cout << "Indirizzo del Server Register : ";
		cin >> SRaddress;
		cout << "Porta del Server Register     : ";
		cin >> SRport;
		cout << endl;
	} else {
		SRaddress = argv[1];
		SRport = argv[2];
	}
	
	/////////////////////////////////////////////////////////////
	// INIZIALIZZAZIONE DEI SERVIZI DA RICHIEDERE              //
	/////////////////////////////////////////////////////////////
	
	vector<parameter> parameters;
	
	// Inizializzazione del serivzio ROTATE
	parameters.clear();
	parameters.push_back(parameter(IN, INT, 90));
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation rotate;
	rotate.setService("rotate", parameters);
	
	// Inizializzazione del servizio HORIZONTAL FLIP
	parameters.clear();
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation horizontalFlip;
	horizontalFlip.setService("horizontal flip", parameters);
	
	// Inizializzazione del servizio STORE IMAGE
	ImageStoring storeImage;
	storeImage.setName("store image");
	
	// Inizializzazione del servizio GET IMAGE
	ImageStoring getImage("Clients");
	getImage.setName("get image");
	
	// Inizializzazione del servizio GET LIST
	parameters.clear();
	parameters.push_back(parameter(OUT, STRING));
	
	ImageStoring getList;
	getList.setService("get list", parameters);
	
	/////////////////////////////////////////////////////////////
	
	// Richiesta dell'indirizzo dei server al Server Register
	SOA global;
	global.setServerRegister(SRaddress, SRport);
	if (!global.getServerAddress(rotate,         RotateServer[0],         RotateServer[1])) return 0;
	if (!global.getServerAddress(horizontalFlip, HorizontalFlipServer[0], HorizontalFlipServer[1])) return 0;
	if (!global.getServerAddress(storeImage,     StoreImageServer[0],     StoreImageServer[1])) return 0;
	if (!global.getServerAddress(getImage,       GetImageServer[0],       GetImageServer[1])) return 0;
	if (!global.getServerAddress(getList,        GetListServer[0],        GetListServer[1])) return 0;
	
	// Lettura dei file contenuti nella cartella IMAGES_DIRECTORY
	DIR * directory;
	struct stat file_info;
	struct dirent * dir_info;
	vector<string> files_from_folder;
	vector<string> files_from_server;
	if (!(directory = opendir(IMAGES_DIRECTORY))) {
		cerr << "Errore durante la lettura della directory\n"
		"Controllare di avere i permessi necessari\n";
		return 0;
	}
	while ((dir_info = readdir(directory))) {
		if (dir_info->d_name[0] == '.') continue;
		if (stat((string(IMAGES_DIRECTORY) + dir_info->d_name).c_str(), &file_info)) continue;
		if (S_ISDIR(file_info.st_mode)) continue;
		files_from_folder.push_back(string(IMAGES_DIRECTORY) + dir_info->d_name);
	}
	if (!files_from_folder.size())
		info.generalWarning(string("Non ci sono immagini nella cartella '") +
							string(IMAGES_DIRECTORY).substr(0, string(IMAGES_DIRECTORY).find_last_of("/")) + "'");
	
	srand((int) time(NULL));
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		
		// Scelta del posto in cui prelevare le immagini
		int getImagesFromServer = files_from_folder.size() ? (int) (rand() % 2) : 1;
		vector<string> * files;
		parameter_value result;
		
		// Scelta dell'operazione da richiedere al primo server
		int operationType = (int) (rand() % 2);
		ImageManipulation * operation;
		operation = operationType == 0 ? &rotate : &horizontalFlip;
		
		// Elenco delle operazioni da eseguire
		int stepNumber = 0;
		cout << "Listato di esecuzione:\n";
		if (getImagesFromServer) cout << "  " << ++stepNumber << ". Richiesta dell'elenco delle immagini sul secondo server\n";
		cout << "  " << ++stepNumber << ". Prelievo di un'immagine random tra quelle esistenti";
		if (!getImagesFromServer) cout << " nella cartella Images";
		cout << endl;
		cout << "  " << ++stepNumber << ". Richiesta del servizio " << operation->getServiceName() << " al primo server\n";
		cout << "  " << ++stepNumber << ". Invio dell'immagine al secondo server\n";
		if (!getImagesFromServer) cout << "  " << ++stepNumber << ". Richiesta dell'elenco delle immagini sul secondo server\n";
		cout << endl;
		
		// Scelta dell'elenco delle immagini disponibili
		if (getImagesFromServer) {
			info.serviceRequestMessage(getList);
			if (!getList.requestService(GetListServer[0], GetListServer[1])) {
				info.serviceRequestFailed();
				continue;
			}
			info.serviceRequestSuccess();
			files_from_server = getList.getStringVectorFromParameter(OUT, 0, "\n");
			if (!files_from_server[0].compare("\033[1;33mNon ci sono immagini nel buffer list\033[0m")) {
				info.generalWarning("Non ci sono immagini nel buffer list");
				return 0;
			}
			files = &files_from_server;
		} else files = &files_from_folder;
		
		// Scelta del file da prelevare/inviare
		int file_index = (int) (rand() % files->size());
		cout << "Immagine scelta: \033[1;37m" << files->at(file_index) << "\033[0m\n\n";
		
		// Eventuale prelievo dell'immagine dal secondo server
		if (getImagesFromServer) {
			parameters.clear();
			parameters.push_back(parameter(IN, STRING, files_from_server[file_index]));
			parameters.push_back(parameter(OUT, BUFFER));
			getImage.setParameters(parameters);
			info.serviceRequestMessage(getImage);
			if (!getImage.requestService(GetImageServer[0], GetImageServer[1])) {
				info.serviceRequestFailed();
				continue;
			}
			info.serviceRequestSuccess();
			result = getImage.getParameterValue(OUT, 0);
		}
		
		// Richiesta del servizio scelto al primo server
		string server[2];
		if (getImagesFromServer) operation->setParameterValue(IN, 1-operationType, result);
		else operation->setImageAsParameter(IN, 1-operationType, files->at(file_index));
		server[0] = operationType == 0 ? RotateServer[0] : HorizontalFlipServer[0];
		server[1] = operationType == 0 ? RotateServer[1] : HorizontalFlipServer[1];
		info.serviceRequestMessage(* operation);
		if (!operation->requestService(server[0], server[1])) {
			info.serviceRequestFailed();
			continue;
		}
		info.serviceRequestSuccess();
		result = operation->getParameterValue(OUT, 0);
		
		// Richiesta di memorizzazione dell'immagine al secondo server
		parameters.clear();
		parameters.push_back(parameter(IN, STRING, files->at(file_index).substr(files->at(file_index).find_last_of("/")+1)));
		parameters.push_back(parameter(IN, BUFFER, result));
		storeImage.setParameters(parameters);
		info.serviceRequestMessage(storeImage);
		if (!storeImage.requestService(StoreImageServer[0], StoreImageServer[1])) {
			info.serviceRequestFailed();
			continue;
		}
		info.serviceRequestSuccess();
		
		// Eventuale elenco dei file presenti sul secondo server
		if (!getImagesFromServer) {
			info.serviceRequestMessage(getList);
			if (getList.requestService(GetListServer[0], GetListServer[1])) cout << getList.getStringFromParameter(OUT, 0) << endl << endl;
			else info.serviceRequestFailed();
		}
	}
}