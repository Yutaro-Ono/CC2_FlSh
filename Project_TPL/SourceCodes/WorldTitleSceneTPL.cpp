#include "WorldTitleSceneTPL.h"
#include "CanvasTitleSceneTPL.h"

WorldTitleSceneTPL::WorldTitleSceneTPL(class TitleSceneTPL* _titleScene)
	:m_titleScene(_titleScene)
{
}

WorldTitleSceneTPL::~WorldTitleSceneTPL()
{
}

bool WorldTitleSceneTPL::Load()
{
	// UIƒLƒƒƒ“ƒoƒX‚Ì¶¬
	m_canvas = new CanvasTitleSceneTPL(this, m_titleScene);

	return true;
}

void WorldTitleSceneTPL::Update(float _deltaTime)
{
	m_canvas->Update(_deltaTime);
}

void WorldTitleSceneTPL::UpdateWorld(float _deltaTime)
{
}
