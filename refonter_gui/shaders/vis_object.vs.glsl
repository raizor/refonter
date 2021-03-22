#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 pm;
uniform mat4 mvm;
uniform mat3 nm;

void main(void)
{	
	gl_Position = pm * mvm * vec4(position, 1.0);	
}