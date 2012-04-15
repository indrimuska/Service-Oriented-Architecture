/**
 * @file	SOA/Parameters.hpp
 * @brief	Dichiarazione dei parametri di ogni servizio e degli oggetti che li caratterizzano
 *
 * @date	01/04/2012
 * @author	Indri Muska <indrimuska@gmail.com>
 * @author	Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
 *
 * @copyright Copyright (c) 2012 Indri Muska, Paolo Antonio Rossi. All rights reserved.
 */

#ifndef SOA_Parameters_hpp
#define SOA_Parameters_hpp

#include <cstdlib>
#include <iostream>
#include <string.h>

/**
 * @enum	parameter_direction
 * @brief	Direzione che può assumere ogni parametro
 *
 * Ogni parametro può essere solamente di ingresso (IN) o di uscita (OUT).
 */
enum parameter_direction { IN, OUT };

/**
 * @enum	parameter_type
 * @brief	Tipo a cui può appartenere ogni parametro
 *
 * Il tipo del parametro può essere uno dei seguenti:
 *   - \b @c INT : numero intero
 *   - \b @c DOUBLE : numero floating point
 *   - \b @c STRING : stringa
 *   - \b @c BUFFER : buffer di bytes
 */
enum parameter_type { INT, DOUBLE, STRING, BUFFER };

/**
 * @class	parameter_value
 * @brief	Generico valore di ogni singolo parametro del servizio
 *
 * Il valore che il parametro può assumere può essere di tipo diverso. A tal scopo,
 * ogni valore è identificato da un puntatore a un buffer di byte e dalla dimensione
 * che tale buffer occupa in memoria dinamica.
 */
class parameter_value {
private:
	void * value;     ///< Indirizzo del buffer associato al valore del parametro
	size_t dimension; ///< Dimensione del buffer associato al valore del parametro
public:
	/**
	 * @brief	Imposta la dimensione del parametro a zero
	 */
	parameter_value();
	/**
	 * @brief	Copia il valore intero nel buffer dedicato al parametro
	 * @param	value	Valore intero da assegnare al parametro
	 */
	parameter_value(int value);
	/**
	 * @brief	Copia il floating point nel buffer dedicato al parametro
	 * @param	value	Floating point da assegnare al parametro
	 */
	parameter_value(double value);
	/**
	 * @brief	Copia la stringa nel buffer dedicato al parametro
	 * @param	value	Stringa da assegnare al parametro
	 */
	parameter_value(std::string value);
	/**
	 * @brief	Copia il buffer in memoria dinamica nel buffer dedicato al parametro
	 * @param	value		Indirizzo del buffer in memoria dinamica
	 * @param	dimension	Dimensione del buffer in memoria dinamica
	 */
	parameter_value(void * value, size_t dimension);
	/**
	 * @brief	Costruttore di copia
	 * @param	p	Riferimento ad un oggetto di tipo parameter_value
	 */
	parameter_value(const parameter_value &p);
	/**
	 * @brief	Imposta l'intero come valore associato al parametro
	 * @param	value	Valore intero da assegnare al parametro
	 */
	void setValue(int value);
	/**
	 * @brief	Imposta il floating point come valore associato parametro
	 * @param	value	Floating point da assegnare al parametro
	 */
	void setValue(double value);
	/**
	 * @brief	Imposta la stringa come valore associato al parametro
	 * @param	value	Stringa da assegnare al parametro
	 */
	void setValue(std::string value);
	/**
	 * @brief	Imposta il buffer in memoria dinamica come valore associato al parametro
	 * @param	value		Indirizzo del buffer in memoria dinamica
	 * @param	dimension	Dimensione del buffer in memoria dinamica
	 */
	void setValue(void * value, size_t dimension);
	/**
	 * @brief	Copia il valore del parametro nell'indirizzo specificato
	 * @param	object	Indirizzo dell'oggetto nel quale copiare il valore
	 */
	void get(void * object);
	/**
	 * @brief	Restituisce la dimensione del valore associato al parametro
	 * @return	Dimensione del valore associato al parametro
	 */
	size_t getDimension();
	/**
	 * @brief	Copia il valore del parametro prelevandolo da un altro parameter_value
	 * @param	p	Riferimento ad un oggetto di tipo parameter_value
	 */
	void operator=(const parameter_value &p);
	/**
	 * @brief	Libera il buffer di memoria associato al valore del parametro
	 */
	~parameter_value();
};

/**
 * @class	parameter
 * @brief	Parametro generico
 *
 * Ogni parametro di cui è composto un servizio è composto da un tipo, una direzione
 * e da un valore ad esso associato.
 */
