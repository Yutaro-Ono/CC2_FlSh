#include "PostEffect.h"
#include "GameMain.h"
#include "GLSLprogram.h"
#include "ShaderManager.h"
#include "VertexArray.h"


// コンストラクタ
PostEffect::PostEffect()
	:m_activeEffect(POST_EFFECT_TYPE::NONE)
{
}

// デストラクタ
PostEffect::~PostEffect()
{
	glDeleteFramebuffers(1, &m_postEffectFBO);
	glDeleteRenderbuffers(1, &m_rbo);
}

/// <summary>
/// ポストエフェクトのロード
/// </summary>
/// <returns> 成功判定：成功 = true、失敗 = false </returns>
bool PostEffect::Load()
{
	if (!GeneratePostEffect())
	{
		return false;
	}

	if (!LoadPostEffectShaders())
	{
		return false;
	}

	return true;
}

// 書き込まれたフレームを編集し描画する
void PostEffect::DrawPostEffect(unsigned int _screenTexture)
{
	// ブレンドの有効化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 2.フレームバッファ内容をスクリーンに描画
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// 深度テストオフ
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// シェーダの有効化
	GLSLprogram* peShader;
	//m_activeShader->SetActive();
	//m_activeShader->SetInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _screenTexture);
	// VAOバインド
	RENDERER->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
}

/// <summary>
/// ポストエフェクトの生成
/// </summary>
/// <returns> 成功判定：成功 = true、失敗 = false </returns>
bool PostEffect::GeneratePostEffect()
{

	//----------------------------------------------------------------------+
    // ポストエフェクト用フレームバッファ定義
    //----------------------------------------------------------------------+
    // フレームバッファオブジェクトを定義して作成(Gen)
	glGenFramebuffers(1, &m_postEffectFBO);
	// activeなフレームバッファとしてバインドすると、描画先になる
	glBindFramebuffer(GL_FRAMEBUFFER, m_postEffectFBO);

	// フレームバッファが正常に完了したかどうかをチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	//-----------------------------------------------------------------------+
	// テクスチャカラーバッファの作成
	//-----------------------------------------------------------------------+
	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 現在のフレームバッファオブジェクトにアタッチする
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

	//-------------------------------------------------------------+
	// レンダーバッファオブジェクトの作成
	//-------------------------------------------------------------+
	// レンダーバッファオブジェクトの作成
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	// レンダーバッファ操作がオブジェクトに影響を与えるようにバインド
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// レンダーバッファは書き込み専用
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	// FBOにRBOをアタッチ
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//次に、フレームバッファが完全であるかどうかをチェックしたいので、完全でない場合はエラーメッセージを表示します。
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::PostEffect:: PostEffect is not complete!" << std::endl;
	}

	// フレームバッファ解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

bool PostEffect::LoadPostEffectShaders()
{
	// シェーダーの生成
	for (unsigned int i = 0; i < static_cast<unsigned int>(POST_EFFECT_TYPE::ALL_NUM); ++i)
	{
		m_postEffectShaders.push_back(new GLSLprogram);
	}

	// シェーダーのロード
	if (!m_postEffectShaders[static_cast<unsigned int>(POST_EFFECT_TYPE::INVERT_COLOR)]->LoadShaders("Shaders/PostEffect/PostEffect.vert", "Shaders/PostEffect/InvertColorScreen.frag", ""))
	{
		return false;
	}



	return true;
}
