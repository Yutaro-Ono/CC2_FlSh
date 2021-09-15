//----------------------------------------------------------------------------------+
// @file        PlayerState_JumpFall.h
// @brief       �v���C���[�X�e�[�g(�W�����v����)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/15   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_JumpFall : public PlayerStateBase
{
public:

	PlayerState_JumpFall();
	~PlayerState_JumpFall();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;
};