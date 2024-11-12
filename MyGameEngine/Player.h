#pragma once
#include "Engine/GameObject.h"

//‘O•ûéŒ¾
class FBX;

class Player
	: public GameObject
{
	float g;
	FBX* pFbx;
public:
	Player(GameObject* parent);
	~Player();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

