#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "GLSLprogram.h"
#include "Math.h"
#include "SkyBox.h"
#include "VertexArray.h"
#include <vector>


CubeMapComponent::CubeMapComponent(Actor* _owner)
	:Component(_owner)
	,m_texture(nullptr)
	,m_luminance(1.0f)
	,m_isVisible(false)
{
}

CubeMapComponent::~CubeMapComponent()
{
	delete m_texture;
}

// テクスチャの生成
void CubeMapComponent::CreateTexture(const std::string& _filePath)
{
	m_texture = new Texture();
	m_texture->LoadCubeMap(_filePath);
}

// キューブマップの描画処理
void CubeMapComponent::Draw(GLSLprogram* _shader)
{
	// 透明にしていなければ
	if (m_isVisible)
	{
		// 深度設定
		glDepthFunc(GL_LEQUAL);
		// 輝度情報をシェーダに渡す
		_shader->SetUniform("u_skyLuminance", m_luminance);
		// テクスチャバインド
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());
		// キューブマップ用頂点配列のアクティブ化
		RENDERER->GetCubeMapVerts()->SetActive();
		// 描画
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 念のためバインド解除
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

}

