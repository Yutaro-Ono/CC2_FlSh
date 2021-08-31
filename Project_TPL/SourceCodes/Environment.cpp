#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "RenderBloom.h"
#include "DirectionalLight.h"

static Vector3 playerPos = Vector3::Zero;
//static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);
static const Vector3 ADJUST_POS = Vector3(-600.0f, 50.0f, 700.0f);

Environment::Environment(GAME_TIME in_gameTime, const Vector3& in_pos)
	:m_gameTime(in_gameTime)
	, m_world(nullptr)
{
	// スカイボックス生成
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	//m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);
	// 設定された時間帯ごとにスカイボックスをセット
	SetSkyBox(m_gameTime);
	SetDirectionalLight(m_gameTime, in_pos);
}


Environment::Environment(GameWorld* in_world, GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
	,m_world(in_world)
{
	// スカイボックス生成
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	//m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);

	playerPos = m_world->GetPlayer()->GetPosition();
	
	// 設定された時間帯ごとにスカイボックスをセット
	SetSkyBox(m_gameTime);

	// 環境光セット
	SetDirectionalLight(in_gameTime, m_world->GetPlayer()->GetPosition());

}

Environment::~Environment()
{

}

void Environment::Update()
{

	// 毎フレームのプレイヤー座標
	Vector3 framePlayerPos = m_world->GetPlayer()->GetPosition();

	// 影のちらつきを抑えるため4000以上の移動で平行ライトの位置をプレイヤーにスナップする
	if (framePlayerPos.x >= playerPos.x + 2000 || framePlayerPos.x <= playerPos.x - 1000 ||
		framePlayerPos.y >= playerPos.y + 2000 || framePlayerPos.y <= playerPos.y - 2000)
	{
		// プレイヤーの位置に応じて平行ライトの座標・向きを最適化
		RENDERER->GetDirectionalLight()->SetPosition(Vector3((int)framePlayerPos.x + (int)ADJUST_POS.x, 
			                                                 (int)framePlayerPos.y + (int)ADJUST_POS.y, 
			                                                  0.0f + (int)ADJUST_POS.z));

		RENDERER->GetDirectionalLight()->SetTarget(framePlayerPos);

		playerPos = framePlayerPos;
	}

}

void Environment::SetSkyBox(GAME_TIME in_gameTime)
{
	if (in_gameTime == GAME_TIME::MORNING || in_gameTime == GAME_TIME::AFTERNOON)
	{
		// レンダラーに日中のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_morningBox->GetCubeMapComp());
		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::EVENING)
	{
		// レンダラーに夕方のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_eveningBox->GetCubeMapComp());
		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::NIGHT)
	{
		// レンダラーに夜のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_nightBox->GetCubeMapComp());
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}

// 時間帯ごとに平行ライトを設定する
void Environment::SetDirectionalLight(GAME_TIME in_gameTime, const Vector3& in_pos)
{
	m_gameTime = in_gameTime;

	if (m_gameTime == GAME_TIME::MORNING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.5f, 0.5f, 0.52f));

		// ディレクショナルライトの調整
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.5f, 0.5f, 0.51f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.4f, 0.5f, 0.5f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::AFTERNOON)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));

		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.7f, 0.7f, 0.75f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.4f, 0.4f, 0.45f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.5f, 0.5f, 0.6f));
		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::EVENING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));

		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.4f, 0.4f, 0.4f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.07f, 0.05f, 0.3f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.01f, 0.0f, 0.1f));
		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;

	}

	if (m_gameTime == GAME_TIME::NIGHT)
	{
		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(5.5f);
		// 夜用スカイボックスの有効化
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);

		return;
	}

}

// ディレクショナルライトの光源と向きを再計算
void Environment::SetDirectionalLightPos(const Vector3& in_pos)
{
	// プレイヤーの位置に応じて平行ライトの座標・向きを最適化
	RENDERER->GetDirectionalLight()->SetPosition(Vector3(in_pos.x + (int)ADJUST_POS.x, 
		                                                 in_pos.y + (int)ADJUST_POS.y, 
		                                                 in_pos.z + (int)ADJUST_POS.z));

	RENDERER->GetDirectionalLight()->SetTarget(in_pos);
}
