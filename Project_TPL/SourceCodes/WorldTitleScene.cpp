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

	MotorBikeParent* motorBike = new MotorBikeParent();
	motorBike->SetScale(0.4f);
	// 環境情報の生成
	m_environment = new Environment(motorBike, ENVIRONMENT_TYPE::NIGHT);

	// 橋オブジェクトの生成
	for (int i = 0; i < 8; i++)
	{
		//BridgeObject* bridge = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	Mesh* mesh1 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Wall/SpaceShip_Wall.gpmesh");
	Mesh* mesh2 = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/SpaceShip/Floor/SpaceShip_Floor.gpmesh");


	LevelBlock* space1 = new LevelBlock();
	space1->SetMesh(mesh1);
	//space1->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	space1->SetScale(1.0f);

	LevelBlock* space2 = new LevelBlock();
	space2->SetMesh(mesh2);
	//space2->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	space2->SetScale(1.0f);

	return true;
}

void WorldTitleScene::UpdateWorld(float _deltaTime)
{
	// ディレクショナルライトの更新
	//m_environment->SetDirectionalLightPos(m_car->GetPosition());
}
