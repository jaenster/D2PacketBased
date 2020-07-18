#ifndef COMPRESSION_H
#define COMPRESSION_H

class Compression
{
public:
	static int CompressPacket(const void* dataIn, int sizeIn, void* dataOut, int sizeOut);
	static int DecompressPacket(const void* dataIn, int sizeIn, void* dataOut, int sizeOut);
	static int GetPacketSize(const void* data, int size);
};

#endif //COMPRESSION_H
