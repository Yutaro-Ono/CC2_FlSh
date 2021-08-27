#include "ShadowMap.h"
#include "GLSLprogram.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CarMeshComponent.h"
#include "GLSLprogram.h"
#include "ShaderManager.h"
#include "DirectionalLight.h"

const int ShadowMap::SHADOW_WIDTH = 8192;
const int ShadowMap::SHADOW_HEIGHT = 8192;

// �R���X�g���N�^
ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	// �f�v�X�}�b�v���o�C���h
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	// �[�x�l�݂̂��K�v�Ȃ̂ŁA�t�H�[�}�b�g��DEPTH_COMPONENT(24bit)��
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// �e�N�X�`�����b�s���O�E�t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// �t���[���o�b�t�@�Ƀf�v�X�}�b�v���A�^�b�`����
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	// ���̎��_����V�[���������_�����O����ۂ̐[�x���݂̂��K�v
	// ���̂��߃J���[�o�b�t�@���g�p���Ȃ����Ƃ𖾎�����
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*
	// �V�F�[�_�̍쐬
	// �f�v�X�}�b�v�V�F�[�_
	m_depthShader = new GLSLprogram();
	m_depthShader->("Data/GLSLprograms/DepthMap.vert", "Data/GLSLprograms/DepthMap.frag");

	// �m�[�}���}�b�v + �e�V�F�[�_
	m_shadowShader = new GLSLprogram();
	m_shadowShader->Load("Data/GLSLprograms/ShadowNormalMap.vert", "Data/GLSLprograms/ShadowNormalMap.frag");
	
	m_depthSkinShader = new GLSLprogram();
	m_depthSkinShader->Load("Data/GLSLprograms/SkinnedDepth.vert", "Data/GLSLprograms/DepthMap.frag");
	m_skinShadowShader = new GLSLprogram();
	m_skinShadowShader->Load("Data/GLSLprograms/SkinnedShadow.vert", "Data/GLSLprograms/PhongShadow.frag");
	*/
}

ShadowMap::~ShadowMap()
{

	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
}


/// <summary>
/// ���C�g���_����I�u�W�F�N�g��`�悵�A�[�x�}�b�v�𓾂�
/// </summary>
/// <param name="in_mesh"></param>     �V���h�E��K�p���郁�b�V���z��
/// <param name="in_skelMesh"></param> �V���h�E��K�p����X�L�����b�V���z��
void ShadowMap::RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*> in_skelMesh, const std::vector<class CarMeshComponent*> in_carMesh)
{
	// �[�x�e�X�g�L����
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���C�g���_�p�̃v���W�F�N�V�����s��ƃr���[�s���p�ӂ���
	// �f�B���N�V���i�����C�g(���s)�ł��邽�߁A�v���W�F�N�V�����s��ɂ͐��ˉe�s����g�p
	//m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, -10.0f, 5000.0f);

	m_lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight()->GetPosition(), RENDERER->GetDirectionalLight()->GetTargetPos(), Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// �V���h�E�}�b�v�̓����_�����O���̉𑜓x�Ƃ͈قȂ�A�V���h�E�}�b�v�̃T�C�Y�ɍ��킹��Viewport�p�����[�^��ύX����K�v������
	// ���̂���glViewport���Ăяo���B
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// �t���[���o�b�t�@�̃o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// �[�x�}�b�v�������ݗp�V�F�[�_�[
	GLSLprogram* depthMesh = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::DEPTH_MESH);
	GLSLprogram* depthSkin = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::DEPTH_SKIN);

	depthMesh->UseProgram();
	depthMesh->SetUniform("u_lightSpaceMatrix", m_lightSpace);

	// �f�v�X�o�b�t�@�𓾂邽�߂Ƀ��C�g���猩���V�[���������_�����O����
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->DrawShadow(depthMesh);
	}
	for (auto mesh : in_carMesh)
	{
		mesh->DrawShadow(depthMesh);
	}

	depthSkin->UseProgram();
	depthSkin->SetUniform("u_lightSpaceMatrix", m_lightSpace);
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->DrawShadow(depthSkin);
		}

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


// �V���h�E�ƃ��b�V���̕`�� (�X�L�����b�V���͑ΏۊO)
void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh)
{
	/*
	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_shadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_shadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_shadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_shadowGLSLprogram->SetInt("u_mat.depthMap", 3);
	
	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowGLSLprogram);
	}
	*/
}

void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*>& in_skelMesh)
{
	/*
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_shadowGLSLprogram->SetActive();
	m_shadowGLSLprogram->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_shadowGLSLprogram->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_shadowGLSLprogram->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_shadowGLSLprogram->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_shadowGLSLprogram->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_shadowGLSLprogram->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_shadowGLSLprogram->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_shadowGLSLprogram->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowGLSLprogram->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_shadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_shadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_shadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_shadowGLSLprogram->SetInt("u_mat.depthMap", 3);

	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowGLSLprogram);
	}

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_skinShadowGLSLprogram->SetActive();
	m_skinShadowGLSLprogram->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_skinShadowGLSLprogram->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_skinShadowGLSLprogram->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_skinShadowGLSLprogram->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_skinShadowGLSLprogram->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_skinShadowGLSLprogram->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_skinShadowGLSLprogram->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_skinShadowGLSLprogram->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_skinShadowGLSLprogram->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_skinShadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_skinShadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_skinShadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_skinShadowGLSLprogram->SetInt("u_mat.depthMap", 3);
	// �V���h�E�V�F�[�_�ɂ��X�L�����b�V���`��
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->Draw(m_skinShadowGLSLprogram);
		}
	}
	*/
}

