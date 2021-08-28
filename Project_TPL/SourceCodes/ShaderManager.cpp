//----------------------------------------------------------------------------------+
// @file        ShaderManager.cpp
// @brief       シェーダークラスの管理
//              シェーダープログラムを種別ごとにmapに保存
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "ShaderManager.h"
#include "GameMain.h"
#include "Renderer.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	// シェーダー連想配列の削除
	auto iter = m_shaders.begin();
	while (iter != m_shaders.end())
	{
		m_shaders.erase(iter++);
	}
}

/// <summary>
/// シェーダークラスの生成
/// </summary>
/// <returns> シェーダープログラムの作成に失敗した場合にfalseを返す </returns>
bool ShaderManager::CreateShaders()
{
	// uniform用
	Matrix4 screenMat = Matrix4::CreateSimpleViewProj(RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());



	// 画面出力用シェーダー
	m_shaders[GLSL_SHADER::OUT_SCREEN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::OUT_SCREEN]->LoadShaders("Shaders/OutScreen.vert", "Shaders/OutScreen.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::OUT_SCREEN]->UseProgram();
	m_shaders[GLSL_SHADER::OUT_SCREEN]->SetUniform("u_screenTexture", 0);


	//-------------------------------------------------------------------------+
	// 標準シェーダー
	//-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::BASIC] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC]->LoadShaders("Shaders/ForwardRendering/BasicMesh.vert", "Shaders/ForwardRendering/BasicMesh.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC]->UseProgram();
	m_shaders[GLSL_SHADER::BASIC]->SetUniform("u_mat.albedo", 0);
	
	// 標準シェーダー+Phongライティング
	m_shaders[GLSL_SHADER::BASIC_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_PHONG]->LoadShaders("Shaders/ForwardRendering/Phong.vert", "Shaders/ForwardRendering/Phong.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::BASIC_PHONG]->UseProgram();
	m_shaders[GLSL_SHADER::BASIC_PHONG]->SetUniform("u_mat.albedo", 0);

	// 標準シェーダー+Phongライティング+シャドウ
	m_shaders[GLSL_SHADER::BASIC_PHONG_SHADOW] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_PHONG_SHADOW]->LoadShaders("Shaders/ForwardRendering/PhongShadow.vert", "Shaders/ForwardRendering/PhongShadow.frag", ""))
	{
		return false;
	}

	// 標準シェーダー+スキンメッシュ
	m_shaders[GLSL_SHADER::BASIC_SKINMESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_SKINMESH]->LoadShaders("Shaders/ForwardRendering/Skinned.vert", "Shaders/ForwardRendering/Phong.frag", ""))
	{
		return false;
	}

	// 標準シェーダー+SkyBox
	m_shaders[GLSL_SHADER::BASIC_SKYBOX] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::BASIC_SKYBOX]->LoadShaders("Shaders/ForwardRendering/SkyBox.vert", "Shaders/ForwardRendering/SkyBox.frag", ""))
	{
		return false;
	}

    //-------------------------------------------------------------------------+
    // 遅延レンダリング用シェーダー
    //-------------------------------------------------------------------------+
	m_shaders[GLSL_SHADER::GBUFFER_BASIC] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_BASIC]->LoadShaders("Shaders/DefferedRendering/GBuffer_Basic.vert", "Shaders/DefferedRendering/GBuffer_Basic.frag", ""))
	{
		return false;
	}

	// GBuffer+Phongライティング
	m_shaders[GLSL_SHADER::GBUFFER_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_PHONG]->LoadShaders("Shaders/DefferedRendering/GBuffer_Phong.vert", "Shaders/DefferedRendering/GBuffer_Phong.frag", ""))
	{
		return false;
	}

	// GBuffer+法線マップ
	m_shaders[GLSL_SHADER::GBUFFER_NORMAL] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_NORMAL]->LoadShaders("Shaders/DefferedRendering/GBuffer_NormalMap.vert", "Shaders/DefferedRendering/GBuffer_NormalMap.frag", ""))
	{
		return false;
	}

	// GBuffer+法線マップ+シャドウ
	m_shaders[GLSL_SHADER::GBUFFER_NORMAL_SHADOW] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_NORMAL_SHADOW]->LoadShaders("Shaders/DefferedRendering/GBuffer_Normal_Shadow.vert", "Shaders/DefferedRendering/GBuffer_Normal_Shadow.frag", ""))
	{
		return false;
	}

	// GBufferスキンメッシュ
	m_shaders[GLSL_SHADER::GBUFFER_SKINMESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_SKINMESH]->LoadShaders("Shaders/DefferedRendering/GBuffer_SkinNormShadow.vert", "Shaders/DefferedRendering/GBuffer_Shadow.frag", ""))
	{
		return false;
	}

	// GBuffer+スカイボックス
	m_shaders[GLSL_SHADER::GBUFFER_SKYBOX] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_SKYBOX]->LoadShaders("Shaders/DefferedRendering/GBuffer_Basic_SkyBox.vert", "Shaders/DefferedRendering/GBuffer_Basic_SkyBox.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::GBUFFER_SKYBOX]->SetUniform("u_cubeMap", 0);

	// GBuffer+環境マップ
	m_shaders[GLSL_SHADER::GBUFFER_ENVIRONMENT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_ENVIRONMENT]->LoadShaders("Shaders/DefferedRendering/GBuffer_EnvironmentMap.vert", "Shaders/DefferedRendering/GBuffer_EnvironmentMap.frag", ""))
	{
		return false;
	}

	// GBuffer+車ボディ用
	m_shaders[GLSL_SHADER::GBUFFER_CAR_BODY] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_CAR_BODY]->LoadShaders("Shaders/DefferedRendering/GBuffer_CarShaderReflect.vert", "Shaders/DefferedRendering/GBuffer_CarShaderReflect.frag", ""))
	{
		return false;
	}

	// GBuffer+ガラス(反射)
	m_shaders[GLSL_SHADER::GBUFFER_GLASS] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::GBUFFER_GLASS]->LoadShaders("Shaders/DefferedRendering/GBuffer_LightGlass.vert", "Shaders/DefferedRendering/GBuffer_LightGlass.frag", ""))
	{
		return false;
	}

	//-------------------------------------------------------------------------+
	// Bloom用シェーダー
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
	// ライトパス
	//---------------------------------------------------------------------------+
	// ディレクショナルライト
	m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DIRECTIONAL_LIGHT]->LoadShaders("Shaders/DefferedRendering/Lighting/GBuffer_LightPass.vert", "Shaders/DefferedRendering/Lighting/GBuffer_DirectionalLight.frag", ""))
	{
		return false;
	}

	// ポイントライト
	m_shaders[GLSL_SHADER::POINT_LIGHT] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::POINT_LIGHT]->LoadShaders("Shaders/DefferedRendering/Lighting/GBuffer_LightPass.vert", "Shaders/DefferedRendering/Lighting/GBuffer_PointLight.frag", ""))
	{
		return false;
	}


	//-------------------------------------------------------------------------+
    // スプライト用シェーダー
    //-------------------------------------------------------------------------+
	// 2D空間
	m_shaders[GLSL_SHADER::SPRITE_2D] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SPRITE_2D]->LoadShaders("Shaders/Sprite/SpriteShader.vert", "Shaders/Sprite/SpriteShader.frag", ""))
	{
		return false;
	}
	m_shaders[GLSL_SHADER::SPRITE_2D]->UseProgram();
	m_shaders[GLSL_SHADER::SPRITE_2D]->SetUniform("u_viewProj", screenMat);

	// 3D空間
	m_shaders[GLSL_SHADER::SPRITE_3D] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SPRITE_3D]->LoadShaders("Shaders/Particle/Particle_Bloom.vert", "Shaders/Sprite/WorldSpaceSprite.frag", ""))
	{
		return false;
	}


	//-------------------------------------------------------------------------+
    // シャドウマップ用シェーダー
    //-------------------------------------------------------------------------+
	// 深度マップ書き込み(通常メッシュ用)
	m_shaders[GLSL_SHADER::DEPTH_MESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DEPTH_MESH]->LoadShaders("Shaders/Depth/DepthMap.vert", "Shaders/Depth/DepthMap.frag", ""))
	{
		return false;
	}
	// 深度マップ書き込み(スキンメッシュ用)
	m_shaders[GLSL_SHADER::DEPTH_SKIN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::DEPTH_SKIN]->LoadShaders("Shaders/Depth/SkinnedDepth.vert", "Shaders/Depth/DepthMap.frag", ""))
	{
		return false;
	}
	// シャドウ描画(通常メッシュ用)
	m_shaders[GLSL_SHADER::SHADOW_MESH] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SHADOW_MESH]->LoadShaders("Shaders/ForwardRendering/ShadowNormalMap.vert", "Shaders/ForwardRendering/ShadowNormalMap.frag", ""))
	{
		return false;
	}
	// シャドウ描画(スキンメッシュ用)
	m_shaders[GLSL_SHADER::SHADOW_SKIN] = new GLSLprogram();
	if (!m_shaders[GLSL_SHADER::SHADOW_SKIN]->LoadShaders("Shaders/ForwardRendering/SkinnedShadow.vert", "Shaders/ForwardRendering/PhongShadow.frag", ""))
	{
		return false;
	}

    //-------------------------------------------------------------------------+
    // マップHUD用シェーダー
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
	// その他のシェーダー (デバッグ用ビジュアライザー系)
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
/// 指定したタイプのシェーダープログラムの有効化
/// </summary>
/// <param name=Shader::SHADER_TYPE> シェーダーのタイプ </param>
void ShaderManager::EnableShaderProgram(GLSL_SHADER _type)
{
	m_shaders[_type]->UseProgram();
}

/// <summary>
/// シェーダークラスのポインタのゲッター
/// </summary>
/// <param name="_type"> シェーダーのタイプ </param>
/// <returns> 指定したタイプのシェーダークラスポインタを返す </returns>
GLSLprogram* ShaderManager::GetShader(GLSL_SHADER _type)
{
	return m_shaders[_type];
}
