#include "server.h"

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		Server server(argv[1]);
		server.open();
		server.list(5);
	}
	else
	{
		std::cerr << "Format: server <port>" << std::endl;
	}
	return 0;
}
