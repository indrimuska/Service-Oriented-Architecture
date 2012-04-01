//
//  SOA/Communication.cpp
//  Service Oriented Architecture
//
//  Created by Indri Muska on 01/04/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Communication.h"

Socket::Socket() {
	sk = -1;
}
Socket::Socket(int sk) {
	this->sk = sk;
}
bool Socket::sendInt(int number) {
	int i = (int) send(sk, &number, sizeof(int), 0);
	if (i == -1 || i < (int) sizeof(int)) {
		cerr << "Errore nell'invio di un intero\n";
		return false;
	}
	return true;
}
bool Socket::sendString(string s_string) {
	if (!sendInt((int) s_string.size())) return false;
	int i = (int) send(sk, s_string.c_str(), s_string.size(), MSG_WAITALL);
	if (i == -1 || i < (int) s_string.size()) {
		cerr << "Errore nel'invio di una stringa\n";
		return false;
	}
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
	char * content = (char *) malloc(info.st_size);
	if ((int) fread(content, 1, info.st_size, file) < info.st_size) {
		cerr << "Impossibile leggere il contenuto del file\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	if (!sendString(filename)) return false;
	if (!sendInt(static_cast<int>(info.st_size))) return false;
	int i = (int) send(sk, content, info.st_size, 0);
	if (i == -1 || i < info.st_size) {
		cerr << "Errore nell'invio del file\n";
		return 0;
	}
	free(content);
	return true;
}
bool Socket::sendBinary(void * binary, size_t length) {
	if (!sendInt((int) length)) return false;
	int i = (int) send(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < (int) length) {
		cerr << "Errore nel'invio di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::serializeObject(void * object, size_t length, string &filename) {
	ofstream file;
	srand((int) time(0));
	stringstream ss_filename;
	ss_filename << rand() << rand();
	filename = ss_filename.str();
	file.open(filename.c_str(), ios_base::binary);
	if (!file.good()) {
		cerr << "Impossibile serializzare l'oggetto\n";
		return false;
	}
	file.write((char *) object, length);
	file.close();
	return true;
}
bool Socket::sendObject(Serializer s) {
	return sendBinary(s.getSerialized(), s.getLength());
}
bool Socket::sendObject(void * object, size_t length) {
	string filename;
	if (!serializeObject(object, length, filename)) return false;
	if (!sendFile(filename)) return false;
	if (remove(filename.c_str())) {
		cerr << "Impossibile eliminare l'oggetto serializzato\n";
		return false;
	}
	return true;
}
bool Socket::receiveInt(int &number) {
	int i = (int) recv(sk, &number, sizeof(int), 0);
	if (i == -1 || i < (int) sizeof(int)) {
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
bool Socket::receiveFile(string where, string &filename) {
	int dimension;
	if (!receiveString(filename)) return false;
	if (!receiveInt(dimension)) return false;
	
	char * binary = (char *) malloc(dimension + 1);
	memset(binary, '\0', dimension + 1);
	int i = (int) recv(sk, binary, dimension, MSG_WAITALL);
	if (i == -1 || i < dimension) {
		cerr << "Il file non è stato ricevuto correttamente.\n";
		return false;
	}
	binary[dimension] = '\0';
	FILE * file;
	if (!(file = fopen((where + '/' + filename).c_str(), "w"))) {
		cerr << "Impossibile creare il file richiesto\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	i = (int) fwrite(binary, 1, dimension, file);
	fclose(file);
	free(binary);
	if (i < dimension) {
		cerr << "Impossibile salvare il contenuto del file richiesto\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	return true;
}
bool Socket::receiveBinary(void * binary, size_t &length) { // binary need to be freed
	int i_length;
	if (!receiveInt(i_length)) return false;
	length = i_length;
	binary = malloc(length);
	bzero(binary, length);
	int i = (int) recv(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < (int) length) {
		cerr << "Errore nella ricezione di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::deserializeObject(void * object, size_t length, string filename) {
	ifstream file;
	file.open(filename.c_str(), ios_base::binary);
	if (!file.good()) {
		cerr << "Impossibile de-serializzare l'oggetto\n";
		return false;
	}
	file.read((char *) object, length);
	file.close();
	return true;
}
bool Socket::receiveObject(Deserializer &d) {
	void * binary = NULL;
	size_t length;
	if (!receiveBinary(binary, length)) return false;
	d = Deserializer(binary, length);
	return true;
}
bool Socket::receiveObject(void * object, size_t length) {
	string where = ".", filename;
	if (!receiveFile(where, filename)) return false;
	if (!deserializeObject(object, length, where + '/' + filename)) return false;
	if (remove((where + '/' + filename).c_str())) {
		cerr << "Impossibile eliminare l'oggetto serializzato\n";
		return false;
	}
	return true;
}
bool Socket::operator==(const Socket &operand) {
	return sk == operand.sk;
}
bool Socket::closeSocket() {
	if (close(sk) == -1) {
		cerr << "Impossibile chiudere la connessione\n";
		return false;
	}
	return true;
}

bool Communicator::startListener(string port, int backlog_queue) {
	listenSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (listenSocket == -1) {
		cerr << "Impossibile connettersi al server richiesto\n"
		"Controllare che sia in linea e disponibile\n";
		return false;
	}
	struct sockaddr_in server;
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(atoi(port.c_str()));
	if (bind(listenSocket, (sockaddr *) &server, sizeof(server)) == -1) {
		cerr << "Impossibile associare l'indirizzo al socket\n"
		"Riprovare più tardi.\n";
		return false;
	}
	if (listen(listenSocket, backlog_queue) == -1) {
		cerr << "Impossibile mettersi in ascolto sul socket creato\n"
		"Riprovare più tardi.\n";
		return false;
	}
	return true;
}
bool Communicator::waitForConnection(Socket &clientSocket) {
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	int client_socket = accept(listenSocket, (sockaddr *) &client, &client_size);
	if (client_socket == -1) {
		cerr << "Impossibile accettare la connessione.\n";
		return false;
	}
	clientSocket = Socket(client_socket);
	sockets.push_back(clientSocket);
	return true;
}
bool Communicator::stopListener() {
	if (close(listenSocket) == -1) {
		cerr << "Impossibile chiudere la connessione.\n"
		"Riprovare più tardi.\n";
		return false;
	}
	return true;
}
bool Communicator::connectTo(string address, string port, Socket &S_socket) {
	int sk = socket(PF_INET, SOCK_STREAM, 0);
	if (sk == -1) {
		cerr << "Impossibile connettersi al server richiesto\n"
		"Controllare che sia in linea e disponibile\n";
		return false;
	}
	struct sockaddr_in server;
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port.c_str()));
	if (!inet_pton(AF_INET, (char *) address.c_str(), &server.sin_addr.s_addr)) {
		cerr << "Indirizzo non valido, il formato deve essere del tipo 127.0.0.1\n";
		return false;
	}
	if (connect(sk, (sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
		cerr << "Impossibile connettersi al server richiesto\n"
		"Controllare che sia in linea e disponibile\n";
		return false;
	}
	S_socket = Socket(sk);
	sockets.push_back(S_socket);
	return true;
}
bool Communicator::closeCommunication(Socket socket) {
	int i;
	for (i = 0; i < (int) sockets.size(); i++)
		if (sockets[i] == socket) break;
	if (!sockets[i].closeSocket()) return false;
	sockets.erase(sockets.begin() + i);
	return true;
}
bool Communicator::closeAllCommunications() {
	for (int i = 0; i < (int) sockets.size(); i++)
		if (!sockets[i].closeSocket()) return false;
	sockets.clear();
	return true;
}
string Communicator::getIP() {
	char hostname[128];
	struct in_addr address;
	struct hostent * ip;
	gethostname(hostname, sizeof(hostname));
	ip = gethostbyname(hostname);
	memcpy(&address, ip->h_addr_list[0], sizeof(in_addr));
	return inet_ntoa(address);
}