#include "CanvasGameScene.h"
#include "WorldGameScene.h"
#include "ScoreUI.h"


CanvasGameScene::CanvasGameScene(WorldGameScene* _world)
	:Canvas(_world)
	,m_gameWorld(_world)
	,m_scoreUI(nullptr)
{
}

CanvasGameScene::~CanvasGameScene()
{
}

bool CanvasGameScene::Load()
{
	m_scoreUI = new ScoreUI(this);

	return true;
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="_deltaTime"> デルタタイム </param>
void CanvasGameScene::Update(float _deltaTime)
{
}
