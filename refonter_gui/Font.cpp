#include "Font.h"

refonter_font* Font::font = 0;
refonter_tesselation_object* Font::tesselated_chars = 0;
GLuint* Font::bufferids = 0;