#pragma once

#include "D2Structs.h"
#include "D2Ptrs.h"
#include "Offset.h"

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

void StartGame(const char* dir);

void __fastcall D2OpenFile(char* szFile);
void D2OpenFile_ASM();
Level* __fastcall GetLevel(ActMisc *misc, unsigned long levelno);
void D2CLIENT_InitGameMisc(void);

void __stdcall ExceptionHandler(void);
unsigned long D2ClientInterface(void);
unsigned long D2LAUNCH_QueryInterface(void);