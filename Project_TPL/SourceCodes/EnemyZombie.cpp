#include "EnemyZombie.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "EnemyZombieStateBase.h"
#include "EnemyZombieState_Idle.h"
#include "EnemyZombieState_Walk.h"
#include "EnemyZombieState_Run.h"
#include "EnemyZombieState_Attack.h"
#include "EnemyZombieState_Death.h"

// メッシュパス
const std::string EnemyZombie::ZOMBIE_MESH_PATH = "Data/Meshes/Actors/Zombie/zombie_A/Enemy_ZombieA.gpmesh";
const std::string EnemyZombie::ZOMBIE_SKEL_PATH = "Data/Meshes/Actors/Zombie/zombie_A/Enemy_ZombieA.gpskel";
// アニメーションパス
const std::string EnemyZombie::ANIM_IDLE_PATH = "Data/Animation/zombieA/zombieA_Idle_1.gpanim";
const std::string EnemyZombie::ANIM_WALK_PATH = "Data/Animation/zombieA/zombieA_Walking.gpanim";
const std::string EnemyZombie::ANIM_RUN_PATH = "Data/Animation/zombieA/zombieA_Biting.gpanim";
const std::string EnemyZombie::ANIM_ATTACK_PATH = "Data/Animation/zombieA/zombieA_Attack_1.gpanim";
const std::string EnemyZombie::ANIM_DEATH_PATH = "Data/Animation/zombieA/zombieA_Dying_1.gpanim";

EnemyZombie::EnemyZombie()
	:Actor(OBJECT_TAG::ACTOR_ENEMY)
{
	// スケルタルメッシュの読み込み
	Mesh* mesh = RENDERER->GetMesh(ZOMBIE_MESH_PATH);
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(ZOMBIE_SKEL_PATH));

	// アニメーションのロード
	m_anims.resize(static_cast<unsigned int>(ZOMBIE_STATE::STATE_ALL_NUM));
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_IDLE)] = RENDERER->GetAnimation(ANIM_IDLE_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_WALK)] = RENDERER->GetAnimation(ANIM_WALK_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_RUN)] = RENDERER->GetAnimation(ANIM_RUN_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_ATTACK)] = RENDERER->GetAnimation(ANIM_ATTACK_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_DEATH)] = RENDERER->GetAnimation(ANIM_DEATH_PATH.c_str(), true);

	// プレイヤーステートプールの生成
	m_statePool.push_back(new EnemyZombieState_Idle);
	m_statePool.push_back(new EnemyZombieState_Walk);
	m_statePool.push_back(new EnemyZombieState_Run);
	m_statePool.push_back(new EnemyZombieState_Attack);
	m_statePool.push_back(new EnemyZombieState_Death);

	// 待機状態を開始
	m_statePool[static_cast<unsigned int>(m_nowState)]->EnterState(this);

}

EnemyZombie::~EnemyZombie()
{
}

void EnemyZombie::UpdateActor(float _deltaTime)
{
	UpdateZombieState(_deltaTime);
}

/// <summary>
/// ゾンビのステート更新
/// </summary>
/// <param name="_deltaTime"></param>
void EnemyZombie::UpdateZombieState(float _deltaTime)
{
	// 外部からのステート変更があったかをチェック
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this);
		m_nowState = m_nextState;
		return;
	}

	// ステート更新
	m_nextState = m_statePool[static_cast<unsigned int>(m_nowState)]->Update(this, _deltaTime);

	// 現在ステートの更新によって、ステート変更があったかをチェック
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this);
		m_nowState = m_nextState;
	}
}

/// <summary>
/// 指定したステートを現在ステートにセット
/// </summary>
/// <param name="_state"></param>
void EnemyZombie::SetZombieState(ZOMBIE_STATE _state)
{
	m_nextState = _state;
}
