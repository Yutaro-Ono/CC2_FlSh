//----------------------------------------------------------------------------------+
// @file        EnemyZombieStateBase.h
// @brief       �G�l�~�[(�]���r)�X�e�[�g�N���X
// @note        �x�[�X�N���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombie.h"

class EnemyZombieStateBase
{

public:

	EnemyZombieStateBase();
	~EnemyZombieStateBase();

	// �X�e�[�g�ŗL�̍X�V�֐�
	virtual ZOMBIE_STATE Update(class EnemyZombie* _zombie, float _deltaTime) = 0;
	// ���̃X�e�[�g�ɓ��������ɕK���Ăяo�����֐�(��ɃA�j���[�V�����̍Đ�������)
	virtual void EnterState(class EnemyZombie* _zombie, float _deltaTime) {};

protected:

	float m_animSpeed;

};