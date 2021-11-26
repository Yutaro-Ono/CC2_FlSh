#include "WorldTitleSceneTPL.h"
#include "CanvasTitleSceneTPL.h"
#include "Environment.h"
#include "EnemyZombie.h"

WorldTitleSceneTPL::WorldTitleSceneTPL(class TitleSceneTPL* _titleScene)
	:m_titleScene(_titleScene)
{
}

WorldTitleSceneTPL::~WorldTitleSceneTPL()
{
}

bool WorldTitleSceneTPL::Load()
{

	// ƒ]ƒ“ƒr(“G)
	EnemyZombie* zombie = new EnemyZombie();
	zombie->SetPosition(Vector3(380.0f, 0.0f, 20.0f));
	zombie->SetScale(0.8f);

	// ŠÂ‹«î•ñ‚Ì¶¬
	m_environment = new Environment(zombie, ENVIRONMENT_TYPE::TITLE);

	return true;
}

void WorldTitleSceneTPL::Update(float _deltaTime)
{
}

void WorldTitleSceneTPL::UpdateWorld(float _deltaTime)
{
}
