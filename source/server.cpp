#include "server.h"

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void* sock_addr(sockaddr *sa)
{
	if (sa->sa_family == AF_INET) 
	{
		return &(((sockaddr_in*)sa)->sin_addr);
	}

	return &(((sockaddr_in6*)sa)->sin6_addr);
}

void Server::open(const char* port)
{
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
			std::cerr << "setsockopt error" << std::endl;
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
		std::cerr << "Server failed" << std::endl;
		exit(1);
	}

	freeaddrinfo(servinfo);
}

void Server::list(const unsigned int& backlog)
{
	if (listen(listen_socket, backlog) == -1)
	{
		std::cerr << "listen error" << std::endl;
		exit (1);
	}
	socklen_t sin_size;

	sa.sa_handler = sigchld_handler; 
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
	{
		std::cerr << "sigaction" << std::endl;
		exit(1);
	}

	while (1)
	{
		sin_size = sizeof(client);
		accept_socket = accept(listen_socket, (sockaddr *)&client, &sin_size);
		if (accept_socket == -1)
		{
			continue;
		}
		char s[INET_ADDRSTRLEN];
		inet_ntop(client.ai_family, sock_addr((sockaddr *)&client), s, sizeof(s));
		std::cout << "server: got connection from " << s << std::endl;
		if (!fork()) 
		{ 
			close(listen_socket); 
			if (send(accept_socket, "Hello, world!", 13, 0) == -1)
			{
				std::cerr << "sending message error" << std::endl;
			}
			close(accept_socket);
			exit(0);
		}
		close(accept_socket); 
	}
}

