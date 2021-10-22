//-------------------------------------------------------------------------------------------+
//
// �x�������_�����O�N���X (GBuffer�Ɋe�`�挋�ʂ�ۑ��EGBuffer�ɑ΂��Č����������s��)
//
//-------------------------------------------------------------------------------------------+
#pragma once
#include <random>
#include <vector>
#include "Math.h"

class DefferedRenderer
{

public:

	DefferedRenderer(class Renderer* in_renderer);
	~DefferedRenderer();

	bool Initialize();                // ���������� (�e��t���[���o�b�t�@���쐬����)

	void DrawGBuffer();               // GBuffer�ւ̏������ݏ���
	void DrawSSAOPath();
	void DrawLightPath();             // GBuffer���烉�C�e�B���O�v�Z���s��
	void Draw();                      // GBuffer�ɏ������܂ꂽ���̕`��


private:

	bool GenerateGBuffer();             // GBuffer�̍쐬
	bool GenerateSSAOBuffer();        // SSAO�o�b�t�@�̍쐬
	bool GenerateLightBuffer();         // ���C�g�o�b�t�@�̍쐬

	//-----------------------------+
	// �����o�ϐ�
	//-----------------------------+
	class Renderer* m_renderer;           // �����_���[�N���X�ւ̃|�C���^

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3�v�f�����t���[���o�b�t�@)
    // G-Buffer�Ɋ֘A�t����e��o��
	unsigned int m_gPos;                  // GBuffer�p3D��ԍ��W
	unsigned int m_gNormal;               // GBuffer�p�@���x�N�g��
	unsigned int m_gAlbedoSpec;           // GBuffer�p�A���x�h(RGB)���X�y�L����(A)
	unsigned int m_gEmissive;             // GBuffer�p�P�x(���G�~�b�V�u)�o�b�t�@
	unsigned int m_gAttachments[4];       // GBuffer�A�^�b�`�����g (�o�b�t�@�����m��)
	unsigned int m_gRBO;                  // G-Buffer�p�̕`��o�b�t�@�I�u�W�F�N�g

	// SSAO
	unsigned int m_ssaoFBO;
	unsigned int m_ssaoBlurFBO;
	// SSAO�o�͐�
	unsigned int m_ssaoColor;
	unsigned int m_ssaoBlurColor;
	// �m�C�Y�e�N�X�`��
	std::vector<Vector3> m_ssaoNoise;
	unsigned int m_noiseTex;
	// �J�[�l���T���v�����O�p
	std::default_random_engine m_generator;
	std::vector<Vector3> m_ssaoKernel;


	// ���C�g�o�b�t�@ (���������p)
	unsigned int m_lightFBO;              // ���C�g�o�b�t�@
	// ���C�g�o�b�t�@�Ɋ֘A�t����o��
	unsigned int m_lightHDR;              // ���������p��HDR�Ή��o�b�t�@ (���������_)
	unsigned int m_lightHighBright;       // ���C�g�p���P�x�o�b�t�@
	unsigned int m_lightRBO;              // ���C�g�p�����_�[�o�b�t�@
	unsigned int m_lightAttachments[2];

	// uniform�o�b�t�@ (GBuffer�p)
	unsigned int m_uboGBuffer;

	friend class DefferedRendererDebugObject;
};