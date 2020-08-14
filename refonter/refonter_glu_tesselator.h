#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// We can't know beforehand how many vertices we'll need per character, so we hope this is enough. 
// We could use a growable array, but that would incur some code size and complexity
#define kMaxTesselatorVertices 8192*2
// This seems like an initially sensible number
#define kMaxTesselatorContours 10

typedef enum kRenderStyle
{
	kRenderStyle_Smooth = 0,
	kRenderStyle_Hard = 1
}kRenderStyle;
	
typedef struct
{
	GLboolean font_is_3d; // todo: get rid of this and just check for depth > 0.0
	double depth;
	double depth_front;
	double depth_back;
	double size_front;
	double size_back;
	kRenderStyle render_style;

} refonter_tesselation_settings;

typedef struct 
{
	// front triangles
	// rear triangles
	// countour edge triangles

	// Vertex storage
	refonter_vertex contour_vertices[kMaxTesselatorContours][kMaxTesselatorVertices]; // outline vertices input to tesselator
	refonter_vertex triangle_vertices[kMaxTesselatorVertices]; // triangle vertices, output from tesselator

	unsigned int num_contour_vertices[kMaxTesselatorContours]; // vertex count for each contour
	unsigned int num_triangle_vertices; // number of front/back face triangles	
	unsigned int num_contours; // contour count

	// offsets into triangles buffer
	unsigned int contour_vertices_index_start;

	// general char-style settings for tesselation
	refonter_tesselation_settings settings;
	
	// GLU tesselator instance
	GLUtesselator* glu_tess_obj;
	
	// Other data
	double flatness_tolerance; // Criteria for ending bezier subdivision

}refonter_tesselation_object;

void refonter_glu_tesselate(refonter_font* p_font, refonter_tesselation_object* tess_objs, refonter_tesselation_settings settings, double flatness_tolerance);
void refonter_create_back_and_sides(refonter_font* cur_font, refonter_tesselation_object* tess_objs, refonter_tesselation_settings settings);
void add_side_normals(refonter_tesselation_object* cur_char);
#ifdef __cplusplus
}
#endif