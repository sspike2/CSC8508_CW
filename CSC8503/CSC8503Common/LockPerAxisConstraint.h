#pragma once
#include "Constraint.h"
#include "GameObject.h"

namespace NCL
{

	namespace CSC8503
	{
		class LockPerAxisConstraint : public Constraint
		{
		public:
			LockPerAxisConstraint(GameObject* a, bool xLock, bool yLock, bool zLock)
			{
				objectA = a;
				x = xLock;
				y = yLock;
				z = zLock;
				lockedPos = objectA->GetTransform().GetPosition();
			}
			~LockPerAxisConstraint() {}

			void UpdateConstraint(float dt) override;
		private:
			GameObject* objectA;

			bool x, y, z;

			Vector3 lockedPos;
		};
	}
}
