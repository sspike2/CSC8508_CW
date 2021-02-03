#include "CollectibleObj.h"
using namespace NCL;
using namespace CSC8503;

void CollectibleObj::Update(float dt)
{
	Vector3 rot = transform.GetOrientation().ToEuler();

	rot.y -= 50.0f * dt;
	rot.y = rot.y >= 0.0f ? rot.y <= 360.0f ? rot.y : rot.y - 360.0f : rot.y + 360.0f;
	transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(rot.x, rot.y, rot.z));

}