#version 330
layout(triangles) in;

// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;

uniform float normal_length;
uniform mat4 mvm;
uniform mat4 pm;

in Vertex
{
  vec3 normal;
  vec4 color;
} vertex[];

out vec4 vertex_color;

void main()
{
  int i;
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal;
	
	mat4 mvpm = pm * mvm;
	mat3 nm = mat3(mvm);
	vec4 p0 = mvpm * vec4(P, 1.0);

    gl_Position = p0;
    vertex_color = vertex[i].color;
    EmitVertex();
    
	vec4 p1 = p0;
	p1.xyz += N * mat3(transpose(inverse(mvm))) * 5.0;
    gl_Position = p1;
	vertex_color = vertex[i].color;
    EmitVertex();
    
    EndPrimitive();
  }
}