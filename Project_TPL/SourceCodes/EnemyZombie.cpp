#include "EnemyZombie.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "EnemyZombieStateBase.h"
#include "EnemyZombieState_Idle.h"
#include "EnemyZombieState_WakeUp.h"
#include "EnemyZombieState_Walk.h"
#include "EnemyZombieState_Run.h"
#include "EnemyZombieState_Attack.h"
#include "EnemyZombieState_Death.h"
#include "DetectionActorComponent.h"
#include "TrackActorComponent.h"
#include "BoxColliderComponent.h"


// メッシュパス
const std::string EnemyZombie::ZOMBIE_MESH_PATH = "Data/Meshes/Actors/Zombie/zombie_A/Enemy_ZombieA.gpmesh";
const std::string EnemyZombie::ZOMBIE_SKEL_PATH = "Data/Meshes/Actors/Zombie/zombie_A/Enemy_ZombieA.gpskel";
// アニメーションパス
const std::string EnemyZombie::ANIM_LAY_WAKEUP_PATH = "Data/Animation/zombieA/zombieA_Laying_StandUp.gpanim";
const std::string EnemyZombie::ANIM_IDLE_1_PATH = "Data/Animation/zombieA/zombieA_Idle_1.gpanim";
const std::string EnemyZombie::ANIM_IDLE_2_PATH = "Data/Animation/zombieA/zombieA_Idle_2.gpanim";
const std::string EnemyZombie::ANIM_WALK_PATH = "Data/Animation/zombieA/zombieA_Walking.gpanim";
const std::string EnemyZombie::ANIM_RUN_PATH = "Data/Animation/zombieA/zombieA_Walking.gpanim";
const std::string EnemyZombie::ANIM_ATTACK_PATH = "Data/Animation/zombieA/zombieA_Attack_1.gpanim";
const std::string EnemyZombie::ANIM_DEATH_PATH = "Data/Animation/zombieA/zombieA_Dying_1.gpanim";

EnemyZombie::EnemyZombie()
	:EnemyBase(OBJECT_TAG::ACTOR_ENEMY)
	,m_isLaying(true)
	,m_detectComp(nullptr)
	,m_boxColBody(nullptr)
{
	SetScale(0.8f);

	// スケルタルメッシュの読み込み
	Mesh* mesh = RENDERER->GetMesh(ZOMBIE_MESH_PATH);
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(ZOMBIE_SKEL_PATH));
	

	// アニメーションのロード
	m_anims.resize(static_cast<unsigned int>(ZOMBIE_STATE::STATE_ALL_NUM));
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_IDLE)] = RENDERER->GetAnimation(ANIM_IDLE_1_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_IDLE_2)] = RENDERER->GetAnimation(ANIM_IDLE_2_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_LAY_WAKEUP)] = RENDERER->GetAnimation(ANIM_LAY_WAKEUP_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_WALK)] = RENDERER->GetAnimation(ANIM_WALK_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_RUN)] = RENDERER->GetAnimation(ANIM_RUN_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_ATTACK)] = RENDERER->GetAnimation(ANIM_ATTACK_PATH.c_str(), false);
	m_anims[static_cast<unsigned int>(ZOMBIE_STATE::STATE_DEATH)] = RENDERER->GetAnimation(ANIM_DEATH_PATH.c_str(), true);

	// プレイヤーステートプールの生成
	m_statePool.push_back(new EnemyZombieState_Idle);
	m_statePool.push_back(new EnemyZombieState_WakeUp);
	m_statePool.push_back(new EnemyZombieState_Walk);
	m_statePool.push_back(new EnemyZombieState_Run);
	m_statePool.push_back(new EnemyZombieState_Attack);
	m_statePool.push_back(new EnemyZombieState_Death);

	// 待機状態を開始
	m_statePool[static_cast<unsigned int>(m_nowState)]->EnterState(this);

	// プレイヤー検出コンポーネントの生成
	m_detectComp = new DetectionActorComponent(this, nullptr);
	new TrackActorComponent(this);


	// 当たり判定(ボックス)
	AABB box = mesh->GetCollisionBox();
	box.m_isRotatable = false;
	m_boxColBody = new BoxColliderComponent(this);
	m_boxColBody->SetObjectBox(box);
}

EnemyZombie::~EnemyZombie()
{
}

void EnemyZombie::UpdateActor(float _deltaTime)
{
	// ステートの更新
	UpdateZombieState(_deltaTime);

	// プレイヤー検出時、起き上がりステートへ移行
	if (m_detectComp->GetIsDetected())
	{
		if (m_isLaying)
		{
			m_nextState = ZOMBIE_STATE::STATE_LAY_WAKEUP;
		}

	}
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
/// 当たり判定処理
/// </summary>
/// <param name="_ownCollComp"></param>
/// <param name="_otherCollComp"></param>
void EnemyZombie::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{

	// 攻撃状態の時は無視
	if (m_enemyState == ENEMY_STATE::STATE_ATTACK)
	{
		return;
	}

	// タグごとに処理を分岐
	OBJECT_TAG otherTag = _otherCollComp->GetOwnerTag();

	// 環境オブジェクトとの当たり判定
	if (otherTag == OBJECT_TAG::STATIC_OBJECT)
	{
		if (_otherCollComp->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			Vector3 fix;

			//壁とぶつかったとき
			AABB bgBox = dynamic_cast<BoxColliderComponent*>(_otherCollComp)->GetWorldBox();
			AABB enemyBox = m_boxColBody->GetWorldBox();

			// めり込みを修正
			CalcCollisionFixVec(enemyBox, bgBox, fix);

			// 補正ベクトル分戻す
			m_position += fix;
			// 位置再計算
			//ComputeWorldTransform();
		}

	}

	// プレイヤーとの当たり判定
	if (otherTag == OBJECT_TAG::ACTOR_PLAYER)
	{
		if (_otherCollComp->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			Vector3 fix;

			//壁とぶつかったとき
			AABB bgBox = dynamic_cast<BoxColliderComponent*>(_otherCollComp)->GetWorldBox();
			AABB enemyBox = m_boxColBody->GetWorldBox();

			// めり込みを修正
			CalcCollisionFixVec(enemyBox, bgBox, fix);

			// 補正ベクトル分戻す
			m_position += fix;
			// 位置再計算
			//ComputeWorldTransform();
		}
	}

	// ゾンビ同士の当たり判定
	if (otherTag == OBJECT_TAG::ACTOR_ENEMY)
	{



		if (_otherCollComp->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			Vector3 fix;

			//壁とぶつかったとき
			AABB bgBox = dynamic_cast<BoxColliderComponent*>(_otherCollComp)->GetWorldBox();
			AABB enemyBox = m_boxColBody->GetWorldBox();

			// めり込みを修正
			CalcCollisionFixVec(enemyBox, bgBox, fix);

			// 補正ベクトル分戻す
			m_position += fix;
			// 位置再計算
			//ComputeWorldTransform();
		}
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

/// <summary>
/// 検出コンポーネントにターゲットをセットする
/// </summary>
/// <param name="_target"></param>
void EnemyZombie::SetTarget(Actor* _target)
{
	m_detectComp->SetTarget(_target);
}

