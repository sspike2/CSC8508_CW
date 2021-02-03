#include "AIClass.h"
#include "../../Common/Maths.h"

using namespace NCL;
using namespace CSC8503;
using namespace Maths;

AIClass::AIClass(AIType aiType, GameObject* player, GameWorld* world, Vector3 endPos)
{
	this->player = player;
	this->endPos = endPos;
	this->world = world;
	speed = 300;

	waitTime = 5;

	aiStateMachine = RunnerMachine();

}

StateMachine* AIClass::RunnerMachine()
{
	StateMachine* mach = new StateMachine();

	State* findPath = new State([&](float dt)-> void
		{
			this->FindPath(endPos);
		}
	);
	findPath->stateName = "findPath";

	State* followPath = new State([&](float dt)->void
		{

			this->FollowPath();
		}
	);

	followPath->stateName = "followPath";

	State* wait = new State([&](float dt)->void
		{
			stuckTimer += dt;
		});

	wait->stateName = "wait";

	State* teleportToPlayer = new State([&](float dt)->void
		{
			stuckTimer = 0;
			Vector3 playerPos = player->GetTransform().GetPosition();
			Vector3 playerOrient = player->GetTransform().GetOrientation() * Vector3(0, 0, -1);
			GetTransform().SetPosition(playerPos - playerOrient * 2);
		});
	teleportToPlayer->stateName = "tele";

	mach->AddState(findPath);
	mach->AddState(followPath);
	mach->AddState(wait);
	mach->AddState(teleportToPlayer);


	mach->AddTransition(new StateTransition(findPath, followPath, [&](void)->bool
		{
			return path.size() > 0;
		}));

	mach->AddTransition(new StateTransition(followPath, wait, [&](void)->bool
		{
			Ray r = Ray(transform.GetPosition() + Vector3(0, 2, 6), Vector3(0, 0, 1));
			RayCollision coll;
			int hitLayer;
			world->Raycast(r, coll, hitLayer, true);

			if (coll.rayDistance < 2)
			{
				if (hitLayer != 1 && hitLayer != 7) // player or door
				{
					return true;

				}
			}
			/*else
			{*/
			return false;
			//}
			//return GetPhysicsObject()->r
			//return GetPhysicsObject()->GetLinearVelocity().Length() < 1;
		}));

	mach->AddTransition(new StateTransition(wait, teleportToPlayer, [&](void)->bool
		{

			return stuckTimer >= waitTime;
		}));

	mach->AddTransition(new StateTransition(wait, followPath, [&](void)->bool
		{

			float remaningWaitTime = waitTime - stuckTimer;

			if (remaningWaitTime < 0)
			{
				Ray r = Ray(transform.GetPosition() + Vector3(0, 0, 6), Vector3(0, 0, 1));
				RayCollision coll;
				int hitLayer;

				world->Raycast(r, coll, hitLayer, true);
				if (coll.rayDistance > 1)
				{
					if (hitLayer != 1 && hitLayer != 7) // player or door
					{
						return true;
					}
				}
			}

			return false;

		}));

	mach->AddTransition(new StateTransition(teleportToPlayer, findPath, [&](void)->bool
		{
			return true;
		}));


	return mach;
}



void AIClass::FollowPath()
{
	if (path.size() < 2)
		return;

	Quaternion agentOrient = this->GetTransform().GetOrientation();
	Vector3 agentPos = this->GetTransform().GetPosition();

	Vector3 dirToNode = (path[0] - agentPos);
	dirToNode.y = 0;
	float length = dirToNode.Length();

	if (length < 2)
	{
		Vector3 dirToNextNode = (path[1] - path[0]).Normalised();
		dirToNextNode.y = 0.0f;

		float cos = Vector3::Dot(Vector3(0, 0, -1), dirToNextNode);
		float angle = std::acos(cos);
		angle = RadiansToDegrees(angle) * -(dirToNextNode.x + dirToNextNode.z);
		Quaternion lookAt = Quaternion::EulerAnglesToQuaternion(0, angle, 0);

		this->GetTransform().SetOrientation(Quaternion::Lerp(agentOrient, lookAt, 0.05));
		this->GetPhysicsObject()->AddForce(dirToNextNode * speed);
		path.erase(path.begin());

	}
	else
	{
		dirToNode.Normalise();
		float cos = Vector3::Dot(Vector3(0, 0, -1), dirToNode);
		float angle = std::acos(cos);
		angle = Maths::RadiansToDegrees(angle) * -(dirToNode.x + dirToNode.z);
		Quaternion lookAt = Quaternion::EulerAnglesToQuaternion(0, angle, 0);

		this->GetTransform().SetOrientation(Quaternion::Lerp(agentOrient, lookAt, 0.05));
		this->GetPhysicsObject()->AddForce(dirToNode * speed);
	}

	Vector3 velocity = this->GetPhysicsObject()->GetLinearVelocity();
	Vector3 vel = Maths::Clamp(velocity, Vector3(-5, -5, -5), Vector3(5, 5, 5));
	physicsObject->SetLinearVelocity(vel);
}


void AIClass::FindPath(Vector3 endPos)
{

	NavigationGrid grid("LevelLayout.txt");

	NavigationPath outPath;
	float multiplier = 1.5f;

	Vector3 startPos = Vector3(100, 0, 10);
	endPos = Vector3(100, 0, 530);
	startPos *= multiplier;
	endPos *= multiplier;

	Vector3 offset = Vector3(-150, 1, 0);


	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;

	while (outPath.PopWaypoint(pos))
	{
		pos += offset;
		path.push_back(pos);
	}


}


void AIClass::Update(float dt)
{
	//std::cout << aiStateMachine->GetActiveState()->stateName;
	//aiStateMachine->
	aiStateMachine->Update(dt);

}

void AIClass::OnCollisionBegin(GameObject* otherObject)
{

	if (otherObject->layer == Layer::Finish)
	{
		hasWon = true;
	}
	if (otherObject->layer == Layer::Collectible)
	{
		//coins += 12;
		score += 50;
		otherObject->GetTransform().SetPosition(Vector3(500, 500, -500));
	}
}