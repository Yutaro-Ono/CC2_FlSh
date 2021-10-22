#include "PlayerHUD.h"
#include "GameMain.h"
#include "Texture.h"
const int PlayerHUD::HP_GAUGE_NUM = 7;

PlayerHUD::PlayerHUD(Player* _player)
	:m_player(_player)
	,m_hpFrame(nullptr)
	,m_rightLine(nullptr)
	,m_tutorialPanel(nullptr)
	,m_stealthGauge(nullptr)
	,m_stealthGaugeBack(nullptr)
{
	Initialize();
}

PlayerHUD::~PlayerHUD()
{
	m_hpGauges.clear();
}

void PlayerHUD::Update(float in_deltaTime)
{
}

void PlayerHUD::Draw(GLSLprogram* _shader)
{

	float hpScale = 0.4f;
	Vector2 adjustHp = Vector2(30.0f, 110.0f);

	// HPゲージの描画
	DrawTexture(_shader, m_hpFrame, 
		        Vector2(-RENDERER->GetScreenWidth() / 2 + m_hpFrame->GetHalfWidth() * hpScale, -RENDERER->GetScreenHeight() / 2) + adjustHp, 
		        hpScale);

	for (auto hpGauge : m_hpGauges)
	{
		DrawTexture(_shader, hpGauge, 
			        Vector2(-RENDERER->GetScreenWidth() / 2 + m_hpFrame->GetHalfWidth() * hpScale, -RENDERER->GetScreenHeight() / 2) + adjustHp,
			        hpScale);
	}

}

/// <summary>
/// 初期化処理
/// コンストラクタで呼び出し
/// </summary>
void PlayerHUD::Initialize()
{
	// HPゲージの生成(7個)
	m_hpGauges.resize(HP_GAUGE_NUM);
	for (int i = 0; i < HP_GAUGE_NUM; i++)
	{
		m_hpGauges[i] = nullptr;
		std::string hpGaugePath = "Data/Interface/TPL/HUD/HP_Gauge/HP_Gauge_" + std::to_string(i) + ".png";

		m_hpGauges[i] = RENDERER->GetTexture(hpGaugePath);
	}

	// HP背景フレーム
	m_hpFrame = RENDERER->GetTexture("Data/Interface/TPL/HUD/HP_Gauge/HP_Gauge_Back.png");

	// タスク表示用ライン
	m_rightLine = RENDERER->GetTexture("Data/Interface/TPL/HUD/Menu_Line.png");

	// 操作説明パネル
	m_tutorialPanel = RENDERER->GetTexture("Data/Interface/TPL/HUD/Tutorial_Panel.png");

	// ステルスゲージ関連
	m_stealthGauge = RENDERER->GetTexture("Data/Interface/TPL/HUD/Stealth/Stealth_Gauge.png");
	m_stealthGaugeBack = RENDERER->GetTexture("Data/Interface/TPL/HUD/Stealth/Stealth_Gauge_Back.png");
}
