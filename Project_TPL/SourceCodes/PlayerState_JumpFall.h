//----------------------------------------------------------------------------------+
// @file        PlayerState_JumpFall.h
// @brief       プレイヤーステート(ジャンプ落下)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/15   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_JumpFall : public PlayerStateBase
{
public:

	PlayerState_JumpFall();
	~PlayerState_JumpFall();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;
};