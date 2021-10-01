//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_Death.h
// @brief       エネミー(ゾンビ)ステートクラス
// @note        死亡状態
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_Death : public EnemyZombieStateBase
{

public:

	EnemyZombieState_Death();
	~EnemyZombieState_Death();

	ZOMBIE_STATE Update(class EnemyZombie* _player, float _deltaTime) override;
	void EnterState(class EnemyZombie* _player, float _deltaTime) override;

protected:


};