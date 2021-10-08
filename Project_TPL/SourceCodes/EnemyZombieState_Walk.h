//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Walk.h
// @brief       エネミー(ゾンビ)ステートクラス
// @note        歩行状態
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Walk : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Walk();
	~EnemyZombieState_Walk();

	ZOMBIE_STATE Update(class EnemyZombie* _player, float _deltaTime) override;
	void EnterState(class EnemyZombie* _player) override;

protected:


};