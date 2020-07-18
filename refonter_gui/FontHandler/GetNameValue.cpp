// GetNameValue.cpp
//
// Copyright (C) 2001 Hans Dietrich
//
// This software is released into the public domain.  
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed 
// or implied warranty.  I accept no liability for any 
// damage or loss of business that this software may cause. 
//
///////////////////////////////////////////////////////////////////////////////

#include "GetNameValue.h"

///////////////////////////////////////////////////////////////////////////////
// GetNameValue
LONG GetNameValue(HKEY key, LPCTSTR subkey, LPCTSTR valuename, LPTSTR retdata)
{
	ASSERT(subkey && subkey[0] != 0);
	ASSERT(valuename);
	ASSERT(retdata);

	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
	
	*retdata = 0;

	if (retval == ERROR_SUCCESS) 
	{
		TRACE(_T("RegOpenKeyEx ok\n"));
		DWORD datasize = MAX_PATH;
		BYTE data[MAX_PATH];
		DWORD type = 0;

		retval = RegQueryValueEx(hkey, valuename, NULL, &type, data, &datasize);
		if (retval == ERROR_SUCCESS) 
		{
			lstrcpy(retdata, (const char *)data);
			TRACE(_T("RegQueryValueEx ok\n"));
		}
		else
		{
			TRACE(_T("RegQueryValueEx failed\n"));
		}
		RegCloseKey(hkey);
	}
	else
	{
		TRACE(_T("RegOpenKeyEx failed\n"));
	}

	return retval;
}

///////////////////////////////////////////////////////////////////////////////
// GetNextNameValue
LONG GetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData)
{
	static HKEY hkey = NULL;
	static DWORD dwIndex = 0;
	LONG retval;

	if (subkey == NULL && szName == NULL && szData == NULL)
	{
		TRACE(_T("closing key\n"));
		if (hkey)
			RegCloseKey(hkey);
		hkey = NULL;
		return ERROR_SUCCESS;
	}

	if (subkey && subkey[0] != 0)
	{
		retval = RegOpenKeyEx(key, subkey, 0, KEY_ALL_ACCESS, &hkey);
		if (retval != ERROR_SUCCESS)
		{
			TRACE(_T("RegOpenKeyEx failed\n"));
			return retval;
		}
		else
		{
			TRACE(_T("RegOpenKeyEx ok\n"));
		}
		dwIndex = 0;
	}
	else
	{
		dwIndex++;
	}
	
	ASSERT(szName != NULL && szData != NULL);

	*szName = 0;
	*szData = 0;

	char szValueName[MAX_PATH];
	DWORD dwValueNameSize = sizeof(szValueName)-1;
	BYTE szValueData[MAX_PATH];
	DWORD dwValueDataSize = sizeof(szValueData)-1;
	DWORD dwType = 0;

	retval = RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL, 
		&dwType, szValueData, &dwValueDataSize);
	if (retval == ERROR_SUCCESS) 
	{
		TRACE(_T("szValueName=<%s>  szValueData=<%s>\n"), szValueName, szValueData);
		lstrcpy(szName, (char *)szValueName);
		lstrcpy(szData, (char *)szValueData);
	}
	else
	{
		TRACE(_T("RegEnumKey failed\n"));
	}

	return retval;
}
