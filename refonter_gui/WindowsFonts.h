#pragma once

#if _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#include <string>
#endif
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include "refonter.h"
#include "refonter.h"
#include "refonter_glu_tesselator.h"
#include "gl/gl8k.h"

using namespace std;

struct WindowsFont
{
public:
	wstring Name;
	wstring Path;
	WindowsFont();
	WindowsFont(wstring name, wstring path);
};

class WindowsFontList
{
public:
	std::vector<WindowsFont> fonts;
	WindowsFontList();

	WindowsFont* GetFont(std::wstring name);
};
