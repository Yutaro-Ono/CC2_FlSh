//-----------------------------------------------------------------------+
// HUDクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "HUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "Shader.h"
#include "WorldGameScene.h"
#include "PlayerManager.h"
#include "Helicopter.h"
#include <sstream>

// コンストラクタ
HUD::HUD(WorldGameScene* _world)
	:m_world(_world)
	,m_dangerCall(nullptr)
	,m_isDanger(false)
	,m_isImpact(false)
	,m_dispAccel(0.0f)
	,m_speedTexPos(600.0f, 340.0f)
{

	// DANGERフォント生成
	m_dangerCall = m_font->RenderText("DANGER", Vector3(1.0f, 0.0f, 0.0f), 48);

	// 集中線
	for (int i = 0; i < 2; i++)
	{
		std::stringstream ssImpact;
		ssImpact << "Data/Interface/HUD/Impact/Impact_" << (i + 1) << ".png";

		m_impactTexture[i] = RENDERER->GetTexture(ssImpact.str());
	}

}

// デストラクタ
HUD::~HUD()
{
	// 集中線スタックの解放
	for (int i = 0; i < 2; i++)
	{
		m_impactTexture[i]->Delete();
	}

}

void HUD::Update(float _deltaTime)
{

}

// 描画処理
void HUD::Draw(GLSLprogram * _shader)
{

	// ヘリに見つかった時にDANGER表示
	if (m_dangerCall && m_isDanger)
	{
		// 描画
		DrawTexture(_shader, m_dangerCall);
	}

	// 集中線の描画
	if (m_impactTexture && m_isImpact == true)
	{

		int pic = rand();

		if (pic % 2 == 0)
		{
			pic = 1;
		}
		else
		{
			pic = 0;
		}

		// 描画
		DrawTexture(_shader, m_impactTexture[pic], Vector2::Zero, 1.0f);
	}

}
