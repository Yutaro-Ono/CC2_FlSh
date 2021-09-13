#include "Player.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerState_Idle.h"
#include "PlayerState_Walk.h"
#include "PlayerState_Jog.h"
#include "PlayerState_Run.h"
#include "TPSCamera.h"
#include "MoveComponentHuman.h"
#include "PointLight.h"

const std::string Player::PLAYER_MESH_PATH = "Data/Meshes/Actors/HumanRace/Player/Player_gonzales.gpmesh";
const std::string Player::PLAYER_SKEL_PATH = "Data/Meshes/Actors/HumanRace/Player/Player_gonzales.gpskel";
const std::string Player::ANIM_IDLE_PATH = "Data/Animation/Player/Player_Idle.gpanim";
const std::string Player::ANIM_WALK_PATH = "Data/Animation/Player/Player_Walk.gpanim";
const std::string Player::ANIM_JOG_PATH = "Data/Animation/Player/Player_Jog.gpanim";
const std::string Player::ANIM_RUN_PATH = "Data/Animation/Player/Player_Run.gpanim";


Player::Player()
	:Actor(OBJECT_TAG::PLAYER)
	,m_nowState(PLAYER_STATE::STATE_IDLE)
	,m_nextState(PLAYER_STATE::STATE_IDLE)
	,m_tpsCamera(nullptr)
	,m_light(nullptr)
{

	// カメラの生成(三人称カメラ)
	m_tpsCamera = new TPSCamera(this);
	m_tpsCamera->SetCameraLength(Vector3(120.0f, 120.0f, 120.0f));
	m_tpsCamera->SetAdjustTargetPos(Vector2(-40.0f, -85.0f));

	// 移動コンポーネントの追加
	m_components.push_back(new MoveComponentHuman(this));

	// ライトの生成
	m_light = new PointLight(PointLight::VL_BIG);
	m_light->SetPosition(m_position);
	m_light->SetLightColor(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	m_light->SetLuminance(5.0f);

	// スケルタルメッシュの読み込み
	Mesh* mesh = RENDERER->GetMesh(PLAYER_MESH_PATH);
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(PLAYER_SKEL_PATH));

	// アニメーションのロード
	m_anims.resize(static_cast<unsigned int>(PLAYER_STATE::STATE_ALL_NUM));
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_IDLE)] = RENDERER->GetAnimation(ANIM_IDLE_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_WALK)] = RENDERER->GetAnimation(ANIM_WALK_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_JOG)] = RENDERER->GetAnimation(ANIM_JOG_PATH.c_str(), true);
	m_anims[static_cast<unsigned int>(PLAYER_STATE::STATE_RUN)] = RENDERER->GetAnimation(ANIM_RUN_PATH.c_str(), true);


	// プレイヤーステートプールの生成
	m_statePool.push_back(new PlayerState_Idle);
	m_statePool.push_back(new PlayerState_Walk);
	m_statePool.push_back(new PlayerState_Jog);
	m_statePool.push_back(new PlayerState_Run);
	// 待機状態を開始
	m_statePool[static_cast<unsigned int>(m_nowState)]->EnterState(this, GAME_INSTANCE.GetDeltaTime());

}

Player::~Player()
{
}

void Player::UpdateActor(float _deltaTime)
{
	// ライト追従
	m_light->SetPosition(m_position);

	// プレイヤーステートの更新
	UpdatePlayerState(_deltaTime);
}


void Player::UpdatePlayerState(float _deltaTime)
{
	// 外部からのステート変更があったかをチェック
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this, _deltaTime);
		m_nowState = m_nextState;
		return;
	}

	// ステート更新
	m_nextState = m_statePool[static_cast<unsigned int>(m_nowState)]->Update(this, _deltaTime);


	// 現在ステートの更新によって、ステート変更があったかをチェック
	if (m_nowState != m_nextState)
	{
		m_statePool[static_cast<unsigned int>(m_nextState)]->EnterState(this, _deltaTime);
		m_nowState = m_nextState;
	}
}
