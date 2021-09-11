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

	POST_EFFECT_TYPE m_activeEffect;          // ���݃A�N�e�B�u�ȃ|�X�g�G�t�F�N�g

	unsigned int m_postEffectFBO;             // �t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int m_rbo;                       // �����_�[�o�b�t�@�I�u�W�F�N�g

	unsigned int m_colorBuffer;               // �J���[�o�b�t�@

	std::vector<class GLSLprogram*> m_postEffectShaders;

};