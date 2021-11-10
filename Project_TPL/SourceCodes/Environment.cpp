#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "WorldGameScene.h"
#include "PlayerManager.h"
#include "RenderBloom.h"
#include "DirectionalLight.h"

static Vector3 playerPos = Vector3::Zero;
//static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);
static const Vector3 ADJUST_POS = Vector3(-600.0f, 50.0f, 700.0f);


Environment::Environment(Actor* _target, ENVIRONMENT_TYPE _envType)
	:m_envType(_envType)
	,m_lightTarget(_target)
{
	// スカイボックス生成
	m_skyBox = new SkyBox(this, m_envType);

	playerPos = m_lightTarget->GetPosition();
	
	// 設定された時間帯ごとにスカイボックスをセット
	SetSkyBox(m_skyBox);

	// 環境光セット
	SetDirectionalLight(_envType, m_lightTarget->GetPosition());

}

Environment::~Environment()
{

}

void Environment::Update()
{

	// 毎フレームのプレイヤー座標
	Vector3 framePlayerPos = m_lightTarget->GetPosition();

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

void Environment::SetSkyBox(class SkyBox* _skybox)
{
	RENDERER->SetActiveSkyBox(m_skyBox->GetCubeMapComp());
	m_skyBox->GetCubeMapComp()->SetIsVisible(true);
}

// 時間帯ごとに平行ライトを設定する
void Environment::SetDirectionalLight(ENVIRONMENT_TYPE _envType, const Vector3& _pos)
{
	m_envType = _envType;

	if (m_envType == ENVIRONMENT_TYPE::MORNING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.5f, 0.5f, 0.52f));

		// ディレクショナルライトの調整
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.741f, 0.735f, 0.735f));
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.872f, 0.725f, 0.751f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.901f, 0.741f, 0.781f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.3f);
		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::AFTERNOON)
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
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::EVENING)
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
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::NIGHT)
	{
		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	if (m_envType == ENVIRONMENT_TYPE::NIGHT_CITY)
	{
		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}


	if (m_envType == ENVIRONMENT_TYPE::SPACE)
	{
		// ディレクショナルライトの調整
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		RENDERER->GetDirectionalLight()->SetPosition(playerPos + ADJUST_POS);
		RENDERER->GetDirectionalLight()->SetTarget(playerPos);
		RENDERER->GetDirectionalLight()->SetDiffuse(Vector3(0.1f, 0.485f, 0.46f));
		RENDERER->GetDirectionalLight()->SetSpecular(Vector3(0.1f, 0.35f, 0.4f));
		RENDERER->GetDirectionalLight()->SetAmbient(Vector3(0.31f, 0.415f, 0.46f));
		RENDERER->GetDirectionalLight()->SetIntensity(0.8f);

		// ブルームの設定
		RENDERER->GetBloom()->SetGamma(3.69f);
		RENDERER->GetBloom()->SetExposureVal(0.13f);
	}

	// スカイボックスの有効化
	m_skyBox->GetCubeMapComp()->SetIsVisible(true);

}

// ディレクショナルライトの光源と向きを再計算
void Environment::SetDirectionalLightPos(const Vector3& _pos)
{
	// プレイヤーの位置に応じて平行ライトの座標・向きを最適化
	RENDERER->GetDirectionalLight()->SetPosition(Vector3(_pos.x + (int)ADJUST_POS.x, 
		                                                 _pos.y + (int)ADJUST_POS.y, 
		                                                 _pos.z + (int)ADJUST_POS.z));

	RENDERER->GetDirectionalLight()->SetTarget(_pos);
}
