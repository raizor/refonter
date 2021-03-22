#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <math.h>

#include "Utils.h"
#include "gl8k.h"

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _DEBUG

#include <stdio.h>

int msys_rand(int *seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	return((seed[0] >> 16) & 32767);
}


float msys_frand(int *seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	//seed[0] *= 16807;

	const unsigned int a = (((unsigned int)seed[0]) >> 9) | 0x3f800000;
	const float res = (*((float*)&a)) - 1.0f;

	return(res);
}

float msys_randFloat(int *seed, float min, float max)
{
	return min + msys_frand(seed)*(max - min);
}

float msys_sfrand(int *seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	//seed[0] *= 16807;

	const unsigned int a = (((unsigned int)seed[0]) >> 9) | 0x40000000;
	const float res = (*((float*)&a)) - 3.0f;

	return(res);
}

// Load shader from disk into a null-terminated string
char* LoadShaderText(const char *fileName)
{
	char shaderPath[MAX_PATH];
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);

	sprintf(&shaderPath[0], "%s%s", path, fileName);
	//sprintf(&shaderPath[0], "%s", fileName);

	char *shaderText = NULL;
	int shaderLength = 0;
	FILE *fp;

	fp = fopen(shaderPath, "r");
	if (fp != NULL)
	{
		while (fgetc(fp) != EOF)
		{
			shaderLength++;
		}
		rewind(fp);
		shaderText = new char[shaderLength + 1];
		if (shaderText != NULL)
		{
			fread(shaderText, 1, shaderLength, fp);
		}
		else {

		}
		shaderText[shaderLength] = NULL;
		fclose(fp);
	}
	return shaderText;
}
#endif

bool zstreql(const char* str1, const char* str2)
{
	while (*str1 == *str2)
	{
		if (*str1 == '\0')
			return true;

		str1++;
		str2++;
	}

	return false;
}

ZRandom::ZRandom(int seed)
{
	this->seed = new int();
	*this->seed = seed;
}

int ZRandom::randomInt()
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	int ret = (seed[0] >> 16) & 32767;
	return ret;
}

int ZRandom::randomInt(int floor, int ceil)
{
	int ret = randomInt() % (ceil + 1 - floor);
	ret += floor;
	return ret;
}

float ZRandom::randomNormalizedFloat()
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	//seed[0] *= 16807;

	const unsigned int a = (((unsigned int)seed[0]) >> 9) | 0x3f800000;
	const float ret = (*((float*)&a)) - 1.0f;
	return ret;
}