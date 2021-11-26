#pragma once
#include "Renderer.h"

// 環境タイプ
enum class ENVIRONMENT_TYPE : unsigned char
{
	MORNING,
	AFTERNOON,
	EVENING,
	NIGHT,
	NIGHT_CITY,
	SPACE,
	TITLE
};

class Environment
{
public:

	Environment(class Actor* _target, ENVIRONMENT_TYPE _envtype);                                // コンストラクタ2
	~Environment();                         // デストラクタ

	void Update();

	void SetSkyBox(class SkyBox* _skybox);
	void SetDirectionalLight(ENVIRONMENT_TYPE _envType, const Vector3& _pos);             // 時間帯ごとに平行ライトを設定する

	void SetDirectionalLightPos(const Vector3& _pos);

	// 環境タイプのゲッター・セッター
	ENVIRONMENT_TYPE GetEnvironmentType() { return m_envType; }
	void SetEnvironmentType(ENVIRONMENT_TYPE _envType) { m_envType = _envType; }
	// スカイボックスクラスのゲッター
	class SkyBox* GetSkyBox() { return m_skyBox; }

private:


	ENVIRONMENT_TYPE m_envType;

	class Actor* m_lightTarget;

	DirectionalLight* m_dirLight;

	// スカイボックス
	class SkyBox* m_skyBox;
};