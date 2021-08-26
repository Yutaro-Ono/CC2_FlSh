#include "PointLightComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GLSLprogram.h"
#include "Mesh.h"

PointLightComponent::PointLightComponent(PointLight* _light)
	:Component(_light)
	,m_light(_light)
	,m_mesh(nullptr)
	,m_constant(1.0f)
{
	// �����_���[�ɂ��̃|�C���g���C�g��ǉ�
	RENDERER->AddPointLightComponent(this);

	// ���C�g�p�����[�^�̃Z�b�g
	SetLightParam(m_light->GetLightVolume());

	// ���b�V���̃��[�h
	m_mesh = RENDERER->GetMesh("Data/Meshes/TND/Light/PointLight/PointLight_Sphere.OBJ");
	
}

PointLightComponent::~PointLightComponent()
{
	RENDERER->RemovePointLightComponent(this);
}


// ���C�g�̕`��
void PointLightComponent::Draw(GLSLprogram* _shader)
{
	// �|�C���g���C�g�p�����[�^���Z�b�g
	_shader->SetUniform("u_pl.position", m_light->GetPosition());
	_shader->SetUniform("u_pl.constant", m_constant);
	_shader->SetUniform("u_pl.linear", m_linear);
	_shader->SetUniform("u_pl.quadratic", m_quadratic);
	// �J���[�̃Z�b�g
	_shader->SetUniform("u_pl.diffuse", m_light->GetDiffuseColor());
	_shader->SetUniform("u_pl.ambient", m_light->GetAmbientColor());
	_shader->SetUniform("u_pl.specular", m_light->GetSpecularColor());
	_shader->SetUniform("u_pl.luminance", m_light->GetLuminance());

	// ���W�̃Z�b�g
	_shader->SetUniform("u_worldTransform", m_light->GetWorldTransform());

	// ���_�z����A�N�e�B�u��
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// �`�悷��
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, 0);
}

/// <summary>
/// ���C�g�p�����[�^�̃Z�b�g�E�{�����[���^�C�v�ɉ������l���Z�b�g
/// </summary>
/// <param name="in_vol"> �|�C���g���C�g�A�N�^�̃{�����[���^�C�v </param>
void PointLightComponent::SetLightParam(PointLight::LIGHT_VOLUME _vol)
{
	// ���T�C�Y
	if (_vol == PointLight::LIGHT_VOLUME::VL_SMALL)
	{
		m_radius = 325.0f;
		m_linear = 0.014;
		m_quadratic = 0.0007;
	}
	else if (_vol == PointLight::LIGHT_VOLUME::VL_VERY_SMALL)
	{
		m_radius = 50.0f;
		m_linear = 0.09;
		m_quadratic = 0.032;
	}
	// ���T�C�Y
	else if (_vol == PointLight::LIGHT_VOLUME::VL_MEDIUM)
	{
		m_radius = 600.0f;
		m_linear = 0.007;
		m_quadratic = 0.0002;
	}
	// ��T�C�Y
	else if (_vol == PointLight::LIGHT_VOLUME::VL_BIG)
	{
		m_radius = 3250.0f;
		m_linear = 0.0014;
		m_quadratic = 0.000007;
	}

	// ���C�g�ɃX�P�[���Ƃ��Ĕ��a�T�C�Y��K�p����
	m_light->SetScale(m_radius);
}