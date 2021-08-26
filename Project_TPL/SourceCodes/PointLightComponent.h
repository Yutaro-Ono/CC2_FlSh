#pragma once
#include "Component.h"
#include "PointLight.h"


class PointLightComponent : public Component
{

public:

	PointLightComponent(class PointLight* _light);
	~PointLightComponent();

	void SetLightParam(PointLight::LIGHT_VOLUME _vol);    // ���C�g�p�����[�^�̃Z�b�g

	void Draw(class GLSLprogram* _shader);


private:

	class PointLight* m_light;      // �|�C���g���C�g�A�N�^

	class Mesh* m_mesh;             // ���C�g���b�V�� (����)

	float m_radius;                 // ���C�g�̉e�����a

	// ���C�g�����p�����[�^
	float m_constant;
	float m_linear;
	float m_quadratic;


};