//-------------------------------------------------------------------------------------------+
//
// �x�������_�����O�N���X (GBuffer�Ɋe�`�挋�ʂ�ۑ��EGBuffer�ɑ΂��Č����������s��)
//
//-------------------------------------------------------------------------------------------+
#include "DefferedRenderer.h"
#include <stdlib.h>
#include <iostream>
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "EnvironmentMapComponent.h"
#include "CubeMapComponent.h"
#include "VertexArray.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "SpriteComponent.h"
#include "WorldSpaceUI.h"
#include "UIScreen.h"
#include "RenderBloom.h"
#include "PostEffect.h"
#include "ParticleManager.h"
#include "CarMeshComponent.h"
#include "CameraComponent.h"
#include "LightGlassComponent.h"
#include "MiniMapHUD.h"
#include "ShaderManager.h"
#include "DirectionalLight.h"
#include "Debugger.h"
#include "DefferedRendererDebugObject.h"

// �R���X�g���N�^
DefferedRenderer::DefferedRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
	,m_gBuffer(0)
	,m_gPos(0)
	,m_gNormal(0)
	,m_gAlbedoSpec(0)
	,m_gEmissive(0)
	,m_lightHDR(0)
	,m_uboGBuffer(0)
{
	std::cout << "CREATE::DefferedRenderer::Instance" << std::endl;
}

// �f�X�g���N�^
DefferedRenderer::~DefferedRenderer()
{
}

// GBuffer�ւ̏������ݏ���
void DefferedRenderer::DrawGBuffer()
{
	// �}�b�vHUD�������ݏ���
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->WriteBuffer(m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::HUD_INPUT), m_renderer->m_meshComponents);
	}
	// �`����GBuffer�Ƃ��ăo�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// �r���[�|�[�g���X�N���[���T�C�Y�ɃZ�b�g
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0, 0.0, 0.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g��ON
	glEnable(GL_DEPTH_TEST);

	// ���C�g��Ԃ̊e�s����`
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(m_renderer->GetDirectionalLight()->GetPosition(), m_renderer->GetDirectionalLight()->GetTargetPos(), Vector3::UnitZ);
	//lightSpace = lightView * lightProj;
	lightSpace = lightProj * lightView;


	//-----------------------------------------------------------+
	// �ʏ탁�b�V��
	//-----------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	GLSLprogram* meshShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_NORMAL_SHADOW);
	meshShader->UseProgram();
	meshShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	meshShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	meshShader->SetUniform("u_mat.diffuseMap", 0);
	meshShader->SetUniform("u_mat.specularMap", 1);
	meshShader->SetUniform("u_mat.normalMap", 2);
	meshShader->SetUniform("u_mat.emissiveMap", 3);
	meshShader->SetUniform("u_mat.depthMap", 4);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(meshShader);
	}

	
	//------------------------------------------------------------+
	// �X�L�����b�V��
	//------------------------------------------------------------+
	// �V�F�[�_��uniform�Z�b�g
	GLSLprogram* skinShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_SKINMESH);
	skinShader->UseProgram();
	skinShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	skinShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	skinShader->SetUniform("u_mat.diffuseMap", 0);
	skinShader->SetUniform("u_mat.specularMap", 1);
	skinShader->SetUniform("u_mat.normalMap", 2);
	skinShader->SetUniform("u_mat.emissiveMap", 3);
	skinShader->SetUniform("u_mat.depthMap", 4);
	// ���b�V���`�� (������GBuffer�̊e�v�f�ɏ�񂪏������܂��)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(skinShader);
	}

	//------------------------------------------------------------+
    // SkyBox
    //------------------------------------------------------------+
	GLSLprogram* skyboxShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_SKYBOX);
	// ���W�n�̖��ŃX�J�C�{�b�N�X������Ȍ����ɕ`�悳��Ȃ��̂ŁA��]�I�t�Z�b�g��ݒ�
	Matrix4 offset;
	offset = Matrix4::CreateRotationX(Math::ToRadians(90.0f));
	// Uniform�ɋt���]�u�s����Z�b�g
	Matrix4 InvTransView = m_renderer->m_viewMat;
	InvTransView.Invert();
	InvTransView.Transpose();
	skyboxShader->UseProgram();
	skyboxShader->SetUniform("u_offset", offset);
	skyboxShader->SetUniform("u_invTransView", InvTransView);
	skyboxShader->SetUniform("u_cubeMap", 0);
	if (m_renderer->GetSkyBox() != nullptr)
	{
		m_renderer->GetSkyBox()->Draw(skyboxShader);
	}
	

	//------------------------------------------------------------+
	// ��
	//------------------------------------------------------------+
	GLSLprogram* carShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_CAR_BODY);
	carShader->UseProgram();
	carShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	carShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	carShader->SetUniform("u_offset", offset);
	carShader->SetUniform("u_mat.diffuseMap", 0);
	carShader->SetUniform("u_mat.specularMap", 1);
	carShader->SetUniform("u_mat.depthMap", 2);
	// �ԃ��b�V���`��
	for (auto car : m_renderer->m_carMeshComponents)
	{
		car->Draw(carShader);
	}


	//------------------------------------------------------------+
	// EnvironmentMap
	//------------------------------------------------------------+
	// ���b�V�������̃J�����O
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	GLSLprogram* envShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_ENVIRONMENT);
	// uniform�Z�b�g
	envShader->UseProgram();
	envShader->SetUniform("u_skybox", 0);
	envShader->SetUniform("u_offset", offset);
	for (auto env : m_renderer->m_envMeshComponents)
	{
		env->DrawEnvironmentMap(envShader);
	}
	// �J�����O�̃I�t
	glDisable(GL_CULL_FACE);

	//------------------------------------------------------------+
    // ���C�g�O���X
    //------------------------------------------------------------+
	GLSLprogram* glassShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_GLASS);
	glassShader->UseProgram();
	glassShader->SetUniform("u_skybox", 0);
	for (auto light : m_renderer->m_lightGlassComponents)
	{
		light->Draw(glassShader);
	}


	// GBuffer�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

