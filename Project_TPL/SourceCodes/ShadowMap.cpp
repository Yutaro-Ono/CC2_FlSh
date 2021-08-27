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

// コンストラクタ
ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	// デプスマップをバインド
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	// 深度値のみが必要なので、フォーマットはDEPTH_COMPONENT(24bit)に
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// テクスチャラッピング・フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// フレームバッファにデプスマップをアタッチする
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	// 光の視点からシーンをレンダリングする際の深度情報のみが必要
	// そのためカラーバッファを使用しないことを明示する
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*
	// シェーダの作成
	// デプスマップシェーダ
	m_depthShader = new GLSLprogram();
	m_depthShader->("Data/GLSLprograms/DepthMap.vert", "Data/GLSLprograms/DepthMap.frag");

	// ノーマルマップ + 影シェーダ
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
/// ライト視点からオブジェクトを描画し、深度マップを得る
/// </summary>
/// <param name="in_mesh"></param>     シャドウを適用するメッシュ配列
/// <param name="in_skelMesh"></param> シャドウを適用するスキンメッシュ配列
void ShadowMap::RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*> in_skelMesh, const std::vector<class CarMeshComponent*> in_carMesh)
{
	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ライト視点用のプロジェクション行列とビュー行列を用意する
	// ディレクショナルライト(平行)であるため、プロジェクション行列には正射影行列を使用
	//m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	m_lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, -10.0f, 5000.0f);

	m_lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight()->GetPosition(), RENDERER->GetDirectionalLight()->GetTargetPos(), Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// シャドウマップはレンダリング時の解像度とは異なり、シャドウマップのサイズに合わせてViewportパラメータを変更する必要がある
	// そのためglViewportを呼び出す。
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// 深度マップ書き込み用シェーダー
	GLSLprogram* depthMesh = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::DEPTH_MESH);
	GLSLprogram* depthSkin = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::DEPTH_SKIN);

	depthMesh->UseProgram();
	depthMesh->SetUniform("u_lightSpaceMatrix", m_lightSpace);

	// デプスバッファを得るためにライトから見たシーンをレンダリングする
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

	//// ビューポートを画面サイズに戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


// シャドウとメッシュの描画 (スキンメッシュは対象外)
void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh)
{
	/*
	// サンプリング用テクスチャセット
	m_shadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_shadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_shadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_shadowGLSLprogram->SetInt("u_mat.depthMap", 3);
	
	// シャドウシェーダによるメッシュ描画
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
	// ビューポートを画面サイズに戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// シャドウシェーダのアクティブ化・uniformへのセット
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

	// サンプリング用テクスチャセット
	m_shadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_shadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_shadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_shadowGLSLprogram->SetInt("u_mat.depthMap", 3);

	// シャドウシェーダによるメッシュ描画
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowGLSLprogram);
	}

	// シャドウシェーダのアクティブ化・uniformへのセット
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

	// サンプリング用テクスチャセット
	m_skinShadowGLSLprogram->SetInt("u_mat.diffuseMap", 0);
	m_skinShadowGLSLprogram->SetInt("u_mat.specularMap", 1);
	m_skinShadowGLSLprogram->SetInt("u_mat.normalMap", 2);
	m_skinShadowGLSLprogram->SetInt("u_mat.depthMap", 3);
	// シャドウシェーダによるスキンメッシュ描画
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->Draw(m_skinShadowGLSLprogram);
		}
	}
	*/
}

