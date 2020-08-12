#pragma once
#include <Windows.h>
#include <GL/gl.h>
#include "glext.h"

extern "C" {

#define DEBUG_FUNC_COUNT 2
#define FUNC_COUNT 43

#ifdef _DEBUG

#include <GL/glu.h>

#define OGL( STAT ) { GLenum err = glGetError(); STAT; err = glGetError(); if( err != GL_NO_ERROR ) zmsg("OpenGL error: %s(%i): %s", __FILE__,__LINE__,(char*)gluErrorString( err ) ); }
#else
#define umsg(...) void(0)
#define zwarning(...) void(0)

#define OGL( STAT ) STAT
#endif
	
#ifdef _DEBUG
	#define GLEXT_FUNCTION_COUNT FUNC_COUNT + DEBUG_FUNC_COUNT
#else
	#define GLEXT_FUNCTION_COUNT FUNC_COUNT
#endif
extern void *myglfunc[GLEXT_FUNCTION_COUNT];

static float cubeVerts[] = {

	1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,

	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

	1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

static const int cubeIndices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23 };

struct color4f
{
	float r, g, b, a;
};

struct color3f
{
	float r, g, b;
};

struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y, z;
};

static const char *strs[] = 
{
	"glCreateProgram",				// 0
	"glCreateShader",				// 1
	"glShaderSource",				// 2
	"glCompileShader",				// 3
	"glAttachShader",				// 4
	"glLinkProgram",				// 5
	"glUseProgram",					// 6

	"glUniform4fv",					// 7
	"glGetUniformLocation",			// 8
	"glGetObjectParameterivARB",	// 9
	"glGetInfoLogARB",				// 10

	"glGenBuffers",					// 11
	"glBindBuffer",					// 12
	"glBufferData",					// 13
	"glEnableVertexAttribArray",	// 14
	"glVertexAttribPointer",		// 15

	"glUniform1f",					// 16
	"glUniform1i",					// 17
	"glUniformMatrix3fv",			// 18
	"glUniformMatrix4fv",			// 19
	"glUniform3fv",					// 20

	"glActiveTexture",				// 21
	"glGenerateMipmapEXT",			// 22
	"glTexImage3D",					// 23

	"glGenFramebuffersEXT",			// 24
	"glBindFramebufferEXT",			// 25
	"glFramebufferTextureEXT",		// 26
	"glUniform2fv",					// 27
	"glDisableVertexAttribArray",	// 28
	"glFramebufferTextureLayerEXT",	// 29
	"glDrawBuffers",				// 30
	"glFramebufferTexture2D",		// 31

	"glGetProgramiv",				// 32
	"glGetProgramInfoLog",			// 33
	"glCheckFramebufferStatusEXT",	// 34
	"glDrawElementsInstanced",	// 35
	"glDrawArraysInstanced",	// 35
	"glGetAttribLocation",
	"glVertexAttribDivisor",

	"glUniform3f",
	"glClientActiveTexture",
	"glUniform1fv",
	"glDeleteBuffers",
	"glBufferSubData",
	
#if _DEBUG

	"glDeleteShader",				// 36
	"glDetachShader",				// 37
#endif
};

#define glCreateProgram	               ((PFNGLCREATEPROGRAMPROC)myglfunc[0])					// 0
#define glCreateShader		           ((PFNGLCREATESHADERPROC)myglfunc[1])					// 1
#define glShaderSource                 ((PFNGLSHADERSOURCEPROC)myglfunc[2])					// 2
#define glCompileShader                ((PFNGLCOMPILESHADERPROC)myglfunc[3])				// 3
#define glAttachShader                 ((PFNGLATTACHSHADERPROC)myglfunc[4])					// 4
#define glLinkProgram                  ((PFNGLLINKPROGRAMPROC)myglfunc[5])					// 5
#define glUseProgram                   ((PFNGLUSEPROGRAMPROC)myglfunc[6])					// 6

#define glUniform4fv                   ((PFNGLUNIFORM4FVPROC)myglfunc[7])					// 7
#define glGetUniformLocation           ((PFNGLGETUNIFORMLOCATIONARBPROC)myglfunc[8])		// 8
#define glGetObjectParameteriv         ((PFNGLGETOBJECTPARAMETERIVARBPROC)myglfunc[9])		// 9
#define glGetInfoLog                   ((PFNGLGETINFOLOGARBPROC)myglfunc[10])				// 10

