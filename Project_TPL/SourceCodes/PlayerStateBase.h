//----------------------------------------------------------------------------------+
// @file        PlayerState.h
// @brief       �v���C���[�ŗL�̃X�e�[�g�x�[�X�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Player.h"

class PlayerStateBase
{

public:

	PlayerStateBase();
	virtual ~PlayerStateBase() {};

	// �X�e�[�g�ŗL�̍X�V�֐�
	virtual PLAYER_STATE Update(class Player* _player, float _deltaTime) = 0;
	// ���̃X�e�[�g�ɓ��������ɕK���Ăяo�����֐�(��ɃA�j���[�V�����̍Đ�������)
	virtual void EnterState(class Player* _player) {};

protected:


	float m_animSpeed;     // �X�e�[�g�A�j���[�V�����̍Đ����x
	static const float JOG_SPEED_LINE;
	static const float WALK_SPEED_LINE;

};