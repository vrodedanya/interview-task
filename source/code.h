#ifndef CODE_H
#define CODE_H

#pragma once

#include <fstream>
#include <cmath>

/*
 * This class contains functions for encoding and decoding length
 * from tag-length-value
 */

class CL
{
private:
/*
 * Returns max bytes to store number
 */
	static unsigned int count_bytes(const unsigned int& length);

public:

/*
 * Returns encoded length
 */
	static unsigned char* encode_length(const unsigned int& length);
/*
 * Returns decoded length
 */
	static unsigned int decode_length(const unsigned char* binary_length);
/*
 * Returns file size in bytes
 */
	static unsigned int get_length(const char* file_name);

};

#endif
