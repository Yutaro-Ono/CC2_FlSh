//-----------------------------------------------------------------------+
// インターフェース基底クラス
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

// コンストラクタ
UIScreen::UIScreen()
	:m_state(ACTIVE)
{

	// ゲームクラスのUIスタックに自身を追加
	GAME_INSTANCE.AddUI(this);

	// フォントのロード
	m_font = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath());

	// スクリーンサイズ格納
	SCREEN_W = GAME_CONFIG->GetScreenWidth();
	SCREEN_H = GAME_CONFIG->GetScreenHeight();
	SCREEN_HALF_W = GAME_CONFIG->GetScreenWidth() / 2;
	SCREEN_HALF_H = GAME_CONFIG->GetScreenHeight() / 2;
}


// デストラクタ
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

// UIを閉じる (この後GameMain側でスタックから削除される)
void UIScreen::Close()
{
	m_state = CLOSE;
}

// タイトルのセット
// 指定した文字列を指定したカラーで、指定したサイズに作成する
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


// 指定したテクスチャを画面上のオフセットに描画する
// ワールド行列を作成し、シェーダへ送信
void UIScreen::DrawTexture(GLSLprogram * _shader, Texture * _texture, const Vector2 & _offset, float _scale)
{
	// テクスチャの縦横サイズにスケールを掛け合わせた値をスケール行列として定義
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(_texture->GetWidth()) * _scale,
		static_cast<float>(_texture->GetHeight()) * _scale,
		1.0f);
	// 指定した画面位置へのスクリーン変換行列を作成
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(_offset.x, _offset.y, 0.0f));
	// スケールと変換行列をワールド行列へ変換
	Matrix4 world = scaleMat * transMat;
	
	// シェーダにワールド変換行列を送信
	_shader->SetUniform("u_worldTransform", world);
	_shader->SetUniform("u_texture", 0);
	//in_texture->SetActive();

	// テクスチャをアクティブ化
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->GetTextureID());

	// 描画する
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool _relative)
{
}
