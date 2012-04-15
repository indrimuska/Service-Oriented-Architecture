/**
 * @file	SOA/SOA.hpp
 * @brief	Dichiarazione della classe globale per l'utilizzo della libreria
 *
 * @date	28/03/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef SOA_SOA_hpp
#define SOA_SOA_hpp

#include "Service.hpp"
#include "Communication.hpp"

/**
 * @def		CONN_ACK_REQ
 * @brief	Messaggio standard per la richiesta di conferma della connessione
 *
 * @def		CONN_ACK_RESP
 * @brief	Messaggio standard per la conferma della connessione
 *
 * @def		SRV_REG_REQ
 * @brief	Messaggio standard per la richiesta di registrazione di un server
 *
 * @def		SRV_REG_RESP
 * @brief	Messaggio standard per la conferma di registrazione di un server
 *
 * @def		SRV_REG_DISP
 * @brief	
 *
 * @def		SRC_REG_REQ
 * @brief	Messaggio standard per la richiesta di registrazione di un servizio
 *
 * @def		SRC_REG_RESP
 * @brief	Messaggio standard per la conferma di registrazione di un servizio
 *
 * @def		SRC_REG_DISP
 * @brief	
 *
 * @def		SRV_UNREG_REQ
 * @brief	Messaggio standard per la richiesta di de-registrazione di un server
 *
 * @def		SRV_UNREG_RESP
 * @brief	Messaggio standard per la conferma di de-registrazione di un server
 *
 * @def		SRC_UNREG_REQ
 * @brief	Messaggio standard per la richiesta di de-registrazione di un servizio
 *
 * @def		SRC_UNREG_RESP
 * @brief	Messaggio standard per la conferma di de-registrazione di un servizio
 *
 * @def		SRV_REQ
 * @brief	Messaggio standard per la richiesta dell'indirizzo di un service provider
 *
 * @def		SRV_RESP
 * @brief	Messaggio standard per la conferma della richiesta indirizzo di un service provider
 *
 */
#define CONN_ACK_REQ    "Could you give an ACK for the connection, please?"
#define CONN_ACK_RESP   "This is a simple ACK packet"
#define SRV_REG_REQ	    "I'd like me to register"
#define SRV_REG_RESP    "Ok, now you're registred"
#define SRV_REG_DISP    "I want to know who is registered"
#define SRC_REG_REQ     "Hey, I have a new service for any client!"
#define SRC_REG_RESP    "Well, your service is now registred"
#define SRC_REG_DISP    "I want to know the available services"
#define SRV_UNREG_REQ   "I don't want to serv clients anymore"
#define SRV_UNREG_RESP  "Starting from now, you will not contacted"
#define SRC_UNREG_REQ   "Sorry, I can't give a service"
#define SRC_UNREG_RESP  "Don't worry, you'll not asked for that service again"
#define SRV_REQ         "I need the address of one Service Provider that support this service"
#define SRV_RESP        "This is the address of the SP you've requested"

using namespace std;

/**
 * @class	SOA
 * @brief	Classe globale per la creazione di un'applicazione di tipo S.O.A\.
 *
 * La classe SOA contiene metodi specifici per:
 *   - Settare globalmente l'indirizzo e la porta del registro dei servizi
 *         (metodo setServiceRegister())
 *   - Registrare un service provider presso il registro dei servizi
 *         (metodo serverRegistration())
 *   - Registrare un servizio presso il registro dei servizi
 *         (metodo serviceRegistration())
 *   - De-registrare un servizio presso il registro dei servizi
 *         (metodo serviceUnRegistration())
 *   - De-registrare un service provider il registro dei servizi
 *         (metodo serverUnRegistration())
 *   - Richiedere l’indirizzo di un service provider al registro dei servizi
 *         (metodo getServerAddress())
 */
class SOA {
private:
	string SRaddress;  ///< Indirizzo IP del service register
	string SRport;     ///< Porta di ascolto del service register
	string SPaddress;  ///< Indirizzo IP del service provider
	string SPport;     ///< Porta di ascolto del service provider
	Communicator comm; ///< Comunicazioni con il service register
	
	/**
	 * @brief	Invia una richiesta al service register
	 * @param	request		Messaggio standard che definisce la richiesta
	 * @param	SRsocket	Socket di connessinoe con il service register
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool sendRequest(string request, Socket &SRsocket);
public:
	/**
	 * @brief	Imposta l'indirizzo e la porta del service register
	 * @param	SRaddress	Indirizzo IP del service register
	 * @param	SRport		Porta di ascolto del service register
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool setServiceRegister(string SRaddres, string SRport);
	/**
	 * @brief	Imposta l'indirizzo e la porta del service provider
	 * @param	SPaddress	Indirizzo IP del service provider
	 * @param	SPport		Porta di ascolto del service provider
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 *
	 * Utilizzato unicamente dal service provider prima di poter fare richieste di registrazione
	 * del server e dei servizi che esso supporta.
	 */
	void setServiceProvider(string SPaddres, string SPport);
	/**
	 * @brief	Richiede la registrazione del service provider al service register
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool serverRegistration();
	/**
	 * @brief	Richiede la registrazione di un servizio al service register
	 * @param	s	Servizio da registrare
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool serviceRegistration(Service &s);
	/**
	 * @brief	Richiede la de-registrazione di un service provider al service register
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool serverUnRegistration();
	/**
	 * @brief	Richiede la de-registrazione di un servizio al service register
	 * @param	s	Servizio da de-registrare
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool serviceUnRegistration(Service &s);
	/**
	 * @brief	Richiede l'indirizzo e la porta di un service provider al service register
	 * @param	s		Servizio del quale richiedere l'indirizzo del service provider
	 * @param	address	Indirizzo del service provider ottenuto
	 * @param	port	Porta di ascolto del service provider ottenuto
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool getServerAddress(Service &s, string &address, string &port);
};

#endif
