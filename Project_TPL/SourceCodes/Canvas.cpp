#include "Canvas.h"
#include "UIScreen.h"
#include "WorldBase.h"
#include "PlayerManager.h"
#include "PlayerControlUI.h"
#include "ScoreUI.h"

// コンストラクタ
Canvas::Canvas(WorldBase* in_world)
	:m_world(in_world)
{
}

// デストラクタ
Canvas::~Canvas()
{
	// 全てのUIを終了
	for (auto ui : m_interfaces)
	{
		ui->Close();
	}
	m_interfaces.clear();
}

/// <summary>
/// インターフェースの更新処理
/// </summary>
/// <param name="_deltaTime"> デルタタイム </param>
void Canvas::Update(float _deltaTime)
{
	for (auto ui : m_interfaces)
	{
		ui->Update(_deltaTime);
	}
}
