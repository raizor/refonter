#include "quatCam.h"
#include "math/vector.h"
#include <math.h>

quatCam::quatCam(void)
{
}


quatCam::~quatCam(void)
{
}

quaternion quatCam::conjugate(quaternion quat)
{
	quat.x = -quat.x;
	quat.y = -quat.y;
	quat.z = -quat.z;
	return quat;
}

double quatCam::length(quaternion quat)
{
	return sqrt(quat.x * quat.x + quat.y * quat.y +
		quat.z * quat.z + quat.w * quat.w);
}

quaternion quatCam::normalize(quaternion quat)
{
	double L = length(quat);

	quat.x /= L;
	quat.y /= L;
	quat.z /= L;
	quat.w /= L;

	return quat;
}

quaternion quatCam::mult(quaternion A, quaternion B)
{
	quaternion C;

	C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
	C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
	C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
	C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
	return C;
}

void quatCam::SetPos(quaternion pos)
{
	eyePos = pos;
}

void quatCam::RotateCamera(double Angle, double x, double y, double z)
{
	quaternion temp, quat_view, result;

	temp.x = x * sin(Angle / 2);
	temp.y = y * sin(Angle / 2);
	temp.z = z * sin(Angle / 2);
	temp.w = cos(Angle / 2);

	quat_view.x = lookPos.x;
	quat_view.y = lookPos.y;
	quat_view.z = lookPos.z;
	quat_view.w = 0;

	result = mult(mult(temp, quat_view), conjugate(temp));

	lookPos.x = result.x;
	lookPos.y = result.y;
	lookPos.z = result.z;
}

void quatCam::Look()
{
	ZVector forward = ZVector((float)lookPos.x, (float)lookPos.y, (float)lookPos.z).Minus(ZVector((float)eyePos.x, (float)eyePos.y, (float)eyePos.z)).Normalize();
	ZVector pseudoup(0.f, 1.f, 0.f);
	ZVector right = forward.Cross(pseudoup);
	ZVector up = right.Cross(forward);

	gluLookAt((float)eyePos.x, (float)eyePos.y, (float)eyePos.z, (float)lookPos.x, (float)lookPos.y, (float)lookPos.z, up.x, up.y, up.z);
}