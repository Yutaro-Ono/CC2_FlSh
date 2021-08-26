//-----------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "UIScreen.h"
#include "Texture.h"
#include "GLSLprogram.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "Font.h"

// �R���X�g���N�^
UIScreen::UIScreen()
	:m_state(ACTIVE)
{

	// �Q�[���N���X��UI�X�^�b�N�Ɏ��g��ǉ�
	GAME_INSTANCE.AddUI(this);

	// �t�H���g�̃��[�h
	m_font = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath());

	// �X�N���[���T�C�Y�i�[
	SCREEN_W = GAME_CONFIG->GetScreenWidth();
	SCREEN_H = GAME_CONFIG->GetScreenHeight();
	SCREEN_HALF_W = GAME_CONFIG->GetScreenWidth() / 2;
	SCREEN_HALF_H = GAME_CONFIG->GetScreenHeight() / 2;
}


// �f�X�g���N�^
UIScreen::~UIScreen()
{

}

void UIScreen::Update(float _deltaTime)
{
}

void UIScreen::Draw(GLSLprogram * _shader)
{

}

void UIScreen::ProcessInput()
{
}

void UIScreen::HandleKeyPress(int _key)
{
}

// UI����� (���̌�GameMain���ŃX�^�b�N����폜�����)
void UIScreen::Close()
{
	m_state = CLOSE;
}

// �^�C�g���̃Z�b�g
// �w�肵����������w�肵���J���[�ŁA�w�肵���T�C�Y�ɍ쐬����
void UIScreen::SetTitle(const std::string & _text, const Vector3 & _color, int _pointSize)
{
	
	if (m_texture)
	{
		m_texture->Delete();
		delete m_texture;
		m_texture = nullptr;
	}
	m_texture = m_font->RenderText(_text, _color, _pointSize);
}


// �w�肵���e�N�X�`������ʏ�̃I�t�Z�b�g�ɕ`�悷��
// ���[���h�s����쐬���A�V�F�[�_�֑��M
void UIScreen::DrawTexture(GLSLprogram * _shader, Texture * _texture, const Vector2 & _offset, float _scale)
{
	// �e�N�X�`���̏c���T�C�Y�ɃX�P�[�����|�����킹���l���X�P�[���s��Ƃ��Ē�`
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(_texture->GetWidth()) * _scale,
		static_cast<float>(_texture->GetHeight()) * _scale,
		1.0f);
	// �w�肵����ʈʒu�ւ̃X�N���[���ϊ��s����쐬
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(_offset.x, _offset.y, 0.0f));
	// �X�P�[���ƕϊ��s������[���h�s��֕ϊ�
	Matrix4 world = scaleMat * transMat;
	
	// �V�F�[�_�Ƀ��[���h�ϊ��s��𑗐M
	_shader->SetUniform("u_worldTransform", world);
	_shader->SetUniform("u_texture", 0);
	//in_texture->SetActive();

	// �e�N�X�`�����A�N�e�B�u��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->GetTextureID());

	// �`�悷��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool _relative)
{
}
