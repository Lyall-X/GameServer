#ifndef _Compression_h_
#define _Compression_h_

class Compression
{
public:
	static int zlib_Compression(char* input, int inCount, char* output, int& outCount);
	static int zlib_UnCompression(char* input, int inCount, char* output, int& outCount);
	static void zlib_Error(int iError);
	static int zlib_HttpGZipDecompress(char* input, int inCount, char* output, int& outCount);
};

#endif