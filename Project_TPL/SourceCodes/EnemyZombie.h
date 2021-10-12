//----------------------------------------------------------------------------------+
// @file        EnemyZombie.h
// @brief       エネミー(ゾンビ)クラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/27   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

enum class ZOMBIE_STATE
{
	STATE_IDLE,
	STATE_WALK,
	STATE_RUN,
	STATE_ATTACK,
	STATE_DEATH,

	STATE_ALL_NUM
};

class EnemyZombie : public Actor
{

public:

	EnemyZombie();
	~EnemyZombie();

	void UpdateActor(float _deltaTime) override;

	void UpdateZombieState(float _deltaTime);

	void SetZombieState(ZOMBIE_STATE _state);


	class SkeletalMeshComponent* GetSkelMesh() { return m_skelComp; }
	const class Animation* GetAnim(ZOMBIE_STATE _state) { return m_anims[static_cast<unsigned int>(_state)]; }

private:


	std::vector<const class Animation*> m_anims;

	// ステート制御用
	ZOMBIE_STATE m_nowState;
	ZOMBIE_STATE m_nextState;
	std::vector<class EnemyZombieStateBase*> m_statePool;


	// メッシュパス
	static const std::string ZOMBIE_MESH_PATH;
	static const std::string ZOMBIE_SKEL_PATH;
	// アニメーションパス
	static const std::string ANIM_IDLE_PATH;
	static const std::string ANIM_WALK_PATH;
	static const std::string ANIM_RUN_PATH;
	static const std::string ANIM_ATTACK_PATH;
	static const std::string ANIM_DEATH_PATH;



};