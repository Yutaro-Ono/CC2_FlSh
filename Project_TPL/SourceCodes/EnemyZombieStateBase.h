//----------------------------------------------------------------------------------+
// @file        EnemyZombieStateBase.h
// @brief       エネミー(ゾンビ)ステートクラス
// @note        ベースクラス
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "EnemyZombie.h"

class EnemyZombieStateBase
{

public:

	EnemyZombieStateBase();
	~EnemyZombieStateBase();

	// ステート固有の更新関数
	virtual ZOMBIE_STATE Update(class EnemyZombie* _zombie, float _deltaTime) = 0;
	// このステートに入った時に必ず呼び出される関数(主にアニメーションの再生をする)
	virtual void EnterState(class EnemyZombie* _zombie, float _deltaTime) {};

protected:

	float m_animSpeed;

};