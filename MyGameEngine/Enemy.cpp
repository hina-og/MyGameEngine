#include "Enemy.h"
#include "Engine/SphereCollider.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), pFbx(nullptr)
{
	transform_.position_ = { 0,0,5 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	pFbx = new FBX();
	pFbx->Load("Assets\\karashi.fbx");

	SphereCollider* col = new SphereCollider(0.5);
	this->AddCollider(col);
}

void Enemy::Update()
{
	
	if (transform_.position_.x > 5)
	{
		dir = -1;
	}
	else if (transform_.position_.x < -5)
	{
		dir = 1;
	}

	//transform_.position_.x += 0.001 * dir;
}

void Enemy::Draw()
{
	pFbx->Draw(transform_);
}

void Enemy::Release()
{
	pFbx->Release();
	delete pFbx;
}
