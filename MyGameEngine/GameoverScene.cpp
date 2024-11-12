#include "GameoverScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

GameoverScene::GameoverScene(GameObject* parent)
	: GameObject(parent, "GameoverScene")
{
}

GameoverScene::~GameoverScene()
{
}

void GameoverScene::Initialize()
{
}

void GameoverScene::Update()
{
	if (Input::IsKey(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameoverScene::Draw()
{
}

void GameoverScene::Release()
{
}
