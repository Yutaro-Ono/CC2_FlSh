#include "Renderer.h"
#include "GameMain.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshGpmesh.h"
#include "MeshObj.h"
#include "Skeleton.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "SkeletalMeshComponent.h"
#include "CubeMapComponent.h"
#include "EnvironmentMapComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Animation.h"
#include "ParticleManager.h"
#include "UIScreen.h"
#include "PostEffect.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "WorldSpaceUI.h"
#include "ShadowMap.h"
#include "CameraComponent.h"
#include "RenderBloom.h"
#include "ForwardRenderer.h"
#include "DefferedRenderer.h"
#include "ShaderManager.h"
#include "../imgui/imconfig.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"
#include <iostream>
#include "DirectionalLight.h"
#include "EffekseerEffect.h"
#include "SkyBox.h"
#include "Debugger.h"

// コンストラクタ
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_uboMatrices(0)
	,m_postEffect(nullptr)
	,m_bloom(nullptr)
	,m_switchShader(0)
	,m_cameraPos(Vector3::Zero)
	,m_fRenderer(nullptr)
	,m_dRenderer(nullptr)
	,m_mapHUD(nullptr)
	,m_shaderManager(nullptr)
{
	m_enableBloom = GAME_CONFIG->GetEnableBloom();
	m_visualizeNormal = false;

	// 描画方法の設定
	if (GAME_CONFIG->GetEnableDeferred())
	{
		m_renderMode = RENDER_MODE::DEFFERED;
	}
}

// デストラクタ
Renderer::~Renderer()
{
}

// 各種初期化処理 (GL SDL)
bool Renderer::Initialize(int in_screenW, int in_screenH, bool in_full)
{
	// 画面サイズを設定
	m_screenWidth = in_screenW;
	m_screenHeight = in_screenH;

	//-----------------------------------------------------------------+
    // OpenGLの属性設定
    //-----------------------------------------------------------------+
    // OpenGLコアプロファイルを使用する
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// 指定 -> OpenGL/ver4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);     // メジャーバージョン
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);     // マイナーバージョン
	// RGBA各チャンネルに8bitのカラーバッファを指定する
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                  // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);                // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);                 // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);                // A
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// ダブルバッファを有効にする
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// ハードウェアアクセラレーションを使用する
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDLウィンドウの作成
	m_window = SDL_CreateWindow
	(
		"ProjectTPL",                                     // ウィンドウの名称
		0,                                                // x座標のウィンドウ描画原点
		0,                                                // y座標のウィンドウ描画原点
		m_screenWidth,                                    // 画面の横幅
		m_screenHeight,                                   // 画面の縦幅
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	// ウィンドウの作成に失敗したら
	if (!m_window)
	{
		std::cout << "FAILED::SDL::CreateWindow::" << SDL_GetError() << std::endl;
		return false;
	}
	// フルスクリーン化処理
	if (in_full)
	{
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	// wminfo構造体
	SDL_SysWMinfo wminfo;
	SDL_VERSION(&wminfo.version);
	SDL_GetWindowWMInfo(m_window, &wminfo);
	m_hwnd = wminfo.info.win.window;
	
    //----------------------------------------------------------------+
    // OpenGLコンテキスト設定
    //----------------------------------------------------------------+
    // OpenGLコンテキストを生成
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

    //----------------------------------------------------------------+
    // GLEW初期化
    //----------------------------------------------------------------+
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "FAILED::GLEW::glewInit()" << std::endl;
		return false;
	}
	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();


	// ビューポート設定
	glViewport(0, 0, m_screenWidth, m_screenHeight);

	//-----------------------------------------------------------------+
	// SDLレンダラーの作成
	//-----------------------------------------------------------------+
	m_SDLRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_SDLRenderer)
	{
		std::cout << "FAILED::SDL::CreateRenderer::" << SDL_GetError() << std::endl;
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << "FAILED::SDL_Image::INIT_PNG::" << SDL_GetError() << std::endl;
		return false;
	}

	//--------------------------------------------+
	// Effekseer初期化
	//--------------------------------------------+
	/*
	m_effekseerRenderer = ::EffekseerRendererGL::Renderer::Create(8000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
	m_effekseerManager = ::Effekseer::Manager::Create(8000);
	// 描画モジュール作成
	m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());
	// Effekseer用のテクスチャ・モデル・マテリアルローダー
	m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	m_effekseerManager->SetMaterialLoader(m_effekseerRenderer->CreateMaterialLoader());
	*/


	// 初期化に成功
	return true;
}

