#include "WorldGameScene.h"
#include "CanvasTitleScene.h"
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

	//MotorBikeParent* motorBike = new MotorBikeParent();
	//motorBike->SetScale(1.0f);
	//motorBike->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	//motorBike->SetRotation(Quaternion(Vector3::UnitZ, Math::ToRadians(-110.0f)));

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


	//// 橋オブジェクトの生成
	//for (int i = 0; i < 8; i++)
	//{
	//	//BridgeObject* bridge = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

	//	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	//}

	//Mesh* meshw1 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Wall_White_A/SpaceShip_Wall_White_A.gpmesh");
	//Mesh* meshw2 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Wall_White_B/SpaceShip_Wall_White_B.gpmesh");

	//Mesh* meshF1 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Black/SpaceShip_Floor_Black.gpmesh");
	//Mesh* meshF2 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Red/SpaceShip_Floor_Red.gpmesh");
	//Mesh* meshF3 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Steel/SpaceShip_Floor_Steel.gpmesh");
	//Mesh* meshG = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Gray/SpaceShip_Gray.gpmesh");
	//Mesh* meshSt = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Steel/SpaceShip_Steel.gpmesh");
	//Mesh* mesh3 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Light_A/SpaceShip_LightA.gpmesh");
	//Mesh* mesh4 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Light_B/SpaceShip_LightB.gpmesh");
	Mesh* meshTer = RENDERER->GetMesh("Data/Meshes/Objects/Environment/Terrain/SM_Field.gpmesh");

	//LevelBlock* spacew1 = new LevelBlock();
	//spacew1->SetMesh(meshw1);
	//spacew1->SetScale(1.0f);
	//spacew1->SetPosition(Vector3(177.0f, 0.0f, 0.0f));

	//LevelBlock* spacew2 = new LevelBlock();
	//spacew2->SetMesh(meshw2);
	//spacew2->SetScale(1.0f);
	//spacew2->SetPosition(Vector3(393.0f, 21.0f, 0.0f));

	//LevelBlock* spaceF1 = new LevelBlock();
	//spaceF1->SetMesh(meshF1);
	//spaceF1->SetScale(1.0f);

	//LevelBlock* spaceF2 = new LevelBlock();
	//spaceF2->SetMesh(meshF2);
	//spaceF2->SetScale(1.0f);

	//LevelBlock* spaceF3 = new LevelBlock();
	//spaceF3->SetMesh(meshF3);
	//spaceF3->SetScale(1.0f);

	//LevelBlock* spaceG = new LevelBlock();
	//spaceG->SetMesh(meshG);
	//spaceG->SetScale(1.0f);

	//LevelBlock* spaceSt = new LevelBlock();
	//spaceSt->SetMesh(meshSt);
	//spaceSt->SetScale(1.0f);
	//spaceSt->SetPosition(Vector3(441.0f, 0.0f, 0.0f));

	//LevelBlock* space3 = new LevelBlock();
	//space3->SetMesh(mesh3);
	//space3->SetScale(1.0f);
	//space3->SetMeshIntensity(3.0f);
	//space3->SetPosition(Vector3(0.0f, 0.0f, 431.0f));


	//LevelBlock* space4 = new LevelBlock();
	//space4->SetMesh(mesh4);
	//space4->SetScale(1.0f);
	//space4->SetMeshIntensity(3.0f);
	//space4->SetPosition(Vector3(0.0f, 0.0f, 12.0f));

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