#define glGenBuffers                   ((PFNGLGENBUFFERSPROC)myglfunc[11])					// 11
#define glBindBuffer                   ((PFNGLBINDBUFFERPROC)myglfunc[12])					// 12
#define glBufferData                   ((PFNGLBUFFERDATAPROC)myglfunc[13])					// 13
#define glEnableVertexAttribArray      ((PFNGLENABLEVERTEXATTRIBARRAYPROC)myglfunc[14])		// 14
#define glVertexAttribPointer          ((PFNGLVERTEXATTRIBPOINTERPROC)myglfunc[15])			// 15

#define glUniform1f                    ((PFNGLUNIFORM1FPROC)myglfunc[16])					// 16
#define glUniform1i                    ((PFNGLUNIFORM1IPROC)myglfunc[17])					// 17
#define glUniformMatrix3fv             ((PFNGLUNIFORMMATRIX3FVPROC)myglfunc[18])			// 18
#define glUniformMatrix4fv             ((PFNGLUNIFORMMATRIX4FVPROC)myglfunc[19])			// 19
#define glUniform3fv                   ((PFNGLUNIFORM3FVPROC)myglfunc[20])					// 20

#define glActiveTexture                ((PFNGLACTIVETEXTUREPROC)myglfunc[21])				// 21
#define glGenerateMipmapEXT            ((PFNGLGENERATEMIPMAPPROC)myglfunc[22])				// 22
#define glTexImage3D				   ((PFNGLTEXIMAGE3DPROC)myglfunc[23])					// 23

#define glGenFramebuffersEXT		   ((PFNGLGENFRAMEBUFFERSEXTPROC)myglfunc[24])			// 24
#define glBindFramebufferEXT		   ((PFNGLBINDFRAMEBUFFEREXTPROC)myglfunc[25])			// 25
#define glFramebufferTextureEXT	       ((PFNGLFRAMEBUFFERTEXTUREEXTPROC)myglfunc[26])		// 26
#define glUniform2fv                   ((PFNGLUNIFORM2FVPROC)myglfunc[27])					// 27

#define glDisableVertexAttribArray     ((PFNGLDISABLEVERTEXATTRIBARRAYPROC)myglfunc[28])	// 28

#define glFramebufferTextureLayerEXT   ((PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC)myglfunc[29])	// 29
#define glDrawBuffers                  ((PFNGLDRAWBUFFERSPROC)myglfunc[30])					// 30
#define glFramebufferTexture2D         ((PFNGLFRAMEBUFFERTEXTURE2DPROC)myglfunc[31])		// 31		

#define glGetProgramiv                 ((PFNGLGETUNIFORMIVPROC)myglfunc[32])					// 32
#define glGetProgramInfoLog            ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[33])				// 33
#define glCheckFramebufferStatusEXT    ((PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)myglfunc[34])		// 34
#define glDrawElementsInstanced        ((PFNGLDRAWELEMENTSINSTANCEDEXTPROC)myglfunc[35])		// 35
#define glDrawArraysInstanced          ((PFNGLDRAWARRAYSINSTANCEDEXTPROC)myglfunc[36])		// 36
#define glGetAttribLocation            ((PFNGLGETATTRIBLOCATIONPROC)myglfunc[37])		// 37
#define glVertexAttribDivisor          ((PFNGLVERTEXATTRIBDIVISORARBPROC)myglfunc[38])		// 38

#define glUniform3f                    ((PFNGLUNIFORM3FPROC)myglfunc[39])		// 39
#define glClientActiveTexture          ((PFNGLCLIENTACTIVETEXTUREPROC)myglfunc[40])		// 40

#define glUniform1fv                   ((PFNGLUNIFORM1FVPROC)myglfunc[41])		// 41

#define glDeleteBuffers                ((PFNGLDELETEBUFFERSPROC)myglfunc[42])		// 42
#define glBufferSubData                ((PFNGLBUFFERSUBDATAPROC)myglfunc[43])		// 43

#if _DEBUG	
	#define glDeleteShader                 ((PFNGLDELETESHADERPROC)myglfunc[44])					// 35
	#define glDetachShader		  		   ((PFNGLDETACHSHADERPROC)myglfunc[45])					// 36
#endif

void InitGlExtensions();

}