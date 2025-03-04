#pragma once

#include <windows.h>
#include <vector>
#include "util.h"
#include <map>
#include <tlhelp32.h>
#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib")

struct Process {
	UINT pid;
	string name;
};

std::map < DWORD, Process > ListProcesses(std::map<DWORD, Process>& pList);