/// <summary>
/// SSAO�p�X
/// </summary>
void DefferedRenderer::DrawSSAOPath()
{
	GLSLprogram* ssaoShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::SSAO);
	GLSLprogram* ssaoBlurShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::SSAO_BLUR);

	//----------------------------------------------------------------------+
	// 1.SSAO
	//----------------------------------------------------------------------+
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoShader->UseProgram();

	// �J�[�l�����V�F�[�_�[�֑��M
	for (unsigned int i = 0; i < 64; ++i)
	{
		std::string kernelStr;
		kernelStr = "u_samples[" + std::to_string(i) + "]";
		ssaoShader->SetUniform(kernelStr.c_str(), m_ssaoKernel[i]);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_noiseTex);

	ssaoShader->SetUniform("u_gPosition", 0);
	ssaoShader->SetUniform("u_gNormal", 1);
	ssaoShader->SetUniform("u_texNoise", 2);
	ssaoShader->SetUniform("u_screenW", RENDERER->GetScreenWidth());
	ssaoShader->SetUniform("u_screenH", RENDERER->GetScreenHeight());

	// �X�N���[���S�̂ɕ`��
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//----------------------------------------------------------------------+
    // 2.SSAO�u���[����
    //----------------------------------------------------------------------+
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoBlurShader->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);

	ssaoBlurShader->SetUniform("u_ssaoInput", 0);

	// �X�N���[���S�̂ɕ`��
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// GBuffer�����Ƀ��C�e�B���O�v�Z���s��
void DefferedRenderer::DrawLightPath()
{
	// ���C�g�o�b�t�@���o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// �u�����h�w��
	glEnablei(GL_BLEND, 0);                                          // ���Z�������s�����߃u�����h��L����
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // ���Z�����̃u�����h���w��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �[�x�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);

	// gBuffer�̊e�e�N�X�`�����o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	// SSAO�e�N�X�`�����o�C���h
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);

	// �J�����O�ݒ�F���C�g�̓��b�V���̗����̂ݕ`�悷��
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//------------------------------------------------------+
	// �|�C���g���C�g
	//------------------------------------------------------+
	// �|�C���g���C�g�V�F�[�_�ւ̃Z�b�g
	GLSLprogram* pointLightShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::POINT_LIGHT);
	pointLightShader->UseProgram();
	pointLightShader->SetUniform("u_gBuffer.position",     0);
	pointLightShader->SetUniform("u_gBuffer.normal",       1);
	pointLightShader->SetUniform("u_gBuffer.albedoSpec",   2);
	pointLightShader->SetUniform("u_gBuffer.emissive",     3);
	pointLightShader->SetUniform("u_ssao",                 4);
	// �|�C���g���C�g�̕`��
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(pointLightShader);
	}


	//------------------------------------------------------+
	// �X�|�b�g���C�g
	//------------------------------------------------------+
	// �X�|�b�g���C�g�V�F�[�_�ւ̃Z�b�g
	//m_spotLightShader->UseProgram();
	//m_spotLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	//m_spotLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	//m_spotLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	//m_spotLightShader->SetInt("u_gBuffer.pos", 0);
	//m_spotLightShader->SetInt("u_gBuffer.normal", 1);
	//m_spotLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	//m_spotLightShader->SetInt("u_gBuffer.emissive", 3);
	//// �X�|�b�g���C�g�̕`��
	//for (auto spotL : m_renderer->m_spotLights)
	//{
	//	spotL->Draw(m_spotLightShader);
	//}

	// �J�����O�̃I�t
	glDisable(GL_CULL_FACE);


	//-----------------------------------------------+
	// �f�B���N�V���i�����C�g�p�X
	//-----------------------------------------------+
	// �P�x��`
	float intensity = 1.35f;
	m_renderer->GetDirectionalLight()->SetIntensity(intensity);
	GLSLprogram* dirLightShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::DIRECTIONAL_LIGHT);
	// �V�F�[�_�̃Z�b�g
	dirLightShader->UseProgram();
	dirLightShader->SetUniform("u_gBuffer.position",   0);
	dirLightShader->SetUniform("u_gBuffer.normal",     1);
	dirLightShader->SetUniform("u_gBuffer.albedoSpec", 2);
	dirLightShader->SetUniform("u_gBuffer.emissive",   3);
	dirLightShader->SetUniform("u_ssao",               4);
	// �X�N���[���S�̂ɕ`��
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Sprite�̕`��
	// �u�����h�̃A�N�e�B�u��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// �[�x�e�X�g�̒�~
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// ���[���h��ԏ�̃X�v���C�g�`��
	Matrix4 view = m_renderer->GetViewMatrix();
	Matrix4 projection = m_renderer->GetProjectionMatrix();
	GLSLprogram* sprite3Dshader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::SPRITE_3D);
	sprite3Dshader->UseProgram();
	sprite3Dshader->SetUniform("u_view", view);
	sprite3Dshader->SetUniform("u_projection", projection);
	for (auto spr : m_renderer->m_worldSprites)
	{
		//spr->Draw(sprite3Dshader);
	}

	// sprite�V�F�[�_�[�̃A�N�e�B�u��
	GLSLprogram* sprite2Dshader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::SPRITE_2D);
	sprite2Dshader->UseProgram();
	sprite2Dshader->SetUniform("u_intensity", 1.0f);
	RENDERER->SetActiveSpriteVAO();
	for (auto sprite : m_renderer->m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			//sprite->Draw(sprite2Dshader);
		}
	}
	// �S�Ă�UI���X�V
	sprite2Dshader->UseProgram();
	sprite2Dshader->SetUniform("u_intensity", 1.0f);
	RENDERER->SetActiveSpriteVAO();
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(sprite2Dshader);
	}

	// �}�b�vHUD
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->DrawMap(m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::HUD_OUTPUT));
	}

	// �u�����h���I�t
	glDisable(GL_BLEND);

	// �u�����h���~����
	glDisablei(GL_BLEND, 0);

	// gBuffer�̐[�x�������C�g�o�b�t�@�փR�s�[����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);              // gBuffer��ǂݎ��t���[���o�b�t�@�Ƃ��Ďw��
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // �������݃o�b�t�@�����C�g�o�b�t�@�Ɏw��
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ���C�g�o�b�t�@�`��֖߂�
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// �[�x�e�X�g���I��
	glEnable(GL_DEPTH_TEST);
	
	//----------------------------------------------------------------+
    // �p�[�e�B�N���`��
    //----------------------------------------------------------------+
	//m_renderer->GetParticleManager()->Draw();


	// ���C�gFBO�ւ̏������݂��~�߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// �{�`�揈��
