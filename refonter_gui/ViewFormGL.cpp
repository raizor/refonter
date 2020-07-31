///////////////////////////////////////////////////////////////////////////////
// ViewFormGL.cpp
// ==============
// View component of OpenGL dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2006-08-15
///////////////////////////////////////////////////////////////////////////////

#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::ViewFormGL()
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewFormGL::~ViewFormGL()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::initControls(HWND handle)
{
    // set all controls
    buttonAnimate.set(handle, IDC_ANIMATE);
    radioFill.set(handle, IDC_FILL);
    radioWireframe.set(handle, IDC_WIREFRAME);
    radioPoint.set(handle, IDC_POINT);
    trackbarRed.set(handle, IDC_RED);
    trackbarGreen.set(handle, IDC_GREEN);
    trackbarBlue.set(handle, IDC_BLUE);
	comboFont.set(handle, IDC_COMBO_FONT);
	trackbarResolution.set(handle, IDC_RESOLUTION);
	trackbarPointSize.set(handle, IDC_POINTSIZE);
	trackbarDepth.set(handle, IDC_DEPTH);
	check3d.set(handle, IDC_3D);
	
    // initial state
    radioFill.check();

    trackbarRed.setRange(0, 255);
    trackbarRed.setPos(0);

    trackbarGreen.setRange(0, 255);
    trackbarGreen.setPos(0);

    trackbarBlue.setRange(0, 255);
    trackbarBlue.setPos(0);
	
	comboFont.setCurrentSelection(0);

	trackbarResolution.setRange(1, 100);
	trackbarResolution.setPos(0);
	
	trackbarPointSize.setRange(1, 100);
	trackbarPointSize.setPos(0);

	trackbarDepth.setRange(-10, 10);

	check3d.check();

	FontList = new WindowsFontList();

	for (auto &font : FontList->fonts) // access by reference to avoid copying
	{
		comboFont.addString(font.Name.c_str());
	}
}



///////////////////////////////////////////////////////////////////////////////
// update caption of animate button
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::animate(bool flag)
{
    if(flag)
        buttonAnimate.setText(L"Stop");
    else
        buttonAnimate.setText(L"Animate");
}



///////////////////////////////////////////////////////////////////////////////
// update trackbars
///////////////////////////////////////////////////////////////////////////////
void ViewFormGL::updateTrackbars(HWND handle, int position)
{
	if (handle == trackbarResolution.getHandle())
	{
		trackbarResolution.setPos(position);
	}
	else if (handle == trackbarPointSize.getHandle())
	{
		trackbarPointSize.setPos(position);
	}
}

WindowsFont* ViewFormGL::GetSelectedFont() {
	int index = SendMessage(comboFont.getHandle(), CB_GETCURSEL, 0, 0);
	wchar_t fontName[MAX_PATH];
	SendMessage(comboFont.getHandle(), CB_GETLBTEXT, (WPARAM)index, (LPARAM)fontName); //error: invalid conversion from 'WORD' to 'HWND__*'
	return FontList->GetFont(fontName);
}
