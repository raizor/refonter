#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "matrix.h"
#include "gl/utils.h"
#include <math.h>

ZMatrix::ZMatrix(void)
{
	// Zero initialized
	for (int i=0; i<16; i++)
	{
		v[i] = 0.f;
	}
}

ZMatrix::~ZMatrix(void)
{
}

ZMatrix ZMatrix::CreateIdentity(void)
{
	ZMatrix mat;
	for (int y=0; y<4; y++)
	{
		for (int x=0; x<4; x++)
		{
			mat.m[y][x] = (x==y) ? 1.f : 0.f;
		}
	}
	return mat;
}

ZMatrix ZMatrix::CreateTranslate(float x, float y, float z) 
{
	ZMatrix mat = ZMatrix::CreateIdentity();
	mat.m[3][0] = x;
	mat.m[3][1] = y;
	mat.m[3][2] = z;
	return mat;
}

ZMatrix ZMatrix::CreateRotateX(float rad)
{
	ZMatrix mat = ZMatrix::CreateIdentity();
	mat.m[1][1] = cosf(rad);
	mat.m[1][2] = sinf(rad);
	mat.m[2][1] = -sinf(rad);
	mat.m[2][2] = cosf(rad);
	return mat;
}

ZMatrix ZMatrix::CreateRotateY(float rad)
{
	ZMatrix mat = ZMatrix::CreateIdentity();
	mat.m[2][2] = cosf(rad);
	mat.m[2][0] = sinf(rad);
	mat.m[0][2] = -sinf(rad);
	mat.m[0][0] = cosf(rad);
	return mat;
}

ZMatrix ZMatrix::CreateRotateZ(float rad)
{
	ZMatrix mat = ZMatrix::CreateIdentity();
	mat.m[0][0] = cosf(rad);
	mat.m[0][1] = sinf(rad);
	mat.m[1][0] = -sinf(rad);
	mat.m[1][1] = cosf(rad);
	return mat;
}

ZMatrix ZMatrix::CreateScale(float x, float y, float z)
{
	ZMatrix mat = ZMatrix::CreateIdentity();
	mat.m[0][0] = x;
	mat.m[1][1] = y;
	mat.m[2][2] = z;
	return mat;
}

ZMatrix ZMatrix::CreateRotationAxis(const ZVector& axis, float angle)
{
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm

	ZMatrix mat = ZMatrix::CreateIdentity();

	float fAngle = angle;
	float fCos = cos( fAngle );
	float fSin = sin( fAngle );
	float t = 1.0f - fCos;
	ZVector a = axis.Normalize();

	mat.m[0][0] = fCos + a.x*a.x*t;
	mat.m[1][1] = fCos + a.y*a.y*t;
	mat.m[2][2] = fCos + a.z*a.z*t;

	float tmp1 = a.x*a.y*t;
	float tmp2 = a.z*fSin;
	mat.m[1][0] = tmp1 + tmp2;
	mat.m[0][1] = tmp1 - tmp2;
	tmp1 = a.x*a.z*t;
	tmp2 = a.y*fSin;
	mat.m[2][0] = tmp1 - tmp2;
	mat.m[0][2] = tmp1 + tmp2;
	tmp1 = a.y*a.z*t;
	tmp2 = a.x*fSin;
	mat.m[2][1] = tmp1 + tmp2;
	mat.m[1][2] = tmp1 - tmp2;

	return mat;
}

ZMatrix ZMatrix::CreateFromNormalizedBases(const ZVector& x, const ZVector& y, const ZVector& z)
{
	ZMatrix mat = CreateIdentity();

	for (int i=0; i<3; i++)
	{
		mat.m[0][i] = x.v[i];
		mat.m[1][i] = y.v[i];
		mat.m[2][i] = z.v[i];
	}

	return mat;
}

ZMatrix ZMatrix::Translate(float x, float y, float z) const
{
	return Mul(ZMatrix::CreateTranslate(x, y, z));
}

ZMatrix ZMatrix::RotateX(float rad) const
{
	return Mul(ZMatrix::CreateRotateX(rad));
}

ZMatrix ZMatrix::RotateY(float rad) const
{
	return Mul(ZMatrix::CreateRotateY(rad));
}

ZMatrix ZMatrix::RotateZ(float rad) const
{
	return Mul(ZMatrix::CreateRotateZ(rad));
}

ZMatrix ZMatrix::Scale(float x, float y, float z) const
{
	return Mul(ZMatrix::CreateScale(x, y, z));
}

ZMatrix ZMatrix::RotationAxis(const ZVector& axis, float angle) const
{
	return Mul(ZMatrix::CreateRotationAxis(axis, angle));
}

ZMatrix ZMatrix::Mul(const ZMatrix& other) const
{
	ZMatrix mat;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			for (int k=0; k<4; k++)
			{
				mat.m[i][j] += other.m[i][k] * m[k][j];
			}
		}
	}
	return mat;
}

ZMatrix ZMatrix::Transpose() const
{
	ZMatrix mat;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			mat.m[i][j] = m[j][i];
		}
	}
	return mat;

}

ZMatrix ZMatrix::Inverse() const
{
	// This assumes an ortogonal matrix. This could be simplified for the ortonormal case, but we scale in some cases.
	ZMatrix inverseRotateScale = ZMatrix::CreateIdentity();	

	// Compute squared lengths of basis vectors
	float lengthsSquared[3] = {0.f, 0.f, 0.f};

	// Transpose
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			inverseRotateScale.m[i][j] = m[j][i];
			lengthsSquared[j] += m[i][j]*m[i][j];
		}
	}

	// Inverse scale
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			inverseRotateScale.m[i][j] /= lengthsSquared[i];
		}
	}

	// Create inverse translation
	ZMatrix inverseTranslation = ZMatrix::CreateIdentity();	
	for (int i=0; i<3; i++)
	{
		inverseTranslation.m[3][i] = -m[3][i];
	}

	return inverseRotateScale.Mul(inverseTranslation);
}

ZMatrix ZMatrix::operator*(const ZMatrix &m) const
{
	return Mul(m);
}
