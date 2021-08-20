//----------------------------------------------------------------------------------+
// @file        ShaderManager.cpp
// @brief       �V�F�[�_�[�N���X�̊Ǘ�
//              �V�F�[�_�[�v���O��������ʂ��Ƃ�map�ɕۑ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	// �V�F�[�_�[�A�z�z��̍폜
	auto iter = m_shaders.begin();
	while (iter != m_shaders.end())
	{
		m_shaders.erase(iter++);
	}
}

/// <summary>
/// �V�F�[�_�[�N���X�̐���
/// </summary>
/// <returns> �V�F�[�_�[�v���O�����̍쐬�Ɏ��s�����ꍇ��false��Ԃ� </returns>
bool ShaderManager::CreateShaders()
{
	// ��ʏo�͗p�V�F�[�_�[
	m_shaders[GLSL_SHADER::OUT_SCREEN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OUT_SCREEN]->LoadShaders("Shaders/OutScreen.vert", "Shaders/OutScreen.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::OUT_SCREEN]->SetUniform("u_screenTexture", 0);


	//-------------------------------------------------------------------------+
	// �W���V�F�[�_�[
	//-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::BASIC] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC]->LoadShaders("Shaders/ForwardRendering/BasicMesh.vert", "Shaders/ForwardRendering/BasicMesh.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC_PHONG]->SetUniform("u_mat.albedo", 0);
	
	// �W���V�F�[�_�[+Phong���C�e�B���O
	m_shaders[GLSL_SHADER::BASIC_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_PHONG]->LoadShaders("Shaders/ForwardRendering/Phong.vert", "Shaders/ForwardRendering/Phong.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC_PHONG]->SetUniform("u_mat.albedo", 0);

    //-------------------------------------------------------------------------+
    // �x�������_�����O�p�V�F�[�_�[
    //-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::GBUFFER_BASIC] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_BASIC]->LoadShaders("Shaders/DefferedRendering/GBuffer_Basic.vert", "Shaders/DefferedRendering/GBuffer_Basic.frag", ""))
	{
		return false;
	}

	// GBuffer+Phong���C�e�B���O
	m_shaders[GLSL_SHADER::GBUFFER_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_PHONG]->LoadShaders("Shaders/DefferedRendering/GBuffer_Phong.vert", "Shaders/DefferedRendering/GBuffer_Phong.frag", ""))
	{
		return false;
	}

	// GBuffer+�@���}�b�v
	m_shaders[GLSL_SHADER::GBUFFER_NORMAL] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_NORMAL]->LoadShaders("Shaders/DefferedRendering/GBuffer_NormalMap.vert", "Shaders/DefferedRendering/GBuffer_NormalMap.frag", ""))
	{
		return false;
	}

	// GBuffer+�X�J�C�{�b�N�X
	m_shaders[GLSL_SHADER::GBUFFER_SKYBOX] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_SKYBOX]->LoadShaders("Shaders/DefferedRendering/GBuffer_Basic_SkyBox.vert", "Shaders/DefferedRendering/GBuffer_Basic_SkyBox.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::GBUFFER_SKYBOX]->SetUniform("u_cubeMap", 0);


	//-------------------------------------------------------------------------+
	// Bloom�p�V�F�[�_�[
	//-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::DOWNSAMPLING] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DOWNSAMPLING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_DownSampling.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::DOWNSAMPLING]->SetUniform("u_scene", 0);

	m_shaders[GLSL_SHADER::GAUSSIAN_BLUR] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GAUSSIAN_BLUR]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_GaussianBlur.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::GAUSSIAN_BLUR]->SetUniform("u_blurSource", 0);

	m_shaders[GLSL_SHADER::TONEMAPPING] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::TONEMAPPING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_ToneMapping.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::TONEMAPPING]->SetUniform("u_scene", 0);

	//---------------------------------------------------------------------------+
	// ���C�g�p�X
	//---------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT]->LoadShaders("Shaders/GBuffer/GBuffer_LightPass.vert", "Shaders/GBuffer/GBuffer_DirectionalLight.frag", ""))
	{
		return false;
	}

    //-------------------------------------------------------------------------+
    // �}�b�vHUD�p�V�F�[�_�[
    //-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::HUD_INPUT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::HUD_INPUT]->LoadShaders("Shaders/HUD/HUD_MapShader.vert", "Shaders/HUD/HUD_MapShader.frag", ""))
	{
		return false;
	}

	m_shaders[GLSL_SHADER::HUD_OUTPUT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::HUD_OUTPUT]->LoadShaders("Shaders/HUD/HUD_MapOutput.vert", "Shaders/HUD/HUD_MapOutput.frag", ""))
	{
		return false;
	}
	

	//---------------------------------------------------------------------------+
	// ���̑��̃V�F�[�_�[ (�f�o�b�O�p�r�W���A���C�U�[�n)
	//---------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE]->LoadShaders("Shaders/Debug/Normal_Visualization.vert", "Shaders/Debug/Normal_Visualization.frag", "Shaders/option/Normal_Visualization.geom"))
	{
		return false;
	}

	m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE_GBUFFER] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE_GBUFFER]->LoadShaders("Shaders/Debug/Normal_Visualization_GBuffer.vert", "Shaders/Debug/Normal_Visualization_GBuffer.frag", "Shaders/Debug/Normal_Visualization_GBuffer.geom"))
	{
		return false;
	}

	return true;
}


/// <summary>
/// �w�肵���^�C�v�̃V�F�[�_�[�v���O�����̗L����
/// </summary>
/// <param name=Shader::SHADER_TYPE> �V�F�[�_�[�̃^�C�v </param>
void ShaderManager::EnableShaderProgram(GLSL_SHADER _type)
{
	m_shaders[_type]->UseProgram();
}

/// <summary>
/// �V�F�[�_�[�N���X�̃|�C���^�̃Q�b�^�[
/// </summary>
/// <param name="_type"> �V�F�[�_�[�̃^�C�v </param>
/// <returns> �w�肵���^�C�v�̃V�F�[�_�[�N���X�|�C���^��Ԃ� </returns>
GLSLprogram* ShaderManager::GetShader(GLSL_SHADER _type)
{
	return m_shaders[_type];
}
