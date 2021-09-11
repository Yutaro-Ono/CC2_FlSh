#pragma once
#include <vector>

class PostEffect
{

public:


	enum class POST_EFFECT_TYPE : unsigned int
	{
		NONE = 0,
		INVERT_COLOR,
		GREY_SCALE,
		CERNEL,
		BLUR,
		SHARP_EDGE,
		ALL_NUM
	};


	PostEffect();
	~PostEffect();

	bool Load();

	void DrawPostEffect(unsigned int _screenTexture);

private:

	bool GeneratePostEffect();
	bool LoadPostEffectShaders();

	POST_EFFECT_TYPE m_activeEffect;          // 現在アクティブなポストエフェクト

	unsigned int m_postEffectFBO;             // フレームバッファオブジェクト
	unsigned int m_rbo;                       // レンダーバッファオブジェクト

	unsigned int m_colorBuffer;               // カラーバッファ

	std::vector<class GLSLprogram*> m_postEffectShaders;

};