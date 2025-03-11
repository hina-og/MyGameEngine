#include "SceneManager.h"

#include "Direct3D.h"
#include "../TestScene.h"
#include "../PlayScene.h"
#include "../TitleScene.h"
#include "../GameoverScene.h"

//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
	currentSceneID_ = SCENE_ID_PLAY;
	nextSceneID_ = currentSceneID_;
	Instantiate<PlayScene>(this);
}

//�X�V
void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜�i�q�I�u�W�F�N�g�����ׂč폜�j
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();
		//Model::Release();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this);
			break;
		case SCENE_ID_PLAY:
			Instantiate<PlayScene>(this);
			break;
		}
		//�V�[���𐶐��ł�����A�J�����g�V�[�����X�V
		currentSceneID_ = nextSceneID_;
	}
}

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID nextScene)
{
	nextSceneID_ = nextScene;
}
