#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <unistd.h>

class Client
{
private:
	int listen_socket;
	addrinfo hints, *servinfo;

public:

	void con(const char* ip, const char* port);
};

#endif
