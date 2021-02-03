#include "HingeJointConstraint.h"

using namespace NCL;
using namespace CSC8503;
using namespace Maths;

void HingeJointConstraint::UpdateConstraint(float dt)
{
	//direction
	Vector3 direction =
		objectA->GetTransform().GetPosition() -
		objectB->GetTransform().GetPosition();

	direction.Normalise();
	direction.y = 0;

	float aRot = -Maths::RadiansToDegrees(atan2f(direction.z, direction.x));
	float bRot = Maths::RadiansToDegrees(atan2f(-direction.z, direction.x));



	objectA->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, aRot, 0));
	objectB->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, bRot, 0));

}