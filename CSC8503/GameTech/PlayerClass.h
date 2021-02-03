#pragma once
#include "../CSC8503Common/GameObject.h"
//#include "CourseGame.h"


namespace NCL
{
	namespace CSC8503
	{
		//class CourseGame; 
		class PlayerClass : public GameObject
		{
		public:

			PlayerClass(string name = "");

			int GetScore() const { return score; }
			void AddScore(int Ammount) { score += Ammount; }

			bool CheckWin() const { return hasWon; }

			virtual void Update(float dt);

		protected:
			int playerHealth = 100;
			int score = 0;
			int speed = 150;
			float rotationSpeed = 0.1f;
			bool isGrounded;
			Vector3 velocity;

			bool hasWon = false;
			//CourseGame* coursegame;

			virtual void OnCollisionBegin(GameObject* otherObject);
			virtual void OnCollisionEnd(GameObject* otherObject);

			void Jump();

		};
	}
}

