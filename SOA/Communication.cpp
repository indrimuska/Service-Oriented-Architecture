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
	int dimension = static_cast<int>(info.st_size);
	FILE * file;
	if (!(file = fopen(filename.c_str(), "r"))) {
		cerr << "Impossibile aprire il file specificato\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	char * content = (char *) malloc(dimension);
	if ((int) fread(content, 1, dimension, file) < dimension) {
		cerr << "Impossibile leggere il contenuto del file\n"
		"Controllare di avere i permessi necessari\n";
		return false;
	}
	fclose(file);
	if (!sendString(filename)) return false;
	if (!sendInt(dimension)) return false;
	int i = (int) send(sk, content, dimension, MSG_WAITALL);
	if (i == -1 || i < dimension) {
		cerr << "Errore nell'invio del file\n";
		return false;
	}
	free(content);
	return true;
}
bool Socket::sendBinary(void * binary, size_t length) {
	int i = (int) send(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < (int) length) {
		cerr << "Errore nel'invio di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::sendParameter(parameter &p) {
	parameter_direction direction = p.getDirection();
	if (!sendBinary((void *) &direction, sizeof(parameter_direction))) return false;
	parameter_type type = p.getType();
	if (!sendBinary((void *) &type, sizeof(parameter_type))) return false;
	if (!sendInt((int) p.getValueDimension())) return false;
	if (p.getValueDimension()) {
		void * buffer = malloc(p.getValueDimension());
		p.getValue(buffer);
		if (!sendBinary(buffer, p.getValueDimension())) return false;
		free(buffer);
	}
	return true;
}
bool Socket::receiveInt(int &number) {
	int i = (int) recv(sk, &number, sizeof(int), MSG_WAITALL);
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
bool Socket::receiveBinary(void * binary, size_t length) {
	bzero(binary, length);
	int i = (int) recv(sk, binary, length, MSG_WAITALL);
	if (i == -1 || i < (int) length) {
		cerr << "Errore nella ricezione di un insieme di bit\n";
		return false;
	}
	return true;
}
bool Socket::receiveParameter(parameter &p) {
	parameter_direction direction;
	if (!receiveBinary((void *) &direction, sizeof(parameter_direction))) return false;
	parameter_type type;
	if (!receiveBinary((void *) &type, sizeof(parameter_type))) return false;
	int length;
	if (!receiveInt(length)) return false;
	p = parameter(direction, type);
	if (length > 0) {
		void * buffer = malloc(length);
		if (!receiveBinary(buffer, length)) return false;
		p.setValue(buffer, length);
		free(buffer);
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
bool Communicator::waitForConnection(Socket &clientSocket, string &clientAddress) {
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	int client_socket = accept(listenSocket, (sockaddr *) &client, &client_size);
	if (client_socket == -1) {
		cerr << "Impossibile accettare la connessione.\n";
		return false;
	}
	clientSocket = Socket(client_socket);
	char client_address[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client.sin_addr, client_address, INET_ADDRSTRLEN);
	clientAddress = client_address;
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
	//cout << address << endl;
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
