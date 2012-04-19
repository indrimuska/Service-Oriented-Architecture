/**
 * @file	Application/Threads.hpp
 * @brief	Definizione delle informazioni per la gestione di server multi-threaded
 *
 * @date	12/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef Application_Threads_hpp
#define Application_Threads_hpp

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>

#include "../SOA/Communication.hpp"

using namespace std;

/**
 * @class	ThreadInfo
 * @brief	Classe per una gestione più semplice di thread in server multi-threaded
 *
 * La classe ThreadInfo risulta molto utile per la gestione dei tre principali server
 * di cui è composta l'applicazione: il service register, il server Image Manipulation
 * e il server Image Storing. Esso contiene parametri e metodi per consentire ad ogni
 * thread di cui ogni server è composto di:
 *   - Impostare un thread sullo stato di libero o sullo stato di occupato quando si
 *         quando si deve occupare di gestire uno dei client che si è connesso
 *   - Controllare se un thread è libero e contestualmente occuparlo
 *   - Assegnare il socket di connessione del client al thread scelto
 *   - Notificare l'avvio del thread quando un client si connette
 *   - Attendere la notifica di connessione di un client prima di risvegliare un thread
 *   - Notificare la terminazione di un thread
 *   - Controllare se un thread deve terminare o meno
 *
 * Ogni operazione sullo stato del thread viene eseguita in mutua esclusione per mezzo
 * di semafori mutex della libreria boost con dei lock di tipo scoped_lock. L'avvio e
 * l'arresto dei thread sfrutta le variabili condition della medesima libreria.
 */
class ThreadInfo {
private:
	bool busy;                           ///< Stato del thread
	bool active;                         ///< Attivazione del thread
	boost::mutex mutex;                  ///< Mutua esclusione per l'accesso allo stato
	boost::condition_variable condition; ///< Condizione per mettere il thread in attesa
public:
	Socket client;                       ///< Socket di connessione associato al client
	
	/**
	 * @brief	Costruttore di default
	 */
	ThreadInfo() {
		busy = false;
		active = true;
	}
	/**
	 * @brief	Imposta il thread su uno stato occupato
	 */
	void setBusy() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		busy = true;
	}
	/**
	 * @brief	Imposta il thread su uno stato libero
	 */
	void setFree() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		busy = false;
	}
	/**
	 * @brief	Controlla che il thread sia libero
	 * @return	Restituisce vero se il thread è libero, falso altrimento
	 */
	bool isFree() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		return busy;
	}
	/**
	 * @brief	Controlla che il thread sia libero e lo occupa contestualmente
	 * @return	Restituisce vero se il thread era precedentemente libero, falso altrimenti
	 */
	bool testAndSet() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		if (!busy) {
			busy = true;
			return true;
		}
		return false;
	}
	/**
	 * @brief	Si mette in attesa che venga notificato l'avvio
	 */
	void waitStart() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		while (!busy) condition.wait(scoped_lock);
	}
	/**
	 * @brief	Invia una notifica di avvio al thread
	 */
	void startThread() {
		condition.notify_one();
	}
	/**
	 * @brief	Controlla se un thread può continuare ad eseguire
	 */
	bool isActive() {
		boost::mutex::scoped_lock scoped_lock(mutex);
		return active;
	}
	/**
	 * @brief	Invia la notifica di terminazione al thread
	 */
	void exitThread() {
		mutex.lock();
		busy = true;
		active = false;
		mutex.unlock();
		condition.notify_one();
	}
};

#endif
