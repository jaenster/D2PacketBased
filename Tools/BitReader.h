#pragma once
#include <iostream>

class BitReader
{
public:
    static const char NULL_CHAR = '\0';

    static const int BYTE_MASK = 0xFF;

    static const int BYTE_BYTES = 1;
    static const int WORD_BYTES = 2;
    static const int DWORD_BYTES = 4;
    static const int QWORD_BYTES = 8;

    static const int BYTE_BITS = 8;
    static const int WORD_BITS = 16;
    static const int DWORD_BITS = 32;
    static const int QWORD_BITS = 64;

//protected:
    bool isBigEndian;
	const unsigned char* data;
	int byteOffset;
    int bitOffset;

public:

    BitReader(const unsigned char* data, int byteOffset)
    {
        this->data = data;
		this->isBigEndian = false;
        this->byteOffset = byteOffset;
		this->bitOffset = 0;
    }

	void SkipBytes(int bytes) noexcept(false)
	{
		this->CheckCountEx(bytes);
		this->byteOffset += bytes;
	}

	bool ReadBoolean() noexcept(false)
	{
		unsigned char retVal = 0x00;
		this->Read((unsigned char*)&retVal, BYTE_BITS);
		return retVal != 0;
	}

	bool ReadBoolean(int bits) noexcept(false)
	{
		this->CheckBitsEx(bits, DWORD_BITS);
		int retVal = 0x00;
		this->Read((unsigned char*)&retVal, bits);
		return retVal != 0;
	}

	unsigned char ReadByte() noexcept(false)
	{
		unsigned char retVal = 0x00;
		this->Read((unsigned char*)&retVal, BYTE_BITS);
		return retVal;
	}

	unsigned char ReadByte(int bits) noexcept(false)
	{
		this->CheckBitsEx(bits, BYTE_BITS);
		unsigned char retVal = 0x00;
		this->Read((unsigned char*)&retVal, bits);
		return retVal;
	}

	unsigned short ReadUInt16() noexcept(false)
	{
		unsigned short retVal = 0x00;
		this->Read((unsigned char*)&retVal, WORD_BITS);
		return retVal;
	}

	unsigned short ReadUInt16(int bits) noexcept(false)
	{
		this->CheckBitsEx(bits, WORD_BITS);
		unsigned short retVal = 0x00;
		this->Read((unsigned char*)&retVal, bits);
		return retVal;
	}

	int ReadInt32() noexcept(false)
	{
		int retVal = 0x00;
		this->Read((unsigned char*)&retVal, DWORD_BITS);
		return retVal;
	}

	int ReadInt32(int bits) noexcept(false)
	{
		this->CheckBitsEx(bits, DWORD_BITS);
		int retVal = 0x00;
		this->Read((unsigned char*)&retVal, bits);
		return retVal;
	}

	unsigned int ReadUInt32() noexcept(false)
	{
		unsigned int retVal = 0x00;
		this->Read((unsigned char*)&retVal, DWORD_BITS);
		return retVal;
	}

	unsigned int ReadUInt32(int bits) noexcept(false)
	{
		this->CheckBitsEx(bits, DWORD_BITS);
		unsigned int retVal = 0x00;
		this->Read((unsigned char*)&retVal, bits);
		return retVal;
	}

	std::string ReadString(int bits, unsigned char terminator, int maxLength) noexcept(false)
    {
        this->CheckBitsEx(bits, WORD_BITS);
        this->CheckCountEx(maxLength);
        std::string retVal;
        int i = 0;

		while (i < maxLength)
        {
			unsigned char c = 0x00;
			this->Read(&c, bits);

			if (c == terminator || c == NULL_CHAR)
                break;

			retVal += c;
			i++;
        }

		return retVal;
    }

	void Read(unsigned char* dest, int bits) noexcept(false)
	{
		int cBits, destByte = 0, destBit = 0;

		if (this->isBigEndian)
		{
			destByte = bits >> 3;
			destBit = bits & 7;
			if (destBit == 0) destByte--;
			else destBit = BYTE_BITS - destBit;

			while (true)
			{
				cBits = BYTE_BITS - (this->bitOffset > destBit ? this->bitOffset : destBit);
				if (bits < cBits) cBits = bits;

				if (cBits == BYTE_BITS)
				{
					dest[destByte--] = this->data[this->byteOffset++];
				}
				else
				{
					//NOTE: for reading and copying to individual vars, preserving low bits shouldn't be needed...
					dest[destByte] = (unsigned char)(
						(dest[destByte] & ((BYTE_MASK << (BYTE_BITS - destBit)) | (BYTE_MASK >> (destBit + cBits))))
						| (((this->data[this->byteOffset] << this->bitOffset)
							& (BYTE_MASK << (BYTE_BITS - cBits)) & BYTE_MASK) >> destBit));

					destBit += cBits;
					if ((destBit & BYTE_BITS) == BYTE_BITS)
					{
						destBit ^= BYTE_BITS;
						destByte--;
					}
					this->bitOffset += cBits;
					if ((this->bitOffset & BYTE_BITS) == BYTE_BITS)
					{
						this->bitOffset ^= BYTE_BITS;
						this->byteOffset++;
					}
				}
				if ((bits -= cBits) == 0)
					return;
			}
		}
		//else: Little Endian
		while (true)
		{
			cBits = BYTE_BITS - (this->bitOffset > destBit ? this->bitOffset : destBit);
			if (bits < cBits) cBits = bits;

			if (cBits == BYTE_BITS)
			{
				dest[destByte++] = this->data[this->byteOffset++];
			}
			else
			{
				//NOTE: for reading and copying to individual vars, preserving high bits shouldn't be needed...
				dest[destByte] = (unsigned char)(
					(dest[destByte] & ((BYTE_MASK >> (BYTE_BITS - destBit)) | (BYTE_MASK << (destBit + cBits))))
					| (((this->data[this->byteOffset] >> this->bitOffset) & (BYTE_MASK >> (BYTE_BITS - cBits)))
					   << destBit));

				destBit += cBits;
				if ((destBit & BYTE_BITS) == BYTE_BITS)
				{
					destBit ^= BYTE_BITS;
					destByte++;
				}
				this->bitOffset += cBits;
				if ((this->bitOffset & BYTE_BITS) == BYTE_BITS)
				{
					this->bitOffset ^= BYTE_BITS;
					this->byteOffset++;
				}
			}
			if ((bits -= cBits) == 0)
				return;
		}
	}

	void CheckCountEx(int count) noexcept(false)
	{
		if (count < 1)
		{
			throw "CheckCountEx(int count)";
		}
	}

	void CheckBitsEx(int val, int max) noexcept(false)
	{
		if (val < 1 || val > max)
		{
			throw "CheckBitsEx(int val, int max)";
		}
	}

	void CheckBitsEx(long val, long max) noexcept(false)
	{
		if (val < 1 || val > max)
		{
			throw "CheckBitsEx(long val, long max)";
		}
	}
};
