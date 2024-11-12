#include "Player.h"
#include "Engine/FBX.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pFbx(nullptr)
{
	g = 0.1;
}

Player::~Player()
{
}

void Player::Initialize()
{
	pFbx = new FBX();
	pFbx->Load("Assets\\oden.fbx");
	SphereCollider* col = new SphereCollider(1.0);
	this->AddCollider(col);
}

void Player::Update()
{
	if (Input::IsKey(DIK_A))
	{
		transform_.position_.x -= 0.01;
	}
	if (Input::IsKey(DIK_D))
	{
		transform_.position_.x += 0.01;
	}
	if (Input::IsKey(DIK_S))
	{
		transform_.position_.z -= 0.01;
	}
	if (Input::IsKey(DIK_W))
	{
		transform_.position_.z += 0.01;
	}
	/*if (Input::IsKey(DIK_LSHIFT))
	{
		transform_.position_.y -= 0.01;
	}
	if (Input::IsKey(DIK_SPACE))
	{
		transform_.position_.y += 0.01;
	}*/
	if (transform_.rotate_.y > 360)
	{
		transform_.rotate_.y -= 360;
	}
	transform_.rotate_.y += g;
}

void Player::Draw()
{
	pFbx->Draw(transform_);
}

void Player::Release()
{
	pFbx->Release();
	delete pFbx;
}

void Player::OnCollision(GameObject* pTarget)
{
	KillMe();
	pTarget->KillMe();
}
