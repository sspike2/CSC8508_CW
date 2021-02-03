#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"
#include "../../Common/Maths.h"
#include "GameObject.h"

namespace NCL
{

	namespace CSC8503
	{
		class GameObject;
		class HingeJointConstraint : public Constraint
		{
		public:
			HingeJointConstraint(GameObject* a, GameObject* b)
			{
				objectA = a;
				objectB = b;
			}

			~HingeJointConstraint()
			{
				delete objectA;
				delete objectB;
			}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;
		};
	}
}
