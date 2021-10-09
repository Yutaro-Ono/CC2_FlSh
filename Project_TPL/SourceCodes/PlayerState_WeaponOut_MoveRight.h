//----------------------------------------------------------------------------------+
// @file        PlayerState_WeaponOut_MoveRight.h
// @brief       プレイヤーステート(武器出し+右方向移動)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/08   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"

class PlayerState_WeaponOut_MoveRight : public PlayerStateBase
{

public:

	PlayerState_WeaponOut_MoveRight();
	~PlayerState_WeaponOut_MoveRight();

	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;


private:

};