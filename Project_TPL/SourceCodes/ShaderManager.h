//----------------------------------------------------------------------------------+
// @file        ShaderManager.h
// @brief       シェーダークラスの管理
//              シェーダープログラムを種別ごとにmapに保存
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include "GLSLprogram.h"

enum class GLSL_SHADER : unsigned char
{

	BASIC,
	BASIC_PHONG,
	BASIC_NORMAL,
	BASIC_SKINMESH,

	GBUFFER_BASIC,
	GBUFFER_PHONG,

	GBUFFER_NORMAL,
	GBUFFER_PHONG_NORMAL,
	GBUFFER_NORMAL_SHADOW,
	GBUFFER_SKINMESH,
	GBUFFER_SKYBOX,
	GBUFFER_ENVIRONMENT,
	GBUFFER_CAR_BODY,
	GBUFFER_GLASS,

	DOWNSAMPLING,
	GAUSSIAN_BLUR,
	TONEMAPPING,

	BASIC_ENVIRONMENT,
	GBUFFER_ENVIRONMENT,

	SPRITE_2D,
	SPRITE_3D,
	HUD_INPUT,
	HUD_OUTPUT,

	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	OUT_SCREEN,
	OPTION_NORMAL_VISUALIZE,
	OPTION_NORMAL_VISUALIZE_GBUFFER
};

class ShaderManager
{

public:

	ShaderManager();
	~ShaderManager();

	bool CreateShaders();

	void EnableShaderProgram(GLSL_SHADER _type);

	class GLSLprogram* GetShader(GLSL_SHADER _type);


private:

	// シェーダークラス格納用の連想配列
	std::unordered_map<GLSL_SHADER, class GLSLprogram*> m_shaders;

};