//----------------------------------------------------------------------------------+
// @file        ActorPool.cpp
// @brief       �A�N�^�[�v�[�� (Actor�N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
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
	// �S�A�N�^�[�̍X�V
	for (auto iter : m_objPool)
	{
		iter->ProcessInput(_deltaTime);
		iter->UpdateActor(_deltaTime);
		iter->Update(_deltaTime);
	}
	// �S�A�N�^�[�X�V(�A�z�z��)
	for (auto iter : m_keyObjPool)
	{
		iter.second->ProcessInput(_deltaTime);
		iter.second->UpdateActor(_deltaTime);
		iter.second->Update(_deltaTime);
	}
	/*
	// ���S��Ԃ̃A�N�^�[��ۊǁ�����
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

	// ���S��Ԃ̃A�N�^�[��ۊǁ����� (�A�z�z��)
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
/// �A�N�^�[�X�e�[�g�̕ύX(��Ɉꎞ��~���Ɏg�p)
/// </summary>
/// <param name="_state"> �K�p�������A�N�^�[�X�e�[�g </param>
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
