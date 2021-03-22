#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out Vertex
{
  vec3 normal;
  vec4 color;
} vertex;

uniform mat4 matrixModelView;
uniform mat4 matrixProjection;

void main()
{
  gl_Position = matrixModelView * vec4(position, 1.0);
  mat3 normalMatrix = mat3(transpose(inverse(matrixModelView)));
  vertex.normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));
  vertex.color =  vec4(1.0, 1.0, 0.0, 1.0);
}