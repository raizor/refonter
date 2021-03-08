#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "matrix.h"
#include "vector.h"
#include "math.h"

ZVector::ZVector(void)
	: x(0.f)
	, y(0.f)
	, z(0.f)
	, w(0.f)
{
}

ZVector::ZVector(float x, float y, float z, float w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{
}

ZVector::ZVector(float x, float y, float z)
	: x(x)
	, y(y)
	, z(z)
	, w(0.f)
{
}

ZVector::~ZVector(void)
{
}

float ZVector::Length() const
{
	//float d = sqrtf(x*x + y*y + z*z + w*w);
	//return d < 0.000001f ? 1.0f : d;
	return sqrtf(x*x + y*y + z*z + w*w);
}

ZVector ZVector::Normalize(void) const
{
	//D3DXVECTOR4 v = D3DXVECTOR4(x, y, z, w);
	//D3DXVECTOR4 nv;
	//D3DXVec4Normalize(&nv, &v);
	//return ZVector(nv.x, nv.y, nv.z, nv.w);

	float lengthInv = 1.f / Length();
	ZVector vec = *this;
	return vec.Mul(lengthInv);
}

ZVector ZVector::Minus(const ZVector& other) const
{
	ZVector otherNegated = other.Negate();
	return Plus(otherNegated);
}

ZVector ZVector::Plus(const ZVector& other) const
{
	ZVector vec = *this;
	for (int i=0; i<4; i++)
	{
		vec.v[i] += other.v[i];
	}
	return vec;
}

ZVector ZVector::Mul(const float factor) const
{
	ZVector vec = *this;
	for (int i=0; i<4; i++)
	{
		vec.v[i] *= factor;
	}
	return vec;
}

ZVector ZVector::Negate(void) const
{
	return Mul(-1.f);
}

ZVector ZVector::Cross(const ZVector& other) const
{
	ZVector vec;
	vec.x = y*other.z - z*other.y;
	vec.y = z*other.x - x*other.z;
	vec.z = x*other.y - y*other.x;
	return vec;
}

ZVector ZVector::Transform(const ZMatrix& mat) const
{
	ZVector vec;

	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			vec.v[j] += mat.m[i][j] * v[i];
		}
	}

	return vec;
}

float ZVector::Dot3(const ZVector& other) const
{
	return x*other.x + y*other.y + z*other.z;
}

// *********************

double ZVector2::Length() const
{
	//float d = sqrtf(x*x + y*y + z*z + w*w);
	//return d < 0.000001f ? 1.0f : d;
	return sqrt(x*x + y*y + z*z);
}

ZVector2 ZVector2::Rotate(ZVector2 &axis, double angle)
{
	ZVector2 v = *this;

	return ((v - axis * (axis * v)) * cos(angle)) + (axis.DotProduct(v) * sin(angle)) + (axis * (axis * v));
}

double ZVector2::NumBounds(double value)
{
	if (fabs(value) < (1 / 1000000.0f))
		return 0; else
		return value;
}