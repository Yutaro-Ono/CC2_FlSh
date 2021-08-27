#include "LightGlassComponent.h"
#include "Mesh.h"
#include "GLSLprogram.h"
#include "VertexArray.h"
#include "Actor.h"
#include "Renderer.h"

LightGlassComponent::LightGlassComponent(Actor* _owner)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_lightColor(Vector3(0.5f, 0.5f, 0.5f))
	,m_luminance(1.0f)
	,m_isVisible(false)
{
	RENDERER->AddLightGlassComponent(this);
}

LightGlassComponent::~LightGlassComponent()
{
	RENDERER->RemoveLightGlassComponent(this);
}

void LightGlassComponent::Draw(GLSLprogram* _shader)
{

	if (!m_isVisible)
	{
		// GLのアルファブレンド・αテストをON
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// uniformへのセット
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());
		_shader->SetUniform("u_lightColor", m_lightColor);
		_shader->SetUniform("u_luminance", m_luminance);

		// 頂点配列のバインド
		VertexArray* vao = m_mesh->GetVertexArray();
		vao->SetActive();

		// 描画
		glDrawElements(GL_TRIANGLES, vao->GetNumIndices(), GL_UNSIGNED_INT, nullptr);

		// アルファテストのオフ
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

}

