//-------------------------------------------------------------------------------------------+
//
// 遅延レンダリングクラス (GBufferに各描画結果を保存・GBufferに対して光源処理を行う)
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

// コンストラクタ
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

// デストラクタ
DefferedRenderer::~DefferedRenderer()
{
}

// GBufferへの書き込み処理
void DefferedRenderer::DrawGBuffer()
{
	// マップHUD書き込み処理
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->WriteBuffer(m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::HUD_INPUT), m_renderer->m_meshComponents);
	}
	// 描画先をGBufferとしてバインドする
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// ビューポートをスクリーンサイズにセット
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0, 0.0, 0.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをON
	glEnable(GL_DEPTH_TEST);

	// ライト空間の各行列を定義
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(m_renderer->GetDirectionalLight()->GetPosition(), m_renderer->GetDirectionalLight()->GetTargetPos(), Vector3::UnitZ);
	//lightSpace = lightView * lightProj;
	lightSpace = lightProj * lightView;


	//-----------------------------------------------------------+
	// 通常メッシュ
	//-----------------------------------------------------------+
	// シェーダにuniformセット
	GLSLprogram* meshShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_NORMAL_SHADOW);
	meshShader->UseProgram();
	meshShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	meshShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	meshShader->SetUniform("u_mat.diffuseMap", 0);
	meshShader->SetUniform("u_mat.specularMap", 1);
	meshShader->SetUniform("u_mat.normalMap", 2);
	meshShader->SetUniform("u_mat.emissiveMap", 3);
	meshShader->SetUniform("u_mat.depthMap", 4);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(meshShader);
	}

	
	//------------------------------------------------------------+
	// スキンメッシュ
	//------------------------------------------------------------+
	// シェーダにuniformセット
	GLSLprogram* skinShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_SKINMESH);
	skinShader->UseProgram();
	skinShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	skinShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	skinShader->SetUniform("u_mat.diffuseMap", 0);
	skinShader->SetUniform("u_mat.specularMap", 1);
	skinShader->SetUniform("u_mat.normalMap", 2);
	skinShader->SetUniform("u_mat.emissiveMap", 3);
	skinShader->SetUniform("u_mat.depthMap", 4);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(skinShader);
	}

	//------------------------------------------------------------+
    // SkyBox
    //------------------------------------------------------------+
	GLSLprogram* skyboxShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_SKYBOX);
	// 座標系の問題でスカイボックスが正常な向きに描画されないので、回転オフセットを設定
	Matrix4 offset;
	offset = Matrix4::CreateRotationX(Math::ToRadians(90.0f));
	// Uniformに逆→転置行列をセット
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
	// 車
	//------------------------------------------------------------+
	GLSLprogram* carShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_CAR_BODY);
	carShader->UseProgram();
	carShader->SetUniform("u_lightSpaceMatrix", lightSpace);
	carShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	carShader->SetUniform("u_offset", offset);
	carShader->SetUniform("u_mat.diffuseMap", 0);
	carShader->SetUniform("u_mat.specularMap", 1);
	carShader->SetUniform("u_mat.depthMap", 2);
	// 車メッシュ描画
	for (auto car : m_renderer->m_carMeshComponents)
	{
		car->Draw(carShader);
	}


	//------------------------------------------------------------+
	// EnvironmentMap
	//------------------------------------------------------------+
	// メッシュ裏側のカリング
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	GLSLprogram* envShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_ENVIRONMENT);
	// uniformセット
	envShader->UseProgram();
	envShader->SetUniform("u_skybox", 0);
	envShader->SetUniform("u_offset", offset);
	for (auto env : m_renderer->m_envMeshComponents)
	{
		env->DrawEnvironmentMap(envShader);
	}
	// カリングのオフ
	glDisable(GL_CULL_FACE);

	//------------------------------------------------------------+
    // ライトグラス
    //------------------------------------------------------------+
	GLSLprogram* glassShader = RENDERER->GetShaderManager()->GetShader(GLSL_SHADER::GBUFFER_GLASS);
	glassShader->UseProgram();
	glassShader->SetUniform("u_skybox", 0);
	for (auto light : m_renderer->m_lightGlassComponents)
	{
		light->Draw(glassShader);
	}


	// GBufferのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

