//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Attack.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        �U�����
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Attack : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Attack();
	~EnemyZombieState_Attack();

	ZOMBIE_STATE Update(class EnemyZombie* _player, float _deltaTime) override;
	void EnterState(class EnemyZombie* _player) override;

protected:


};