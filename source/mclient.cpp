#include "client.h"
#include <iostream>

int main(int argc, char** argv)
{
	if (argc == 4)
	{
		Client client;
		client.con(argv[1], argv[2], argv[3]);
	}
	else
	{	
		std::cout << "Format: client <ip> <port> <file_name>" << std::endl;
	}
	return 0;
}
