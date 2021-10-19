//----------------------------------------------------------------------------------+
// @file        PlayerState_Aim.h
// @brief       プレイヤーステート(エイム)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/15   新規作成
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