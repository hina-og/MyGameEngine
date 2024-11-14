#include "GameObject.h"
#include "Direct3D.h"
#include "SphereCollider.h"

GameObject::GameObject()
	: pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string str)
	:pParent_(parent), objectName_(str), isDead_(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
 	RoundRobin(GetRootJob());

	for (auto itr : childList_)
	{
		itr->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end(); )
	{
		if ((*itr)->isDead_)
		{
			(*itr)->ReleaseSub();
			//(*itr)->Release();
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}

	}

}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr : childList_)
	{
		itr->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();
	for (auto itr : childList_)
	{
		itr->ReleaseSub();
	}
}

void GameObject::KillMe()
{
	isDead_ = true;
}

GameObject* GameObject::FindObject(std::string objName)
{
	GameObject* rootJob = GetRootJob();
	GameObject* res = rootJob->FindChildObject(objName);
	return(res);
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
	{
		return this;
	}
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindChildObject(std::string objName)
{
	if (this->objectName_ == objName)
	{
		return this;
	}
	else
	{
		for (auto itr : childList_)
		{
			GameObject* obj = itr->FindChildObject(objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (this->pCollider_ == nullptr || pTarget->pCollider_ == nullptr || this == pTarget)
		return;

	XMVECTOR me = XMLoadFloat3(&(transform_.position_));
	XMVECTOR ta = XMLoadFloat3(&(pTarget->transform_.position_));

	float dist = XMVectorGetX(XMVector3Length(me - ta));
	float rDist = this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius();
	if (dist <= rDist)
	{
		OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	//自分とターゲットの当たり判定
	if (this->pCollider_ == nullptr)
		return;
	if(pTarget->pCollider_ != nullptr)
		Collision(pTarget);
	//自分とターゲットの子オブジェクト全部の当たり判定
	for (auto& itr : pTarget->childList_)
	{
		RoundRobin(itr);
	}
}