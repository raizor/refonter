#include "watchfile.h"

watchfile::watchfile(const char* path)
	: data(NULL)
	, path(path)
{
	hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ftLastRead.dwLowDateTime = 0;
	ftLastRead.dwHighDateTime = 0;
}


watchfile::~watchfile(void)
{
	CloseHandle(hFile);
}

bool watchfile::did_change(void)
{
	FILETIME ftCreate, ftAccess, ftWrite;

	// Retrieve the file times for the file.
	GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);

	if (CompareFileTime(&ftWrite, &ftLastRead) == 1)
	{
		// Must close and reopen file to read from start - hack!
		CloseHandle(hFile);
		hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		LARGE_INTEGER size;
		GetFileSizeEx(hFile, &size);

		if (data)
			delete[] data;

		// Use ints to ensure 4-byte align
		data = new unsigned int[(size.LowPart + 3) / 4];

		DWORD  dwBytesRead = 0;

		ReadFile(hFile, data, size.LowPart, &dwBytesRead, NULL);

		ftLastRead.dwLowDateTime = ftWrite.dwLowDateTime;
		ftLastRead.dwHighDateTime = ftWrite.dwHighDateTime;

		return true;
	}

	return false;

}
