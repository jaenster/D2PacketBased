#ifndef _OFFSET_H
#define _OFFSET_H



void DefineOffsets();

unsigned long GetDllOffset(int num);
unsigned long GetDllOffset(char *DllName, int Offset);


#endif