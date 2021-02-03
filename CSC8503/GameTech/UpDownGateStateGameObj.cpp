#include "UpDownGateStateGameObj.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

UpDownGateStateGameObj::UpDownGateStateGameObj(float counter)
{
	this->counter = counter;

	stateMachine = new StateMachine();


	State* moveUp = new State([&](float dt)-> void
		{
			this->MoveUp(dt);
		}
	);
	State* moveDown = new State([&](float dt)-> void
		{
			this->MoveDown(dt);
		}
	);

	stateMachine->AddState(moveUp);
	stateMachine->AddState(moveDown);

	stateMachine->AddTransition(new StateTransition(moveUp, moveDown,
		[&]() -> bool
		{
			GetPhysicsObject()->SetLinearVelocity(Vector3());
			return this->counter > 10.0f;
		}
	));

	stateMachine->AddTransition(new StateTransition(moveDown, moveUp,
		[&]() -> bool
		{
			GetPhysicsObject()->SetLinearVelocity(Vector3());
			return this->counter < 0.0f;
		}
	));

}

UpDownGateStateGameObj::~UpDownGateStateGameObj()
{
	delete stateMachine;
}

void UpDownGateStateGameObj::Update(float dt)
{
	stateMachine->Update(dt);
}

void UpDownGateStateGameObj::MoveUp(float dt)
{

	GetPhysicsObject()->AddForce({ 0, -100, 0 });
	counter += dt;
}


void UpDownGateStateGameObj::MoveDown(float dt)
{
	GetPhysicsObject()->AddForce({ 0,100, 0 });
	counter -= dt;
}







