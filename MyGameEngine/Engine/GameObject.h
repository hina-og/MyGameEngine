#pragma once
#include <string>
#include <list>
#include "Transform.h"

class SphereCollider;

class GameObject
{
	bool isDead_;
protected:
	std::list<GameObject*> childList_;
	Transform transform_;
	GameObject* pParent_;
	std::string objectName_;
	SphereCollider* pCollider_;
public:
	GameObject();
	GameObject(GameObject* parent, const std::string str);
	~GameObject();
	virtual void  Initialize() = 0;
	virtual void  Update() = 0;
	virtual void  Draw() = 0;
	virtual void  Release() = 0;
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	void KillMe();

	void SetPosition(XMFLOAT3 _pos) { transform_.position_ = _pos; }
	void SetPosition(float _x, float _y, float _z) {
		transform_.position_.x = _x;
		transform_.position_.y = _y;
		transform_.position_.z = _z;
	}

	void SetRotateX(float xrotate) { transform_.rotate_.x = xrotate; }
	void SetRotateY(float yrotate) { transform_.rotate_.y = yrotate; }
	void SetRotateZ(float zrotate) { transform_.rotate_.z = zrotate; }

	GameObject* FindObject(std::string objName);
	GameObject* GetRootJob();
	GameObject* FindChildObject(std::string objName);
	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget) {};

	//オブジェクトを作成するテンプレート
	template <class T>
	T* Instantiate(GameObject* pParent)
	{
		T* pNewObject = new T(pParent);
		if (pNewObject != nullptr)
		{
			pNewObject->Initialize();
			childList_.push_back(pNewObject);
		}
		return pNewObject;
	}
};

