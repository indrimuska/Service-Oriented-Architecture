/**
 * @file	Application/ImageStoring.hpp
 * @brief	Dichiarazione dei servizi associati ad un server per la memorizzazione di immagini
 *
 * @date	09/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef Application_ImageStoring_h
#define Application_ImageStoring_h

#include <string>
#include <iostream>
#include <dirent.h>
#include <boost/thread/shared_mutex.hpp>

#include "../SOA/Service.hpp"

using namespace std;

/**
 * @class	ImageStoring
 * @brief	Generico servizio per la memorizzazione delle immagini
 *
 * I server che si occuperano di fornire servizi per la memorizzazione delle immagini
 * devo istanziare oggetti derivati da questa classe, la quale si occupa di fornire
 * dei metodi utili per impostare i parametri del servizio o per restituirli. Tali
 * metodi saranno utili anche ai client che infatti devono istanziare solo oggetti di
 * questo tipo.
 */
class ImageStoring : public Service {
protected:
	string workDirectory; ///< Directory di lavoro del server
	
	/**
	 * @brief	Preleva il valore da un parametro di tipo @c BUFFER e lo salva come immagine
	 * @param	p			Parametro dal quale prelevare il valore
	 * @param	filename	Nome da assegnare all'immagine
	 * @param	hideWarning	Permette di nascondere gli avvisi se esiste un file con lo stesso nome
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool getImageFromBuffer(parameter &p, string filename, bool hideWarning = false);
	/**
	 * @brief	Inserisce un'immagine come valore di un parametro di tipo @c BUFFER
	 * @param	p			Parametro nel quale inserire l'immagine
	 * @param	filename	Percorso dell'immagine
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool putImageInBuffer(parameter &p, string filename);
public:
	/**
	 * @brief	Costruttore di default
	 *
	 * Per default imposta la @c workDirectory a @c "Servers/ImageStoringServer/"
	 */
	ImageStoring();
	/**
	 * @brief	Imposta la directory di lavoro
	 * @param	workDirectory	Percorso della directory di lavoro
	 */
	ImageStoring(string workDirectory);
	/**
	 * @brief	Inserisce un'immagine come valore di un parametro di ingresso o uscita di tipo @c BUFFER
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @param	filaneme			Percorso dell'immagine
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool setImageAsParameter(parameter_direction direction, int parameter_number, string filename);
	/**
	 * @brief	Preleva il valore da un parametro di ingresso o uscita di tipo @c BUFFER e lo salva come immagine
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @param	filename			Nome da assegnare all'immagine
	 * @return	Restituisce vero se l'operazione è andata a buon fine, falso altrimenti
	 */
	bool getImageFromParameter(parameter_direction direction, int parameter_number, string filename);
	/**
	 * @brief	Restituisce il valore stringa associato ad uno dei parametri di ingresso o uscita
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @return	Valore di tipo stringa associato al parametro scelto
	 */
	string getStringFromParameter(parameter_direction direction, int parameter_number);
	/**
	 * @brief	Restituisce un vettore di stringhe prelevandolo dal valore stringa di uno dei parametri
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @param	delimiter			Stringa con cui splittare la stringa associata al valore del parametro
	 * @return	Vettore di stringhe risultate
	 */
	vector<string> getStringVectorFromParameter(parameter_direction direction, int parameter_number, string delimiter);
	/**
	 * @brief	Restituisce il valore associato ad uno dei parametri di ingresso o di uscita
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @return	Restituisce il valore del parametro attraverso un oggetto di tipo parameter_value
	 */
	parameter_value getParameterValue(parameter_direction direction, int parameter_number);
};

/**
 * @class	StoreImageService
 * @brief	Servizio per la memorizzazione delle immagini sul server
 *
 * Questa classe dev'essere istanziata unicamente dai service provider che intendono
 * fornire questo servizio. Nel costruttore sono impostati i parametri di ingresso e
 * di uscita, oltre al nome finale da assegnare al servizio. L'esecuzione del servizio
 * viene implementato nel metodo execute()
 */
class StoreImageService : public ImageStoring {
private:
	boost::shared_mutex * mutex; ///< Mutua esclusione per l'accesso in scrittura alla cartella
public:
	/**
	 * @brief	Imposta il nome della classe e i parametri di ingresso/uscita
	 * @param	mutex	Semaforo per l'accesso in mutua esclusione alla cartella di lavoro
	 */
	StoreImageService(boost::shared_mutex * mutex);
	/**
	 * @brief	Esegue il servizio
	 *
	 * Il servizio @c 'store image' accede alla cartella secondo la nota polita dei
	 * lettori/scrittori sfruttando i metodi offerti dalla libreria boost. Dovendo
	 * memorizzare un'immagine nella directory di lavoro, il suo ruolo è quello tipico
	 * di uno scrittore.
	 */
	bool execute();
};

/**
 * @class	GetImageService
 * @brief	Servizio per il recupero delle immagini dal server
 *
 * Questa classe dev'essere istanziata unicamente dai service provider che intendono
 * fornire questo servizio. Nel costruttore sono impostati i parametri di ingresso e
 * di uscita, oltre al nome finale da assegnare al servizio. L'esecuzione del servizio
 * viene implementato nel metodo execute()
 */
class GetImageService : public ImageStoring {
private:
	boost::shared_mutex * mutex; ///< Mutua esclusione per l'accesso in lettura alla cartella
public:
	/**
	 * @brief	Imposta il nome della classe e i parametri di ingresso/uscita
	 * @param	mutex	Semaforo per l'accesso in mutua esclusione alla cartella di lavoro
	 */
	GetImageService(boost::shared_mutex * mutex);
	/**
	 * @brief	Esegue il servizio
	 *
	 * Il servizio @c 'get image' accede alla cartella secondo la nota polita dei
	 * lettori/scrittori sfruttando i metodi offerti dalla libreria boost. Dovendo
	 * solamente leggere il contenuto di un'immagine, il suo ruolo è quello tipico
	 * di un lettore.
	 */
	bool execute();
};

/**
 * @class	GetListService
 * @brief	Servizio per il recupero dell'elenco delle immagini memorizzate sul server
 *
 * Questa classe dev'essere istanziata unicamente dai service provider che intendono
 * fornire questo servizio. Nel costruttore sono impostati i parametri di ingresso e
 * di uscita, oltre al nome finale da assegnare al servizio. L'esecuzione del servizio
 * viene implementato nel metodo execute()
 */
class GetListService : public ImageStoring {
private:
	boost::shared_mutex * mutex; ///< Mutua esclusione per l'accesso in lettura alla cartella
public:
	/**
	 * @brief	Imposta il nome della classe e i parametri di ingresso/uscita
	 * @param	mutex	Semaforo per l'accesso in mutua esclusione alla cartella di lavoro
	 */
	GetListService(boost::shared_mutex * mutex);
	/**
	 * @brief	Esegue il servizio
	 *
	 * Il servizio @c 'get list' accede alla cartella secondo la nota polita dei
	 * lettori/scrittori sfruttando i metodi offerti dalla libreria boost. Dovendo
	 * solamente leggere il contenuto della directory, il suo ruolo è quello tipico
	 * di un lettore.
	 */
	bool execute();
};

#endif
