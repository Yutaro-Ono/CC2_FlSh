#include "PostEffect.h"
#include "GameMain.h"
#include "GLSLprogram.h"
#include "ShaderManager.h"
#include "VertexArray.h"


// �R���X�g���N�^
PostEffect::PostEffect()
	:m_activeEffect(POST_EFFECT_TYPE::NONE)
{
}

// �f�X�g���N�^
PostEffect::~PostEffect()
{
	glDeleteFramebuffers(1, &m_postEffectFBO);
	glDeleteRenderbuffers(1, &m_rbo);
}

/// <summary>
/// �|�X�g�G�t�F�N�g�̃��[�h
/// </summary>
/// <returns> ��������F���� = true�A���s = false </returns>
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

// �������܂ꂽ�t���[����ҏW���`�悷��
void PostEffect::DrawPostEffect(unsigned int _screenTexture)
{
	// �u�����h�̗L����
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 2.�t���[���o�b�t�@���e���X�N���[���ɕ`��
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// �[�x�e�X�g�I�t
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// �V�F�[�_�̗L����
	GLSLprogram* peShader;
	//m_activeShader->SetActive();
	//m_activeShader->SetInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _screenTexture);
	// VAO�o�C���h
	RENDERER->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
}

/// <summary>
/// �|�X�g�G�t�F�N�g�̐���
/// </summary>
/// <returns> ��������F���� = true�A���s = false </returns>
bool PostEffect::GeneratePostEffect()
{

	//----------------------------------------------------------------------+
    // �|�X�g�G�t�F�N�g�p�t���[���o�b�t�@��`
    //----------------------------------------------------------------------+
    // �t���[���o�b�t�@�I�u�W�F�N�g���`���č쐬(Gen)
	glGenFramebuffers(1, &m_postEffectFBO);
	// active�ȃt���[���o�b�t�@�Ƃ��ăo�C���h����ƁA�`���ɂȂ�
	glBindFramebuffer(GL_FRAMEBUFFER, m_postEffectFBO);

	// �t���[���o�b�t�@������Ɋ����������ǂ������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	//-----------------------------------------------------------------------+
	// �e�N�X�`���J���[�o�b�t�@�̍쐬
	//-----------------------------------------------------------------------+
	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���݂̃t���[���o�b�t�@�I�u�W�F�N�g�ɃA�^�b�`����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

	//-------------------------------------------------------------+
	// �����_�[�o�b�t�@�I�u�W�F�N�g�̍쐬
	//-------------------------------------------------------------+
	// �����_�[�o�b�t�@�I�u�W�F�N�g�̍쐬
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	// �����_�[�o�b�t�@���삪�I�u�W�F�N�g�ɉe����^����悤�Ƀo�C���h
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	// �����_�[�o�b�t�@�͏������ݐ�p
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	// FBO��RBO���A�^�b�`
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//���ɁA�t���[���o�b�t�@�����S�ł��邩�ǂ������`�F�b�N�������̂ŁA���S�łȂ��ꍇ�̓G���[���b�Z�[�W��\�����܂��B
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::PostEffect:: PostEffect is not complete!" << std::endl;
	}

	// �t���[���o�b�t�@����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

bool PostEffect::LoadPostEffectShaders()
{
	// �V�F�[�_�[�̐���
	for (unsigned int i = 0; i < static_cast<unsigned int>(POST_EFFECT_TYPE::ALL_NUM); ++i)
	{
		m_postEffectShaders.push_back(new GLSLprogram);
	}

	// �V�F�[�_�[�̃��[�h
	if (!m_postEffectShaders[static_cast<unsigned int>(POST_EFFECT_TYPE::INVERT_COLOR)]->LoadShaders("Shaders/PostEffect/PostEffect.vert", "Shaders/PostEffect/InvertColorScreen.frag", ""))
	{
		return false;
	}



	return true;
}
