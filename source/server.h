#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

class Server
{
private:
	addrinfo hints;
	addrinfo* servinfo;
	addrinfo client;
	int listen_socket, accept_socket;
	int yes = 1;
	struct sigaction sa;
public:

	void open(const char* port);

	void list(const unsigned int& backlog);
};

#endif
