#pragma once
#include "Component.h"
#include "Math.h"

class LightGlassComponent : public Component
{
public:

	LightGlassComponent(class Actor* _owner);
	~LightGlassComponent();

	// ���b�V���̃Z�b�g
	void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }

	// �`�揈��
	void Draw(class GLSLprogram* _shader);


	//----------------------------------------------+
	// Setter / Getter
	//----------------------------------------------+
	void SetLightColor(const Vector3& _color) { m_lightColor = _color; }         // ���C�g�J���[�̃Z�b�g
	void SetLightLuminance(float _luminance) { m_luminance = _luminance; }       // ���C�g�P�x�̃Z�b�g
	void SetVisible(bool _visible) { m_isVisible = _visible; }

private:

	class Mesh* m_mesh;

	Vector3 m_lightColor;           // ���C�g�J���[
	float m_luminance;              // ���C�g�̋P�x

	bool m_isVisible;               // �`�悷�邩���Ȃ���


};