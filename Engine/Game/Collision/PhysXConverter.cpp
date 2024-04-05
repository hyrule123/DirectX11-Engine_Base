#include "MathUtil.h"

namespace ehw
{

	Quaternion PhysXConverter::MakeRotationByASF(float x, float y, float z, eRotationOrder order)
	{
		const Quaternion xAxis = Quaternion::CreateFromAxisAngle({ 1.f, 0.f, 0.f }, x);
		const Quaternion yAxis = Quaternion::CreateFromAxisAngle({ 0.f, 1.f, 0.f }, y);
		const Quaternion zAxis = Quaternion::CreateFromAxisAngle({ 0.f, 0.f, 1.f }, z);

		switch (order)
		{
		case eRotationOrder::XYZ:
			return xAxis * yAxis * zAxis;
		case eRotationOrder::XZY:
			return xAxis * zAxis * yAxis;
		case eRotationOrder::YXZ:
			return yAxis * xAxis * zAxis;
		case eRotationOrder::YZX:
			return yAxis * zAxis * xAxis;
		case eRotationOrder::ZXY:
			return zAxis * xAxis * yAxis;
		case eRotationOrder::ZYX:
			return zAxis * yAxis * xAxis;
		default:
			ASSERT(false && "WRONG INPUT");
		}
		return yAxis * xAxis * zAxis;
	}

}
