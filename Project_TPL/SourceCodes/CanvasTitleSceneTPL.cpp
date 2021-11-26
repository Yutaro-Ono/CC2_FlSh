#include "CanvasTitleSceneTPL.h"
#include "WorldTitleSceneTPL.h"
#include "TitleSceneTPL.h"
#include "GameMain.h"
#include "TitleUIScreenTPL.h"

CanvasTitleSceneTPL::CanvasTitleSceneTPL(WorldTitleSceneTPL* _world, TitleSceneTPL* _titleScene)
    :Canvas(_world)
    ,m_titleScene(_titleScene)
{
}

CanvasTitleSceneTPL::~CanvasTitleSceneTPL()
{
}

bool CanvasTitleSceneTPL::Load()
{

    m_titleUI = new TitleUIScreenTPL(m_titleScene);

    return true;
}

void CanvasTitleSceneTPL::Update(float _deltaTime)
{
}

void CanvasTitleSceneTPL::EnterTitle()
{
    m_titleUI->EnterTitle();
}

void CanvasTitleSceneTPL::EnterSelect()
{
    m_titleUI->EnterSelect();
}

void CanvasTitleSceneTPL::BackTitle()
{
    m_titleUI->BackTitle();
}
