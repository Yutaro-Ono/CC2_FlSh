//----------------------------------------------------------------------------------+
// @file        PlayerState_WeaponOut_MoveLeft.h
// @brief       �v���C���[�X�e�[�g(����o��+�������ړ�)�N���X
// @note        �X�e�[�g�p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/08   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_WeaponOut_MoveLeft : public PlayerStateBase
{

public:

	PlayerState_WeaponOut_MoveLeft();
	~PlayerState_WeaponOut_MoveLeft();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:

};