/**
 * @file	Application/ServiceRegister.hpp
 * @brief	Dichiarazione del registro dei servizi e dei suoi metodi
 *
 * @date	15/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef Application_ServiceRegister_hpp
#define Application_ServiceRegister_hpp

#include <map>
#include <list>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "../SOA/SOA.hpp"
#include "../SOA/Communication.hpp"

using namespace std;

/**
 * @class	ServiceRegister
 * @brief	Registro dei servizi
 *
 * Il registro dei servizi è un particolare server che si occupa di memorizzare l'elenco dei
 * service provider e dei servizi che ognuno di questi vuole offrire. Inoltre permette
 * l'interazione con la classe SOA per la ricezione di tutti i messaggi standard dichiarati
 * nella libreria (ad esempio per la richiesta di un  service provider in grado di fornire
 * determinati servizi specifici, o per la richiesta di registrazione di un servizio).
 */
class ServiceRegister {
private:
	/**
	 * @class	ServerInfo
	 * @brief	Informazioni relative ad un service provider
	 */
	class ServerInfo {
	private:
		string address; ///< Indirizzo IP del service provider
		string port;    ///< Porta di ascolto del service provider
	public:
		/**
		 * @brief	Inizializza le informazioni di un server
		 * @param	serverName	Nome del server
		 */
		ServerInfo(string serverName);
		/**
		 * @brief	Verifica la correttezza delle informazioni inserite
		 * @return	Restituisce vero se il nome del server è corretto, falso altrimenti
		 */
		bool isValid();
		/**
		 * @brief	Restituisce l'indirizzo IP del service provider
		 * @return	Indirizzo IP del service provider
		 */
		string getAddress();
		/**
		 * @brief	Restituisce la porta di ascolto del service provider
		 * @return	Porta di ascolto del service provider
		 */
		string getPort();
		/**
		 * @brief	Restituisce il nome del service provider
		 * @return	Nome del service provider
		 */
		string getServerName();
	};
	/**
	 * @class	ServiceInfo
	 * @brief	Informazioni relative ad un service provider che fornisce un servizio
	 */
	class ServiceInfo {
	private:
		string address; ///< Indirizzo IP del service provider
		string port;    ///< Porta di ascolto del service provider
		string service; ///< Nome del servizio offerto
	public:
		/**
		 * @brief	Inizializza le informazioni di un server che fornisce un servizio
		 * @param	serviceName	Nome completo del servizio offerto dal service provider
		 */
		ServiceInfo(string serviceName);
		/**
		 * @brief	Verifica la correttezza delle informazioni inserite
		 * @return	Restituisce vero se il nome completo del servizio è corretto, falso altrimenti
		 */
		bool isValid();
		/**
		 * @brief	Restituisce l'indirizzo IP del service provider
		 * @return	Indirizzo IP del service provider
		 */
		string getAddress();
		/**
		 * @brief	Restituisce la porta di ascolto del service provider
		 * @return	Porta di ascolto del service provider
		 */
		string getPort();
		/**
		 * @brief	Restituisce il nome del servizio
		 * @return	Nome del servizio
		 */
		string getService();
		/**
		 * @brief	Restituisce il nome del service provider
		 * @return	Nome del service provider
		 */
		string getServerName();
		/**
		 * @brief	Restituisce il nome completo del servizio offerto dal service provider
		 * @return	Nome completo del servizio offerto dal service provider
		 */
		string getServiceName();
	};
	
	string address;                           ///< Indirizzo del service register
	string port;                              ///< Porta di ascolto del server register
	vector<ServerInfo> servers;               ///< Elenco dei service provider registrati
	map<string, list<ServiceInfo> > services; ///< Elenco dei servizi registrati e dei server associati
	
	/**
	 * @brief	Invia un messaggio di ACK
	 * @param	sk	Socket di connessione
	 * @param	ack	Messaggio di ACK
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendAck(Socket * sk, string ack);
	/**
	 * @brief	Controlla che un service provider sia registrato
	 * @param	serverName	Nome del service provider
	 * @return	Restituisce vero se il server è registrato, falso altrimenti
	 */
	bool isServerRegistered(string serverName);
	/**
	 * @brief	Controlla che un servizio offerto da un server sia registrato
	 * @param	serverName	Nome completo del server
	 * @param	serviceName	Nome del servizio
	 * @return	Restituisce vero se il servizio è registrato, falso altrimenti
	 */
	bool isServiceRegistered(string serverName, string service);
	/**
	 * @brief	Elimina un server dall'elenco dei server associato ad un servizio
	 * @param	service		Nome del servizio in cui cercare il server
	 * @param	serverName	Nome del server da eliminare
	 */
	void deleteServerFromServersList(string service, string serverName);
	
	/**
	 * @brief	Invia un messaggio di conferma della connessione ai client/server che lo richiedono
	 * @param	sk	Socket di connessione con il service provider
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool confirmConnection(Socket * sk);
	/**
	 * @brief	Registra un service provider
	 * @param	sk	Socket di connessione con il service provider
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serverRegistration(Socket * sk);
	/**
	 * @brief	Registra un servizio
	 * @param	sk	Socket di connessione con il service provider
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serviceRegistration(Socket * sk);
	/**
	 * @brief	De-registra un service provider
	 * @param	sk	Socket di connessione con il service provider
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serverUnRegistration(Socket * sk);
	/**
	 * @brief	De-registra un servizio
	 * @param	sk	Socket di connessione con il service provider
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serviceUnRegistration(Socket * sk);
	/**
	 * @brief	Invia l'indirizzo di un server in grado di fornire il servizio richiesto
	 * @param	sk	Socket di connessione con il client
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serverRequest(Socket * sk);
	
public:
	/**
	 * @brief	Imposta l'indirizzo e la porta del service register
	 * @param	address	Indirizzo IP del service register
	 * @param	port	Porta di ascolto del service register
	 */
	ServiceRegister(string address, string port);
	/**
	 * @brief	Gestisce le richieste in arrivo
	 * @param	sk	Socket di connessione con il client
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool serveRequest(Socket * sk);
};


#endif
