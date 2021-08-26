#include "DirectionalLight.h"
#include "DirectionalLightDebug.h"
#include "GameMain.h"
#include "Debugger.h"

DirectionalLight::DirectionalLight()
	:m_diffuse(Vector3(0.1f, 0.485f, 0.46f))
	,m_specular(Vector3(0.1f, 0.35f, 0.4f))
	,m_ambient(Vector3(0.31f, 0.415f, 0.46f))
	,m_position(0.0f, 5000.0f, 1000.0f)
	,m_targetPos(Vector3::Zero)
	,m_direction(m_targetPos - m_position)
	,m_intensity(0.5f)
	,m_readjustDir(false)
	,m_debugObj(nullptr)
{
	// デバッグオブジェクトの生成
#ifdef _DEBUG

	m_debugObj = new DirectionalLightDebug(this);
	DEBUGGER->AddDebugObject(m_debugObj, OBJECT_TAG::SYSTEM);

#endif
}

DirectionalLight::~DirectionalLight()
{
}

/// <summary>
/// ターゲット座標が更新された際
/// 光の方向を再調整する
/// </summary>
void DirectionalLight::ReAdjustmentDirection()
{
	m_direction = m_targetPos - m_position;
	m_direction.Normalize();

	m_readjustDir = false;
}
