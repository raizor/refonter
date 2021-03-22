#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out Vertex
{
  vec3 normal;
  vec4 color;
} vertex;

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  vertex.normal = normal;
  vertex.color =  vec4(1.0, 1.0, 0.0, 1.0);
}