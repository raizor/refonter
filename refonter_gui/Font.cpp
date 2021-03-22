#include "Font.h"

refonter_font* Font::font = 0;
refonter_tesselation_object* Font::tesselated_chars = 0;
refonter_tesselation_settings* Font::settings = new refonter_tesselation_settings();
Shader* Font::shaderVisNormals = 0;
Shader* Font::shaderVisObject = 0;
Shader* Font::shaderLambert = 0;

GLuint* Font::bufferids = 0;

Font::Font(unsigned char* font_blob, refonter_tesselation_settings* settings, double subdivision_tolerance)
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
	bufferids = new GLuint[font->num_chars + 1];

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

	shaderVisObject = new Shader(
		"\\shaders\\vis_object.vs.glsl",
		"",
		"\\shaders\\vis_object.fs.glsl",
		"",
		"VisObject"
	);

	shaderLambert = new Shader(
		"\\shaders\\lambert.vs.glsl",
		"",
		"\\shaders\\lambert.fs.glsl",
		"",
		"Lambert"
	);	
}

void Font::DrawLetterInternal(uint32_t i, bool drawNormals, float offset[3], float camPos[3], float camRot[3])
{
	refonter_tesselation_object* ch = &Font::tesselated_chars[i];
	GLenum err = glGetError();

	if (drawNormals)
	{
		// NORMALS
		
		Font::shaderVisNormals->SetActive(drawNormals);
		err = glGetError();

		GLfloat model[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		err = glGetError();

		GLfloat proj[16];
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		err = glGetError();

		GLint uniformLoc = GetUniformLocation(Font::shaderVisNormals->shaderProg, "normal_length");
		glUniform1f(uniformLoc, 1.0f);
		err = glGetError();

		uniformLoc = GetUniformLocation(Font::shaderVisNormals->shaderProg, "mvm");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &model[0]);
		err = glGetError();

		uniformLoc = GetUniformLocation(Font::shaderVisNormals->shaderProg, "pm");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &proj[0]);
		err = glGetError();

	}
	else
	{
		// OBJECT
		
		Font::shaderLambert->SetActive(!drawNormals);
		err = glGetError();

		GLfloat model[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		err = glGetError();

		GLfloat proj[16];
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		err = glGetError();

		/*
		GLint uniformLoc = GetUniformLocation(shaderVisNormals->shaderProg, "normal_length");
		glUniform1f(uniformLoc, 1.0f);
		err = glGetError();
		 */
		
		GLint uniformLoc = GetUniformLocation(Font::shaderLambert->shaderProg, "mvm");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &model[0]);
		err = glGetError();
		
		uniformLoc = GetUniformLocation(Font::shaderLambert->shaderProg, "pm");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &proj[0]);
		err = glGetError();

		uniformLoc = GetUniformLocation(Font::shaderLambert->shaderProg, "pos_offset");
		glUniform3fv(uniformLoc, 1, &offset[0]);
		err = glGetError();

		uniformLoc = GetUniformLocation(Font::shaderLambert->shaderProg, "cam_pos");
		glUniform3fv(uniformLoc, 1, &camPos[0]);
		err = glGetError();
				
		uniformLoc = GetUniformLocation(Font::shaderLambert->shaderProg, "cam_rot");
		glUniform3fv(uniformLoc, 1, &camRot[0]);
		err = glGetError();
	}

	glBindBuffer(GL_ARRAY_BUFFER, Font::bufferids[i]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_DOUBLE, sizeof(refonter_vertex), (GLvoid*)0);
	glNormalPointer(GL_DOUBLE, sizeof(refonter_vertex), (GLvoid*)(3 * sizeof(double)));

	// front/back faces
	glDrawArrays(GL_TRIANGLES, 0, ch->contour_vertices_index_start);

	// sides
	//glDrawArrays(GL_TRIANGLES, ch->contour_vertices_index_start, ch->num_triangle_vertices - ch->contour_vertices_index_start - (0));

	if (drawNormals)
	{
		Font::shaderVisNormals->SetActive(false);
	}
	else
	{
		Font::shaderLambert->SetActive(false);
	}
}

float Font::GetWidth(const char* str)
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

void Font::DrawLetter(uint32_t i, float offset[3], float camPos[3], float camRot[3])
{
	DrawLetterInternal(i, false, offset, camPos, camRot);
	//DrawLetterInternal(i, true);

	// draw normals		
}