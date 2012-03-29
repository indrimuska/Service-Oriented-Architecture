//
//  SOA/Socket.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Socket.h"

Socket::Socket() {
	sk = -1;
}
Socket::Socket(int sk) {
	this->sk = sk;
}
bool Socket::sendInt(int number) {
	int i = (int) send(sk, &number, sizeof(int), 0);
	if (i == -1 || i < sizeof(int)) {
		cerr << "Errore nell'invio di un intero\n";
		return false;
	}
	return true;
}
bool Socket::sendString(string s_string) {
	if (!sendInt((int) s_string.size())) return false;
	int i = (int) send(sk, s_string.c_str(), s_string.size(), MSG_WAITALL);
	if (i == -1 || i < s_string.size()) {
		cerr << "Errore nel'invio di una stringa\n";
		return false;
	}
	return true;
}
bool Socket::sendBinary(char * binary, int length) {
	if (!sendInt(length)) return false;
	int i = (int) send(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < length) {
		cerr << "Errore nel'invio di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::serializeObject(void * object, size_t length, string &filename) {
	ofstream file;
	stringstream ss_filename;
	ss_filename << rand();
	file.open(ss_filename.str().c_str(), ios_base::binary);
	if (!file.good()) {
		cerr << "Impossibile serializzare l'oggetto\n";
		return false;
	}
	file.write((char *) object, length);
	file.close();
	filename = ss_filename.str();
	return true;
}
bool Socket::sendObject(void * object, size_t length) {
	string filename;
	if (!serializeObject(object, length, filename)) return false;/*
	int i = (int) send(sk, object, length, MSG_WAITALL);
	if (i == -1 || i < length) {
		cerr << "Errore nel'invio di un insieme di bit\n";
		return false;
	}*/
	return true;
}
bool Socket::sendFile(string filename) {
	struct stat info;
	if (stat(filename.c_str(), &info) == -1) {
		cerr << "Impossibile determinare la dimensione del file specificato\n"
		"Controllare che il file esista e che si abbiano i permessi necessari\n";
		return false;
	}
	FILE * file;
	if (!(file = fopen(filename.c_str(), "r"))) {
		cerr << "Impossibile aprire il file specificato\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	char * content = (char *) malloc(info.st_size + 1);
	if (fread(content, 1, info.st_size, file) < info.st_size) {
		cerr << "Impossibile leggere il contenuto del file\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	content[info.st_size] = '\0';
	if (!sendString(filename)) return false;
	if (!sendInt(info.st_size)) return false;
	if (!sendBinary(content, info.st_size)) return false;
	free(content);
	return true;
}
bool Socket::receiveInt(int &number) {
	int i = (int) recv(sk, &number, sizeof(int), 0);
	if (i == -1 || i < sizeof(int)) {
		cerr << "Errore nella ricezione di un intero\n";
		return false;
	}
	return true;
}
bool Socket::receiveString(string &s_string) {
	int length;
	if (!receiveInt(length)) return false;
	char * c_string = (char *) malloc(length + 1);
	memset(c_string, '\0', length + 1);
	int i = (int) recv(sk, c_string, length, MSG_WAITALL);
	if (i == -1 || i < length) {
		cerr << "Errore nella ricezione di una stringa\n";
		return false;
	}
	c_string[length] = '\0';
	s_string = string(c_string);
	free(c_string);
	return true;
}
bool Socket::receiveBinary(char * binary, int &length) { // RICORDARSI DI FARE LA FREE DI binary
	if (!receiveInt(length)) return false;
	binary = (char *) malloc(length);
	bzero(binary, length);
	int i = (int) recv(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < length) {
		cerr << "Errore nella ricezione di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::operator==(const Socket &operand) {
	return this->sk == operand.sk;
}
bool Socket::closeSocket() {
	if (close(sk) == -1) {
		cerr << "Impossibile chiudere la connessione.\n"
		"Riprovare più tardi.\n";
		return false;
	}
	return true;
}