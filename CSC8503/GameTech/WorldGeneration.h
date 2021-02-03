#pragma once
#include "CourseGame.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "UpDownGateStateGameObj.h"

//using namespace NCL;
//using namespace CSC8503;
namespace NCL
{
	namespace CSC8503
	{
		class CourseGame;

		class WorldGeneration
		{
		public:
			WorldGeneration(GameWorld* world, CourseGame* courseGame)
			{
				this->world = world;
				this->courseGame = courseGame;
				InitialiseAssets();
			}
			~WorldGeneration();

			void	Update(float dt);

			AIClass* AddAI();

		protected:

			CourseGame* courseGame;
			GameWorld* world;

			//Coursework Meshes

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;

			OGLShader* basicShader = nullptr;



			OGLTexture* basicTex = nullptr;
			OGLTexture* playerTex = nullptr;
			OGLTexture* blankTex = nullptr;

			const Vector3 borderPosition[4] = { Vector3(-180,-37.4,400),
												Vector3(180,-37.4,400),
												Vector3(0,-37.4,-120),
												Vector3(0,-37.4,900) };

			const Vector3 borderScale[4] = { Vector3(50,85,500),
											 Vector3(50,85,500),
											 Vector3(230,85,20),
											 Vector3(230,85,20) };


			const Vector3 obstaclePosition[10] = { Vector3(26   ,5  ,60),
													 Vector3(-26  ,5  ,60),
													 Vector3(60   ,5  ,120),
													 Vector3(0    ,5  ,120),
													 Vector3(-60  ,5  ,120),
													 Vector3(26   ,5  ,180),
													 Vector3(-26  ,5  ,180),
													 Vector3(60   ,5  ,240),
													 Vector3(0    ,5  ,240),
													 Vector3(-60  ,5  ,240), };


			const Vector3 StateMachineWallPosition[4] = { Vector3(-44   ,5.5f  ,550),
														  Vector3(44   ,5.5f  ,550),
														  Vector3(-44   ,5.5f  ,640),
														  Vector3(44   ,5.5f  ,640) };


			const Vector3 StateMachineDoorPosition[6] = { Vector3(-90   ,9   ,550),
														  Vector3(0     ,21  ,550),
														  Vector3(90    ,9   ,550),
														  Vector3(-90   ,21  ,640),
														  Vector3(0     ,9   ,640),
														  Vector3(90    ,21  ,640) };

			const Vector3 hingePolePosition[4] = { Vector3(81  ,5,350),
												   Vector3(-81 ,5,350),
												   Vector3(128 ,5,350),
												   Vector3(-128,5,350) };

			const Vector3 hingeDoorPosition[4] = { Vector3(40  ,7,350),
												   Vector3(-40 ,7,350),
												   Vector3(104 ,7,350),
												   Vector3(-104,7,350) };

			const Vector3 hingeDoorScale[4] = { Vector3(39.5f ,5 ,1),
												Vector3(39.5f ,5 ,1),
												Vector3(21    ,5 ,1),
												Vector3(21    ,5 ,1) };



			const Vector3 bonusPositions[18] = { Vector3(0   ,5  ,60),
													 Vector3(-10   ,5  ,60),
													 Vector3(10   ,5  ,60),

													 Vector3(-16    ,5  ,120),
													 Vector3(-26    ,5  ,120),
													 Vector3(-36    ,5  ,120),

													 Vector3(16    ,5  ,120),
													 Vector3(26    ,5  ,120),
													 Vector3(36    ,5  ,120),


													 Vector3(0      ,5  ,180),
													 Vector3(-10    ,5  ,180),
													 Vector3(10     ,5  ,180),

													 Vector3(-16    ,5  ,240),
													 Vector3(-26    ,5  ,240),
													 Vector3(-36    ,5  ,240),

													 Vector3(16     ,5  ,240),
													 Vector3(26     ,5  ,240),
													 Vector3(36     ,5  ,240),


			};








			void InitialiseAssets();
			void InitWorld();

			// Gameobjs
			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& scale);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass,
				OGLTexture* tex, string name = "Cube", Vector4 color = Vector4(1, 1, 1, 1));

			GameObject* AddOBBToWorld(const Vector3& position, Vector3 dimensions, Vector3 eulerAngles, float inverseMass,
				OGLTexture* tex, string name = "OBB", Vector4 color = Vector4(1, 1, 1, 1));


			//TODO
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);
			AIClass* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			UpDownGateStateGameObj* AddStateObjectToWorld(const Vector3& position, float counter);
			std::vector <UpDownGateStateGameObj*> stateObjects;



			GameObject* AddPlayerToWorld();

			void BridgeConstraintTest();

			void AddHingeJoints(int index);
			GameObject* player;

		};
	}
}