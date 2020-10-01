#include "client.h"

void* get_sockaddr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Client::con(const char* ip, const char* port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int rv;
	if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddinfo error" << std::endl;
	}

	addrinfo* p;
	for (p = servinfo ; p != NULL ; p = p->ai_next)
	{
		if ((listen_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			continue;
		}
		if (connect(listen_socket, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(listen_socket);
			continue;
		}
		break;
	}
	if (p == NULL)
	{
		std::cerr << "Connection failed" << std::endl;
		exit(1);
	}

	char s[INET_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_sockaddr((sockaddr*)p->ai_addr), s, sizeof(s));
	std::cout << "Connecting to " << s << std::endl;

	freeaddrinfo(servinfo);

	int numbytes;
	char buf[100];
	if ((numbytes = recv(listen_socket, buf, 99, 0)) == -1) {
		std::cerr << "Reading error" << std::endl;
		exit(1);
	}

	buf[numbytes] = '\0';

	std::cout << "client: received " << buf << std::endl;

	close(listen_socket);
}
