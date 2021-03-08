///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// ===========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-10
// UPDATED: 2017-11-11
///////////////////////////////////////////////////////////////////////////////

#include "ModelGL.h"
#include "Bmp.h"
#include "Log.h"
#include "refonter_export.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ModelGL::ModelGL() : windowWidth(0), windowHeight(0), animateFlag(false),
                     changeDrawMode(false), drawMode(0),
                     mouseLeftDown(false), mouseRightDown(false),
                     mouseX(0), mouseY(0), cameraAngleX(0), cameraAngleY(0),
                     cameraDistance(5), textureId(0), bgFlag(0),
                     windowResized(false), frameBuffer(0), bufferSize(0)
{
    bgColor[0] = bgColor[1] = bgColor[2] = bgColor[3] = 0;

    // set radius, sectors and stacks for sphere
    sphere.set(1.0f, 72, 36);
	// todo clean this up
	WindowsFont* f = new WindowsFont(L"Cheri", L"cheri.ttf");
	winFont = f;
	fontResolution = 1;
	fontPointSize = 1;
	cameraDistance = -30;
}



///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
ModelGL::~ModelGL()
{
    // deallocate framebuffer
    //delete [] frameBuffer;
    //frameBuffer = 0;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL states and scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::init(refonter_tesselation_settings* tesselation_settings)
{
	this->tesselation_settings = tesselation_settings;

    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);   // background color
    glClearStencil(0);                              // clear stencil buffer
    glClearDepth(1.0f);                             // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
    setCamera(0, 0, 8, 0, 0, 0);

    // set material for sphere
    GLfloat matAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 10.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

	glShadeModel(GL_SMOOTH);
	
	shouldRegenerateFont = true;
	
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void ModelGL::initLights()
{
	const unsigned int NUM_LIGHTS = 3;
	GLfloat lightKa[NUM_LIGHTS][4];
	GLfloat lightKd[NUM_LIGHTS][4];
	GLfloat lightKs[NUM_LIGHTS][4];
	float lightPos[NUM_LIGHTS][4];
	   
    // set up light colors (ambient, diffuse, specular)

	// light 1
	lightKa[0][0] = 0;
	lightKa[0][1] = 0;
	lightKa[0][2] = 0;
	lightKa[0][3] = 1.0f;

	lightKd[0][0] = 0.9f;
	lightKd[0][1] = 0.9f;
	lightKd[0][2] = 0.9f;
	lightKd[0][3] = 1.0f;

	lightKs[0][0] = 1;
	lightKs[0][1] = 1;
	lightKs[0][2] = 1;
	lightKs[0][3] = 1;

	lightPos[0][0] = 0;
	lightPos[0][1] = 0;
	lightPos[0][2] = 5;
	lightPos[0][3] = 0;

	// light 2
	lightKa[1][0] = 0;
	lightKa[1][1] = 0;
	lightKa[1][2] = 0;
	lightKa[1][3] = 1.0f;

	lightKd[1][0] = 0.2f;
	lightKd[1][1] = 0.3f;
	lightKd[1][2] = 0.9f;
	lightKd[1][3] = 1.0f;

	lightKs[1][0] = 1;
	lightKs[1][1] = 1;
	lightKs[1][2] = 1;
	lightKs[1][3] = 1;

	lightPos[1][0] = 0;
	lightPos[1][1] = 5;
	lightPos[1][2] = 2.5f;
	lightPos[1][3] = 0;

	// light 3
	lightKa[2][0] = 0;
	lightKa[2][1] = 0;
	lightKa[2][2] = 0;
	lightKa[2][3] = 1.0f;

	lightKd[2][0] = 0.9f;
	lightKd[2][1] = 0.3f;
	lightKd[2][2] = 0.3f;
	lightKd[2][3] = 1.0f;

	lightKs[2][0] = 0.2f;
	lightKs[2][1] = 0.2f;
	lightKs[2][2] = 0.2f;
	lightKs[2][3] = 0.2f;

	lightPos[2][0] = 0;
	lightPos[2][1] = -2.5f;
	lightPos[2][2] = -0.5f;
	lightPos[2][3] = 0;

	for (unsigned int i = 0; i < NUM_LIGHTS; i++)
	{
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lightKa[i]);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lightKd[i]);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lightKs[i]);
		// position the light    
		glLightfv(GL_LIGHT0 + i, GL_POSITION, lightPos[i]);
		glEnable(GL_LIGHT0+i);                            // MUST enable each light source after configuration
	}


	/*
	glGenBuffers(1, &cubeBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBufferId);
	glBufferData(GL_ARRAY_BUFFER, cubeVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}



///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewport(int w, int h)
{
    // assign the width/height of viewport
    windowWidth = w;
    windowHeight = h;
    //bufferSize = w * h * 4; // rgba
    //frameBuffer = new unsigned char [bufferSize];

    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    float aspectRatio = (float)w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(w)/h, 0.1f, 300.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
}



///////////////////////////////////////////////////////////////////////////////
// toggle to resize window
///////////////////////////////////////////////////////////////////////////////
void ModelGL::resizeWindow(int w, int h)
{
    // assign the width/height of viewport
    windowWidth = w;
    windowHeight = h;
    windowResized = true;
}

void ModelGL::generateFont() {
	unsigned char* blob = 0;
	unsigned int blob_size;

	char path[MAX_PATH];
	wstring your_wchar_in_ws(winFont->Path);
	string your_wchar_in_str(your_wchar_in_ws.begin(), your_wchar_in_ws.end());
	const char* font_path = your_wchar_in_str.c_str(); //wctomb(path, winFont.Path.c_str());
	char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789,.'-!() ";
	//int point_size = 16;
	//int resolution = 72;

	int point_size = 16;
	int resolution = 72;

	if (font)
	{
		free(blob);
	}

	// Create font blob and prepare for export
	refonter_status status = refonter_create_font_blob(&blob, &blob_size, font_path, chars, point_size*kRefonterSubdivision, resolution);
	
	if (status == kStatusOk)
	{
		refonter_font* p_font = (refonter_font*)blob;
		
		delta_encode_points(p_font);
		transform_pointers_to_offsets(p_font);	
		font = new Font((unsigned char*)blob, tesselation_settings, 1.0);
	}
}

void ModelGL::drawCube() {
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), cubeVerts + 0);
	glNormalPointer(GL_FLOAT, 6 * sizeof(float), cubeVerts + 3);

	glPushMatrix();
	//glScalef(0.25f, 0.25f, 0.25f);
	//glTranslatef(cube.x, cube.y, cube.z);
	//shaderPass->SetActive(true);
	glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_INT, cubeIndices);

	//glTranslatef(1.0, 0.0, 0.0);

	//glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
	//shaderPass->SetActive(false);
	glPopMatrix();

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void ModelGL::drawStringPreview(Font* font, char* str) {

	glPushMatrix();

	float textWidth = 0;
	float textHeight = 0;
	char* str2 = str;
	while (*str2)
	{
		char c = *str2;
		for (uint32_t i = 0; i < font->font->num_chars; i++)
		{
			// Find letter
			if (font->font->chars[i].id == c)
			{
				float charWidth = float(font->font->chars[i].width) / float(kRefonterSubdivision);
				float charHeight = float(font->font->chars[i].height) / float(kRefonterSubdivision);

				textWidth += charWidth;
				if (charHeight > textHeight)
					textHeight = charHeight;
			}
		}
		str2++;
	}

	glTranslatef(-(0.5f*textWidth), -(0.25f*textHeight), 0.0f);

	while (*str)
	{
		char c = *str;

		for (uint32_t i = 0; i < font->font->num_chars; i++)
		{
			// Find letter
			if (font->font->chars[i].id == c)
			{
				// Draw letter
				font->DrawLetter(i);

				// Advance right
				float charWidth = float(font->font->chars[i].width) / float(kRefonterSubdivision);
				float charHeight = float(font->font->chars[i].height) / float(kRefonterSubdivision);

				glTranslatef(charWidth, 0.f, 0.f);
			}
		}
		str++;
	}
	glPopMatrix();
}

void ModelGL::drawFontPreview(Font* font)
{
	char* strings[6];
	strings[0] = "ABCDEFGHIJKLMNOPQ";
	strings[1] = "RSTUVWXYZ";
	strings[2] = "abcdefghijklmnopq";
	strings[3] = "rstuvwxyz";
	strings[4] = "1234567890";
	strings[5] = ", . / , =+-[]() {}";

	char* str = strings[0];
	int line = 0;
	float xpos = 0;
	glPushMatrix();
	glTranslatef(0, 0, 0);

	float maxHeight = 0;

	for (uint32_t i = 0; i < font->font->num_chars; i++)
	{
		float charHeight = float(font->font->chars[i].height) / float(kRefonterSubdivision);
		if (charHeight > maxHeight)
		{
			maxHeight = charHeight;
		}
	}

	glTranslatef(0, (maxHeight * 2), 0);

	float lineWidth = font->GetWidth(strings[0]);
	for (unsigned int line = 0; line < 6; line++)
	{
		str = strings[line];
		xpos = -(lineWidth * 0.5f);
		glTranslatef(xpos, 0, 0);

		while (*str)
		{
			char c = *str;
			refonter_coord width = 0;
			refonter_coord height = 0;			

			for (uint32_t i = 0; i < font->font->num_chars; i++)
			{
				// Find letter
				if (font->font->chars[i].id == c)
				{
					// Draw letter
					font->DrawLetter(i);

					// Advance right
					float charWidth = float(font->font->chars[i].width) / float(kRefonterSubdivision);

					glTranslatef(charWidth, 0.f, 0.f);
					xpos += charWidth;

				}
			}
			str++;
		}
		glTranslatef(-xpos, -maxHeight, 0);
	}
	glPopMatrix();
}

void ModelGL::draw2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);

	// TODO: deactivate all textures and program

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);

	// Draw black rects
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	drawFontPreview(font);

	// Restore IntroState
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void ModelGL::draw3D()
{	
	// TODO: deactivate all textures and program
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);   // pitch
	glRotatef(cameraAngleY, 0, 1, 0);   // heading

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);

	// Draw black rects
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//drawFontPreview(font);
	drawStringPreview(font, "raizor");
	//drawCube();
}

///////////////////////////////////////////////////////////////////////////////
// draw 2D/3D scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::draw()
{
    static float angle = 0;

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

	int drawMode2 = 0;

	if (drawMode2 == 0)           // fill mode
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	else if (drawMode2 == 1)      // wireframe mode
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	else if (drawMode2 == 2)      // point mode
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	if (tesselation_settings->font_is_3d)
	{
		draw3D();
	}
	else {
		draw2D();
	}

    glPopMatrix();

	if (windowResized)
	{
		setViewport(windowWidth, windowHeight);
		windowResized = false;
	}

	/*

    // read color framebuffer
    //glReadBuffer(GL_BACK);
    //glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)frameBuffer);

    //for (int i = 0; i < bufferSize; i+=4)
    //{
    //    frameBuffer[i] = (unsigned char)255;
    //    frameBuffer[i+1] = 16;
    //    frameBuffer[i+2] = 16;
    //}
    //glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)frameBuffer);



    if(changeDrawMode)
    {
        if(drawMode == 0)           // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(drawMode == 1)      // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(drawMode == 2)      // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
    }
	*/
    // check if background colour was changed
    if(bgFlag)
    {
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        bgFlag = false;
    }
}