bool Renderer::Load()
{
	//------------------------------------------------------------------+
    // パーティクル関連
    //------------------------------------------------------------------+
    // パーティクルマネージャー作成
	m_particleManager = new ParticleManager;

	//------------------------------------------------------------------+
	// 頂点配列オブジェクト系の生成
	//------------------------------------------------------------------+
	// パーティクル用
	m_particleVerts = new VertexArray();
	m_particleVerts->CreateSpriteVerts();
	// スプライト用
	m_spriteVerts = new VertexArray();
	m_spriteVerts->CreateSpriteVerts();
	// キューブマップ用
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeVerts();
	// 画面出力用
	m_screenVerts = new VertexArray();
	m_screenVerts->CreateScreenVerts();



	//--------------------------------------------+
	// uniform Buffer Object
	//--------------------------------------------+
	CreateUBOs();

	//------------------------------------------------------------------+
    // ポストエフェクト
    //------------------------------------------------------------------+
	m_postEffect = new PostEffect();
	//m_postEffect->Load();

	//--------------------------------------------+
	// シャドウマップ
	//--------------------------------------------+
	m_shadowMap = new ShadowMap();

	//--------------------------------------------+
	// Bloom
	//--------------------------------------------+
	m_bloom = new RenderBloom();

	//--------------------------------------------+
	// Forward・DeferedRenderer
	//--------------------------------------------+
	m_fRenderer = new ForwardRenderer(this);
	m_dRenderer = new DefferedRenderer(this);
	if (!m_dRenderer->Initialize())
	{
		return false;
	}

	//--------------------------------------------+
    // シェーダー管理クラス
    //--------------------------------------------+
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "ERROR::ShaderManager::Func::CreateShaders()" << std::endl;
		return false;
	}

	// ディレクショナルライト
	m_dirLight = new DirectionalLight();

	// 仮のスカイボックス生成(キューブマップ)
	SkyBox* skybox = new SkyBox(ENVIRONMENT_TYPE::NIGHT);
	m_activeSkyBox = skybox->GetCubeMapComp();

	return true;
}

void Renderer::Delete()
{
	// テクスチャの解放
	for (auto i : m_textures)
	{
		printf("Textures Release : %s\n", i.first.c_str());
		i.second->Delete();
		delete i.second;
	}
	m_textures.clear();
	// パーティクルマネージャー破棄
	delete m_particleManager;

	// メッシュの解放
	for (auto i : m_meshes)
	{
		printf("Meshes Release : %s\n", i.first.c_str());
		i.second->Delete();
		delete i.second;
	}
	m_meshes.clear();
	// スケルトンの破棄
	for (auto sk : m_skeletons)
	{
		delete sk.second;
	}
	m_skeletons.clear();
	// アニメーションの破棄
	for (auto anim : m_animations)
	{
		delete anim.second;
	}
	m_animations.clear();

	// 車メッシュ配列の解放
	for (auto car : m_carMeshComponents)
	{
		delete car;
	}
	m_carMeshComponents.clear();
	// ライトグラス配列の解放
	for (auto light : m_lightGlassComponents)
	{
		delete light;
	}
	m_lightGlassComponents.clear();
	// 環境マップオブジェクト配列の解放
	for (auto env : m_envMeshComponents)
	{
		delete env;
	}
	m_envMeshComponents.clear();

	// スプライトの解放
	for (auto sprite : m_worldSprites)
	{
		delete sprite;
	}
	m_worldSprites.clear();
	// スプライトの解放
	for (auto sprite : m_spriteComponents)
	{
		delete sprite;
	}
	m_spriteComponents.clear();
	// ポイントライト配列の解放
	for (auto light : m_pointLights)
	{
		delete light;
	}
	// スポットライト配列の解放
	for (auto spLight : m_spotLights)
	{
		delete spLight;
	}

	// シェーダー管理クラスの解放
	delete m_shaderManager;

	delete m_dirLight;
	delete m_spriteVerts;
	delete m_cubeVerts;
	delete m_fRenderer;
	delete m_dRenderer;
	delete m_postEffect;
	delete m_shadowMap;
	delete m_bloom;

	/*
	// Effekseer関連の解放
	m_effekseerManager.Reset();
	m_effekseerRenderer.Reset();
	*/

	// コンテキストの破棄
	SDL_GL_DeleteContext(m_context);
	// ウィンドウの破棄
	SDL_DestroyWindow(m_window);
}

