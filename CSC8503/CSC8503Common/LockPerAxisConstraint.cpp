#include "LockPerAxisConstraint.h"

using namespace NCL;
using namespace CSC8503;

void LockPerAxisConstraint::UpdateConstraint(float dt)
{
	Vector3 pos = objectA->GetTransform().GetPosition();
	if (x)
	{
		pos.x = lockedPos.x;
	}
	if (y)
	{
		pos.y = lockedPos.y;
	}
	if (z)
	{
		pos.z = lockedPos.z;
	}

	objectA->GetTransform().SetPosition(pos);

}