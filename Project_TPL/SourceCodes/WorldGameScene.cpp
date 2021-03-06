#include "WorldGameScene.h"
#include "Environment.h"
#include "GameMain.h"
#include "LoadScreen.h"
#include "BridgeObject.h"
#include "TitleCar.h"
#include "ClientActor.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "MotorBikeParent.h"
#include "Player.h"
#include "EnemyZombie.h"
#include "WeaponAR4.h"
#include "PlayerHUD.h"

#include "MeshGpmesh.h"
#include "LevelBlock.h"
#include "CanvasGameScene.h"
#include "LandMarkIndicator.h"
#include "MiniMapHUD.h"

#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <random>

static Vector3 playerPos;
static Vector3 tempPos;

// コンストラクタ
WorldGameScene::WorldGameScene()
	:m_player(nullptr)
	,m_level(nullptr)
{
}

WorldGameScene::~WorldGameScene()
{
	RENDERER->RemoveMapHUD();
}

bool WorldGameScene::Load()
{
	// UIキャンバスの生成
	m_canvas = new CanvasGameScene(this);

	// プレイヤー
	m_player = new Player();
	m_player->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	m_player->SetScale(1.0f);

	// 武器(AR4)
	WeaponAR4* ar4 = new WeaponAR4(m_player);
	ar4->SetPlayer(m_player);

	// 環境情報の生成
	m_environment = new Environment(m_player, ENVIRONMENT_TYPE::MORNING);
	//m_environment = new Environment(player, ENVIRONMENT_TYPE::NIGHT);

	Mesh* meshTer = RENDERER->GetMesh("Data/Meshes/Objects/Environment/Terrain/SM_Field.gpmesh");


	LevelBlock* spaceTer = new LevelBlock();
	spaceTer->SetMesh(meshTer);
	spaceTer->SetScale(100.0f);
	spaceTer->SetMeshIntensity(3.0f);
	spaceTer->SetPosition(Vector3(0.0f, 0.0f, 12.0f));

	// ゾンビ(敵)
	EnemyZombie* zombie = new EnemyZombie();
	zombie->SetPosition(Vector3(380.0f, 0.0f, 20.0f));
	zombie->SetScale(0.8f);
	zombie->SetTarget(m_player);

	// プレイヤー用HUD
	PlayerHUD* pHUD = new PlayerHUD(m_player);

	// マップHUD生成
	MiniMapHUD* mapHUD = new MiniMapHUD(m_player);

	return true;
}

void WorldGameScene::Update(float in_deltaTime)
{
	m_environment->Update();
}

void WorldGameScene::UpdateWorld(float _deltaTime)
{
}


// 依頼人アクタ配列のシャッフル
//void WorldGameScene::ShuffleClientActor()
//{
//	std::random_device seed_gen;
//	std::mt19937 engine(seed_gen());
//	std::shuffle(m_clients.begin(), m_clients.end() - 1, engine);
//}

void WorldGameScene::ImGuiDebugWorld()
{
}
