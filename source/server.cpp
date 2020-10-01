#include "server.h"

Server::Server(const char* port)
{
	this->port = new char[strlen(port)];
	strcpy(this->port,port);
}

void* sock_addr(sockaddr *sa)
{
	return &(((sockaddr_in*)sa)->sin_addr);
}

void read_fromSock(int& accept_socket)
{
	unsigned char first_byte = '0';
	if (recv(accept_socket, &first_byte, 1, 0) == -1)
	{
		std::cerr << "[SERVER]: Receiving message error" << std::endl;
	}
	int length = 0;
	if ((first_byte & 128) == 128)
	{
		length = (first_byte & 127);
		std::cout << "[SERVER]: Bytes count: " << length << std::endl;
		unsigned char* buf = new unsigned char[length + 1];
		buf[0] = first_byte;
		for (int i = 1 ; i < length + 1 ; i++)
		{
			if (recv(accept_socket, &buf[i], 1, 0) == -1)
			{
				std::cerr << "[SERVER]: Receiving message error" << std::endl;
			}
		}
		length = CL::decode_length(buf);
		delete[] buf;	
	}
	else
	{
		length = CL::decode_length(&first_byte);
	}

	std::cout << "[SERVER]: Message length: " << length << std::endl;
	std::cout << std::endl;
	int n = 0;
	int buf;
	for (int i = 0 ; i < length ; i++)
	{
		if ((buf = recv(accept_socket, &first_byte, 1, 0)) == -1)
		{
			std::cerr << "[SERVER]: Receiving message error" << std::endl;
		}
		n += buf;
		std::cout << first_byte;
	}
	std::cout << std::endl;
	std::cout << "[SERVER]: Recieved " << n << " out of " << length << std::endl;
	std::cout << "[SERVER]: Bytes lost: " << length - n << std::endl;

	close(accept_socket);
}

void command_parser()
{
	while (1)
	{
		std::cout << "[user]: ";
		std::string buf;
		std::cin >> buf;
		if (buf == "exit")
		{
			exit(0);
		}
		else if (buf == "clear")
		{
			system("clear");
		}
		else if (buf == "help")
		{
			std::cout << "\tclear" << std::endl;
			std::cout << "\texit" << std::endl;
		}
		else
		{
			std::cout << "[SERVER]: Command '" << buf << "' doesn't exist! Try 'help' to see available commands" << std::endl; 
		}
	}
}

void Server::open()
{
// Initializing
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int rv;
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo : " << gai_strerror(rv) << std::endl;
		exit(1);
	}	
	addrinfo* p;
	for (p = servinfo ; p != NULL ; p = p->ai_next)
	{
		if ((listen_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			std::cerr << "Creating socket error" << std::endl;
			continue;
		}
		
		if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
		{
			std::cerr << "[SERVER]: setsockopt error" << std::endl;
			exit(1);
		}
		
		if (bind(listen_socket, p->ai_addr,p->ai_addrlen) == -1)
		{
			close (listen_socket);
			std::cerr << "bind server error" << std::endl;
			continue;
		}
		break;
	}
	if (p == NULL)
	{
		std::cerr << "[SERVER]: starting failed" << std::endl;
		exit(1);
	}

	freeaddrinfo(servinfo);
}

void Server::list(const unsigned int& backlog)
{
	if (listen(listen_socket, backlog) == -1)
	{
		std::cerr << "[SERVER]: listen error" << std::endl;
		exit (1);
	}
	socklen_t sin_size;

	std::thread parser(command_parser);

	while (1)
	{
		sin_size = sizeof(client);
		accept_socket = accept(listen_socket, (sockaddr *)&client, &sin_size);
		if (accept_socket == -1)
		{
			continue;
		}

		std::cout << "\n[SERVER]: got connection" << std::endl;
	
		std::thread handler(read_fromSock, std::ref(accept_socket));
		handler.detach();

	}
	close(listen_socket);
	parser.join();
}

