#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "../CSC8503Common/StateMachine.h"


namespace NCL
{
	namespace CSC8503
	{
		class UpDownGateStateGameObj : public GameObject
		{

		public:

			UpDownGateStateGameObj(float counter);
			~UpDownGateStateGameObj();

			virtual	void Update(float dt);
		
		protected:

			void MoveUp(float dt);
			void MoveDown(float dt);

			StateMachine* stateMachine;

			float counter;

		
		};

	}
}