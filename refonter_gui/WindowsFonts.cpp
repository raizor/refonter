#include "WindowsFonts.h"

WindowsFont::WindowsFont(wstring name, wstring path)
{
	Name = name;
	Path = path;
}

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
		WindowsFont* f = new WindowsFont(szValueName, szValueData);
		fonts.push_back(*f);
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
		WindowsFont* f = new WindowsFont(szValueName, szValueData);
		fonts.push_back(*f);
		dwIndex++;
	}
}

WindowsFont WindowsFontList::GetFont(std::wstring name)
{
	for (int i = 0; i < fonts.size(); i++)
	{
		if (wcscmp(fonts.at(i).Name.c_str(), name.c_str()) == 0) {
			return fonts.at(i);
		}
		WindowsFont* f = new WindowsFont(L"", L"");
		return *f;
	}
}
