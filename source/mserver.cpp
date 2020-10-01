#include "server.h"

int main()
{
	Server server;
	server.open("2000");
	server.list(5);

	return 0;
}
