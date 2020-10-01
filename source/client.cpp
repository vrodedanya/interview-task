#include "client.h"

void* get_sockaddr(sockaddr *sa)
{
	return &(((sockaddr_in*)sa)->sin_addr);
}

unsigned char* Client::load_file(const char* file_name, int& len)
{
// Creating length binary code and moving it to first bytes of array
	int length = CL::get_length(file_name);	
	std::cout << "File length: " << length << std::endl;

	unsigned char* code = CL::encode_length(length);
	int bytes = ((*code & 128) == 128) ? (*code & 127) + 1 : 1;
	std::cout << "Code length: " << bytes << std::endl;

	std::cout << "Total length: " << bytes + length << std::endl;	

	unsigned char* buf = new unsigned char[bytes + length]; 
	int i = 0;
	for (; i < bytes ; i++)
	{
		buf[i] = code[i];
	}
	delete[] code;

// Loading file to array
	std::ifstream file(file_name);
	if (!file.is_open()) 
	{
		perror(file_name);
		exit(1);
	}
	while(!file.eof())
	{
		buf[i] = (unsigned char)file.get();
		i++;
	}
	file.close();
	len = length + bytes; 
	return buf;
}


void Client::con(const char* ip, const char* port, const char* file_name)
{
	int length = 0;
	unsigned char* buf = load_file(file_name, length);

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


	int res = 0;
	if ((res = send(listen_socket, buf, length, 0)) == -1) {
		std::cerr << "Sending error" << std::endl;
		exit(1);
	}
	std::cout << "Sending bytes: "<< res << std::endl;
	delete[] buf;

	close(listen_socket);
}
