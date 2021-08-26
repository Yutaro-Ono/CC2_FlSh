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
	// レンダラーにこのポイントライトを追加
	RENDERER->AddPointLightComponent(this);

	// ライトパラメータのセット
	SetLightParam(m_light->GetLightVolume());

	// メッシュのロード
	m_mesh = RENDERER->GetMesh("Data/Meshes/TND/Light/PointLight/PointLight_Sphere.OBJ");
	
}

PointLightComponent::~PointLightComponent()
{
	RENDERER->RemovePointLightComponent(this);
}


// ライトの描画
void PointLightComponent::Draw(GLSLprogram* _shader)
{
	// ポイントライトパラメータをセット
	_shader->SetUniform("u_pl.position", m_light->GetPosition());
	_shader->SetUniform("u_pl.constant", m_constant);
	_shader->SetUniform("u_pl.linear", m_linear);
	_shader->SetUniform("u_pl.quadratic", m_quadratic);
	// カラーのセット
	_shader->SetUniform("u_pl.diffuse", m_light->GetDiffuseColor());
	_shader->SetUniform("u_pl.ambient", m_light->GetAmbientColor());
	_shader->SetUniform("u_pl.specular", m_light->GetSpecularColor());
	_shader->SetUniform("u_pl.luminance", m_light->GetLuminance());

	// 座標のセット
	_shader->SetUniform("u_worldTransform", m_light->GetWorldTransform());

	// 頂点配列をアクティブに
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// 描画する
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, 0);
}

/// <summary>
/// ライトパラメータのセット・ボリュームタイプに応じた値をセット
/// </summary>
/// <param name="in_vol"> ポイントライトアクタのボリュームタイプ </param>
void PointLightComponent::SetLightParam(PointLight::LIGHT_VOLUME _vol)
{
	// 小サイズ
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
	// 中サイズ
	else if (_vol == PointLight::LIGHT_VOLUME::VL_MEDIUM)
	{
		m_radius = 600.0f;
		m_linear = 0.007;
		m_quadratic = 0.0002;
	}
	// 大サイズ
	else if (_vol == PointLight::LIGHT_VOLUME::VL_BIG)
	{
		m_radius = 3250.0f;
		m_linear = 0.0014;
		m_quadratic = 0.000007;
	}

	// ライトにスケールとして半径サイズを適用する
	m_light->SetScale(m_radius);
}