//----------------------------------------------------------------------------------+
// @file        PlayerState_CrouchMove.h
// @brief       プレイヤーステート(かがみ移動)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/14   新規作成
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