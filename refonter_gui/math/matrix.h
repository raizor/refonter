#pragma once

#include "../gl/gl8k.h"
#include "vector.h"

class ZMatrix
{
public:
	ZMatrix(void);
	~ZMatrix(void);

	static ZMatrix CreateIdentity(void);
	static ZMatrix CreateTranslate(float x, float y, float z);
	static ZMatrix CreateRotateX(float rad); 
	static ZMatrix CreateRotateY(float rad);
	static ZMatrix CreateRotateZ(float rad);
	static ZMatrix CreateScale(float x, float y, float z);
	static ZMatrix CreateRotationAxis(const ZVector& axis, float angle);
	static ZMatrix CreateFromNormalizedBases(const ZVector& x, const ZVector& y, const ZVector& z);

	ZMatrix Translate(float x, float y, float z) const;
	ZMatrix RotateX(float rad) const; 
	ZMatrix RotateY(float rad) const;
	ZMatrix RotateZ(float rad) const;
	ZMatrix Scale(float x, float y, float z) const;
	ZMatrix RotationAxis(const ZVector& axis, float angle) const;

	ZMatrix Transpose() const;
	ZMatrix Inverse() const;

	ZMatrix Mul(const ZMatrix& other) const;

	union
	{
		float v[16];
		float m[4][4];
	};

	ZMatrix operator*(const ZMatrix &m) const;

private:
};
