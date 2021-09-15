//----------------------------------------------------------------------------------+
// @file        PlayerState_JumpLand.h
// @brief       プレイヤーステート(ジャンプ着地)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/15   新規作成
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