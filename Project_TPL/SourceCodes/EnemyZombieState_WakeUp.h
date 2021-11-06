//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_WakeUp.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        �N���オ����
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/05   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_WakeUp : public EnemyZombieStateBase
{

public:

	EnemyZombieState_WakeUp();
	~EnemyZombieState_WakeUp();

	ZOMBIE_STATE Update(class EnemyZombie* _zombie, float _deltaTime) override;
	void EnterState(class EnemyZombie* _zombie) override;

protected:


};