// 描画処理
void Renderer::Draw()
{

	//------------------------------------------------+
	// レンダリング (Forward or Deffered)
	//------------------------------------------------+
	// 共通処理
	// ディレクショナルライトの更新
	m_dirLight->Update();
	// uniformバッファへ共通情報を格納する
	UpdateUBO();

	// シャドウ描画用の深度マップにライト視点から見た空間で書き込む
	//m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents, m_carMeshComponents);
	// ここから分岐
	if (m_renderMode == RENDER_MODE::FORWARD)
	{
		m_fRenderer->Draw();
	}
	else if (m_renderMode == RENDER_MODE::DEFFERED)
	{
		m_dRenderer->Draw();
	}

#ifdef _DEBUG

	// デバッグの描画
	GAME_INSTANCE.DebugRenderer();

#endif

	// 画面のスワップ
	SDL_GL_SwapWindow(m_window);
}


void Renderer::AddSprite(SpriteComponent * in_sprite)
{

	int myDrawOrder = in_sprite->GetDrawOrder();
	auto iter = m_spriteComponents.begin();
	for (;
		iter != m_spriteComponents.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレータで指定した番号にスプライトを挿入
	m_spriteComponents.insert(iter, in_sprite);
}

// 指定したスプライトの削除
void Renderer::RemoveSprite(SpriteComponent * in_sprite)
{
	// イテレータで削除指示の出たスプライトを探索
	auto iter = std::find(m_spriteComponents.begin(), m_spriteComponents.end(), in_sprite);
	m_spriteComponents.erase(iter);
}

// ワールド上のスプライトを配列に追加
void Renderer::AddSpriteWorld(WorldSpaceUI* in_sprite)
{
	m_worldSprites.push_back(in_sprite);
}

// ワールドスプライト配列から指定したスプライトを削除
void Renderer::RemoveSpriteWorld(WorldSpaceUI* in_sprite)
{
	auto iter = std::find(m_worldSprites.begin(), m_worldSprites.end(), in_sprite);
	m_worldSprites.erase(iter);
}

// メッシュコンポーネント追加
void Renderer::AddMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		m_skeletalMeshComponents.emplace_back(sk);
	}
	else
	{
		m_meshComponents.emplace_back(in_mesh);
	}

}

// メッシュコンポーネント格納配列から削除
void Renderer::RemoveMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		auto iter = std::find(m_skeletalMeshComponents.begin(), m_skeletalMeshComponents.end(), sk);
		m_skeletalMeshComponents.erase(iter);
	}
	else
	{
		auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), in_mesh);
		m_meshComponents.erase(iter);
	}

}
/// <summary>
/// 車メッシュ格納
/// </summary>
/// <param name="in_carMesh">追加する車用メッシュコンポーネント</param>
void Renderer::AddCarMeshComponent(CarMeshComponent* in_carMesh)
{
	m_carMeshComponents.emplace_back(in_carMesh);
}
/// <summary>
/// 車メッシュ削除
/// </summary>
/// <param name="in_carMesh">削除する車用メッシュコンポーネント</param>
void Renderer::RemoveCarMeshComponent(CarMeshComponent* in_carMesh)
{
	auto iter = std::find(m_carMeshComponents.begin(), m_carMeshComponents.end(), in_carMesh);
	m_carMeshComponents.erase(iter);
}

// 環境マップオブジェクト配列への追加
void Renderer::AddEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	m_envMeshComponents.push_back(in_envMesh);
}

// 環境マップオブジェクト配列から削除
void Renderer::RemoveEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	auto iter = std::find(m_envMeshComponents.begin(), m_envMeshComponents.end(), in_envMesh);
	m_envMeshComponents.erase(iter);
}

