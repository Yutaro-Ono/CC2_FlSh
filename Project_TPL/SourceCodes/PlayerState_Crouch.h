//----------------------------------------------------------------------------------+
// @file        PlayerState_Crouch.h
// @brief       プレイヤーステート(かがみ)クラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/14   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "PlayerStateBase.h"


class PlayerState_Crouch : public PlayerStateBase
{

public:

	PlayerState_Crouch();
	~PlayerState_Crouch();


	PLAYER_STATE Update(class Player* _player, float _deltaTime) override;
	void EnterState(class Player* _player) override;




private:





};