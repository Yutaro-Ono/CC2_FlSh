//----------------------------------------------------------------------------------+
// @file        PlayerState_JumpLand.h
// @brief       �v���C���[�X�e�[�g(�W�����v���n)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/15   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_JumpLand : public PlayerStateBase
{

public:

	PlayerState_JumpLand();
	~PlayerState_JumpLand();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;
};