#pragma once
#include "Engine/GameObject.h"

class GameoverScene
	: public GameObject
{
public:
	GameoverScene(GameObject* parent);
	~GameoverScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};
