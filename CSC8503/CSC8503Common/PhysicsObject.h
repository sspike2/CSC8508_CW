#pragma once
#include "../../Common/Vector3.h"
#include "../../Common/Matrix3.h"

using namespace NCL::Maths;

namespace NCL
{
	class CollisionVolume;

	namespace CSC8503
	{
		class Transform;

		enum class CollisionType
		{
			Projection,
			Penalty,
			Impulse
		};


		class PhysicsObject
		{
		public:
			PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume);
			~PhysicsObject();

			Vector3 GetLinearVelocity() const
			{
				return linearVelocity;
			}

			Vector3 GetAngularVelocity() const
			{
				return angularVelocity;
			}

			Vector3 GetTorque() const
			{
				return torque;
			}

			Vector3 GetForce() const
			{
				return force;
			}


			void SetAffectedByGravity(bool useGravity)
			{
				this->useGravity = useGravity;
			}

			bool AffectedByGravity() const
			{
				return useGravity;
			}

			void SetInverseMass(float invMass)
			{
				inverseMass = invMass;
			}

			float GetInverseMass() const
			{
				return inverseMass;
			}


			void SetElasticity(float elasticity)
			{
				this->elasticity = elasticity;
			}

			float GetElasticity() const
			{
				return elasticity;
			}



			void SetFriction(float fric)
			{
				friction = fric;
			}

			float GetFriction() const
			{
				return friction;
			}

			void SetCollisionType(CollisionType type)
			{
				collType = type;
			}


			CollisionType GetCollisionType() const
			{
				return collType;
			}

			void ApplyAngularImpulse(const Vector3& force);
			void ApplyLinearImpulse(const Vector3& force);

			void AddForce(const Vector3& force);

			void AddForceAtPosition(const Vector3& force, const Vector3& position);

			void AddTorque(const Vector3& torque);

			void IntegrateAccel(float dt);

			void ClearForces();

			void OnTriggerEnter(CollisionVolume* otherVolume);

			void SetLinearVelocity(const Vector3& v)
			{
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v)
			{
				angularVelocity = v;
			}

			void InitCubeInertia();
			void InitSphereInertia();
			void InitCapsuleIntertia(float radius, float height);


			void UpdateInertiaTensor();

			Matrix3 GetInertiaTensor() const
			{
				return inverseInteriaTensor;
			}

		protected:
			const CollisionVolume* volume;
			Transform* transform;


			bool useGravity;

			float inverseMass;
			float elasticity;
			float friction;

			


			//linear stuff
			Vector3 linearVelocity;
			Vector3 force;


			//angular stuff
			Vector3 angularVelocity;
			Vector3 torque;
			Vector3 inverseInertia;
			Matrix3 inverseInteriaTensor;

			CollisionType collType = CollisionType::Impulse;
		};
	}
}

