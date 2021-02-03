#include "CourseGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"

using namespace NCL;
using namespace CSC8503;
int a = 0;

CourseGame::CourseGame()
{
	std::cout << std::endl << std::endl << a << std::endl << std::endl << std::endl;
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physics = new PhysicsSystem(*world);
	Debug::SetRenderer(renderer);
	forceMagnitude = 10.0f;
	useGravity = true;
	inSelectionMode = false;
	physics->UseGravity(useGravity);

	physics->Clear();


}


void CourseGame::InitWorld()
{


	world->ClearAndErase();
	worldGen = new WorldGeneration(world, this);
	InitCamera();

	winner = 0;
}

void CourseGame::InitAI()
{
	aIClass = worldGen->AddAI();
}



CourseGame::~CourseGame()
{

	delete physics;
	delete renderer;
	delete world;
}

int CourseGame::GetPlayerScore()
{
	if (player != nullptr)
	{
		return player->GetScore();
	}

	return 0;

}

int CourseGame::GetAIScore()
{
	if (aIClass != nullptr)
	{
		return aIClass->GetScore();
	}

	return 0;

}



void CourseGame::UpdateGame(float dt)
{
	if (!inSelectionMode)
	{
		world->GetMainCamera()->UpdateCamera(dt);
	}

	UpdateKeys();
	if (player != nullptr)
	{
		//player->Update(dt);
		int score = player->GetScore();
		//int aiScore = ai

		Debug::Print("Player Score: " + std::to_string(score), Vector2(5, 5));
		

		if (player->CheckWin())
		{
			SetWinner(1);
		}

	}

	if (aIClass != nullptr)
	{
		//aIClass->Update(dt);
		int score = aIClass->GetScore();
		Debug::Print("Ai Score    : " + std::to_string(score), Vector2(5, 10));

		if (aIClass->CheckWin())
		{
			SetWinner(2);
		}
	}

	if (worldGen != nullptr)
	{
		worldGen->Update(dt);
	}

	//if (useGravity)
	//{
	//	Debug::Print("(G)ravity on", Vector2(5, 95));
	//}
	//else
	//{
	//	Debug::Print("(G)ravity off", Vector2(5, 95));
	//}

	SelectObject();
	//MoveSelectedObject();
	physics->Update(dt);

	if (lockedObject != nullptr)
	{
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedObject->GetTransform().GetOrientation() * lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	/*if (testStateObject)
	{
		testStateObject->Update(dt);
	}*/

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	//renderer->Render();
}


void CourseGame::UpdatePause(float dt)
{
	//renderer->Update(dt);
	Debug::Print("Press P To Resume", Vector2(20, 50), Vector4(1, 0, 0, 1));
	Debug::FlushRenderables(dt);
	//renderer->Render();
}

void CourseGame::UpdateKeys()
{
	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1))
	//{
	//	InitWorld(); //We can reset the simulation at any time with F1
	//	selectionObject = nullptr;
	//	lockedObject = nullptr;
	//}

	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2))
	//{
	//	InitCamera(); //F2 will reset the camera to a specific default place
	//}

	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G))
	//{
	//	useGravity = !useGravity; //Toggle gravity!
	//	physics->UseGravity(useGravity);
	//}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9))
	{
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10))
	{
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7))
	{
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8))
	{
		world->ShuffleObjects(false);
	}*/

	//if (lockedObject)
	//{
		//LockedObjectMovement();
	//}
	//else
	//{
		//DebugObjectMovement();
	//}
}

void CourseGame::LockedObjectMovement()
{
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT))
	{
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
	{
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP))
	{
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))
	{
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT))
	{
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
	}
}

