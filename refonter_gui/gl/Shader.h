#ifndef SHADER_H
#define SHADER_H

#include <windows.h>
#include "gl8k.h"
#include "Watchfile.h"
#ifdef _DEBUG
#include "watchfile.h"
#endif

class Shader
{
public:
	char definesString[5000];
	const char *strVs, *strFs, *strGs;
	GLint shaderProg;
	Shader(const char* vs, const char* gs, const char* fs, const char* defines, const char* shaderName);
	void SetActive(bool active);
#ifdef _DEBUG
	bool reload();
	void refresh();
#endif

private:
	GLuint shaderVertex;
	GLuint shaderGeometry;
	GLuint shaderFragment;

#ifdef _DEBUG
	bool haveGeomShader;
	const char* vsTxtPath;
	const char* gsTxtPath;
	const char* fsTxtPath;
	watchfile* watchFileVertex;
	watchfile* watchFileGeometry;
	watchfile* watchFileFragment;

#endif
};

#endif

