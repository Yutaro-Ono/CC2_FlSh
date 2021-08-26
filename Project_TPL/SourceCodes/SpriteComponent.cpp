//-----------------------------------------------------------------------+
// SpriteComponentクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "SpriteComponent.h"
#include "GameMain.h"
#include "Texture.h"
#include "GLSLprogram.h"
#include "Actor.h"
#include "Renderer.h"


// コンストラクタ
SpriteComponent::SpriteComponent(Actor * _owner, int _drawOrder)
	:Component(_owner)
	,m_texture(nullptr)
	,m_drawOrder(_drawOrder)
	,m_textureWidth(0)
	,m_textureHeight(0)
	,m_visible(true)
{

	GAME_INSTANCE.GetRenderer()->AddSprite(this);

}


// デストラクタ
SpriteComponent::~SpriteComponent()
{

	GAME_INSTANCE.GetRenderer()->RemoveSprite(this);

}


// 描画処理
void SpriteComponent::Draw(GLSLprogram * _shader)
{

	if (m_texture)
	{
		// スケール行列の作成
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(m_textureWidth),
			static_cast<float>(m_textureHeight),
			1.0f);
		// スケール * 画面上の座標 行列を合成
		Matrix4 world = scaleMat * m_owner->GetWorldTransform();

		// WorldTransformをセット
		_shader->SetUniform("u_worldTransform", world);
		_shader->SetUniform("u_texture", 0);
		// テクスチャをアクティブ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
		//RENDERER->SetActiveSpriteVAO();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}


// テクスチャのセット
void SpriteComponent::SetTexture(Texture * _texture)
{

	m_texture = _texture;
	// 横幅と縦幅のセット
	m_textureWidth = _texture->GetWidth();
	m_textureHeight = _texture->GetHeight();

}
