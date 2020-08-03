#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// We can't know beforehand how many vertices we'll need per character, so we hope this is enough. 
// We could use a growable array, but that would incur some code size and complexity
#define kMaxTesselatorVertices 8192*2
#define kMaxTesselatorContours 10

typedef struct
{
	GLboolean font_is_3d;
	float depth;
} refonter_tesselation_settings;

typedef struct 
{
	// Vertex storage
	refonter_vertex contour_vertices[kMaxTesselatorVertices]; // outline vertices input to tesselator
	refonter_vertex triangle_vertices[kMaxTesselatorVertices]; // triangle vertices, output from tesselator
	//refonter_vertex edge_vertices[kMaxTesselatorVertices];

	unsigned int num_contour_vertices;
	unsigned int num_triangle_vertices;
	//unsigned int num_edge_vertices;

	unsigned int num_contours;
	unsigned int contour_indices[kMaxTesselatorContours];

	refonter_tesselation_settings settings;
	
	// GLU tesselator instance
	GLUtesselator* glu_tess_obj;

	// note: enabling GLU edge callbacks forces all output prims to be standard triangles. this will come with a draw-time
	// penalty and we should consider performing a seperate tesselation run for the edges, so our front and back faces are
	// as optimized as GLU will allow...

	// Other data
	double flatness_tolerance; // Criteria for ending bezier subdivision

}refonter_tesselation_object;

void refonter_glu_tesselate(refonter_font* p_font, refonter_tesselation_object* tess_objs, refonter_tesselation_settings settings, double flatness_tolerance);
void refonter_create_back_and_sides(refonter_font* cur_font, refonter_tesselation_object* tess_objs, refonter_tesselation_settings settings);

#ifdef __cplusplus
}
#endif