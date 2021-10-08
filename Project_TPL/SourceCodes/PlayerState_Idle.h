//----------------------------------------------------------------------------------+
// @file        PlayerStateIdle.h
// @brief       �v���C���[�X�e�[�g(�ҋ@)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_Idle : public PlayerStateBase
{

public:

	PlayerState_Idle();
	~PlayerState_Idle();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:

};