#pragma once

#if _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#include <string>
#endif
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include "refonter.h"
#include "refonter.h"
#include "refonter_glu_tesselator.h"
#include "gl8k.h"


class WindowsFontList
{
public:
	std::vector<std::wstring> fontNames;
	std::vector<std::wstring> fontPaths;
	WindowsFontList();
};

// Convenience font class
// This class uses OpenGL 3.3 array buffers to draw characters
class Font
{
public:
	refonter_font* font;
	refonter_tesselation_object* tesselated_chars;
	GLuint* bufferids;

	Font(unsigned char* font_blob, double subdivision_tolerance)
	{
		// Init and tesselate
		font = refonter_init_blob(font_blob);

		tesselated_chars = new refonter_tesselation_object[font->num_chars]; // create one tesselation object per char
		refonter_glu_tesselate(font, tesselated_chars, subdivision_tolerance);

		// Generate buffer ids
		bufferids = new GLuint[font->num_chars];
		GLenum xx = glGetError();
		glGenBuffers(font->num_chars, bufferids);

		for (uint32_t i = 0; i < font->num_chars; i++)
		{
			// Generate array buffers, one per character
			glBindBuffer(GL_ARRAY_BUFFER, bufferids[i]);
			glBufferData(GL_ARRAY_BUFFER, tesselated_chars[i].num_triangle_vertices * sizeof(refonter_vertex), tesselated_chars[i].triangle_vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	}

	void DrawLetter(uint32_t i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferids[i]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(refonter_vertex), (GLvoid*)(3 * sizeof(double)));

		glDrawArrays(GL_TRIANGLES, 0, tesselated_chars[i].num_triangle_vertices);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Write(const char* str)
	{
		while (*str)
		{
			char c = *str;

			for (uint32_t i = 0; i < font->num_chars; i++)
			{
				// Find letter
				if (font->chars[i].id == c)
				{
					// Draw letter
					DrawLetter(i);

					// Advance right
					glTranslatef(float(font->chars[i].width) / float(kRefonterSubdivision), 0.f, 0.f);
				}
			}

			str++;
		}
	}

	float GetWidth(const char* str)
	{
		float width = 0.f;

		while (*str)
		{
			char c = *str;

			// Find letter
			for (uint32_t i = 0; i < font->num_chars; i++)
			{
				if (font->chars[i].id == c)
				{
					width += float(font->chars[i].width) / 64.f;
				}
			}

			str++;
		}

		return width;
	}
};

