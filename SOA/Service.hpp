/**
 * @file	SOA/Service.hpp
 * @brief	Dichiarazione dei servizi e delle risposte ottenute ad ogni richiesta
 *
 * @date	27/03/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef SOA_Service_hpp
#define SOA_Service_hpp

#include <vector>
#include <iostream>

#include "Parameters.hpp"
#include "Communication.hpp"

/**
 * @def		SERVICE_REQ
 * @brief	Messaggio standard per la richiesta di un servizio
 *
 * @def		SERVICE_RESP
 * @brief	Messaggio standard per la conferma di accettazione di un servizio
 */

#define SERVICE_REQ		"C'mon server, give me this service!"
#define SERVICE_RESP	"Et-voilà, here is your service"

using namespace std;

/**
 * @class	Response
 * @brief	Gestisce le risposte alle richieste di servizio
 *
 * Ogni volta che viene richiesto un servizio, il Service Provider istanzia un oggetto
 * di tipo Reponse nel quale imposta il risultato della richiesta, il messaggio da
 * comunicare e gli eventuali parametri d'uscita del servizio. L'oggetto viene poi
 * inviato al client che controlla l'esito della richiesta e memorizza il risultato.
 */
class Response {
private:
	bool result;                  ///< Esito della risposta
	string message;               ///< Messaggio associato alla risposta
	vector<parameter> parameters; ///< Vettore dei parametri d'uscita
public:
	/**
	 * @brief	Costruttore di default
	 */
	Response();
	/**
	 * @brief	Imposta l'esito della richiesta
	 * @param	result	Esito della richiesta
	 */
	void setResult(bool result);
	/**
	 * @brief	Imposta il messaggio d'errore della richiesta
	 * @param	error	Messaggio di errore
	 *
	 * Quando viene chiamato questo metodo, il risultato viene automaticamente impostato
	 * come negativo, rifiutando di fatto la richiesta di servizio.
	 */
	void setError(string error);
	/**
	 * @brief	Imposta il messaggio di risposta alla richiesta
	 * @param	messaggio	Messaggio di risposta
	 */
	void setMessage(string message);
	/**
	 * @brief	Restituisce l'esito della richiesta
	 * @result	Restituisce vero se la richiesta è stata accettata, falso altrimenti
	 */
	bool getResult();
	/**
	 * @brief	Restituisce il messaggio associato alla richiesta
	 * @result	Messaggio associato alla richiesta
	 */
	string getMessage();
	/**
	 * @brief	Restituisce il vettore di parametri di uscita del servizio
	 * @result	Vettore di parametri di uscita del servizio
	 */
	vector<parameter>& getParameters();
	/**
	 * @brief	Imposta i parametri di uscita del servizio
	 * @param	parameters	Parametri di uscita del servizio
	 */
	void setParameters(vector<parameter> &parameters);
};

/**
 * @class	Service
 * @brief	Generico servizio da richiedere o da implementare
 *
 * La classe Service identifica un servizio per mezzo di una stringa contenente il nome e
 * una lista ordinata di parametri di ingresso e di uscita. Essa presenta:
 *   - @b Interfaccia: attraverso i metodi pubblici setName() e setParameters() oppure
 *         direttamente attraverso il metodo setService() è possibile descrivere il servizio
 *         in termini di nome e del numero e tipo dei parametri. Tale informazioni saranno
 *         utilizzate per controllare la consistenza delle richieste dei client.
 *   - @b Comunicazione: per mezzo del metodo requestService() è possibile fare
 *         richiesta di servizio ai service Provider attraverso i socket (oggetti della
 *         classe Socket). I service Provider che si occupano di gestire le richieste
 *         attraverso il metodo serveRequest(), ricevono il nome del servizio richiesto,
 *         i parametri di ingresso e di uscita e generano una risposta (ogetto della classe
 *         Response) da inviare ai client richiedenti.
 *   - @b Realizzazione: quando una richiesta è stata accettata, prima di inviare la
 *         risposta, viene invocato il metodo execute(). Quest'ultimo si occupa di eseguire
 *         il servizio richiesto e di impostare i parametri di uscita nell'apposito vettore
 *         outParameters. Se l'esito del servizio è positivo, tale vettore viene codificato
 *         nella risposta e inviato al client.
 *
 * Il metodo execute() è definito virtuale e va implementato a seconda dell'applicazione
 * che si vuole realizzare. Non è stato definito virtuale puro (evitando di ottenere di
 * conseguenza una classe Service astratta) in modo da permettere agli utilizzatori di
 * poter creare puntatori generici a oggetti di tipo Service o derivati.
 */
class Service {
protected:
	string name;                     ///< Nome del servizio
	vector<parameter> inParameters;  ///< Vettore ordinato dei parametri d'ingresso
	vector<parameter> outParameters; ///< Vettore ordinato dei parametri d'uscita
	
	/**
	 * @brief	Invia una risposta al client attraverso un socket
	 * @param	sk			Socket di connessione con il client
	 * @param	response	Risposta alla richiesta di servizio
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendResponse(Socket &sk, Response &response);
	/**
	 * @brief	Riceve la risposta da parte del service provider attraverso un socket
	 * @param	sk			Socket di connessione con il service provider
	 * @param	response	Risposta alla richiesta di servizio
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveResponse(Socket &sk, Response &response);
	/**
	 * @brief	Invia un vettore di parametri attraverso un socket
	 * @param	sk			Socket di connessione
	 * @param	parameters	Vettore dei parametri da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendParameters(Socket &sk, vector<parameter> &parameters);
	/**
	 * @brief	Riceve un vettore di parametri attraverso un socket
	 * @param	sk			Socket di connessione
	 * @param	parameters	Vettore dei parametri da ricevere
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveParameters(Socket * sk, vector<parameter> &parameters);
	
	/**
	 * @brief	Esecuzione del servizio
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 *
	 * Da implmentare nell'applicazione
	 */
	virtual bool execute();
public:
	/**
	 * @brief	Imposta il nome del servizio
	 * @param	name	Nome del servizio
	 */
	void setName(string name);
	/**
	 * @brief	Imposta i parametri di ingresso e di uscita del servizio
	 * @param	parameters	Vettore di parametri di ingresso/uscita
	 */
	void setParameters(vector<parameter> &parameters);
	/**
	 * @brief	Imposta nome e parametri di ingresso/uscita del servizio
	 * @param	name		Nome del servizio
	 * @param	parameters	Vettore di parametri di ingresso/uscita
	 */
	void setService(string name, vector<parameter> &parameters);
	/**
	 * @brief	Restituisce il nome del servizio
	 * @return	Nome del servizio
	 */
	string getServiceName();
	
	/**
	 * @brief	Gestisce una richiesta di servizio
	 * @param	sk	Socket di connessione
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 *
	 * Questo metodo viene utilizzato unicamente da parte dei service provider.
	 */
	bool serveRequest(Socket * sk);
	
	/**
	 * @brief	Richiede un servizio
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 *
	 * Questo metodo viene utilizzato unicamente da parte dei client.
	 */
	bool requestService(string SPaddress, string SPport);
};

#endif
