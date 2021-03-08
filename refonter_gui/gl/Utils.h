#ifndef UTILS_H
#define UTILS_H

// debug helpers

#ifdef _DEBUG
char* LoadShaderText(const char *fileName);
#endif

// string stuff
bool zstreql(const char* str1, const char* str2);

class ZRandom
{
public:
	int *seed;
	ZRandom(int seed);
	int randomInt(int floor, int ceil);
	int randomInt();
	float randomNormalizedFloat();
};

int msys_rand(int *seed);
float msys_frand(int *seed);
float msys_randFloat(int *seed, float min, float max);
float msys_sfrand(int *seed);

/*
// assets
#ifdef ENABLE_ASSERTS
//#if defined(__i386__) && defined( WIN32 )
#if defined( WIN32 )
#ifdef WIN32
#pragma warning( disable: 4793 )
#pragma warning( disable : 4800 )
#endif
#if !defined( ZASSERT )
#define ZASSERT(exp) if( !static_cast<bool>(exp) ) { _asm{ int 3 } }
#endif
#ifdef WIN32
#pragma warning( default: 4793 )
#endif
#else
#if !defined( ZASSERT )
#define ZASSERT( exp ) // TODO
#endif
#endif
#else
#ifndef ZASSERT
#define ZASSERT( exp )
#endif
#endif
*/

#endif