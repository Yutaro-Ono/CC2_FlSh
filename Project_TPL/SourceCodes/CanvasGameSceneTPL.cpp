#include "CanvasGameSceneTPL.h"
#include "WorldGameSceneTPL.h"
#include "Player.h"
#include "PlayerHUD.h"
#include "MiniMapHUD.h"

CanvasGameSceneTPL::CanvasGameSceneTPL(WorldGameSceneTPL* _world, GameSceneTPL* _scene)
	:Canvas(_world)
	,m_world(_world)
	,m_scene(_scene)
{
}

CanvasGameSceneTPL::~CanvasGameSceneTPL()
{
}

bool CanvasGameSceneTPL::Load()
{
	m_interfaces.push_back(new PlayerHUD(m_world->GetPlayer()));
	m_interfaces.push_back(new MiniMapHUD(m_world->GetPlayer()));

	return true;
}

void CanvasGameSceneTPL::Update(float _deltaTime)
{

}
