//----------------------------------------------------------------------------------+
// @file        PlayerStateJog.h
// @brief       �v���C���[�X�e�[�g(������)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_Jog : public PlayerStateBase
{

public:

	PlayerState_Jog();
	~PlayerState_Jog();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;


private:

};