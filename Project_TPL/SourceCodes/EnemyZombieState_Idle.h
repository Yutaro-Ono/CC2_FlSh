//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Idle.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        �ҋ@���
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Idle : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Idle();
	~EnemyZombieState_Idle();

	ZOMBIE_STATE Update(class EnemyZombie* _zombie, float _deltaTime) override;
	void EnterState(class EnemyZombie* _zombie) override;

protected:

	unsigned short int m_motionNum;

};