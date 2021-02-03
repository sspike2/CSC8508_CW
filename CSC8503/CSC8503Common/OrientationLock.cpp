#include "OrientationLock.h"
using namespace NCL;
using namespace CSC8503;

void OrientationLock::UpdateConstraint(float dt)
{
	Vector3 orientation = objectA->GetTransform().GetOrientation().ToEuler();

	if (x)
	{
		orientation.x = lockedOri.x;
	}
	if (y)
	{
		orientation.y = lockedOri.y;
	}
	if (z)
	{
		orientation.z = lockedOri.z;
	}
	objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(orientation));
	//objectA->GetTransform() (pos);
	//object

}