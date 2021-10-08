//----------------------------------------------------------------------------------+
// @file        PlayerState_JumpStart.h
// @brief       プレイヤーステート(ジャンプ開始)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/15   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_JumpStart : public PlayerStateBase
{

public:

	PlayerState_JumpStart();
	~PlayerState_JumpStart();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


};