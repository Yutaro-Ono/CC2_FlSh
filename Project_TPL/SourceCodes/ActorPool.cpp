//----------------------------------------------------------------------------------+
// @file        ActorPool.cpp
// @brief       アクタープール (Actorクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#include "ActorPool.h"

ActorPool::ActorPool()
{
}

ActorPool::~ActorPool()
{
}

void ActorPool::UpdateObjects(float _deltaTime)
{
	// 全アクターの更新
	for (auto iter : m_objPool)
	{
		iter->ProcessInput(_deltaTime);
		iter->UpdateActor(_deltaTime);
		iter->Update(_deltaTime);
	}
	// 全アクター更新(連想配列)
	for (auto iter : m_keyObjPool)
	{
		iter.second->ProcessInput(_deltaTime);
		iter.second->UpdateActor(_deltaTime);
		iter.second->Update(_deltaTime);
	}
	/*
	// 死亡状態のアクターを保管→消去
	std::vector<Actor*> deadActors;
	for (auto actor : m_objPool)
	{
		if (actor->GetState() == Actor::STATE_DEAD)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors)
	{
		DeleteObject(actor);
	}
	deadActors.clear();

	// 死亡状態のアクターを保管→消去 (連想配列)
	std::vector<std::string> deadKeys;
	for (auto key : m_keyObjPool)
	{
		if (key.second->GetState() == Actor::STATE_DEAD)
		{
			deadKeys.emplace_back(key);
		}
	}
	for (auto key : deadKeys)
	{
		DeleteKeyObject(key);
	}
	deadKeys.clear();
	*/

}

/// <summary>
/// アクターステートの変更(主に一時停止時に使用)
/// </summary>
/// <param name="_state"> 適用したいアクターステート </param>
void ActorPool::SetActorState(Actor::ACTOR_STATE _state)
{
	for (auto iter : m_objPool)
	{
		if (iter->GetState() != Actor::STATE_DEAD)
		{
			iter->SetState(_state);
		}
	}

	for (auto iter : m_keyObjPool)
	{
		if (iter.second->GetState() != Actor::STATE_DEAD)
		{
			iter.second->SetState(_state);
		}
	}
}
