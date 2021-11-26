#include "LoadScreen.h"
#include "Texture.h"
#include "Font.h"
#include "GameMain.h"
#include "AudioManager.h"
#include <Windows.h>
#include <sstream>

const int LoadScreen::GAUGE_ALL_NUM = 18;

// コンストラクタ
LoadScreen::LoadScreen()
	:m_state(DISABLE)
	,m_gaugeNum(0)
	,m_isGame(false)
	,m_loadGaugeScale(0.25f)
{

	// "Loading"生成
	m_loading = m_font->RenderText("Loading", Vector3(1.0f, 1.0f, 1.0f), 64);

	// ロード用ゲージ
	for (int i = 0; i < GAUGE_ALL_NUM; i++)
	{
		std::stringstream ssGauge;
		// ファイルパス
		ssGauge << "Data/Interface/TPL/LoadScreen/LoadScreen_Gauge_" << i << ".png";
		m_loadGauges.push_back(RENDERER->GetTexture(ssGauge.str()));
	}

	// 背景
	m_bgTexture = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_BackGround.png");

	// チュートリアルメッセージ
	// m_tutorialMsg = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_tuto_1.png");

	// ロード時の効果音をロード
	m_sound["Load"] = "Data/Music/SE/System/Load/decision-6 (mp3cut.net).wav";
	AUDIO->GetSound(m_sound["Load"]);
	AUDIO->SetSoundVolume(m_sound["Load"], 2);

	// 座標
	m_loadingPos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 2 + m_loading->GetHeight() + 240.0f);
	m_loadGaugePos = Vector2(0.0f,
		                     -RENDERER->GetScreenHeight() / 2 + m_loadGauges[0]->GetHalfWidth() * m_loadGaugeScale);
}

// デストラクタ
LoadScreen::~LoadScreen()
{
	// 処理なし
}

// 初期化処理
void LoadScreen::Initialize()
{
	m_gaugeNum = 1;
	m_state = ENABLE;
}

void LoadScreen::Update(float _deltaTime)
{
}

void LoadScreen::Draw(GLSLprogram * _shader)
{

	if (m_state == ENABLE)
	{
		// 背景
		if (m_bgTexture)
		{
			glDisable(GL_BLEND);
			DrawTexture(_shader, m_bgTexture, Vector2(0.0f, 0.0f), 1.0f);
			glEnable(GL_BLEND);
		}

		// 操作説明
		if (m_tutorialMsg && m_isGame == true)
		{
			DrawTexture(_shader, m_tutorialMsg, Vector2(0.0f, 0.0f), 1.0f);
		}

		if (m_loading)
		{
			DrawTexture(_shader, m_loading, m_loadingPos, 1.0f);
		}

		// ロードゲージの描画
		for (int i = 0; i < m_gaugeNum; i++)
		{
			DrawTexture(_shader, m_loadGauges[i], m_loadGaugePos, m_loadGaugeScale);
		}
	}

}

void LoadScreen::EnableScreen()
{
	if (m_state == LOAD_STATE::ENABLE)
	{
		return;
	}

	m_state = LOAD_STATE::ENABLE;
	RENDERER->Draw();
}

void LoadScreen::DisableScreen()
{
	Sleep(150);
	m_state = LOAD_STATE::DISABLE;
}


void LoadScreen::SetIsGame()
{
	m_isGame = true;
	RENDERER->Draw();
}

void LoadScreen::AddGauge()
{
	if (m_gaugeNum < GAUGE_ALL_NUM)
	{
		// 効果音再生
		AUDIO->PlaySoundTND(m_sound["Load"]);

		// ゲージ数を増やす
		m_gaugeNum++;
	}
	else
	{
		return;
	}

	// 描画
	RENDERER->Draw();

	// ウェイト
	Sleep(80);
}
