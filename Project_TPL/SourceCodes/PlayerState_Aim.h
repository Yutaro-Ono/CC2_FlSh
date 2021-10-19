//----------------------------------------------------------------------------------+
// @file        PlayerState_Aim.h
// @brief       �v���C���[�X�e�[�g(�G�C��)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/15   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_Aim : public PlayerStateBase
{

public:

	PlayerState_Aim();
	~PlayerState_Aim();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:

};