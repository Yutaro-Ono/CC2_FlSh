#pragma once
#include "Component.h"
#include "Math.h"

class LightGlassComponent : public Component
{
public:

	LightGlassComponent(class Actor* _owner);
	~LightGlassComponent();

	// メッシュのセット
	void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }

	// 描画処理
	void Draw(class GLSLprogram* _shader);


	//----------------------------------------------+
	// Setter / Getter
	//----------------------------------------------+
	void SetLightColor(const Vector3& _color) { m_lightColor = _color; }         // ライトカラーのセット
	void SetLightLuminance(float _luminance) { m_luminance = _luminance; }       // ライト輝度のセット
	void SetVisible(bool _visible) { m_isVisible = _visible; }

private:

	class Mesh* m_mesh;

	Vector3 m_lightColor;           // ライトカラー
	float m_luminance;              // ライトの輝度

	bool m_isVisible;               // 描画するかしないか


};