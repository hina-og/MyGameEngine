#pragma once
#include "Engine/GameObject.h"
#include "Engine/FBX.h"

class Enemy
	: public GameObject
{
	int dir = 1;
	FBX* pFbx;
public:
	Enemy(GameObject* parent);
	~Enemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

