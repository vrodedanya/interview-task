#include "code.h"

#ifdef DEBUG
	#include <iostream>
#endif

unsigned int CL::get_length(const char* file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open()) 
	{
		perror(file_name);
		exit(1);
	}
	int length{0};
	while(!file.eof())
	{
		file.get();
		length++;
	}
	length--;
	file.close();
	return length;
}

unsigned int CL::count_bytes(const unsigned int& length)
{
	unsigned int bytes = 0;
	while (pow(2, bytes * 8) <= length)
	{
		bytes++;
	}
	return bytes;
}

unsigned char* CL::encode_length(const unsigned int& length)
{
	if (length >= 128) // if length >= 128 allocate 2 or more bytes and first byte store count of bytes with 8-bit equal 1
	{
		unsigned int bytes_count = count_bytes(length);
		unsigned char* binary_length = new unsigned char[bytes_count + 1];
		unsigned int buf_len = length;
		
		binary_length[0] = 128;
		binary_length[0] |= bytes_count;

		for (unsigned int i = bytes_count ; i > 0 ; i--)
		{
			binary_length[i] = 0;
			binary_length[i] = (char)buf_len;
			buf_len = buf_len >> 8;
		}

		return binary_length;
	}
	else // just store length in 7-1 bits and 8 bit is equal zero
	{
		unsigned char* binary_length = new unsigned char[1];
		binary_length[0] = length;
		return binary_length;
	}
}

unsigned int CL::decode_length(const unsigned char* binary_length)
{
	unsigned char first_byte = binary_length[0];
	unsigned int length = 0;
	if ((first_byte & 128) == 128)
	{
		int bytes_count = first_byte & 127;
		for (int i = 1 ; i < bytes_count + 1 ; i++)
		{
			length = length << 8;
			length |= binary_length[i];
		}
	}
	else
	{
		length = first_byte & 127;
	}
	return length;
}
