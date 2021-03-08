#pragma once

class ZMatrix;

class ZVector
{
public:
	ZVector(void);
	ZVector(float x, float y, float z, float w);
	ZVector(float x, float y, float z);
	~ZVector(void);

	float Length() const;

	ZVector Normalize(void) const;
	ZVector Minus(const ZVector& other) const;
	ZVector Plus(const ZVector& other) const;
	ZVector Mul(const float v) const;
	ZVector Negate(void) const;
	ZVector Cross(const ZVector& other) const;
	ZVector Transform(const ZMatrix& vec) const;

	float Dot3(const ZVector& other) const;



#ifndef SWIG // we don't access this directly from Lua, because SWIG doesn't support nested classes. Instead we use the getter/setters below.
	union{
		struct {float x; float y; float z; float w;};
		struct {float r; float g; float b; float a;};
		float v[4];
	};
#endif

#ifdef ENABLE_GUI // these 
	float GetX(void) { return x; };
	float GetY(void) { return y; };
	float GetZ(void) { return z; };
	float GetW(void) { return w; };

	void SetX(float x) { this->x = x; };
	void SetY(float y) { this->y = y; };
	void SetZ(float z) { this->z = z; };
	void SetW(float w) { this->w = w; };
#endif
};

//extern double sqrt(_In_ double _X);

class ZVector2
{
private:
	

public:
	double x, y, z;
	static double NumBounds(double value);

	ZVector2(const ZVector2 &v)
	{
		x = NumBounds(v.x); y = NumBounds(v.y); z = NumBounds(v.z);
	}

	ZVector2(double _x, double _y, double _z)
	{
		x = NumBounds(_x); y = NumBounds(_y); z = NumBounds(_z);
	}
	
	double Length() const;

	ZVector2 Normalize()
	{
		if (Length() != 0)
			return ZVector2(x / Length(), y / Length(), z / Length()); else
			return *this;
	}

	double operator[](unsigned int index) const
	{
		if (index == 0)
			return NumBounds(x); else
			if (index == 1)
				return NumBounds(y); else
				if (index == 2)
					return NumBounds(z); else
					return 0;
	}

	void operator=(const ZVector2 &v)
	{
		x = NumBounds(v.x); y = NumBounds(v.y); z = NumBounds(v.z);
	}

	ZVector2 operator+(const ZVector2 &v)
	{
		return ZVector2(x + v.x, y + v.y, z + v.z);
	}

	ZVector2 operator-(const ZVector2 &v)
	{
		return ZVector2(x - v.x, y - v.y, z - v.z);
	}

	double operator*(const ZVector2 &v)
	{
		return NumBounds((x * v.x) + (y * v.y) + (z * v.z));
	}

	ZVector2 operator*(double s)
	{
		return ZVector2(x * s, y * s, z * s);
	}

	ZVector2 DotProduct(const ZVector2 &v)
	{
		double k1 = (y * v.z) - (z * v.y);
		double k2 = (z * v.x) - (x * v.z);
		double k3 = (x * v.y) - (y * v.x);

		return ZVector2(NumBounds(k1), NumBounds(k2), NumBounds(k3));
	}

	ZVector2 Rotate(ZVector2 &axis, double angle);
};