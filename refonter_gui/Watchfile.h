#pragma once
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

class watchfile
{
public:
	watchfile(const char* path);
	~watchfile(void);

	bool did_change(void);

	unsigned int* data;
	HANDLE hFile;
	FILETIME ftLastRead;

private:
	const char* path;
};

