#include "CanvasTitleScene.h"
#include "WorldTitleScene.h"

CanvasTitleScene::CanvasTitleScene(WorldTitleScene* _world)
    :Canvas(_world)
{
}

CanvasTitleScene::~CanvasTitleScene()
{
}

bool CanvasTitleScene::Load()
{
    return false;
}

void CanvasTitleScene::Update(float _deltaTime)
{
}
