/**
 * @file	Application/ImageManipulation.hpp
 * @brief	Dichiarazione dei servizi associati ad un server per la manipolazione di immagini
 *
 * @date	07/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef Application_ImageManipulation_h
#define Application_ImageManipulation_h

#include <fstream>
#include <iostream>

#include "../SOA/Service.hpp"

#include "CImg.h"

using namespace cimg_library;

/**
 * @class	ImageManipulation
 * @brief	Generico servizio per la manipolazione delle immagini
 *
 * I server che si occuperano di fornire servizi per la manipolazione delle immagini
 * devo istanziare oggetti derivati da questa classe, la quale si occupa di fornire
 * dei metodi utili per impostare i parametri del servizio o per restituirli. Tali
 * metodi saranno utili anche ai client che infatti devono istanziare solo oggetti di
 * questo tipo.
 */
class ImageManipulation : public Service {
protected:
	bool removeImages;    ///< Rimozione delle immagini temporanee dal server
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
	 * Per default imposta @c removeImages a @c true e la @c workDirectory a
	 * @c "Servers/ImageManipulationServer/"
	 */
	ImageManipulation();
	/**
	 * @brief	Imposta la directory di lavoro
	 * @param	workDirectory	Percorso della directory di lavoro
	 */
	ImageManipulation(string workDirectory);
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
	 * @brief	Restituisce il valore associato ad uno dei parametri di ingresso o di uscita
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @return	Restituisce il valore del parametro attraverso un oggetto di tipo parameter_value
	 */
	parameter_value getParameterValue(parameter_direction direction, int parameter_number);
	/**
	 * @brief	Imposta il valore associato ad uno dei parametri di ingresso o di uscita
	 * @param	direction			Direzione del parametro da scegliere
	 * @param	parameter_number	Indice del parametro da scegliere nel vettore dei parametri
	 * @param	value				Valore da associare al parametro scelto
	 */
	void setParameterValue(parameter_direction direction, int parameter_number, parameter_value &value);
};

/**
 * @class	RotateService
 * @brief	Servizio per la rotazione delle immagini
 *
 * Questa classe dev'essere istanziata unicamente dai service provider che intendono
 * fornire questo servizio. Nel costruttore sono impostati i parametri di ingresso e
 * di uscita, oltre al nome finale da assegnare al servizio. L'esecuzione del servizio
 * viene implementato nel metodo execute()
 */
class RotateService : public ImageManipulation {
public:
	/**
	 * @brief	Imposta il nome della classe e i parametri di ingresso/uscita
	 */
	RotateService();
	/**
	 * @brief	Esegue il servizio
	 */
	bool execute();
};

/**
 * @class	HorizontalFlipService
 * @brief	Servizio per il flip orizzontale delle immagini
 *
 * Questa classe dev'essere istanziata unicamente dai service provider che intendono
 * fornire questo servizio. Nel costruttore sono impostati i parametri di ingresso e
 * di uscita, oltre al nome finale da assegnare al servizio. L'esecuzione del servizio
 * viene implementato nel metodo execute()
 */
class HorizontalFlipService : public ImageManipulation {
public:
	/**
	 * @brief	Imposta il nome della classe e i parametri di ingresso/uscita
	 */
	HorizontalFlipService();
	/**
	 * @brief	Esegue il servizio
	 */
	bool execute();
};

#endif
