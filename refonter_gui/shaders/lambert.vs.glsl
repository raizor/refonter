#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 mvm;
uniform mat4 pm;
//uniform mat3 nm;
uniform vec3 pos_offset;
uniform vec3 cam_pos;
uniform vec3 cam_rot;

out vec3 viewspacePosition3, viewspaceNormal;
out vec4 viewspacePosition4;

mat3 rotz(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		ca,sa,0.0,
		-sa,ca,0.0,
		0.0,0.0,1.0
	);
}

mat3 rotx(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		1.0,0.0,0.0,
		0.0,ca,-sa,
		0.0,sa,ca
	);
}

mat3 roty(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		ca,0.0,sa,
		0.0,1.0,0.0,
		-sa,0.0,ca
	);
}


void main()
{
	vec4 pos = position;
	pos.xyz += pos_offset;	

	pos.xyz *= rotz(cam_rot.z);
	pos.xyz *= rotx(cam_rot.x);	
	pos.xyz *= roty(cam_rot.y);

	pos = mvm * pos;
	
	pos.z-=cam_pos.z;
	
	
	//pos.xyz += cam_pos.xyz;
	
	viewspacePosition4 = pos;
	viewspacePosition3 = viewspacePosition4.xyz;
	viewspaceNormal = normalize(vec3(normal) * mat3(transpose(inverse(mvm))));

	gl_Position = pm * pos;
}