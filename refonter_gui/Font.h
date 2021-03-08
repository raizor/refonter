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

	Font(unsigned char* font_blob, refonter_tesselation_settings* settings, double subdivision_tolerance)
	{
		if (Font::bufferids)
		{
			glDeleteBuffers(font->num_chars, Font::bufferids);
			delete(Font::bufferids);
		}

		if (Font::font)
			delete(Font::font);

		if (Font::tesselated_chars)
			delete(Font::tesselated_chars);
				
		// Init and tesselate
		this->settings = settings;
		font = refonter_init_blob(font_blob);

		tesselated_chars = new refonter_tesselation_object[font->num_chars]; // create one tesselation object per char
		refonter_glu_tesselate(font, tesselated_chars, *settings, subdivision_tolerance);

		// Generate buffer ids
		bufferids = new GLuint[font->num_chars+1];

		GLenum xx = glGetError();

		glGenBuffers(font->num_chars, bufferids);

		for (uint32_t i = 0; i < font->num_chars; i++)
		{
			// Generate array buffers, one per character
			glBindBuffer(GL_ARRAY_BUFFER, bufferids[i]);
			glBufferData(GL_ARRAY_BUFFER, tesselated_chars[i].num_triangle_vertices * sizeof(refonter_vertex), tesselated_chars[i].triangle_vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// init shaders
		shaderVisNormals = new Shader(
			"\\shaders\\vis_normals.vs.glsl",
			"\\shaders\\vis_normals.gs.glsl",
			"\\shaders\\vis_normals.fs.glsl",
			"",
			"VisNormals"
		);
	}

	void DrawLetterInternal(uint32_t i, bool drawNormals)
	{
		refonter_tesselation_object* ch = &tesselated_chars[i];

		GLenum err = glGetError();

		shaderVisNormals->SetActive(drawNormals);
		err = glGetError();

		GLfloat model[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		err = glGetError();

		GLfloat proj[16];
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		err = glGetError();

		GLint uniformLoc = glGetUniformLocation(shaderVisNormals->shaderProg, "normal_length");
		glUniform1f(uniformLoc, 1.0f);
		err = glGetError();

		GLint uniformLoc2 = glGetUniformLocation(shaderVisNormals->shaderProg, "matrixModelView");
		glUniformMatrix4fv(uniformLoc2, 1, GL_FALSE, &model[0]);
		err = glGetError();

		GLint uniformLoc3 = glGetUniformLocation(shaderVisNormals->shaderProg, "matrixProjection");
		glUniformMatrix4fv(uniformLoc3, 1, GL_FALSE, &proj[0]);
		err = glGetError();

		glBindBuffer(GL_ARRAY_BUFFER, bufferids[i]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_DOUBLE, sizeof(refonter_vertex), (GLvoid*)0);
		glNormalPointer(GL_DOUBLE, sizeof(refonter_vertex), (GLvoid*)(3 * sizeof(double)));
		// front/back faces
		//glDrawArrays(GL_TRIANGLES, 0, ch->contour_vertices_index_start);
		// sides
		glDrawArrays(GL_TRIANGLES, ch->contour_vertices_index_start, ch->num_triangle_vertices - ch->contour_vertices_index_start - (0));

		shaderVisNormals->SetActive(false);
	}
	
	void DrawLetter(uint32_t i)
	{		
		DrawLetterInternal(i, false);
		DrawLetterInternal(i, true);

		// draw normals
		
	}

	void Write(const char* str)
	{		
		float width = GetWidth(str);
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