void Renderer::AddLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	m_lightGlassComponents.push_back(in_glassMesh);
}

void Renderer::RemoveLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	auto iter = std::find(m_lightGlassComponents.begin(), m_lightGlassComponents.end(), in_glassMesh);
	m_lightGlassComponents.erase(iter);
}

/// <summary>
/// ポイントライトコンポーネント追加
/// 追加されると描画に適用される
/// </summary>
/// <param name="in_pointL">追加するポイントライトクラス</param>
void Renderer::AddPointLightComponent(PointLightComponent* in_pointL)
{
	m_pointLights.push_back(in_pointL);
}

/// <summary>
/// ポイントライトコンポーネント削除
/// </summary>
/// <param name="in_pointL">削除するポイントライトクラス</param>
void Renderer::RemovePointLightComponent(PointLightComponent* in_pointL)
{
	auto iter = std::find(m_pointLights.begin(), m_pointLights.end(), in_pointL);
	m_pointLights.erase(iter);
}

/// <summary>
/// スポットライトコンポーネント追加
/// </summary>
/// <param name="in_spotL">追加するスポットライトコンポーネント</param>
void Renderer::AddSpotLightComponent(SpotLightComponent* in_spotL)
{
	m_spotLights.push_back(in_spotL);
}

/// <summary>
/// スポットライトコンポーネント削除
/// </summary>
/// <param name="in_spotL">削除するスポットライトコンポーネント</param>
void Renderer::RemoveSpotLightComponent(SpotLightComponent* in_spotL)
{
	auto iter = std::find(m_spotLights.begin(), m_spotLights.end(), in_spotL);
	m_spotLights.erase(iter);
}

/// <summary>
/// マップHUDの単体削除
/// </summary>
void Renderer::RemoveMapHUD()
{
	delete m_mapHUD;
	m_mapHUD = nullptr;
}

// 指定したテクスチャの削除
void Renderer::RemoveTexture(Texture* in_texture)
{
	//auto iter = std::find(m_textures.begin(), m_textures.end(), in_texture);
	//m_textures.erase(iter);

	for (auto iter : m_textures)
	{
		if (iter.second == in_texture)
		{
			m_textures.erase(iter.first);

			break;
		}
	}
}

// ウィンドウのタイトル
void Renderer::SetWindowTitle(const std::string & in_title)
{
	SDL_SetWindowTitle(m_window, in_title.c_str());
}

// パーティクルの頂点情報をバインドしアクティブ化
void Renderer::SetWorldSpriteVertex()
{
	m_particleVerts->SetActive();
}


// スプライト頂点配列のアクティブ化
void Renderer::SetActiveSpriteVAO()
{
	m_spriteVerts->SetActive();
}

/// <summary>
/// 各種UBOの生成処理
/// </summary>
void Renderer::CreateUBOs()
{
	// 変換行列UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 128);
	// カメラUBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, 16);
	// トリガーUBO
	glGenBuffers(1, &m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, m_uboTriggers, 0, 16);
	// ディレクショナルライトUBO
	glGenBuffers(1, &m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 3, m_uboDirLights, 0, 64);
}

