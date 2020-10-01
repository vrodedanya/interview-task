#ifndef CLIENT_H
#define CLIENT_H

#include "code.h"
#include <fstream>
#include <iostream>
#include <cstring>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Client
{
private:
	int listen_socket;
	addrinfo hints, *servinfo;
public:

// Connect to server and send file
	void con(const char* ip, const char* port, const char* file_name);

	unsigned char* load_file(const char* file, int& length);
};

#endif
