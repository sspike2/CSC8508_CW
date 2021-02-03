#include "PlayerClass.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"



using namespace NCL;
using namespace CSC8503;



PlayerClass::PlayerClass(string name)
{
	this->name = name;
	worldID = -1;
	isActive = true;
	boundingVolume = nullptr;
	physicsObject = nullptr;
	renderObject = nullptr;
}

void PlayerClass::Update(float dt)
{


	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W))
	{
		physicsObject->AddForce(transform.GetOrientation() * Vector3(0, 0, 1) * speed);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S))
	{
		physicsObject->AddForce(transform.GetOrientation() * Vector3(0, 0, 1) * -speed);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A))
	{
		physicsObject->AddForce(transform.GetOrientation() * Vector3(1, 0, 0) * speed);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D))
	{
		physicsObject->AddForce(transform.GetOrientation() * Vector3(1, 0, 0) * -speed);
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::Q))
	{
		//physicsObject->AddForce(Vector3(-speed, 0, 0));
		physicsObject->AddTorque(Vector3(0, rotationSpeed, 0));
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::E))
	{
		//physicsObject->AddForce(Vector3(-speed, 0, 0));
		physicsObject->AddTorque(Vector3(0, -rotationSpeed, 0));
	}



	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && isGrounded)
	{
		Jump();
	}

	velocity = physicsObject->GetLinearVelocity();


	float clamp = 25;

	Vector3  vel = Maths::Clamp(velocity, Vector3(-clamp, -clamp, -clamp), Vector3(clamp, clamp, clamp));
	physicsObject->SetLinearVelocity(vel);




}



void PlayerClass::OnCollisionBegin(GameObject* otherObject)
{


	if (otherObject->layer == Layer::Ground)
	{
		isGrounded = true;
	}
	if (otherObject->layer == Layer::Collectible)
	{
		//coins += 12;
		score += 50;
		otherObject->GetTransform().SetPosition(Vector3(500, 500, -500));
	}
	if (otherObject->layer == Layer::Ice)
	{

		GetPhysicsObject()->SetFriction(.0f);
	}
	if (otherObject->layer == Layer::Sticky)
	{

	}
	if (otherObject->layer == Layer::Bouncy)
	{

	}
	if (otherObject->layer == Layer::Finish)
	{
		hasWon = true;
	}


}


void PlayerClass::OnCollisionEnd(GameObject* otherObject)
{
	if (otherObject->layer == Layer::Ground)
	{
		//isGrounded = true;
	}
	if (otherObject->layer == Layer::Collectible)
	{
		//coins += 12;
	}
	if (otherObject->layer == Layer::Ice)
	{
		GetPhysicsObject()->SetFriction(.5f);
	}
	if (otherObject->layer == Layer::Sticky)
	{

	}
	if (otherObject->layer == Layer::Bouncy)
	{

	}




}

void PlayerClass::Jump()
{
	isGrounded = false;
	physicsObject->AddForce(Vector3(0, 500, 0));
}
