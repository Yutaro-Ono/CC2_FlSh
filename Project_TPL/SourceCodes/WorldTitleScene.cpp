#include "WorldTitleScene.h"
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

#include "MeshGpmesh.h"
#include "LevelBlock.h"

WorldTitleScene::WorldTitleScene()
	:m_car(nullptr)
	,m_client(nullptr)
{
}

WorldTitleScene::~WorldTitleScene()
{
	
}

bool WorldTitleScene::Load()
{


	// UIキャンバスの生成
	m_canvas = new CanvasTitleScene(this);

	// タイトル用車の生成
	//m_car = new TitleCar();
	//m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	//m_car->SetScale(0.4f);

	//MotorBikeParent* motorBike = new MotorBikeParent();
	//motorBike->SetScale(1.0f);
	//motorBike->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	//motorBike->SetRotation(Quaternion(Vector3::UnitZ, Math::ToRadians(-110.0f)));

	Player* player = new Player();
	player->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	player->SetScale(1.0f);

	// 環境情報の生成
	m_environment = new Environment(player, ENVIRONMENT_TYPE::NIGHT);
	//m_environment = new Environment(m_car, ENVIRONMENT_TYPE::NIGHT);


	//// 橋オブジェクトの生成
	//for (int i = 0; i < 8; i++)
	//{
	//	//BridgeObject* bridge = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

	//	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	//}

	Mesh* meshw1 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Wall_White_A/SpaceShip_Wall_White_A.gpmesh");
	Mesh* meshw2 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Wall_White_B/SpaceShip_Wall_White_B.gpmesh");

	Mesh* meshF1 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Black/SpaceShip_Floor_Black.gpmesh");
	Mesh* meshF2 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Red/SpaceShip_Floor_Red.gpmesh");
	Mesh* meshF3 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor_Steel/SpaceShip_Floor_Steel.gpmesh");
	Mesh* meshG = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Gray/SpaceShip_Gray.gpmesh");
	Mesh* meshSt = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Steel/SpaceShip_Steel.gpmesh");
	Mesh* mesh3 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Light_A/SpaceShip_LightA.gpmesh");
	Mesh* mesh4 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Light_B/SpaceShip_LightB.gpmesh");


	LevelBlock* spacew1 = new LevelBlock();
	spacew1->SetMesh(meshw1);
	spacew1->SetScale(1.0f);

	LevelBlock* spacew2 = new LevelBlock();
	spacew2->SetMesh(meshw2);
	spacew2->SetScale(1.0f);

	LevelBlock* spaceF1 = new LevelBlock();
	spaceF1->SetMesh(meshF1);
	spaceF1->SetScale(1.0f);

	LevelBlock* spaceF2 = new LevelBlock();
	spaceF2->SetMesh(meshF2);
	spaceF2->SetScale(1.0f);

	LevelBlock* spaceF3 = new LevelBlock();
	spaceF3->SetMesh(meshF3);
	spaceF3->SetScale(1.0f);

	LevelBlock* spaceG = new LevelBlock();
	spaceG->SetMesh(meshG);
	spaceG->SetScale(1.0f);

	LevelBlock* spaceSt = new LevelBlock();
	spaceSt->SetMesh(meshSt);
	spaceSt->SetScale(1.0f);

	LevelBlock* space3 = new LevelBlock();
	space3->SetMesh(mesh3);
	space3->SetScale(1.0f);
	space3->SetMeshIntensity(5.0f);

	LevelBlock* space4 = new LevelBlock();
	space4->SetMesh(mesh4);
	space4->SetScale(1.0f);
	space4->SetMeshIntensity(5.0f);



	EnemyZombie* zombie = new EnemyZombie();
	zombie->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	zombie->SetScale(1.0f);

	return true;
}

void WorldTitleScene::UpdateWorld(float _deltaTime)
{
	// ディレクショナルライトの更新
	//m_environment->SetDirectionalLightPos(m_car->GetPosition());
}
