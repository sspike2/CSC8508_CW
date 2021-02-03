#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "StateGameObject.h"
#include "PlayerClass.h"
#include "AIClass.h"
#include "WorldGeneration.h"

namespace NCL
{
	namespace CSC8503
	{
		class WorldGeneration;

		class CourseGame
		{
		public:
			CourseGame();
			~CourseGame();

			void InitWorld();

			void InitAI();

			virtual void UpdateGame(float dt);
			void UpdatePause(float dt);

			GameWorld* world;
			bool inSelectionMode;

			void SetPlayer(PlayerClass* player) { this->player = player; }
			PlayerClass* GetPlayer() { return player; }

			GameTechRenderer* renderer;


			int GetWinner()const { return winner; }
			int GetPlayerScore() ;
			int GetAIScore();


			void SetWinner(int id) { winner = id; }

		protected:


			WorldGeneration* worldGen;

			void InitCamera();
			void UpdateKeys();


			int winner = 0;

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();


			Vector4 selectedObjColor;

			PhysicsSystem* physics;

			//WorldGeneration* worldGen;

			PlayerClass* player = nullptr;
			AIClass* aIClass = nullptr;

			bool useGravity;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;



			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset = Vector3(0, 14, -20);
			void LockCameraToObject(GameObject* o)
			{
				lockedObject = o;
			}


		};
	}
}

