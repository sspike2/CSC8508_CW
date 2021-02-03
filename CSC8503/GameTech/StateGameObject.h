#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "..\CSC8503Common\StateMachine.h"
namespace NCL
{
	namespace CSC8503
	{
		class StateGameObject : public GameObject
		{
		public:
			StateGameObject();
			~StateGameObject();

			virtual void Update(float dt);

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);

			StateMachine* stateMachine;
			float counter;
		};

	}
}