///////////////////////////////////////////////////////////////////////////////
// create texture object from bitmap
///////////////////////////////////////////////////////////////////////////////
void ModelGL::createTexture(int width, int height, int bitCount, const GLvoid* data)
{
    GLenum format, internalFormat;
    switch(bitCount)
    {
        case 8:
            format = GL_LUMINANCE;
            internalFormat = GL_LUMINANCE8;
            break;
        case 24:
            format = GL_RGB;
            internalFormat = GL_RGB8;
            break;
        case 32:
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
            break;
    }

    // gen texture object
    glGenTextures(1, &textureId);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, textureId);

    // select modulate to mix texture with color for shading
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // generate texture mipmaps
    int channelCount = bitCount >> 3;
    switch(channelCount)
    {
    case 1:
        gluBuild2DMipmaps(GL_TEXTURE_2D, channelCount, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        gluBuild2DMipmaps(GL_TEXTURE_2D, channelCount, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case 4:
        gluBuild2DMipmaps(GL_TEXTURE_2D, channelCount, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    }
    //glGenerateMipmap(GL_TEXTURE_2D);
    //Win::log(L"textureID: %ld, %x, %x", textureId, glGetError(), format);

    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}



///////////////////////////////////////////////////////////////////////////////
// load a BMP as texture
///////////////////////////////////////////////////////////////////////////////
unsigned int ModelGL::loadTextureBmp(const char* fileName)
{
    int chans, x, y;
    void* buf;
    Image::Bmp bmp;

    bmp.read(fileName);
    x = bmp.getWidth();
    y = bmp.getHeight();
    chans = bmp.getBitCount() / 8;
    buf = (void*)bmp.getDataRGB();

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // build our texture mipmaps
    switch(chans)
    {
    case 1:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf);
        break;
    case 3:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_RGB, GL_UNSIGNED_BYTE, buf);
        break;
    case 4:
        gluBuild2DMipmaps(GL_TEXTURE_2D, chans, x, y, GL_RGBA, GL_UNSIGNED_BYTE, buf);
        break;
    }

    return texture;
}



///////////////////////////////////////////////////////////////////////////////
// rotate the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::rotateCamera(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
}



///////////////////////////////////////////////////////////////////////////////
// zoom the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::zoomCamera(int delta)
{
    if(mouseRightDown)
    {
        cameraDistance += (delta - mouseY) * 0.05f;
        mouseY = delta;
    }
}
void ModelGL::zoomCameraByWheel(float delta)
{
    cameraDistance += delta;
}



///////////////////////////////////////////////////////////////////////////////
// change drawing mode
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setDrawMode(int mode)
{
    if(drawMode != mode)
    {
        changeDrawMode = true;
        drawMode = mode;
    }
}



///////////////////////////////////////////////////////////////////////////////
// change background colour, the value should be between 0 and 1
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setResolution(int value)
{
	fontResolution = (int)value;
}

void ModelGL::setPointSize(int value)
{
	fontPointSize = (int)value;
}
