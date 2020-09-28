#include <iostream>
#include <string>
#include <bitset>
#include "code.h"

int main(int argc, char** argv)
{
	CL cl;
	if (argc > 1)
	{
		std::cout << "Before encoding: " << cl.get_length(argv[1]) << std::endl;

		unsigned char* buf = cl.encode_length(cl.get_length(argv[1]));
		std::cout << "After decoding: " << cl.decode_length(buf) << std::endl;
		delete[] buf;
	}
	else
	{
		std::cout << "Format:" << std::endl;
		std::cout << "cl <file_name>" << std::endl;
	}
	return 0;
}
