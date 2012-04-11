//
//  client.cpp
//  Service Oriented Arichitecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <time.h>
#include <iostream>

#include "Application/ImageManipulation.h"
#include "Application/ImageStoring.h"

using namespace std;

#define NUM_ITERATIONS 1
#define IMAGES_DIRECTORY "Images/"

int main(int argc, char ** argv) {
	
	string IMSaddress, IMSport, ISSaddress, ISSport;
	
	if (argc != 5) {
		cout << "ImageManipulationServer Address : ";
		cin >> IMSaddress;
		cout << "ImageManipulationServer Port    : ";
		cin >> IMSport;
		cout << "ImageStoringServer Address      : ";
		cin >> ISSaddress;
		cout << "ImageStoringServer Port         : ";
		cin >> ISSport;
		cout << endl;
	} else {
		IMSaddress = argv[1];
		IMSport = argv[2];
		ISSaddress = argv[3];
		ISSport = argv[4];
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
	rotate.setServer(IMSaddress, IMSport);
	rotate.setService("rotate", parameters);
	
	// Inizializzazione del servizio HORIZONTAL FLIP
	parameters.clear();
	parameters.push_back(parameter(IN, BUFFER));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageManipulation horizontalFlip;
	horizontalFlip.setServer(IMSaddress, IMSport);
	horizontalFlip.setService("horizontal flip", parameters);
	
	// Inizializzazione del servizio STORE IMAGE
	ImageStoring storeImage;
	storeImage.setName("store image");
	storeImage.setServer(ISSaddress, ISSport);
	
	// Inizializzazione del servizio GET IMAGE
	ImageStoring getImage("Clients");
	getImage.setName("get image");
	getImage.setServer(ISSaddress, ISSport);
	
	// Inizializzazione del servizio GET LIST
	parameters.clear();
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageStoring getList;
	getList.setServer(ISSaddress, ISSport);
	getList.setService("get list", parameters);
	
	/////////////////////////////////////////////////////////////
	
	// Leggo i file contenuti nella cartella Images
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
	if (!files_from_folder.size()) {
		cerr << "\033[1;33mNon ci sono immagini nella cartella '" <<
		string(IMAGES_DIRECTORY).substr(0, string(IMAGES_DIRECTORY).find_last_of("/")) << "'\033[0m\n\n";
	}
	
	// TODO: mettere i JPEG al posto dei GIF
	// TODO: controllare anche i parametri di uscita
	
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
		cout << "\n";
		cout << "  " << ++stepNumber << ". Richiesta del servizio " << operation->getServiceName() << " al primo server\n";
		cout << "  " << ++stepNumber << ". Invio dell'immagine al secondo server\n";
		if (!getImagesFromServer) cout << "  " << ++stepNumber << ". Richiesta dell'elenco delle immagini sul secondo server\n";
		cout << endl;
		
		// Scelta dell'elenco delle immagini disponibili
		if (getImagesFromServer) {
			cout << "Richiesta del servizio \033[1;34mget list\033[0m:\n";
			if (!getList.requestService()) {
				cout << "\033[1;31mRichiesta rifiutata\033[0m\n\n";
				continue;
			}
			cout << "\033[1;32mServizio confermato\033[0m\n\n";
			files_from_server = getList.getStringVectorFromParameter(OUT, 0, "\n");
			if (!files_from_server[0].compare("\033[1;33mNon ci sono immagini nel buffer list\033[0m")) {
				cerr << "\033[1;33mNon ci sono immagini nel buffer list\033[0m\n\n";
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
			cout << "Richiesta del servizio \033[1;34mget image\033[0m:\n";
			if (!getImage.requestService()) {
				cout << "\033[1;31mRichiesta rifiutata\033[0m\n\n";
				continue;
			}
			cout << "\033[1;32mServizio confermato\033[0m\n\n";
			result = getImage.getParameterValue(OUT, 0);
		}
		
		// Richiesta del servizio scelto al primo server
		if (getImagesFromServer) operation->setParameterValue(IN, 1-operationType, result);
		else operation->setImageAsParameter(IN, 1-operationType, files->at(file_index));
		cout << "Richiesta del servizio \033[1;34m" << operation->getServiceName() << "\033[0m:\n";
		if (!operation->requestService()) {
			cout << "\033[1;31mRichiesta rifiutata\033[0m\n\n";
			continue;
		}
		cout << "\033[1;32mServizio confermato\033[0m\n\n";
		result = operation->getParameterValue(OUT, 0);
		
		// Richiesta di memorizzazione dell'immagine al secondo server
		parameters.clear();
		parameters.push_back(parameter(IN, STRING, files->at(file_index).substr(files->at(file_index).find_last_of("/")+1)));
		parameters.push_back(parameter(IN, BUFFER, result));
		storeImage.setParameters(parameters);
		cout << "Richiesta del servizio \033[1;34mstore image\033[0m:\n";
		if (!storeImage.requestService()) {
			cout << "\033[1;31mRichiesta rifiutata\033[0m\n\n";
			continue;
		}
		cout << "\033[1;32mServizio confermato\033[0m\n\n";
		
		// Eventuale elenco dei file presenti sul secondo server
		if (!getImagesFromServer) {
			cout << "Richiesta del servizio \033[1;34mget list\033[0m:\n";
			if (getList.requestService()) cout << getList.getStringFromParameter(OUT, 0) << endl << endl;
			else cout << "\033[1;31mRichiesta rifiutata\033[0m\n\n";
		}
	}
}