void CourseGame::DebugObjectMovement()
{
	//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject)
	{
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT))
		{
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
		{
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7))
		{
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8))
		{
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
		{
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP))
		{
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))
		{
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5))
		{
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void CourseGame::InitCamera()
{
	world->GetMainCamera()->SetNearPlane(5);
	world->GetMainCamera()->SetFarPlane(50000.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(180);
	world->GetMainCamera()->SetPosition(Vector3(14, 23.5f, -12));
	//world->GetMainCamera()->
	lockedObject = player;
}

//void CourseGame::InitWorld()
//{
//	world->ClearAndErase();
//	physics->Clear();
//	physics->UseGravity(false);
//
//
//
//	//InitMixedGridWorld(5, 5, 3.5f, 3.5f);
//	//InitGameExamples();
//	InitDefaultFloor();
//	//InitPlayer();
//	//GameObject* obb = AddOBBToWorld(Vector3(3, 10, 3), Vector3(1, 1, 1), 1);
//	//BridgeConstraintTest();
//	//GameObject* capsule = AddCapsuleToWorld(Vector3(3, 10, 8), 2, 1);
//	//capsule->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 100, 0));
//	//testStateObject = AddStateObjectToWorld(Vector3(0, 10, 0));
//}





/*

A single function to add a large immoveable cube to the bottom of our world

*/








void CourseGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius)
{
	for (int x = 0; x < numCols; ++x)
	{
		for (int z = 0; z < numRows; ++z)
		{
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			//AddSphereToWorld(position, radius, 1.0f);
		}
	}
	//AddFloorToWorld(Vector3(0, -2, 0));
}

void CourseGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing)
{
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x)
	{
		for (int z = 0; z < numRows; ++z)
		{
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2)
			{
				//AddCubeToWorld(position, cubeDims);
			}
			else
			{
				//AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void CourseGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims)
{
	for (int x = 1; x < numCols + 1; ++x)
	{
		for (int z = 1; z < numRows + 1; ++z)
		{
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			//AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void CourseGame::InitDefaultFloor()
{
	//AddFloorToWorld(Vector3(0, -2, 0));
}

void CourseGame::InitGameExamples()
{
	//AddPlayerToWorld(Vector3(0, 5, 0));
	//AddEnemyToWorld(Vector3(5, 5, 0));
	//AddBonusToWorld(Vector3(10, 5, 0));
}


/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool CourseGame::SelectObject()
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T))
	{
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode)
		{

			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else
		{
			if (selectionObject)
			{
				selectionObject->GetRenderObject()->SetColour(selectedObjColor);
				selectionObject = nullptr;
				lockedObject = player;
			}
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode)
	{
		renderer->DrawString("Press T to change to Play mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT))
		{
			if (selectionObject)
			{

				selectionObject->GetRenderObject()->SetColour(selectedObjColor);
				selectionObject = nullptr;
				lockedObject = player;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true))
			{
				selectionObject = (GameObject*)closestCollision.node;
				RenderObject* render = selectionObject->GetRenderObject();
				selectedObjColor = render->GetColour();
				render->SetColour(Vector4(0, 1, 0, 1));

				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		renderer->DrawString("Press T to change to Debug mode!", Vector2(5, 85));
	}

	//if (lockedObject)
	//{
		//renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	//}

	if (selectionObject != nullptr)
	{
		int x = selectionObject->GetTransform().GetOrientation().ToEuler().x;
		int y = selectionObject->GetTransform().GetOrientation().ToEuler().y;
		int z = selectionObject->GetTransform().GetOrientation().ToEuler().z;

		string str = "Object orientation " + std::to_string(x) + " "
			+ std::to_string(y) + " "
			+ std::to_string(z) + " ";

		x = selectionObject->GetTransform().GetPosition().x;
		y = selectionObject->GetTransform().GetPosition().y;
		z = selectionObject->GetTransform().GetPosition().z;
		string str2 = "Object Position " + std::to_string(x) + " "
			+ std::to_string(y) + " "
			+ std::to_string(z) + " ";

		//renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
		renderer->DrawString(str, Vector2(5, 75));
		renderer->DrawString(str2, Vector2(5, 70));
		renderer->DrawString(selectionObject->GetName(), Vector2(5, 65));
	}

	/*if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L))
	{
		if (selectionObject)
		{
			if (lockedObject == selectionObject)
			{
				lockedObject = nullptr;
			}
			else
			{
				lockedObject = selectionObject;
			}
		}

	}*/

	return false;
}





/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void CourseGame::MoveSelectedObject()
{

	renderer->DrawString(" Click Force :" + std::to_string(forceMagnitude),
		Vector2(10, 20)); // Draw debug text at 10 ,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject)
	{
		return;
	}

	// Push the selected object !
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT))
	{
		Ray ray = CollisionDetection::BuildRayFromMouse(
			*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true))
		{
			if (closestCollision.node == selectionObject)
			{
				selectionObject->GetPhysicsObject()->
					AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);

				/*selectionObject->GetPhysicsObject()->AddForceAtPosition(
					 ray.GetDirection() * forceMagnitude,
					 closestCollision.collidedAt);*/
			}
		}
	}
}