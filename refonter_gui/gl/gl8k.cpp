#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "gl8k.h"

void *myglfunc[GLEXT_FUNCTION_COUNT];

void InitGlExtensions()
{
	// init GL extensions (defined in gl8k.h)
	for( int i=0; i<GLEXT_FUNCTION_COUNT; i++ )
	{
		myglfunc[i] = wglGetProcAddress( strs[i] );	
		if( !myglfunc[i] )			
			return;
	}
}