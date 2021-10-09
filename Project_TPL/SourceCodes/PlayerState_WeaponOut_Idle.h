//----------------------------------------------------------------------------------+
// @file        PlayerState_WeaponOut_Idle.h
// @brief       �v���C���[�X�e�[�g(����o��+�ҋ@)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/08   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_WeaponOut_Idle : public PlayerStateBase
{

public:

	PlayerState_WeaponOut_Idle();
	~PlayerState_WeaponOut_Idle();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:

};