//-----------------------------------------------------------------------+
// SpriteComponent�N���X
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


// �R���X�g���N�^
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


// �f�X�g���N�^
SpriteComponent::~SpriteComponent()
{

	GAME_INSTANCE.GetRenderer()->RemoveSprite(this);

}


// �`�揈��
void SpriteComponent::Draw(GLSLprogram * _shader)
{

	if (m_texture)
	{
		// �X�P�[���s��̍쐬
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(m_textureWidth),
			static_cast<float>(m_textureHeight),
			1.0f);
		// �X�P�[�� * ��ʏ�̍��W �s�������
		Matrix4 world = scaleMat * m_owner->GetWorldTransform();

		// WorldTransform���Z�b�g
		_shader->SetUniform("u_worldTransform", world);
		_shader->SetUniform("u_texture", 0);
		// �e�N�X�`�����A�N�e�B�u
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
		//RENDERER->SetActiveSpriteVAO();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}


// �e�N�X�`���̃Z�b�g
void SpriteComponent::SetTexture(Texture * _texture)
{

	m_texture = _texture;
	// �����Əc���̃Z�b�g
	m_textureWidth = _texture->GetWidth();
	m_textureHeight = _texture->GetHeight();

}
