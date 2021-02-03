#pragma once
#include "Constraint.h"
#include "GameObject.h"

namespace NCL
{
	namespace CSC8503
	{

		class OrientationLock : public Constraint
		{

		public:
			OrientationLock(GameObject* a, bool xLock, bool yLock, bool zLock)
			{
				objectA = a;
				x = xLock;
				y = yLock;
				z = zLock;
				lockedOri = objectA->GetTransform().GetOrientation().ToEuler();
			}
			~OrientationLock() { delete objectA; }

			void UpdateConstraint(float dt) override;
		private:
			GameObject* objectA;

			bool x, y, z;

			Vector3 lockedOri;
		};
	};

}
