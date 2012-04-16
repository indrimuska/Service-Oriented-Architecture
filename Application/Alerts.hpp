/**
 * @file	Application/Alerts.hpp
 * @brief	Dichiarazione e definizione di una classe per la notifica di messaggi
 *
 * @date	11/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#include <iostream>

#include "../SOA/Service.hpp"

using namespace std;

/**
 * @class	Alerts
 * @brief	Stampa messaggi nello standard stream output/error
 */
class Alerts {
public:
	/**
	 * @brief	Stampa un messaggio di warning in giallo nello standard output
	 * @param	warning	Testo del messaggio di warning da stampare
	 */
	void generalWarning(string warning) {
		cout << "\033[1;33m" << warning << "\033[0m\n\n";
	}
	/**
	 * @brief	Stampa un messaggio di errore in rosso nello standard error
	 * @param	error	Testo del messaggio di error da stampare
	 */
	void notifyError(string error) {
		cerr << "\033[1;31m" << error << "\033[0m\n\n";
	}
	/**
	 * @brief	Stampa un messaggio di conferma in verde nello standard output
	 * @param	message	Testo del messaggio di conferma da stampare
	 */
	void successMessage(string message) {
		cerr << "\033[1;32m" << message << "\033[0m\n\n";
	}
	/**
	 * @brief	Stampa una richiesta di servizio in blu nello standard output
	 * @param	s	Servizio richiesto
	 */
	void serviceRequestMessage(Service &s) {
		cout << "Richiesta del servizio \033[1;34m" << s.getServiceName() << "\033[0m:\n";
	}
	/**
	 * @brief	Notifica il rifiuto di una richiesta nello standard error
	 */
	void serviceRequestFailed() {
		notifyError("Richiesta rifiutata");
	}
	/**
	 * @brief	Notifica la conferma di una richiesta nello standard output
	 */
	void serviceRequestSuccess() {
		successMessage("Servizio confermato");
	}
};