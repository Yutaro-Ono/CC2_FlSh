//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       ディレクショナルライトクラス
// @note        レンダラーに登録することで、描画時にライティングが反映される
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Math.h"



class DirectionalLight
{

public:

	DirectionalLight();
	~DirectionalLight();

	void ReAdjustmentDirection();

	// Setter/Getter
	void SetTarget(const Vector3& _targetPos) { m_targetPos = _targetPos; m_readjustDir = true; }
	void SetLightDirection(const Vector3& _lightDir) { m_direction = _lightDir; }
	void SetDiffuse(const Vector3& _color) { m_diffuse = _color; }
	void SetSpecular(const Vector3& _color) { m_specular = _color; }
	void SetAmbient(const Vector3& _color) { m_ambient = _color; }
	void SetIntensity(const float& _intens) { m_intensity = _intens; }

	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetTargetPos() const { return m_targetPos; }
	const Vector3& GetDirection() const { return m_direction; }
	const Vector3& GetDiffuse() const { return m_diffuse; }
	const Vector3& GetSpecular() const { return m_specular; }
	const Vector3& GetAmbient() const { return m_ambient; }
	const float& GetIntensity() const { return m_intensity; }

private:

	Vector3 m_position;           // 光源位置
	Vector3 m_targetPos;             // ターゲットする位置
	Vector3 m_direction;          // 光の向き

	Vector3 m_diffuse;            // ディフューズ
	Vector3 m_specular;           // スペキュラ
	Vector3 m_ambient;            // アンビエント
	float m_intensity;            // 光の放出するエネルギー量

	bool m_readjustDir;           // 光の方向の再調整フラグ

	// デバッグ用
	class DirectionalLightDebug* m_debugObj;

	// friend関係
	friend class DirectionalLightDebug;
	friend class Renderer;

};
