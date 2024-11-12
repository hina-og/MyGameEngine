#pragma once
///#include "global.h"
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_TEST = 0,
	SCENE_ID_PLAY,
	SCENE_ID_MAX
};

//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager 
	: public GameObject
{
private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え関数
	void ChangeScene(SCENE_ID nextScene);


};