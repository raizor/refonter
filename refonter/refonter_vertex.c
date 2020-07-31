#include "refonter_vertex.h"
#include <math.h>

refonter_vec3 refonter_zero_vertex()
{
	refonter_vec3 res;
	res.x = 0;
	res.y = 0;
	res.z = 0;
	return res;
}

refonter_vec3 refonter_vertex_minus(const refonter_vec3 p1, const refonter_vec3 p2)
{
	refonter_vec3 res;
	res.x = p1.x - p2.x;
	res.y = p1.y - p2.y;
	res.z = p1.z - p2.z;
	return res;
}

refonter_vec3 refonter_vertex_plus(const refonter_vec3 p1, const refonter_vec3 p2)
{
	refonter_vec3 res;
	res.x = p1.x + p2.x;
	res.y = p1.y + p2.y;
	res.z = p1.z + p2.z;
	return res;
}

refonter_vec3 refonter_vertex_mid(const refonter_vec3 p1, const refonter_vec3 p2)
{
	refonter_vec3 res = refonter_vertex_plus(p1, p2);
	res.x /= 2;
	res.y /= 2;
	res.z /= 2;
	return res;
}

refonter_vec3 refonter_vertex_cross(const refonter_vec3 p1, const refonter_vec3 p2)
{
	refonter_vec3 vec;
	vec.x = p1.y * p2.z - p1.z * p2.y;
	vec.y = p1.z * p2.x - p1.x * p2.z;
	vec.z = p1.x * p2.y - p1.y * p2.x;
	return vec;
}

refonter_vec3 refonter_vertex_mul(const refonter_vec3 p1, const refonter_vec3 p2)
{
	refonter_vec3 p3;
	p3.x = p1.x * p2.x;
	p3.y = p1.y * p2.y;
	p3.z = p1.z * p2.z;
	return p3;
}

refonter_vec3 refonter_vertex_mul_val(const refonter_vec3 p1, double amount)
{
	refonter_vec3 p3;
	p3.x = p1.x * amount;
	p3.y = p1.y * amount;
	p3.z = p1.z * amount;
	return p3;
}


refonter_vec3 refonter_vertex_normalize(const refonter_vec3 p1) {
	double length = sqrt(p1.x*p1.x + p1.y * p1.y + p1.z * p1.z);
	double lengthInv = 1.f / length;
	return refonter_vertex_mul_val(p1, lengthInv);
}