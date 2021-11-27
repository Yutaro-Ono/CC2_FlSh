#include "CanvasGameSceneTPL.h"
#include "WorldGameSceneTPL.h"
#include "Player.h"
#include "PlayerHUD.h"
#include "MiniMapHUD.h"
#include "GameMain.h"

CanvasGameSceneTPL::CanvasGameSceneTPL(WorldGameSceneTPL* _world, GameSceneTPL* _scene)
	:Canvas(_world)
	,m_world(_world)
	,m_scene(_scene)
{
}

CanvasGameSceneTPL::~CanvasGameSceneTPL()
{
	for (auto ui : m_interfaces)
	{
		GAME_INSTANCE.RemoveUI(ui);
	}
	m_interfaces.clear();
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
