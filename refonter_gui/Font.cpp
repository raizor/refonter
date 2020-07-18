#include "Font.h"


WindowsFontList::WindowsFontList() {
	//IDC_COMBO_FONT
	wchar_t* fontpath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
	static HKEY hkey = NULL;

	RegOpenKey(
		HKEY_CURRENT_USER,
		fontpath,
		&hkey);

	static DWORD dwIndex = 0;
	wchar_t szValueName[MAX_PATH];
	wchar_t szValueData[MAX_PATH];
	DWORD dwType = 0;

	while (true) {

		DWORD dwValueNameSize = sizeof(szValueName) - 1;
		DWORD dwValueDataSize = sizeof(szValueData) - 1;
		if (RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL,
			&dwType, (BYTE*)szValueData, &dwValueDataSize) != ERROR_SUCCESS)
		{
			break;
		}
		fontNames.push_back(szValueName);
		fontPaths.push_back(szValueData);
		dwIndex++;
	}

	RegCloseKey(hkey);

	dwIndex = 0;
	RegOpenKey(
		HKEY_LOCAL_MACHINE,
		fontpath,
		&hkey);
	while (true) {

		DWORD dwValueNameSize = sizeof(szValueName) - 1;
		DWORD dwValueDataSize = sizeof(szValueData) - 1;
		if (RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL,
			&dwType, (BYTE*)szValueData, &dwValueDataSize) != ERROR_SUCCESS)
		{
			break;
		}
		fontNames.push_back(szValueName);
		fontPaths.push_back(szValueData);
		dwIndex++;
	}
}