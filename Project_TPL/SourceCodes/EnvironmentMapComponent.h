//------------------------------------------------------------------+
// ���}�b�v�I�u�W�F�N�g�R���|�[�l���g
//------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <vector>

class EnvironmentMapComponent : public Component
{

public:

	EnvironmentMapComponent(class Actor* _owner);     // �R���X�g���N�^
	~EnvironmentMapComponent();                         // �f�X�g���N�^

	void SetMesh(class Mesh* _mesh) { m_mesh = _mesh; }

	// ���}�b�v�I�u�W�F�N�g�̕`�揈��
	void DrawEnvironmentMap(class GLSLprogram* _envShader);

	void SetLuminance(float _lum) { m_luminance = _lum; }
	void SetAlpha(float _alpha) { m_alpha = _alpha; }

private:

	bool m_isVisible;

	float m_luminance;       // �P�x
	float m_alpha;           // ���ߓx

	class Mesh* m_mesh;

};