#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <vector>

using std::vector;

namespace NCL
{
	namespace CSC8503
	{

		enum Layer
		{
			Ground = 0,
			Player = 1,
			Collectible = 2,
			Bouncy = 3,
			Ice = 4,
			Sticky = 5,
			Finish = 6,
			Door = 7
		};

		class GameObject
		{
		public:
			GameObject(string name = "");
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol)
			{
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const
			{
				return boundingVolume;
			}

			bool IsActive() const
			{
				return isActive;
			}

			Transform& GetTransform()
			{
				return transform;
			}

			RenderObject* GetRenderObject() const
			{
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const
			{
				return physicsObject;
			}

			void SetRenderObject(RenderObject* newObject)
			{
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject)
			{
				physicsObject = newObject;
			}

			const string& GetName() const
			{
				return name;
			}

			virtual void OnCollisionBegin(GameObject* otherObject)
			{
				//std::cout << "OnCollisionBegin event occured!\n";
			}

			virtual void OnCollisionEnd(GameObject* otherObject)
			{
				//std::cout << "OnCollisionEnd event occured!\n";
			}

			bool GetBroadphaseAABB(Vector3& outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID)
			{
				worldID = newID;
			}

			int		GetWorldID() const
			{
				return worldID;
			}


			Layer layer;

			virtual void Update(float dt){}

		protected:


			Transform			transform;

			CollisionVolume* boundingVolume;
			PhysicsObject* physicsObject;
			RenderObject* renderObject;




			bool	isActive;
			int		worldID;
			string	name;

			Vector3 broadphaseAABB;
		};
	}
}

