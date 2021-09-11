//----------------------------------------------------------------------------------+
// @file        PlayerStateRun.h
// @brief       プレイヤーステート(走り)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"


class PlayerState_Run : public PlayerStateBase
{

public:

	PlayerState_Run();
	~PlayerState_Run();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player, float _deltaTime) override;


private:


};