/// <summary>
/// SSAOパス
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

	// カーネルをシェーダーへ送信
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

	// スクリーン全体に描画
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//----------------------------------------------------------------------+
    // 2.SSAOブラー効果
    //----------------------------------------------------------------------+
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoBlurShader->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);

	ssaoBlurShader->SetUniform("u_ssaoInput", 0);

	// スクリーン全体に描画
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// GBufferを元にライティング計算を行う
void DefferedRenderer::DrawLightPath()
{
	// ライトバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// ブレンド指定
	glEnablei(GL_BLEND, 0);                                          // 加算合成を行うためブレンドを有効化
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // 加算合成のブレンドを指定
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをオフ
	glDisable(GL_DEPTH_TEST);

	// gBufferの各テクスチャをバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	// SSAOテクスチャをバインド
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);

	// カリング設定：ライトはメッシュの裏側のみ描画する
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//------------------------------------------------------+
	// ポイントライト
	//------------------------------------------------------+
	// ポイントライトシェーダへのセット
	GLSLprogram* pointLightShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::POINT_LIGHT);
	pointLightShader->UseProgram();
	pointLightShader->SetUniform("u_gBuffer.position",     0);
	pointLightShader->SetUniform("u_gBuffer.normal",       1);
	pointLightShader->SetUniform("u_gBuffer.albedoSpec",   2);
	pointLightShader->SetUniform("u_gBuffer.emissive",     3);
	pointLightShader->SetUniform("u_ssao",                 4);
	// ポイントライトの描画
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(pointLightShader);
	}


	//------------------------------------------------------+
	// スポットライト
	//------------------------------------------------------+
	// スポットライトシェーダへのセット
	//m_spotLightShader->UseProgram();
	//m_spotLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	//m_spotLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	//m_spotLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	//m_spotLightShader->SetInt("u_gBuffer.pos", 0);
	//m_spotLightShader->SetInt("u_gBuffer.normal", 1);
	//m_spotLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	//m_spotLightShader->SetInt("u_gBuffer.emissive", 3);
	//// スポットライトの描画
	//for (auto spotL : m_renderer->m_spotLights)
	//{
	//	spotL->Draw(m_spotLightShader);
	//}

	// カリングのオフ
	glDisable(GL_CULL_FACE);


	//-----------------------------------------------+
	// ディレクショナルライトパス
	//-----------------------------------------------+
	// 輝度定義
	float intensity = 1.35f;
	m_renderer->GetDirectionalLight()->SetIntensity(intensity);
	GLSLprogram* dirLightShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::DIRECTIONAL_LIGHT);
	// シェーダのセット
	dirLightShader->UseProgram();
	dirLightShader->SetUniform("u_gBuffer.position",   0);
	dirLightShader->SetUniform("u_gBuffer.normal",     1);
	dirLightShader->SetUniform("u_gBuffer.albedoSpec", 2);
	dirLightShader->SetUniform("u_gBuffer.emissive",   3);
	dirLightShader->SetUniform("u_ssao",               4);
	// スクリーン全体に描画
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Spriteの描画
	// ブレンドのアクティブ化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 深度テストの停止
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// ワールド空間上のスプライト描画
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

	// spriteシェーダーのアクティブ化
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
	// 全てのUIを更新
	sprite2Dshader->UseProgram();
	sprite2Dshader->SetUniform("u_intensity", 1.0f);
	RENDERER->SetActiveSpriteVAO();
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(sprite2Dshader);
	}

	// マップHUD
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->DrawMap(m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::HUD_OUTPUT));
	}

	// ブレンドをオフ
	glDisable(GL_BLEND);

	// ブレンドを停止する
	glDisablei(GL_BLEND, 0);

	// gBufferの深度情報をライトバッファへコピーする
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);              // gBufferを読み取りフレームバッファとして指定
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // 書き込みバッファをライトバッファに指定
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ライトバッファ描画へ戻す
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// 深度テストをオン
	glEnable(GL_DEPTH_TEST);
	
	//----------------------------------------------------------------+
    // パーティクル描画
    //----------------------------------------------------------------+
	//m_renderer->GetParticleManager()->Draw();


	// ライトFBOへの書き込みを止める
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 本描画処理
void DefferedRenderer::Draw()
{

	// GBufferへの書き込み
	DrawGBuffer();

	// SSAO処理
	//DrawSSAOPath();

	// ライトバッファへの書き込み
	DrawLightPath();

	// Bloom処理を施した描画
	RenderBloom* bloom = m_renderer->GetBloom();
	bloom->DrawDownSampling(m_lightHighBright);
	bloom->DrawGaussBlur();
	bloom->DrawBlendBloom(m_lightHDR);

	//----------------------------------------------------------------+
	// 最終出力結果を描画
	//----------------------------------------------------------------+
	// GBufferに書き込まれた要素をスクリーンに描画
	//GLSLprogram* screenShader = m_renderer->GetShaderManager()->GetShader(GLSL_SHADER::OUT_SCREEN);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	//// スクリーンシェーダのuniformセット
	//screenShader->UseProgram();
	//screenShader->SetUniform("u_screenTexture", 0);

	//// GBufferテクスチャセット
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	////glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	////glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	//// スクリーンに描画
	//m_renderer->m_screenVerts->SetActive();
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

// GBufferを作成する
// 座標用バッファ・法線用バッファ・アルベド＆スペキュラバッファ・レンダーバッファ
bool DefferedRenderer::GenerateGBuffer()
{
	// GBufferの登録・バインド
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//----------------------------------------------------------------+
	// 各バッファを登録し、2DテクスチャとしてGBufferに紐付ける
	//----------------------------------------------------------------+
	// 3D空間座標バッファ (浮動小数点バッファ/カラー0番目として登録)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// 法線ベクトルバッファ (浮動小数点バッファ/カラー1番目として登録)
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// アルベド(RGB)＆スペキュラ(A)用カラーバッファ (A成分含む8bitカラーバッファ/2番目として登録)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// 高輝度バッファの作成 (エミッシブ出力用輝度バッファ/3番目として登録)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);


	// 各テクスチャをGBufferの描画先としてGL側に明示する
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

	// レンダーバッファの作成 (ステンシルバッファとして定義)
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::GBUFFER::Generate Failed" << std::endl;
		return false;
	}
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/// <summary>
/// SSAO用のバッファ生成処理
/// </summary>
/// <returns></returns>
bool DefferedRenderer::GenerateSSAOBuffer()
{
	// SSAO用フレームバッファの登録
	glGenFramebuffers(1, &m_ssaoFBO);
	// ssaoフレームバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	// SSAO用カラーバッファの登録
	glGenTextures(1, &m_ssaoColor);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColor, 0);
	// フレームバッファの不具合をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::SSAO::Generate Failed" << std::endl;
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// SSAO用フレームバッファの登録
	glGenFramebuffers(1, &m_ssaoBlurFBO);
	// ssaoブラーバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	// SSAO用カラーバッファの登録
	glGenTextures(1, &m_ssaoBlurColor);
	glBindTexture(GL_TEXTURE_2D, m_ssaoBlurColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurColor, 0);
	// フレームバッファの不具合をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::SSAO::Generate Failed" << std::endl;
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// バインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// サンプリング用カーネルの生成
	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);   // ランダム生成用(0.0f ～ 1.0f)
	for (unsigned int i = 0; i < 64; ++i)
	{
		Vector3 sample(randomFloats(m_generator) * 2.0f - 1.0f, randomFloats(m_generator) * 2.0f - 1.0f, randomFloats(m_generator));
		sample.Normalize();
		sample *= randomFloats(m_generator);
		float scale = static_cast<float>(i) / 64.0f;

		// sampleをS.T.にスケーリングすることで、カーネルの中心に一致させる
		scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
		sample *= scale;
		m_ssaoKernel.push_back(sample);
	}

	// ノイズテクスチャの生成
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

// ライトバッファを作成する
// 光源処理用のHDRバッファ・レンダーバッファ
bool DefferedRenderer::GenerateLightBuffer()
{
	glGenFramebuffers(1, &m_lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// HDRバッファの作成
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// 高輝度バッファの作成
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// アタッチメント設定
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

	// レンダーバッファを作成する
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::DefferedRenderer::LIGHTBUFFER::Generate False" << std::endl;
		return false;
	}
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return true;
}


/// <summary> 
/// Defferedレンダリングに用いる変数の初期化・インスタンスの生成
/// </summary>
/// <returns></returns>
bool DefferedRenderer::Initialize()
{
	// Gバッファ・SSAOバッファ・ライトバッファの生成
	GenerateGBuffer();
	GenerateSSAOBuffer();
	GenerateLightBuffer();

	// デバッグオブジェクト
#ifdef _DEBUG

	DefferedRendererDebugObject* defDebugObj = new DefferedRendererDebugObject(this);
	DEBUGGER->AddDebugObject(defDebugObj, Debugger::DEBUG_TAG::SYSTEM);

#endif

	return true;
}