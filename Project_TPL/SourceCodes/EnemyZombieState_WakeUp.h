//----------------------------------------------------------------------------------+
// @file        EnemyZombieState_WakeUp.h
// @brief       エネミー(ゾンビ)ステートクラス
// @note        起き上がり状態
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/05   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombieStateBase.h"

class EnemyZombieState_WakeUp : public EnemyZombieStateBase
{

public:

	EnemyZombieState_WakeUp();
	~EnemyZombieState_WakeUp();

	ZOMBIE_STATE Update(class EnemyZombie* _zombie, float _deltaTime) override;
	void EnterState(class EnemyZombie* _zombie) override;

protected:


};