//----------------------------------------------------------------------------------+
// @file        PlayerState_Crouch.h
// @brief       �v���C���[�X�e�[�g(������)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"


class PlayerState_Crouch : public PlayerStateBase
{

public:

	PlayerState_Crouch();
	~PlayerState_Crouch();


	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;




private:





};