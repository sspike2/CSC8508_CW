#pragma once
#include "..\CSC8503Common\GameObject.h"
#include "..\CSC8503Common\NavigationGrid.h"
#include "..\CSC8503Common\StateMachine.h"
#include "..\CSC8503Common\State.h"
#include "..\CSC8503Common\StateTransition.h"

#include "PlayerClass.h"
#include "../CSC8503Common/GameWorld.h"
namespace NCL
{
	namespace CSC8503
	{
		enum AIType
		{
			Runner, chaser
		};

		class AIClass :public GameObject
		{
		public:

			AIClass(AIType aiType, GameObject* player, GameWorld* world, Vector3 endPos);



			void FollowPath();
			void FindPath(Vector3 endPos);


			virtual void Update(float dt);


			bool CheckWin() const { return hasWon; }

			int GetScore()  const { return score; }

		protected:

			GameObject* player;

			Vector3 endPos;
			float waitTime;
			float stuckTimer;

			StateMachine* RunnerMachine();

			AIType currentType;
			float speed;

			std::vector<Vector3> path;

			StateMachine* aiStateMachine;
			GameWorld* world;

			bool hasWon = false;

			int score;

			virtual void OnCollisionBegin(GameObject* otherObject);
			//virtual void OnCollisionEnd(GameObject* otherObject);

		};
	}
}
