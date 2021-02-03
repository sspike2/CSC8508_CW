#pragma once
#include "..\CSC8503Common\GameObject.h"
namespace NCL
{
	namespace CSC8503
	{
		class CollectibleObj : public GameObject
		{
		public:
			CollectibleObj(string name) { this->name = name; }
			virtual void Update(float dt);

		};
	}
}