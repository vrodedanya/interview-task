#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <string>
#include "code.h"

class Server
{
private:
	addrinfo hints;
	addrinfo* servinfo;
	addrinfo client;
	int listen_socket, accept_socket;
	int yes = 1;
	char* port;
public:
	Server(const char* port);

	void open();

	void list(const unsigned int& backlog);

};

void command_parser();
void read_fromSock(int& accept_socket);
#endif