void DefferedRenderer::Draw()
{

	// GBuffer�ւ̏�������
	DrawGBuffer();

	// SSAO����
	//DrawSSAOPath();

	// ���C�g�o�b�t�@�ւ̏�������
	DrawLightPath();

	// Bloom�������{�����`��
	RenderBloom* bloom = m_renderer->GetBloom();
	bloom->DrawDownSampling(m_lightHighBright);
	bloom->DrawGaussBlur();
	bloom->DrawBlendBloom(m_lightHDR);

	//----------------------------------------------------------------+
	// �ŏI�o�͌��ʂ�`��
	//----------------------------------------------------------------+
	// GBuffer�ɏ������܂ꂽ�v�f���X�N���[���ɕ`��
	//GLSLprogram* screenShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::OUT_SCREEN);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	//// �X�N���[���V�F�[�_��uniform�Z�b�g
	//screenShader->UseProgram();
	//screenShader->SetUniform("u_screenTexture", 0);

	//// GBuffer�e�N�X�`���Z�b�g
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	////glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	////glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	//// �X�N���[���ɕ`��
	//m_renderer->m_screenVerts->SetActive();
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

// GBuffer���쐬����
// ���W�p�o�b�t�@�E�@���p�o�b�t�@�E�A���x�h���X�y�L�����o�b�t�@�E�����_�[�o�b�t�@
bool DefferedRenderer::GenerateGBuffer()
{
	// GBuffer�̓o�^�E�o�C���h
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//----------------------------------------------------------------+
	// �e�o�b�t�@��o�^���A2D�e�N�X�`���Ƃ���GBuffer�ɕR�t����
	//----------------------------------------------------------------+
	// 3D��ԍ��W�o�b�t�@ (���������_�o�b�t�@/�J���[0�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// �@���x�N�g���o�b�t�@ (���������_�o�b�t�@/�J���[1�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// �A���x�h(RGB)���X�y�L����(A)�p�J���[�o�b�t�@ (A�����܂�8bit�J���[�o�b�t�@/2�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// ���P�x�o�b�t�@�̍쐬 (�G�~�b�V�u�o�͗p�P�x�o�b�t�@/3�ԖڂƂ��ēo�^)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);


	// �e�e�N�X�`����GBuffer�̕`���Ƃ���GL���ɖ�������
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

	// �����_�[�o�b�t�@�̍쐬 (�X�e���V���o�b�t�@�Ƃ��Ē�`)
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::GBUFFER::Generate Failed" << std::endl;
		return false;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/// <summary>
/// SSAO�p�̃o�b�t�@��������
/// </summary>
/// <returns></returns>
bool DefferedRenderer::GenerateSSAOBuffer()
{
	// SSAO�p�t���[���o�b�t�@�̓o�^
	glGenFramebuffers(1, &m_ssaoFBO);
	// ssao�t���[���o�b�t�@���o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	// SSAO�p�J���[�o�b�t�@�̓o�^
	glGenTextures(1, &m_ssaoColor);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColor, 0);
	// �t���[���o�b�t�@�̕s����`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::SSAO::Generate Failed" << std::endl;
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// SSAO�p�t���[���o�b�t�@�̓o�^
	glGenFramebuffers(1, &m_ssaoBlurFBO);
	// ssao�u���[�o�b�t�@�̃o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	// SSAO�p�J���[�o�b�t�@�̓o�^
	glGenTextures(1, &m_ssaoBlurColor);
	glBindTexture(GL_TEXTURE_2D, m_ssaoBlurColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurColor, 0);
	// �t���[���o�b�t�@�̕s����`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::SSAO::Generate Failed" << std::endl;
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);     // �J���[�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// �T���v�����O�p�J�[�l���̐���
	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);   // �����_�������p(0.0f �` 1.0f)
	for (unsigned int i = 0; i < 64; ++i)
	{
		Vector3 sample(randomFloats(m_generator) * 2.0f - 1.0f, randomFloats(m_generator) * 2.0f - 1.0f, randomFloats(m_generator));
		sample.Normalize();
		sample *= randomFloats(m_generator);
		float scale = static_cast<float>(i) / 64.0f;

		// sample��S.T.�ɃX�P�[�����O���邱�ƂŁA�J�[�l���̒��S�Ɉ�v������
		scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
		sample *= scale;
		m_ssaoKernel.push_back(sample);
	}

	// �m�C�Y�e�N�X�`���̐���
	for (unsigned int i = 0; i < 16; i++)
	{
		Vector3 noise(randomFloats(m_generator) * 2.0f - 1.0f, randomFloats(m_generator) * 2.0f - 1.0f, 0.0f);
		m_ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &m_noiseTex);
	glBindTexture(GL_TEXTURE_2D, m_noiseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &m_ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;
}

// ���C�g�o�b�t�@���쐬����
// ���������p��HDR�o�b�t�@�E�����_�[�o�b�t�@
bool DefferedRenderer::GenerateLightBuffer()
{
	glGenFramebuffers(1, &m_lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// HDR�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// ���P�x�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// �A�^�b�`�����g�ݒ�
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

	// �����_�[�o�b�t�@���쐬����
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::LIGHTBUFFER::Generate False" << std::endl;
		return false;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return true;
}


/// <summary> 
/// Deffered�����_�����O�ɗp����ϐ��̏������E�C���X�^���X�̐���
/// </summary>
/// <returns></returns>
bool DefferedRenderer::Initialize()
{
	// G�o�b�t�@�ESSAO�o�b�t�@�E���C�g�o�b�t�@�̐���
	GenerateGBuffer();
	GenerateSSAOBuffer();
	GenerateLightBuffer();

	// �f�o�b�O�I�u�W�F�N�g
#ifdef _DEBUG

	DefferedRendererDebugObject* defDebugObj = new DefferedRendererDebugObject(this);
	DEBUGGER->AddDebugObject(defDebugObj, Debugger::DEBUG_TAG::SYSTEM);

#endif

	return true;
}