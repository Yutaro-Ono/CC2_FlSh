#include "CanvasTitleSceneTPL.h"
#include "WorldTitleSceneTPL.h"
#include "TitleSceneTPL.h"
#include "GameMain.h"
#include "TitleUIScreenTPL.h"

CanvasTitleSceneTPL::CanvasTitleSceneTPL(WorldTitleSceneTPL* _world, TitleSceneTPL* _titleScene)
    :Canvas(_world)
{
}

CanvasTitleSceneTPL::~CanvasTitleSceneTPL()
{
}

bool CanvasTitleSceneTPL::Load()
{

    m_interfaces.push_back(new TitleUIScreenTPL());

    return true;
}

void CanvasTitleSceneTPL::Update(float _deltaTime)
{
    // UI‚ÌXVˆ—
    for (auto ui : m_interfaces)
    {
        ui->Update(_deltaTime);
    }
}
