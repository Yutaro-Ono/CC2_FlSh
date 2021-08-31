#include "PauseScreen.h"
#include "GameMain.h"
#include "Texture.h"
#include "Font.h"

// コンストラクタ
PauseScreen::PauseScreen()
	:m_pause(MODE_OFF)
{
	m_pauseTex = m_font->RenderText("PAUSE", Vector3(1.0f, 1.0f, 1.0f), 64);

	m_backGroundTex = RENDERER->GetTexture("Data/Interface/System/Pause/PauseScreen_bg.png");
}

// デストラクタ
PauseScreen::~PauseScreen()
{
}

void PauseScreen::Update(float _deltaTime)
{
}

// 描画処理
void PauseScreen::Draw(GLSLprogram * _shader)
{
	if (m_pause == MODE_ON)
	{
		// 背景
		DrawTexture(_shader, m_backGroundTex, Vector2(0.0f, 0.0f), 1.0f);

		// "Pause"
		DrawTexture(_shader, m_pauseTex, Vector2(0.0f, 0.0f), 1.0f);

	}

}

