#include "client.h"
#include <iostream>

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		Client client;
		client.con(argv[1], argv[2]);
	}
	else
	{	
		std::cout << "Format: client <ip> <port>" << std::endl;
	}
	return 0;
}
