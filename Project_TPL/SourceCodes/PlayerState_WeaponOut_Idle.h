//----------------------------------------------------------------------------------+
// @file        PlayerState_WeaponOut_Idle.h
// @brief       プレイヤーステート(武器出し+待機)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/08   新規作成
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