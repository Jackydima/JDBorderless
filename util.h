#pragma once
#include <string>
#include <iostream>
#include <tchar.h>


#ifdef UNICODE
using string = std::wstring;
#define tstrcpy_s wcscpy_s
#define print(format, ...) std::wprintf(format, __VA_ARGS__)
#else
using string = std::string;
#define tstrcpy_s strcpy_s
#define print(format, ...) std::fprintf(stdout, format, __VA_ARGS__)
#endif
//If not Build in DEBUG-Mode, than remove ignore all print's for performance hihi
#ifndef DEBUG
#define print(format, ...) ;
#endif

