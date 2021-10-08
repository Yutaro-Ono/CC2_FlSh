//----------------------------------------------------------------------------------+
// @file        PlayerStateSprint.h
// @brief       �v���C���[�X�e�[�g(����)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"


class PlayerState_Sprint : public PlayerStateBase
{

public:

	PlayerState_Sprint();
	~PlayerState_Sprint();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:


};