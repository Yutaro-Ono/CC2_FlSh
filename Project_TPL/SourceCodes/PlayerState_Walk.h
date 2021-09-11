//----------------------------------------------------------------------------------+
// @file        PlayerStateWalk.h
// @brief       プレイヤーステート(歩き)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_Walk : public PlayerStateBase
{

public:

	PlayerState_Walk();
	~PlayerState_Walk();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;


private:


};