class parameter {
private:
	parameter_direction direction; ///< Direzione del parametro
	parameter_type type;           ///< Tipo del parametro
	parameter_value value;         ///< Valore del parametro
public:
	/**
	 * @brief	Costruttore di default
	 */
	parameter();
	/**
	 * @brief	Inizializza un parametro impostando direzione e tipo
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type);
	/**
	 * @brief	Inizializza un parametro impostando direzione, tipo e un valore intero
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 * @param	value		Valore intero da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type, int value);
	/**
	 * @brief	Inizializza un parametro impostando direzione, tipo e un valore floating point
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 * @param	value		Valore floating point da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type, double value);
	/**
	 * @brief	Inizializza un parametro impostando direzione, tipo e un valore stringa
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 * @param	value		Valore stringa da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type, std::string value);
	/**
	 * @brief	Inizializza un parametro impostando direzione, tipo e un valore intero generico
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 * @param	value		Indirizzo del valore da assegnare al parametro
	 * @param	dimension	Dimensione del valore da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type, void * value, size_t dimension);
	/**
	 * @brief	Inizializza un parametro impostando direzione, tipo e valore
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 * @param	value		Valore da assegnare al parametro
	 */
	parameter(parameter_direction direction, parameter_type type, parameter_value value);
	/**
	 * @brief	Imposta direzione e tipo del parametro
	 * @param	direction	Direzione da assegnare al parametro
	 * @param	type		Tipo da assegnare al parametro
	 */
	void init(parameter_direction direction, parameter_type type);
	/**
	 * @brief	Imposta il valore del parametro attraverso un intero
	 * @param	value	Valore intero da assegnare al parametro
	 */
	void setValue(int value);
	/**
	 * @brief	Imposta il valore del parametro attraverso un intero
	 * @param	value	Valore intero da assegnare al parametro
	 */
	void setValue(double value);
	/**
	 * @brief	Imposta il valore del parametro attraverso un floating point
	 * @param	value	Valore floating point da assegnare al parametro
	 */
	void setValue(std::string value);
	/**
	 * @brief	Imposta il valore del parametro attraverso un buffer in memoria
	 * @param	value		Indirizzo del buffer in memoria
	 * @param	dimension	Dimensione del buffer in memoria
	 */
	void setValue(void * value, size_t dimension);
	/**
	 * @brief	Imposta il valore del parametro attraverso un altro valore
	 * @param	value	Riferimento ad un oggetto di tipo parameter_value
	 */
	void setValue(parameter_value &value);
	/**
	 * @brief	Restituisce la direzione e il tipo del parametro
	 * @param	direction	Riferimento ad un oggetto di tipo parameter_direction
	 * @param	type		Riferimento ad un oggetto di tipo parameter_type
	 */
	void getInfo(parameter_direction &direction, parameter_type &type);
	/**
	 * @brief	Restituisce l'intero corrispondente al valore del parametro
	 * @param	int_value	Riferimento ad un oggetto di tipo intero
	 */
	void getValue(int &int_value);
	/**
	 * @brief	Restituisce il floating point corrispondente al valore del parametro
	 * @param	double_value	Riferimento ad un oggetto di tipo floating point
	 */
	void getValue(double &double_value);
	/**
	 * @brief	Restituisce la stringa corrispondente al valore del parametro
	 * @param	string_value	Riferimento ad un oggetto di tipo stringa
	 */
	void getValue(std::string &string_value);
	/**
	 * @brief	Copia il valore del parametro nell'indirizzo specificato
	 * @param	generic_value	Indirizzo di un oggetto generico
	 */
	void getValue(void * generic_value);
	/**
	 * @brief	Restituisce la direzione del parametro
	 * @return	Direzione del parametro
	 */
	parameter_direction getDirection();
	/**
	 * @brief	Restituisce il tipo del parametro
	 * @return	Tipo del parametro
	 */
	parameter_type getType();
	/**
	 * @brief	Restituisce il valore del parametro
	 * @return	Valore del parametro (oggetto di tipo parameter_value)
	 */
	parameter_value getParameterValue();
	/**
	 * @brief	Restituisce la dimensione del valore del parametro
	 * @return	Dimensione del valore del parametro
	 */
	size_t getValueDimension();
	/**
	 * @brief	Verifica che due parametri siano uguali
	 * @param	p	Parametro con il quale fare il confronto
	 * @return	Vero se i due parametri sono uguali, falso altrimenti
	 */
	bool operator==(const parameter &p);
	/**
	 * @brief	Verifica che due parametri siano diversi
	 * @param	p	Parametro con il quale fare il confronto
	 * @return	Vero se i due parametri sono diversi, falso altrimenti
	 */
	bool operator!=(const parameter &p);
	/**
	 * @brief	Stampa il parametro attraverso standard output stream
	 * @param	o	Standard output stream
	 * @param	p	Parametro da stampare
	 * @return	Stream di uscita dello standard output
	 *
	 * Stampa il parametro nello standard output stream e, laddove possibile, mostra
	 * anche il valore assunto dal parametro se impostato.
	 *
	 * Alcuni esempi di uscita sono:
	 *
	 * \code
	 * direction: IN
	 * type:      DOUBLE
	 * \endcode
	 *
	 * \code
	 * direction: OUT
	 * type:      STRING
	 * value:     Questa è una stringa
	 * \endcode
	 *
	 * \code
	 * direction: IN
	 * type:      BUFFER
	 * value:     value not representable
	 * \endcode
	 */
	friend std::ostream& operator<<(std::ostream &o, const parameter &p);
};

#endif
