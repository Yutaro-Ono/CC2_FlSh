//-----------------------------------------------------------------------+
// テクスチャクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+

// インクルードファイル
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"

// コンストラクタ
Texture::Texture()
	:m_textureID(0)
	,m_width(0)
	,m_height(0)
{
	// 処理なし
}

// デストラクタ
Texture::~Texture()
{
	//RENDERER->RemoveTexture(this);
}

// テクスチャのロード
bool Texture::Load(const std::string & in_fileName)
{

	// SDLサーフェス(面)を作成
	SDL_Surface* surface = IMG_Load(in_fileName.c_str());          // ロード
	// ロードに失敗したら
	if (!surface)
	{
		SDL_Log("Texture Load Failed : %s", in_fileName.c_str());
		return false;
	}

	// テクスチャ画像の幅、高さを取得
	m_width = surface->w;
	m_height = surface->h;
	int channels = surface->format->BytesPerPixel;

	// OpenGLにテクスチャを登録
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	// テクスチャオブジェクトの生成
	// 2次元としてバインド
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// テクスチャの割り当て
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	// SDLサーフェスは不要なので解放しておく
	SDL_FreeSurface(surface);

	// テクスチャのフィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ミップマップのセット
	SetMipMap();

	// ロードに成功
	return true;
}

// 6つの面を持つキューブマップ用テクスチャの生成
bool Texture::LoadCubeMap(const std::string& in_filePath)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	SDL_Surface* surface;

	// 各スカイボックス用画像のパス
	std::string faces[] =
	{
		in_filePath + "_posx.jpg",
		in_filePath + "_negx.jpg",
		in_filePath + "_posy.jpg",
		in_filePath + "_negy.jpg",
		in_filePath + "_posz.jpg",
		in_filePath + "_negz.jpg"
	};


	for (int i = 0; i < 6; i++)
	{

		surface = IMG_Load(faces[i].c_str());

		// テクスチャ画像の幅、高さを取得
		m_width = surface->w;
		m_height = surface->h;
		int channels = surface->format->BytesPerPixel;

		if (surface)
		{
			// キューブの一面ずつ読み込み
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		}
		else
		{
			std::cout << "CubeMap Texture Load Failed at Path : " << faces[i] << std::endl;
		}

		// SDLサーフェスは不要なので解放しておく
		SDL_FreeSurface(surface);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;

}

// テクスチャ解放処理
void Texture::Delete()
{
	glDeleteTextures(1, &m_textureID);
}

// サーフェスからテクスチャを生成
void Texture::CreateSurface(SDL_Surface * surface)
{
	m_width = surface->w;
	m_height = surface->h;

	// テクスチャを生成し、OpenGL側でタグ付けされたIDを返す
	glGenTextures(1, &m_textureID);
	// IDに設定されたテクスチャを2Dとして読み込む
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	/*   ↓ 以降は上で生成されたIDを基に設定していく ↓   */
	
	// テクスチャの割り当て
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	// テクスチャのフィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// 描画時のアクティブ化
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

// ミップマップ有効化
void Texture::SetMipMap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