/// <summary>
/// UBOに値をセットする
/// </summary>
void Renderer::UpdateUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);

	// 変換行列UBO
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, m_projMat.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, m_viewMat.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラUBO
	Vector3 camPos = m_viewMat.GetTranslation();
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, camPos.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// トリガーFBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	int bloom = static_cast<int>(GAME_CONFIG->GetEnableBloom());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &bloom);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// ディレクショナルライト
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, m_dirLight->GetDirection().GetAsFloatPtr());
	// ※ストライドが合わない+最初のfloatがスルーされるので
	// ※対策として最初が0.0fのVector2に一個ずつ値を突っ込む→UBOの最後から値を送信
	Vector2 diff1 = Vector2(0.0f, m_dirLight->GetDiffuse().x);
	Vector2 diff2 = Vector2(0.0f, m_dirLight->GetDiffuse().y);
	Vector2 diff3 = Vector2(0.0f, m_dirLight->GetDiffuse().z);
	Vector2 spec1 = Vector2(0.0f, m_dirLight->GetSpecular().x);
	Vector2 spec2 = Vector2(0.0f, m_dirLight->GetSpecular().y);
	Vector2 spec3 = Vector2(0.0f, m_dirLight->GetSpecular().z);
	Vector2 amb1 = Vector2(0.0f, m_dirLight->GetAmbient().x);
	Vector2 amb2 = Vector2(0.0f, m_dirLight->GetAmbient().y);
	Vector2 amb3 = Vector2(0.0f, m_dirLight->GetAmbient().z);
	Vector2 intens = Vector2(0.0f, m_dirLight->GetIntensity());
	glBufferSubData(GL_UNIFORM_BUFFER, 56, 8, intens.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 52, 8, amb3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 48, 8, amb2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 44, 8, amb1.GetAsFloatPtr());

	glBufferSubData(GL_UNIFORM_BUFFER, 36, 8, spec3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 32, 8, spec2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 28, 8, spec1.GetAsFloatPtr());

	glBufferSubData(GL_UNIFORM_BUFFER, 20, 8, diff3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 8, diff2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 8, diff1.GetAsFloatPtr());

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// テクスチャの取得
Texture * Renderer::GetTexture(const std::string & in_fileName)
{
	Texture* tex = nullptr;
	auto iter = m_textures.find(in_fileName);     // 同じパスのテクスチャがあるかを検索
	// 同じテクスチャが登録してあった
	if (iter != m_textures.end())
	{
		// 同じテクスチャを指す
		tex = iter->second;
	}
	else
	{
		// なかったらテクスチャを生成し、配列へ入れる
		tex = new Texture();
		if (tex->Load(in_fileName))
		{
			m_textures.emplace(in_fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf("Load Texture Success : %s \n", in_fileName.c_str());
	return tex;
}

// メッシュの取得
Mesh * Renderer::GetMesh(const std::string & in_fileName)
{
	// 同じパスのメッシュを検索する
	Mesh* mesh = nullptr;
	auto iter = m_meshes.find(in_fileName);
	// すでに同じメッシュが登録してあった場合は、要素の二つ目(メッシュクラス)を複製
	if (iter != m_meshes.end())
	{
		mesh = iter->second;
	}
	else
	{

		// ファイルパス拡張子を調べ、それに応じてメッシュを生成
		int extNum = in_fileName.find_last_of(".");     // "."以降が何文字あるか調べる
		std::string fileExtension = in_fileName.substr(extNum, in_fileName.size() - extNum);    // ".#####"という形で拡張子を取得

		if (fileExtension == ".gpmesh")       // gpmesh形式
		{
			mesh = new MeshGpmesh();
			if (mesh->Load(in_fileName))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: gpmeshファイルのロード失敗" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}
		else if (fileExtension == ".obj" || fileExtension == ".OBJ")     // obj形式
		{
			mesh = new MeshObj();
			if (mesh->Load(in_fileName))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: objファイルのロード失敗" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}

	}

	printf("Mesh Load Success : %s\n", in_fileName.c_str());

	return mesh;
}

//-------------------------------------------------------------------------------------+
// スケルタル情報の取得
// in  : スケルタル情報 .gpskelファイル名
// out : Skeleton情報へのポインタ
// Desc: gpskelファイル名より、スケルタル情報を返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpskelファイル名をキーとするスケルタル情報のmapが作成される
//-------------------------------------------------------------------------------------+
const Skeleton* Renderer::GetSkeleton(const std::string& in_fileName)
{
	std::string file(in_fileName);
	auto iter = m_skeletons.find(file);
	if (iter != m_skeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			m_skeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//-------------------------------------------------------------------------------------------+
// アニメーション情報の取得
// in  : アニメーションデータが格納されている .gpanimファイル名
// out : アニメーション情報へのポインタ
// Desc: gpanimファイル名よりアニメーションデータを返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpanimファイル名をキーとするアニメーション情報のmapが作成される
//-------------------------------------------------------------------------------------------+
const Animation* Renderer::GetAnimation(const char* in_fileName, bool in_loop)
{
	auto iter = m_animations.find(in_fileName);
	if (iter != m_animations.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(in_fileName, in_loop))
		{
			m_animations.emplace(in_fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

// パーティクルマネージャー取得
ParticleManager * Renderer::GetParticleManager() const
{
	return m_particleManager;
}

// マップHUDのセッター
void Renderer::SetMapHUD(MiniMapHUD* in_mapHUD)
{
	// マップHUDが空でなかったら削除
	if (m_mapHUD != nullptr)
	{
		delete m_mapHUD;
	}
	m_mapHUD = in_mapHUD;
}

// シェーダーのロード
bool Renderer::LoadShaders()
{
	/*
	// スプライトシェーダー
	m_spriteShader = new Shader();

	if (!m_spriteShader->Load("Data/Shaders/SpriteShader.vert", "Data/Shaders/SpriteShader.frag"))
	{
		return false;
	}
	m_spriteShader->SetActive();
	// スクリーン用の行列を作成 (UIやスプライトは以降この行列を基準に描画)
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("u_viewProj", viewProj);

	// ワールド空間用スプライトシェーダー
	m_worldSpaceSpriteShader = new Shader();
	if (!m_worldSpaceSpriteShader->Load("Data/Shaders/WorldSpaceSprite.vert", "Data/Shaders/WorldSpaceSprite.frag"))
	{
		return false;
	}


	// メッシュシェーダー
	m_meshShader = new Shader();

	m_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	m_projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(m_screenWidth),
		static_cast<float>(m_screenHeight),
		1.0f, 10000.0f);

	if (!m_meshShader->Load("Data/Shaders/Phong.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}

	m_meshShader->SetActive();
	m_meshShader->SetMatrixUniform("u_viewProj", m_view * m_projection);

	// メッシュシェーダー(法線マップ)
	m_meshNormalShader = new Shader();
	if (!m_meshNormalShader->Load("Data/Shaders/NormalMap.vert", "Data/Shaders/NormalMap.frag"))
	{
		return false;
	}
	m_meshNormalShader->SetActive();
	m_meshNormalShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
	m_meshNormalShader->SetVectorUniform("u_lightPos", m_directionalLight.position);
	m_meshNormalShader->SetVectorUniform("u_viewPos", m_cameraPos);
	// サンプリング用テクスチャセット
	m_meshNormalShader->SetInt("u_mat.diffuseMap", 0);
	m_meshNormalShader->SetInt("u_mat.specularMap", 1);
	m_meshNormalShader->SetInt("u_mat.normalMap", 2);
	m_meshNormalShader->SetInt("u_mat.depthMap", 3);

	// スキンシェーダー
	m_skinnedShader = new Shader();
	if (!m_skinnedShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}
	m_skinnedShader->SetActive();
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// サンプリング用テクスチャセット
	m_skinnedShader->SetInt("u_mat.diffuseMap", 0);
	m_skinnedShader->SetInt("u_mat.specularMap", 1);
	m_skinnedShader->SetInt("u_mat.normalMap", 2);
	m_skinnedShader->SetInt("u_mat.depthMap", 3);

	// 環境マップシェーダ
	m_environmentMapShader = new Shader();
	if (!m_environmentMapShader->Load("Data/Shaders/EnvironmentMap.vert", "Data/Shaders/EnvironmentMap.frag"))
	{
		return false;
	}

	// スカイボックス用シェーダー
	m_skyboxShader = new Shader();
	if (!m_skyboxShader->Load("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"))
	{
		return false;
	}

	// マップHUD入力用シェーダー
	m_mapInputShader = new Shader();
	if (!m_mapInputShader->Load("Data/Shaders/HUD/HUD_MapInput.vert", "Data/Shaders/HUD/HUD_MapInput.frag"))
	{
		return false;
	}

	// マップHUD出力用シェーダー
	m_mapOutputShader = new Shader();
	if (!m_mapOutputShader->Load("Data/Shaders/HUD/HUD_MapOutput.vert", "Data/Shaders/HUD/HUD_MapOutput.frag"))
	{
		return false;
	}
	m_mapOutputShader->SetActive();
	// スクリーン用の行列を作成 (UIやスプライトは以降この行列を基準に描画)
	m_mapOutputShader->SetMatrixUniform("u_viewProj", viewProj);
	*/
	return true;
	
}
