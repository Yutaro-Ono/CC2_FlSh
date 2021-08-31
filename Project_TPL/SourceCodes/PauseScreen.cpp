#include "PauseScreen.h"
#include "GameMain.h"
#include "Texture.h"
#include "Font.h"

// �R���X�g���N�^
PauseScreen::PauseScreen()
	:m_pause(MODE_OFF)
{
	m_pauseTex = m_font->RenderText("PAUSE", Vector3(1.0f, 1.0f, 1.0f), 64);

	m_backGroundTex = RENDERER->GetTexture("Data/Interface/System/Pause/PauseScreen_bg.png");
}

// �f�X�g���N�^
PauseScreen::~PauseScreen()
{
}

void PauseScreen::Update(float _deltaTime)
{
}

// �`�揈��
void PauseScreen::Draw(GLSLprogram * _shader)
{
	if (m_pause == MODE_ON)
	{
		// �w�i
		DrawTexture(_shader, m_backGroundTex, Vector2(0.0f, 0.0f), 1.0f);

		// "Pause"
		DrawTexture(_shader, m_pauseTex, Vector2(0.0f, 0.0f), 1.0f);

	}

}

