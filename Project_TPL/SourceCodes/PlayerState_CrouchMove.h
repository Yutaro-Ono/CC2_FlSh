//----------------------------------------------------------------------------------+
// @file        PlayerState_CrouchMove.h
// @brief       �v���C���[�X�e�[�g(�����݈ړ�)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_CrouchMove : public PlayerStateBase
{

public:

	PlayerState_CrouchMove();
	~PlayerState_CrouchMove();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;

};