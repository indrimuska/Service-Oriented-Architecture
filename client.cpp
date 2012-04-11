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

int main(int argc, char ** argv) {
	
	string IMSaddress, IMSport, ISSaddress, ISSport;
	
	/*cout << "ImageManipulationServer.address : ";
	cin >> IMSaddress;
	cout << "ImageManipulationServer.port    : ";
	cin >> IMSport;
	cout << "ImageStoringServer.address      : ";
	cin >> ISSaddress;
	cout << "ImageStoringServer.port         : ";
	cin >> ISSport;
	cout << endl;*/
	
	IMSaddress = argv[1];
	IMSport = argv[2];
	ISSaddress = argv[3];
	ISSport = argv[4];
	
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
	storeImage.setServer(ISSaddress, ISSport);
	storeImage.setService("store image", parameters);
	
	// Inizializzazione del servizio GET IMAGE
	parameters.clear();
	parameters.push_back(parameter(IN, STRING));
	parameters.push_back(parameter(OUT, BUFFER));
	
	ImageStoring getImage("Clients");
	getImage.setServer(ISSaddress, ISSport);
	getImage.setService("get image", parameters);
	
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
	if (!(directory = opendir("Images"))) {
		cerr << "Errore durante la lettura della directory\n"
		"Controllare di avere i permessi necessari\n";
		return 0;
	}
	while ((dir_info = readdir(directory))) {
		if (dir_info->d_name[0] == '.') continue;
		if (stat((string("Images/") + dir_info->d_name).c_str(), &file_info)) continue;
		if (S_ISDIR(file_info.st_mode)) continue;
		files_from_folder.push_back(string("Images/") + dir_info->d_name);
	}
	if (!files_from_folder.size()) {
		cerr << "\033[1;33mNon ci sono immagini nella cartella 'Images'\033[0m";
		return 0;
	}
	
	// TODO: mettere i JPEG al posto dei GIF
	// TODO: controllare anche i parametri di uscita
	
	srand((int) time(NULL));
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		
		// Scelta del posto in cui prelevare le immagini
		int getImagesFromServer = (int) (rand() % 2);
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
			files = &files_from_server;
		} else files = &files_from_folder;
		
		// Scelta del file da prelevare/inviare
		int file_index = (int) (rand() % files->size());
		
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
	
	
	/*cout << "Richiesta del servizio ROTATE:\n";
	if (rotate.requestService()) {
		cout << "richiesta accettata\n";
		if (!rotate.getImageFromParameter(OUT, 0, argv[4])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[4] << "'\n";
	} else cout << "richiesta rifiutata\n";
	cout << endl;*/
	
	/*cout << "Richiesta del servizio HORIZONTAL FLIP:\n";
	if (horizontalFlip.requestService()) {
		cout << "richiesta accettata\n";
		if (!horizontalFlip.getImageFromParameter(OUT, 0, argv[5])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[5] << "'\n";
	} else cout << "richiesta rifiutata\n";
	cout << endl;*/
	
	/*cout << "Richiesta del servizio STORE IMAGE:\n";
	if (storeImage.requestService()) cout << "richiesta accettata\n";
	else cout << "richiesta rifiutata\n";
	cout << endl;*/
	
	/*cout << "Richiesta del servizio GET IMAGE:\n";
	if (getImage.requestService()) {
		cout << "richiesta accettata\n";
		if (!getImage.getImageFromParameter(OUT, 0, argv[4])) cout << "impossibile creare l'immagine\n";
		else cout << "creata immagine '" << argv[4] << "'\n";
	} else cout << "richiesta rifiutata\n";
	cout << endl;*/
	
	/*cout << "Richiesta del servizio GET LIST:\n";
	if (getList.requestService()) cout << "richiesta accettata:\n" << getList.getStringFromParameter(OUT, 0) << endl;
	else cout << "richiesta rifiutata\n";
	cout << endl;*/
}