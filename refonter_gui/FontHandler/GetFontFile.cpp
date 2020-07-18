// GetFontFile.cpp
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

#include "GetFontFile.h"
#include "GetNameValue.h"
#include "GetWinVer.h"

///////////////////////////////////////////////////////////////////////////////
// GetFontFile
//
// Note:  This is *not* a foolproof method for finding the name of a font file.
//        If a font has been installed in a normal manner, and if it is in
//        the Windows "Font" directory, then this method will probably work.
//        It will probably work for most screen fonts and TrueType fonts.
//        However, this method might not work for fonts that are created 
//        or installed dynamically, or that are specific to a particular
//        device, or that are not installed into the font directory.

BOOL GetFontFile(LPCTSTR lpszFontName, CString& strDisplayName, CString& strFontFile)
{
	ASSERT(lpszFontName && lpszFontName[0] != 0);

	_TCHAR szName[2 * MAX_PATH];
	_TCHAR szData[2 * MAX_PATH];

	int nVersion;
	CString strVersion;
	GetWinVer(strVersion, &nVersion);
	TRACE(_T("strVersion=%s\n"), strVersion);

	CString strFont;
	if ((nVersion >= WNTFIRST) && (nVersion <= WNTLAST)) 
		strFont = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	else
		strFont = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");

	strFontFile.Empty();

	BOOL bResult = FALSE;

	while (GetNextNameValue(HKEY_LOCAL_MACHINE, strFont, szName, szData) == ERROR_SUCCESS)
	{
		if (strnicmp(lpszFontName, szName, strlen(lpszFontName)) == 0)
		{
			TRACE(_T("found font\n"));
			strDisplayName = szName;
			strFontFile = szData;
			bResult = TRUE;
			break;
		}

		strFont.Empty();	// this will get next value, same key
	}

	GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);	// close the registry key

	return bResult;
}
