//----------------------------------------------------------------------------------+
// @file        EnemyBase.h
// @brief       エネミーベースクラス
// @note        Actorクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/05   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

enum class ENEMY_STATE
{
	STATE_IDLE,
	STATE_PATROL,
	STATE_TRACK,
	STATE_ATTACK,
	STATE_DEATH
};

class EnemyBase : public Actor
{
public:


	EnemyBase(OBJECT_TAG _tag);
	~EnemyBase();

	void AddHealth(unsigned short int _addVal);
	void ReduceHealth(unsigned short int _redVal);

	ENEMY_STATE GetEnemyState() { return m_enemyState; }
	void SetEnemyState(ENEMY_STATE _state) { m_enemyState = _state; }

private:

	void SetHealth(unsigned short int _health) { m_health = _health; };     // 体力値のセッター


protected:

	ENEMY_STATE m_enemyState;        // エネミーの状態

	unsigned short int m_health;     // 体力値(0で死亡)



};