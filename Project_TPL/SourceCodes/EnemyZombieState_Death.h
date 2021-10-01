//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Death.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        ���S���
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Death : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Death();
	~EnemyZombieState_Death();

	ZOMBIE_STATE Update(class EnemyZombie* _player, float _deltaTime) override;
	void EnterState(class EnemyZombie* _player, float _deltaTime) override;

protected:


};