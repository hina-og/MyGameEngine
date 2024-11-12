#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::IsKey(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
