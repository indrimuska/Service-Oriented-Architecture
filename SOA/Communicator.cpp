//
//  Communicator.cpp
//  SOA
//
//  Created by Indri Muska on 14/03/12.
//  Copyright (c) 2012 Indri Muska. All rights reserved.
//

#include "Communicator.h"

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
bool Communicator::waitForClients(Socket &clientSocket) {
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	char * client_address = (char *) malloc(INET_ADDRSTRLEN);
	int client_socket = accept(listenSocket, (sockaddr *) &client, &client_size);
	if (client_socket == -1) {
		cerr << "Impossibile accettare la connessione.\n";
		return false;
	}
	inet_ntop(AF_INET, &client.sin_addr, client_address, INET_ADDRSTRLEN);
	clientSocket = Socket(client_socket);
	sockets.push_back(clientSocket);
	free(client_address);
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
	for (i = 0; i < sockets.size(); i++)
		if (sockets[i] == socket) break;
	if (!sockets[i].closeSocket()) return false;
	sockets.erase(sockets.begin() + i);
	return true;
}
bool Communicator::closeAllCommunications() {
	for (int i = 0; i < sockets.size(); i++)
		if (!sockets[i].closeSocket()) return false;
	sockets.clear();
	return true;
}