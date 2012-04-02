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
	string localServerPort = "";
	SOA global;

	/*global.setServerRegister(SRaddress, SRport);
		if (!global.serverRegistration(SPaddress, SPport)) return 0;
		if (!global.serviceRegistration(rotate)) return 0;*/

	/*Da utilizzare poi per le demo su lan
	 // Connessione al Service Register
	 cout << "Insert server register address: ";
	 cin >> SRaddress;
	 cout << "Insert server register port: ";
	 cin >> SRport;
	 */

	cout << " * LOCAL SERVER * \n\n";
	//cout << "Porta del server Register: ";
	//cin >> port;
	//server.startListener(argv[1]);
	//serverPaolo.connectTo("127.0.0.1", port, serverRegister);

	while (1) {
		cout << endl << "-------------------------------------------------------" << endl;
		cout << "Scegli l'operazione da effettuare" << endl;
		cout << "Premi 1 se vuoi impostare il service register a cui collegarsi (setServerRegistrer)" << endl;
		cout << "Premi 2 se vuoi visualizzare tutti i server registrati" << endl;
		cout << "Premi 3 se vuoi registrare un servizio (per farlo devi aver già registrato il server)" << endl;
		cout << "Premi 4 se vuoi visualizzare tutti i SERVIZI registrati" << endl;
		cout << "Premi 5 per vedere le impostazioni di questo server" << endl;
		cout << "Premi 6 se vuoi registrare un server" << endl;
		cout << "-------------------------------------------------------" << endl;
		int richiesta;
		cin >> richiesta;

		switch (richiesta) {

		case 1:{
			string SRaddress, SRport;
			cout << "Digitare l'indirizzo del service register: es 127.0.0.1" << endl;
			cin >> SRaddress;
			//cout << SRaddress << endl;
			cout << "Digitare il numero di porta del service register" << endl;
			cin >> SRport;
			//cout << SRport << endl;
			if (!global.setServerRegister(SRaddress, SRport)){
				SRaddress = "";
				SRport = "";
				cout << "Impossibile registrare il service register" << endl;
			} else {
				cout << "Registrazione del service register avvenuta correttamente" << endl;
			}


		}
		break;

		case 6: {
			string SPaddress = serverPaolo.getIP();
			cout << "Porta su cui far girare questo server: ";
			cin >> localServerPort;
			cout << "SERVER REGISTER - SRaddress: " << global.SRaddress << endl;
			cout << "SERVER REGISTER - SRport: " << global.SRport << endl;
			cout << "LOCAL SERVER - SPaddress: " << SPaddress << endl;
			cout << "LOCAL SERVER - SPport: " << localServerPort << endl;
			if (!global.serverRegistration(SPaddress, localServerPort)){
				cout << "Impossibile registrare il server" << endl;
				return 0; //questo return va tolto, al posto va gestito l'errore, in modo che il server rimanga comunque attivo
			}
			//global.serverRegistration(SPaddress, localServerPort);

			/*if(localServerPort != ""){
				cout << "Questo server è stato già registrato" << endl;
				break;
			}
			cout << "È stato scelto di fare la registrazione di un server"
					<< endl;
			string regReq = SRV_REG_REQ;
			serverRegister.sendString(regReq);

			cout << serverPaolo.getIP() << endl;
			cout << "Porta su cui far girare questo server: ";
			cin >> localServerPort;
			port = localServerPort; //mi salvo la port che poi verrà utilizzata sempre per questo server
			string serverInfo = serverPaolo.getIP() + ":" + localServerPort;
			serverRegister.sendString(serverInfo);
			serverRegister.sendString(serverPaolo.getIP());

			serverRegister.sendString(localServerPort);
			cout << "Voglio registrare: " + serverInfo << endl;
			*/

		}
			break;
		case 2: {
			cout << "È stato scelto di visualizzare i server registrati"
					<< endl;
			string regDisp = SRV_REG_DISP;
			serverRegister.sendString(regDisp);
			cout
					<< "Ho inviato la richiesta per visualizzare i server registrati: " << regDisp
					<< endl;
		}
			break;
		case 3: {

			cout << "È stato scelto di fare la registrazione di un servizio"
					<< endl;
			if (!localServerPort.compare("")) {
				cout
						<< "Non è stata ancora registrata la porta del server, per farlo occorre registrare il server sul register"
						<< endl;
				return false;
			}
			string servReq = SRC_REG_REQ;
			serverRegister.sendString(servReq); //prologo
			cout << "Qui ho inviato la richiesta di registrazione del servizio"
					<< endl;
			//fino a qui OK
			string serverInfo = serverPaolo.getIP() + ":" + localServerPort;
			cout << "Sto per inviare serverInfo = " << endl;
			cout << serverInfo << endl;
			serverRegister.sendString(serverInfo); //1
			cout << "In questo momento ho già inviato serverInfo " << endl;

			string infoRegistrazione;
			serverRegister.receiveString(infoRegistrazione); //2
			cout << "Info registrazione = " << infoRegistrazione << endl;
			cout << "superato il controllo" << endl;

			if (!infoRegistrazione.compare("non_registrato")) {
				cout
						<< "Prima di poter registrare un servizio è necessario che il server sia registrato"
						<< endl;
				return true;
			}

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
			cout
					<< "Ho inviato la richiesta per visualizzare i servizi registrati"
					<< endl;
			cout << SRC_REG_DISP << endl;
		}
			break;
		case 5: {
					cout << "Indirizzo IP: " << serverPaolo.getIP() << endl;
					cout << "Porta del server: " <<  localServerPort <<endl;
				}
				break;
		}


	}

	serverPaolo.closeAllCommunications();
}
