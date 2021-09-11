//----------------------------------------------------------------------------------+
// @file        PlayerStateIdle.h
// @brief       プレイヤーステート(待機)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerStateIdle : public PlayerStateBase
{

public:

	PlayerStateIdle();
	~PlayerStateIdle();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;


};