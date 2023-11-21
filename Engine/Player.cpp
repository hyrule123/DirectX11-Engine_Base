#include "PCH_Engine.h"

#include "Player.h"


namespace ehw
{
	Player::Player()
	{
	}

	Player::~Player()
	{
	}

	void Player::Init()
	{
		GameObject::Init();
	}

	void Player::Update()
	{
		GameObject::Update();
		//head->update();
		//body->update();
	}

	void Player::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Player::Render()
	{
		GameObject::Render();
	}
}
