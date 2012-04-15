/**
 * @file	SOA/Communication.hpp
 * @brief	Dichiarazione degli oggetti necessari alla comunicazione attraverso i socket TCP
 *
 * @date	01/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef SOA_Communication_hpp
#define SOA_Communication_hpp

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Parameters.hpp"

using namespace std;

/**
 * @class	Socket
 * @brief	Identifica un socket e tutte le operazioni necessarie per la comunicazione
 *
 * Gli oggetti di tipo Socket rappresentano i mezzi di comunicazione attravero cui
 * poter comunicare in modo semplice ed immediato sfruttando i socket TCP di sistema.
 */
class Socket {
private:
	int sk; ///< Socket di connessione
public:
	/**
	 * @brief	Costruttore di default
	 */
	Socket();
	/**
	 * @brief	Inizializza un oggetto di tipo Socket assegnando l'indice del socket
	 * @param	sk	Indice del socket associato
	 */
	Socket(int sk);
	/**
	 * @brief	Invia un intero attraverso il socket
	 * @param	number	Intero da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendInt(int number);
	/**
	 * @brief	Invia una stringa attraverso il socket
	 * @param	s_string	Stringa da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendString(string s_string);
	/**
	 * @brief	Invia un file attraverso il socket
	 * @param	filename	Percorso del file da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendFile(string filename);
	/**
	 * @brief	Invia un insieme di bit attraverso il socket
	 * @param	binary	Indirizzo iniziale dell'insieme di bit da inviare
	 * @param	lenght	Lunghezza dell'insieme di bit da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendBinary(void * binary, size_t length);
	/**
	 * @brief	Invia un parametro attraverso il socket
	 * @param	p	Parametro da inviare
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool sendParameter(parameter &p);
	/**
	 * @brief	Riceve un intero attraverso il socket
	 * @param	number	Intero ricevuto
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveInt(int &number);
	/**
	 * @brief	Riceve una stringa attraverso il socket
	 * @param	string	Stringa ricevuta
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveString(string &s_string);
	/**
	 * @brief	Riceve un file attraverso il socket
	 * @param	where		Cartella di destinazione in cui verrà salvato il file
	 * @param	filename	Nome del file ricevuto
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveFile(string where, string &filename);
	/**
	 * @brief	Riceve un insieme di bit attraverso il socket
	 * @param	binary	Indirizzo iniziale in cui memorizzare l'insieme di bit
	 * @param	length	Lunghezza dell'insieme di bit da ricevere
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 *
	 * Il buffer di memoria in cui verrà ricevuto l'insieme di bit dev'essere già
	 * stato preallocato in precedenza.
	 */
	bool receiveBinary(void * binary, size_t length);
	/**
	 * @brief	Riceve un parametro attraverso il socket
	 * @param	p	Parametro ricevuto
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool receiveParameter(parameter &p);
	/**
	 * @brief	Controlla se due oggetti di tipo Socket sono uguali
	 * @param	operand	Oggetto di tipo Socket con cui fare il confronto
	 * @return	Restituisce vero se i due oggetti sono uguali, falso altrimenti
	 */
	bool operator==(const Socket &operand);
	/**
	 * @brief	Chiude un socket
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool closeSocket();
};

/**
 * @class	Communicator
 * @brief	Inizializza una comunicazione sia dal lato dei client che dal lato dei server
 *
 * Attraverso un oggetto di tipo Communicator un client può iniziare un comunicazione con
 * uno o più server memorizzando lo stato di ogni connessione al suo interno. D'altro lato,
 * un server può facilmente aprire un socket di ascolto e mettersi in attesa di uno o più
 * client contemporaneamente. Al termine delle operazioni, è possibile terminare e chiudere
 * tutti i socket precedentemente aperti per mezzo del metodo closeAllCommunications().
 */
class Communicator {
private:
	int listenSocket;       ///< Socket di ascolto
	vector<Socket> sockets; ///< Vettore dei socket di connessione aperti
public:
	/**
	 * @brief	Apre un socket di ascolto
	 * @param	port			Porta nella quale attendere le connessioni
	 * @param	backlog_queue	Numero di client in attesa di essere serviti
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool startListener(string port, int backlog_queue = 10);
	/**
	 * @brief	Si mette in attesa che un client si connetta (bloccante)
	 * @param	clientSocket	Socket di connessione col client
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool waitForConnection(Socket &clientSocket);
	/**
	 * @brief	Si mette in attesa che un client si connetta (bloccante)
	 * @param	clientSocket	Socket di connessione col client
	 * @param	clientAddress	Indirizzo IP del client collegato
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool waitForConnection(Socket &clientSocket, string &clientAddress);
	/**
	 * @brief	Chiude il socket di ascolto
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool stopListener();
	/**
	 * @brief	Apre una connessione con un server
	 * @param	address		Indirizzo del server a cui ci si vuole collegare
	 * @param	port		Porta in cui il server si è messo in attesa
	 * @param	S_socket	Socket di connessione col server
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool connectTo(string address, string port, Socket &S_socket);
	/**
	 * @brief	Chiude tutti i socket precedentemente aperti
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool closeAllCommunications();
	/**
	 * @brief	Restituisce l'indirizzo IP della macchina
	 * @return	Indirizzo IP della macchina
	 */
	string getIP();
};

#endif
