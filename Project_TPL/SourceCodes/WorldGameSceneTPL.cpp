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


	LevelBlock* spaceTer = new LevelBlock();
	spaceTer->SetMesh(meshTer);
	spaceTer->SetScale(100.0f);
	spaceTer->SetMeshIntensity(3.0f);
	spaceTer->SetPosition(Vector3(0.0f, 0.0f, 12.0f));

	// ƒ]ƒ“ƒr(“G)
	EnemyZombie* zombie = new EnemyZombie();
	zombie->SetPosition(Vector3(380.0f, 0.0f, 20.0f));
	zombie->SetScale(0.8f);
	zombie->SetTarget(m_player);

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
