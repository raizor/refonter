#pragma once

#if _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#include <string>
#endif
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include "refonter.h"
#include "refonter_glu_tesselator.h"
#include "WindowsFonts.h"
#include "gl/gl8k.h"
#include "gl/Shader.h"

// Convenience font class
// This class uses OpenGL 3.3 array buffers to draw characters
class Font
{
public:
	static refonter_font* font;
	static refonter_tesselation_object* tesselated_chars;
	static refonter_tesselation_settings* settings;
	static GLuint* bufferids;
	static Shader* shaderVisNormals;
	static Shader* shaderVisObject;
	static Shader* shaderLambert;

	Font(unsigned char* font_blob, refonter_tesselation_settings* settings, double subdivision_tolerance);

	void DrawLetterInternal(uint32_t i, bool drawNormals, float offset[3], float camPos[3], float camRot[3]);
	void DrawLetter(uint32_t i, float offset[3], float camPos[3], float camRot[3]);
	void Write(const char* str);
	float GetWidth(const char* str);
};

