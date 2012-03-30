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

using namespace std;

int main(int argc, char ** argv) {

	Socket serverRegister;
	Communicator serverPaolo;
	string port;
	cout << "Porta server?" << endl;
	cin >> port;
	//server.startListener(argv[1]);
	serverPaolo.connectTo("127.0.0.1", port, serverRegister);
	//cout << "Ciao" << endl;
	string regReq = SRV_REG_REQ;
    serverRegister.sendString(regReq);

    cout << serverPaolo.getIP() << endl;
    string serverInfo = serverPaolo.getIP() + ":" + port;
    serverRegister.sendString(serverInfo);

    string regDisp = SRV_REG_DISP;
        serverRegister.sendString(regDisp);

    for(;;);




	serverPaolo.closeAllCommunications();
}
