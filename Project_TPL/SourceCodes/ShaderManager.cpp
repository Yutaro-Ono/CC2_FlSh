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
#include "GameMain.h"
#include "Renderer.h"

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
	// uniform�p
	Matrix4 screenMat = Matrix4::CreateSimpleViewProj(RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());



	// ��ʏo�͗p�V�F�[�_�[
	m_shaders[GLSL_SHADER::OUT_SCREEN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OUT_SCREEN]->LoadShaders("Shaders/OutScreen.vert", "Shaders/OutScreen.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::OUT_SCREEN]->UseProgram();
	m_shaders[GLSL_SHADER::OUT_SCREEN]->SetUniform("u_screenTexture", 0);


	//-------------------------------------------------------------------------+
	// �W���V�F�[�_�[
	//-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::BASIC] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC]->LoadShaders("Shaders/ForwardRendering/BasicMesh.vert", "Shaders/ForwardRendering/BasicMesh.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC]->UseProgram();
	m_shaders[GLSL_SHADER::BASIC]->SetUniform("u_mat.albedo", 0);
	
	// �W���V�F�[�_�[+Phong���C�e�B���O
	m_shaders[GLSL_SHADER::BASIC_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_PHONG]->LoadShaders("Shaders/ForwardRendering/Phong.vert", "Shaders/ForwardRendering/Phong.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC_PHONG]->UseProgram();
	m_shaders[GLSL_SHADER::BASIC_PHONG]->SetUniform("u_mat.albedo", 0);

	// �W���V�F�[�_�[+Phong���C�e�B���O+�V���h�E
	m_shaders[GLSL_SHADER::BASIC_PHONG_SHADOW] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_PHONG_SHADOW]->LoadShaders("Shaders/ForwardRendering/PhongShadow.vert", "Shaders/ForwardRendering/PhongShadow.frag", ""))
	{
		return false;
	}

	// �W���V�F�[�_�[+�X�L�����b�V��
	m_shaders[GLSL_SHADER::BASIC_SKINMESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_SKINMESH]->LoadShaders("Shaders/ForwardRendering/Skinned.vert", "Shaders/ForwardRendering/Phong.frag", ""))
	{
		return false;
	}

	// �W���V�F�[�_�[+SkyBox
	m_shaders[GLSL_SHADER::BASIC_SKYBOX] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_SKYBOX]->LoadShaders("Shaders/ForwardRendering/SkyBox.vert", "Shaders/ForwardRendering/SkyBox.frag", ""))
	{
		return false;
	}

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

	// GBuffer+�@���}�b�v+�V���h�E
	m_shaders[GLSL_SHADER::GBUFFER_NORMAL_SHADOW] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_NORMAL_SHADOW]->LoadShaders("Shaders/DefferedRendering/GBuffer_Normal_Shadow.vert", "Shaders/DefferedRendering/GBuffer_Normal_Shadow.frag", ""))
	{
		return false;
	}

	// GBuffer�X�L�����b�V��
	m_shaders[GLSL_SHADER::GBUFFER_SKINMESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_SKINMESH]->LoadShaders("Shaders/DefferedRendering/GBuffer_SkinNormShadow.vert", "Shaders/DefferedRendering/GBuffer_Shadow.frag", ""))
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

	// GBuffer+���}�b�v
	m_shaders[GLSL_SHADER::GBUFFER_ENVIRONMENT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_ENVIRONMENT]->LoadShaders("Shaders/DefferedRendering/GBuffer_EnvironmentMap.vert", "Shaders/DefferedRendering/GBuffer_EnvironmentMap.frag", ""))
	{
		return false;
	}

	// GBuffer+�ԃ{�f�B�p
	m_shaders[GLSL_SHADER::GBUFFER_CAR_BODY] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_CAR_BODY]->LoadShaders("Shaders/DefferedRendering/GBuffer_CarShaderReflect.vert", "Shaders/DefferedRendering/GBuffer_CarShaderReflect.frag", ""))
	{
		return false;
	}

	// GBuffer+�K���X(����)
	m_shaders[GLSL_SHADER::GBUFFER_GLASS] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_GLASS]->LoadShaders("Shaders/DefferedRendering/GBuffer_LightGlass.vert", "Shaders/DefferedRendering/GBuffer_LightGlass.frag", ""))
	{
		return false;
	}

	//-------------------------------------------------------------------------+
	// Bloom�p�V�F�[�_�[
	//-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::DOWNSAMPLING] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DOWNSAMPLING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_DownSampling.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::DOWNSAMPLING]->UseProgram();
	m_shaders[GLSL_SHADER::DOWNSAMPLING]->SetUniform("u_scene", 0);

	m_shaders[GLSL_SHADER::GAUSSIAN_BLUR] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GAUSSIAN_BLUR]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_GaussianBlur.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::GAUSSIAN_BLUR]->UseProgram();
	m_shaders[GLSL_SHADER::GAUSSIAN_BLUR]->SetUniform("u_blurSource", 0);

	m_shaders[GLSL_SHADER::TONEMAPPING] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::TONEMAPPING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom/Bloom_ToneMapping.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::TONEMAPPING]->UseProgram();
	m_shaders[GLSL_SHADER::TONEMAPPING]->SetUniform("u_scene", 0);

	//---------------------------------------------------------------------------+
	// ���C�g�p�X
	//---------------------------------------------------------------------------+
	// �f�B���N�V���i�����C�g
	m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT]->LoadShaders("Shaders/DefferedRendering/Lighting/GBuffer_LightPass.vert", "Shaders/DefferedRendering/Lighting/GBuffer_DirectionalLight.frag", ""))
	{
		return false;
	}

	// �|�C���g���C�g
	m_shaders[GLSL_SHADER::POINT_LIGHT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::POINT_LIGHT]->LoadShaders("Shaders/DefferedRendering/Lighting/GBuffer_LightPass.vert", "Shaders/DefferedRendering/Lighting/GBuffer_PointLight.frag", ""))
	{
		return false;
	}


	//-------------------------------------------------------------------------+
    // �X�v���C�g�p�V�F�[�_�[
    //-------------------------------------------------------------------------+
	// 2D���
	m_shaders[GLSL_SHADER::SPRITE_2D] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SPRITE_2D]->LoadShaders("Shaders/Sprite/SpriteShader.vert", "Shaders/Sprite/SpriteShader.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::SPRITE_2D]->UseProgram();
	m_shaders[GLSL_SHADER::SPRITE_2D]->SetUniform("u_viewProj", screenMat);

	// 3D���
	m_shaders[GLSL_SHADER::SPRITE_3D] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SPRITE_3D]->LoadShaders("Shaders/Particle/Particle_Bloom.vert", "Shaders/Sprite/WorldSpaceSprite.frag", ""))
	{
		return false;
	}


	//-------------------------------------------------------------------------+
    // �V���h�E�}�b�v�p�V�F�[�_�[
    //-------------------------------------------------------------------------+
	// �[�x�}�b�v��������(�ʏ탁�b�V���p)
	m_shaders[GLSL_SHADER::DEPTH_MESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DEPTH_MESH]->LoadShaders("Shaders/Depth/DepthMap.vert", "Shaders/Depth/DepthMap.frag", ""))
	{
		return false;
	}
	// �[�x�}�b�v��������(�X�L�����b�V���p)
	m_shaders[GLSL_SHADER::DEPTH_SKIN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DEPTH_SKIN]->LoadShaders("Shaders/Depth/SkinnedDepth.vert", "Shaders/Depth/DepthMap.frag", ""))
	{
		return false;
	}
	// �V���h�E�`��(�ʏ탁�b�V���p)
	m_shaders[GLSL_SHADER::SHADOW_MESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SHADOW_MESH]->LoadShaders("Shaders/ForwardRendering/ShadowNormalMap.vert", "Shaders/ForwardRendering/ShadowNormalMap.frag", ""))
	{
		return false;
	}
	// �V���h�E�`��(�X�L�����b�V���p)
	m_shaders[GLSL_SHADER::SHADOW_SKIN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SHADOW_SKIN]->LoadShaders("Shaders/ForwardRendering/SkinnedShadow.vert", "Shaders/ForwardRendering/PhongShadow.frag", ""))
	{
		return false;
	}

    //-------------------------------------------------------------------------+
    // �}�b�vHUD�p�V�F�[�_�[
    //-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::HUD_INPUT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::HUD_INPUT]->LoadShaders("Shaders/HUD/HUD_MapInput.vert", "Shaders/HUD/HUD_MapInput.frag", ""))
	{
		return false;
	}

	m_shaders[GLSL_SHADER::HUD_OUTPUT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::HUD_OUTPUT]->LoadShaders("Shaders/HUD/HUD_MapOutput.vert", "Shaders/HUD/HUD_MapOutput.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::HUD_OUTPUT]->UseProgram();
	m_shaders[GLSL_SHADER::HUD_OUTPUT]->SetUniform("u_viewProj", screenMat);

	//---------------------------------------------------------------------------+
	// ���̑��̃V�F�[�_�[ (�f�o�b�O�p�r�W���A���C�U�[�n)
	//---------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OPTION_NORMAL_VISUALIZE]->LoadShaders("Shaders/Debug/Normal_Visualization.vert", "Shaders/Debug/Normal_Visualization.frag", "Shaders/Debug/Normal_Visualization.geom"))
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
