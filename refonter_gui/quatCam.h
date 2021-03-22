#pragma once
#include <Windows.h>
#include <gl/GLU.h>

struct quaternion
{
	double x, y, z, w;
};

class ZVector;

class quatCam
{
public:
	quaternion eyePos;
	quaternion lookPos;

	quatCam(void);
	~quatCam(void);
	void RotateCamera(double Angle, double x, double y, double z);
	void SetPos(quaternion pos);
	void Look();
	quaternion conjugate(quaternion quat);
	quaternion quatCam::mult(quaternion A, quaternion B);
	quaternion normalize(quaternion quat);
	double length(quaternion quat);
};

