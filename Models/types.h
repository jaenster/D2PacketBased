//
// Created by jaenster on 17/07/2020.
//

#ifndef D2PacketBased_TYPES_H
#define D2PacketBased_TYPES_H

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef word
#define word  unsigned short
#endif

#ifndef dword
#define dword unsigned int
#endif

#define rbyte(offset) data[offset]
#define rword(offset) (*reinterpret_cast<word *>(data + offset))
#define rdword(offset) (*reinterpret_cast<dword *>(data + offset))
#define rstring(name, offset, size) char #name[size]; strcpy_s(#name, sizeof(#name), reinterpret_cast<const char *>(data + offset));

;


#endif //D2PacketBased_TYPES_H
