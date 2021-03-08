#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "Shader.h"
#include "utils.h"
#include <stdio.h>

void MsgBox(const char* title, const char* msg)
{
	MessageBoxA(0, msg, title, MB_OK | MB_TOPMOST);
}

void compileShader(GLuint id, const char* shaderName = NULL)
{
	glCompileShader(id);
#ifdef _DEBUG
	int		result;
	char    info[1536];
	char    msgTitle[2000];

	glGetObjectParameteriv(id, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	glGetInfoLog(id, 1024, NULL, (char *)info);
	if (!result)
	{
		if (strlen(info) > 0)
		{
			sprintf(msgTitle, "SHADER - %s", shaderName);
			MsgBox(msgTitle, info);
		}
	}
#endif
}

Shader::Shader(const char* vs, const char* gs, const char* fs, const char* defines, const char* shaderName)
{

#ifdef _DEBUG
	sprintf(definesString, defines);
#endif
	char scode[65536];
	shaderProg = glCreateProgram();

	shaderVertex = NULL;
	shaderGeometry = NULL;
	shaderFragment = NULL;

	if (vs != NULL)
		shaderVertex = glCreateShader(GL_VERTEX_SHADER);

	if (gs != NULL)
		shaderGeometry = glCreateShader(GL_GEOMETRY_SHADER);

	if (fs != NULL)
		shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);

#ifdef _DEBUG
	// save paths to shader files, so we can reload later 
	vsTxtPath = vs;
	gsTxtPath = gs;
	fsTxtPath = fs;

	watchFileVertex = new watchfile(vsTxtPath);
	if (gsTxtPath) {
		watchFileGeometry = new watchfile(gsTxtPath);
	}
	watchFileFragment = new watchfile(fsTxtPath);

	strVs = vs = LoadShaderText(vsTxtPath);

	haveGeomShader = gs != NULL;

	if (haveGeomShader)
		strGs = gs = LoadShaderText(gsTxtPath);

	strFs = fs = LoadShaderText(fsTxtPath);

#endif

	// set source
	sprintf(scode, "%s%s", defines, vs);
	const char* s = scode;
	glShaderSource(shaderVertex, 1, &s, 0);
	if (gs != NULL)
	{
		sprintf(scode, "%s%s", defines, gs);
		const char* s = scode;
		glShaderSource(shaderGeometry, 1, &s, 0);
	}
	sprintf(scode, "%s%s", defines, fs);
	s = scode;
	glShaderSource(shaderFragment, 1, &s, 0);

	// compile
	compileShader(shaderVertex, shaderName);
	if (gs != NULL) {
		compileShader(shaderGeometry, shaderName);
	}
	compileShader(shaderFragment, shaderName);

	// attach
	glAttachShader(shaderProg, shaderVertex);
	if (gs != NULL)
		glAttachShader(shaderProg, shaderGeometry);

	glAttachShader(shaderProg, shaderFragment);

	glLinkProgram(shaderProg);

	//#if DEBUG
		// Check for errors

	char *errorString;
	GLint status = 0;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &status);

	if (status == 0)
	{
		// Set UI error string
		errorString = "Linking error.";

		// Get error string
		static const int kMaxLogBufferSize = 5000;
		char logBuffer[kMaxLogBufferSize];
		int logBufferSize;
		glGetProgramInfoLog(shaderProg, kMaxLogBufferSize, &logBufferSize, logBuffer);
		MessageBoxA(0, logBuffer, "Shader Compile Error", 0);
	}
	//#endif
}

void Shader::SetActive(bool active)
{
	glUseProgram(active ? shaderProg : 0);
}

#ifdef _DEBUG

void Shader::refresh()
{
	if (watchFileVertex->did_change() || (haveGeomShader && watchFileGeometry->did_change()) || watchFileFragment->did_change()) {
		reload();
	}
}


bool Shader::reload()
{
	char scode[65536];

	if (shaderVertex)
	{
		sprintf(scode, "%s%s", definesString, LoadShaderText(vsTxtPath));
		glDetachShader(shaderProg, shaderVertex);
		glDeleteShader(shaderVertex);
		shaderVertex = glCreateShader(GL_VERTEX_SHADER);
		const char* s = scode;
		glShaderSource(shaderVertex, 1, &s, 0);
		compileShader(shaderVertex);
		glAttachShader(shaderProg, shaderVertex);
	}

	if (shaderGeometry)
	{
		sprintf(scode, "%s%s", definesString, LoadShaderText(gsTxtPath));
		glDetachShader(shaderProg, shaderGeometry);
		glDeleteShader(shaderGeometry);
		shaderGeometry = glCreateShader(GL_GEOMETRY_SHADER);
		const char* s = scode;
		glShaderSource(shaderGeometry, 1, &s, 0);
		compileShader(shaderGeometry);
		glAttachShader(shaderProg, shaderGeometry);
	}

	if (shaderFragment)
	{
		sprintf(scode, "%s%s", definesString, LoadShaderText(fsTxtPath));
		glDetachShader(shaderProg, shaderFragment);
		glDeleteShader(shaderFragment);
		shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
		const char* s = scode;
		glShaderSource(shaderFragment, 1, &s, 0);
		compileShader(shaderFragment);
		glAttachShader(shaderProg, shaderFragment);
	}

	glLinkProgram(shaderProg);

	// Check for errors
	char *errorString;
	GLint status = 0;
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &status);

	if (status == 0)
	{
		// Set UI error string
		errorString = "Linking error.";

		// Get error string
		static const int kMaxLogBufferSize = 5000;
		char logBuffer[kMaxLogBufferSize];
		int logBufferSize;
		glGetProgramInfoLog(shaderProg, kMaxLogBufferSize, &logBufferSize, logBuffer);
		MessageBoxA(0, logBuffer, "Shader Compile Error", 0);
	}

	return status != 0;
}
#endif
