//
//  Application/Alerts.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 11/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include <iostream>

#include "../SOA/Service.h"

using namespace std;

class Alerts {
public:
	void generalWarning(string warning) {
		cout << "\033[1;33m" << warning << "\033[0m\n\n";
	}
	void notifyError(string error) {
		cerr << "\033[1;31m" << error << "\033[0m\n\n";
	}
	void successMessage(string message) {
		cerr << "\033[1;32m" << message << "\033[0m\n\n";
	}
	void serviceRequestMessage(Service &s) {
		cout << "Richiesta del servizio \033[1;34m" << s.getServiceName() << "\033[0m:\n";
	}
	void serviceRequestFailed() {
		notifyError("Richiesta rifiutata");
	}
	void serviceRequestSuccess() {
		successMessage("Servizio confermato");
	}
};