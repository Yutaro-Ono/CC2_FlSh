//------------------------------------------------------------------+
// 環境マップオブジェクトコンポーネント
//------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <vector>

class EnvironmentMapComponent : public Component
{

public:

	EnvironmentMapComponent(class Actor* _owner);     // コンストラクタ
	~EnvironmentMapComponent();                         // デストラクタ

	void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }

	// 環境マップオブジェクトの描画処理
	void DrawEnvironmentMap(class GLSLprogram* _envShader);

	void SetLuminance(float _lum) { m_luminance = _lum; }
	void SetAlpha(float _alpha) { m_alpha = _alpha; }

private:

	bool m_isVisible;

	float m_luminance;       // 輝度
	float m_alpha;           // 透過度

	class Mesh* m_mesh;

};