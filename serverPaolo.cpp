//
//  server.cpp
//  Service Oriented Architecture
//
//  Created by par on 30/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

//questo esempio serve solo per provare il funzionamento della registrazione di un server e di un registro
//sul serverRegister

#include <vector>
#include <iostream>

#include "SOA/SOA.h"
#include "SOA/Service.h"
#include "Application/RotateService.cpp"

using namespace std;

int main(int argc, char ** argv) {

	Socket serverRegister;
	Communicator serverPaolo;
	string port;

	/*Da utilizzare poi per le demo su lan
	 // Connessione al Service Register
	 cout << "Insert server register address: ";
	 cin >> SRaddress;
	 cout << "Insert server register port: ";
	 cin >> SRport;
	 */

	cout << " * LOCAL SERVER * \n\n";
	cout << "Porta del server Register: ";
	cin >> port;
	//server.startListener(argv[1]);
	serverPaolo.connectTo("127.0.0.1", port, serverRegister);

	cout << "Scegli l'operazione da effettuare" << endl;
	cout << "Premi 1 se vuoi registrare un server" << endl;
	cout << "Premi 2 se vuoi visualizzare tutti i server registrati" << endl;
	cout << "Premi 3 se vuoi registrare un servizio (per farlo devi aver già registrato il server)" << endl;
	cout << "Premi 4 se vuoi visualizzare tutti i SERVIZI registrati" << endl;
	int richiesta;
	cin >> richiesta;

	switch (richiesta) {

	case 1: {
		cout << "È stato scelto di fare la registrazione di un server" << endl;
		string regReq = SRV_REG_REQ;
		serverRegister.sendString(regReq);

		cout << serverPaolo.getIP() << endl;
		cout << "Porta su cui far girare questo server: ";
		string localServerPort;
		cin >> localServerPort;
		port = localServerPort; //mi salvo la port che poi verrà utilizzata sempre per questo server
		string serverInfo = serverPaolo.getIP() + ":" + localServerPort;
		serverRegister.sendString(serverInfo);
		serverRegister.sendString(serverPaolo.getIP());

		serverRegister.sendString(localServerPort);
		cout << "Voglio registrare: " + serverInfo << endl;
	}
		break;
	case 2: {
		cout << "È stato scelto di visualizzare i server registrati" << endl;
		string regDisp = SRV_REG_DISP;
		serverRegister.sendString(regDisp);
		cout << "Ho inviato la richiesta per visualizzare i server registrati"
				<< endl;
	}
		break;
	case 3: {

		string serverInfo = serverPaolo.getIP() + ":" + port;
		serverRegister.sendString(serverInfo);



		cout << "È stato scelto di fare la registrazione di un servizio"
				<< endl;
		string servReq = SRC_REG_REQ;
		serverRegister.sendString(servReq);

		string infoRegistrazione;
		serverRegister.receiveString(infoRegistrazione);
		cout << infoRegistrazione << endl;
		cout << "superato il controllo" << endl;
		if(!infoRegistrazione.compare("non_registrato")) return true;


		//------ qui già so se posso registrare il servizio
		cout << "Scrivi il nome del servizio che vuoi registrate" << endl;
		string serviceToReg;
		cin >> serviceToReg;
		//Service s = new Service(serviceToReg);

		RotateService rotate;
		SOA global;
		if (!global.serviceRegistration(rotate))
			return 0;
	}
		break;

	case 4: {
		cout << "È stato scelto di visualizzare tutti i servizi registrati "
				<< endl;
		string regDispServices = SRC_REG_DISP;
		serverRegister.sendString(regDispServices);
		cout << "Ho inviato la richiesta per visualizzare i servizi registrati"
				<< endl;
		cout << SRC_REG_DISP << endl;
	}
		break;
	}

	serverPaolo.closeAllCommunications();
}
