//----------------------------------------------------------------------------------+
// @file        PlayerState.h
// @brief       プレイヤー固有のステートベースクラス
// @note        ステートパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/11   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Player.h"

class PlayerStateBase
{

public:

	PlayerStateBase();
	virtual ~PlayerStateBase() {};

	// ステート固有の更新関数
	virtual PLAYER_STATE Update(class Player* _player, float _deltaTime) = 0;
	// このステートに入った時に必ず呼び出される関数(主にアニメーションの再生をする)
	virtual void EnterState(class Player* _player) {};

protected:


	float m_animSpeed;     // ステートアニメーションの再生速度
	static const float JOG_SPEED_LINE;
	static const float WALK_SPEED_LINE;

};