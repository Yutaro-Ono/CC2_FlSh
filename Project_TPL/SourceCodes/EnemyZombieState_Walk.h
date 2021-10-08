//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Walk.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        ���s���
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Walk : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Walk();
	~EnemyZombieState_Walk();

	ZOMBIE_STATE Update(class EnemyZombie* _player, float _deltaTime) override;
	void EnterState(class EnemyZombie* _player) override;

protected:


};