#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

uniform float normal_length;
uniform mat4 matrixModelView;
uniform mat4 matrixProjection;

void GenerateLine(int index)
{
	gl_Position = matrixProjection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = matrixProjection * (gl_in[index].gl_Position +vec4(gs_in[index].normal, 0.0) * normal_length);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0); // first vertex normal
	GenerateLine(1); // second vertex normal
	GenerateLine(2); // third vertex normal
}