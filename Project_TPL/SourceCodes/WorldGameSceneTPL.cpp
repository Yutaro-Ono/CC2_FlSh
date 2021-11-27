#include "WorldGameSceneTPL.h"
#include "Environment.h"
#include "GameMain.h"
#include "LoadScreen.h"
#include "MotorBikeParent.h"
#include "Player.h"
#include "EnemyZombie.h"
#include "WeaponAR4.h"
#include "PlayerHUD.h"

#include "MeshGpmesh.h"
#include "LevelBlock.h"
#include "GameSceneTPL.h"
#include "CanvasGameScene.h"
#include "LandMarkIndicator.h"
#include "MiniMapHUD.h"

WorldGameSceneTPL::WorldGameSceneTPL(class GameSceneTPL* _scene)
    :m_player(nullptr)
	,m_scene(_scene)
{
}

WorldGameSceneTPL::~WorldGameSceneTPL()
{
}

bool WorldGameSceneTPL::Load()
{

	// ƒvƒŒƒCƒ„[
	m_player = new Player();
	m_player->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	m_player->SetScale(1.0f);

	// •Ší(AR4)
	WeaponAR4* ar4 = new WeaponAR4(m_player);
	ar4->SetPlayer(m_player);

	// ŠÂ‹«î•ñ‚Ì¶¬
	m_environment = new Environment(m_player, ENVIRONMENT_TYPE::MORNING);
	//m_environment = new Environment(player, ENVIRONMENT_TYPE::NIGHT);

	Mesh* meshTer = RENDERER->GetMesh("Data/Meshes/Objects/Environment/Terrain/SM_Field.gpmesh");

	Mesh* meshStorage = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/Storage/storageBuilding.OBJ");
	Mesh* meshGarage = RENDERER->GetMesh("Data/Meshes/Objects/Buildings/Garage/garage_Internal.OBJ");


	LevelBlock* spaceTer = new LevelBlock();
	spaceTer->SetMesh(meshTer);
	spaceTer->SetScale(10.0f);
	spaceTer->SetMeshIntensity(3.0f);
	spaceTer->SetPosition(Vector3(0.0f, 0.0f, 12.0f));

	LevelBlock* spaceStor = new LevelBlock();
	spaceStor->SetMesh(meshStorage);
	//spaceStor->SetScale(70.0f);
	spaceStor->SetMeshIntensity(3.0f);
	spaceStor->SetPosition(Vector3(0.0f, 3000.0f, 12.0f));

	LevelBlock* spaceStor2 = new LevelBlock();
	spaceStor2->SetMesh(meshStorage);
	spaceStor2->SetMeshIntensity(3.0f);
	spaceStor2->SetPosition(Vector3(-1917.0f, -260.0f, 12.0f));

	LevelBlock* spaceGarage = new LevelBlock();
	spaceGarage->SetMesh(meshGarage);
	spaceGarage->SetScale(50.0f);
	spaceGarage->SetMeshIntensity(3.0f);
	spaceGarage->SetPosition(Vector3(0.0f, -900.0f, 12.0f));

	LevelBlock* spaceGarage2 = new LevelBlock();
	spaceGarage2->SetMesh(meshGarage);
	spaceGarage2->SetScale(50.0f);
	spaceGarage2->SetMeshIntensity(3.0f);
	spaceGarage2->SetPosition(Vector3(2500.0f, -900.0f, 12.0f));



	LevelBlock* spaceGarage3 = new LevelBlock();
	spaceGarage3->SetMesh(meshGarage);
	spaceGarage3->SetScale(50.0f);
	spaceGarage3->SetMeshIntensity(3.0f);
	spaceGarage3->SetPosition(Vector3(0.0f, 900.0f, 12.0f));
	spaceGarage3->SetRotation(Quaternion::Quaternion(Vector3::UnitZ, Math::ToRadians(180.0f)));

	LevelBlock* spaceGarage4 = new LevelBlock();
	spaceGarage4->SetMesh(meshGarage);
	spaceGarage4->SetScale(50.0f);
	spaceGarage4->SetMeshIntensity(3.0f);
	spaceGarage4->SetPosition(Vector3(2500.0f, 900.0f, 12.0f));
	spaceGarage4->SetRotation(Quaternion::Quaternion(Vector3::UnitZ, Math::ToRadians(180.0f)));

	// ƒ]ƒ“ƒr(“G)
	EnemyZombie* zombie = new EnemyZombie();
	zombie->SetPosition(Vector3(380.0f, 0.0f, 20.0f));
	zombie->SetScale(0.8f);
	zombie->SetTarget(m_player);

	EnemyZombie* zombie1 = new EnemyZombie();
	zombie1->SetPosition(Vector3(680.0f, 0.0f, 20.0f));
	zombie1->SetScale(0.8f);
	zombie1->SetTarget(m_player);

	EnemyZombie* zombie2 = new EnemyZombie();
	zombie2->SetPosition(Vector3(520.0f, 220.0f, 20.0f));
	zombie2->SetScale(0.8f);
	zombie2->SetTarget(m_player);

	EnemyZombie* zombie3 = new EnemyZombie();
	zombie3->SetPosition(Vector3(820.0f, -220.0f, 20.0f));
	zombie3->SetScale(0.8f);
	zombie3->SetTarget(m_player);

    return true;
}

void WorldGameSceneTPL::Update(float in_deltaTime)
{

}

void WorldGameSceneTPL::UpdateWorld(float _deltaTime)
{
}

void WorldGameSceneTPL::ImGuiDebugWorld()
{
}
