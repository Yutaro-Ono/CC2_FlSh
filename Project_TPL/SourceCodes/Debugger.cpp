//----------------------------------------------------------------------------------+
// @file        Debugger.cpp
// @brief       デバッグ用UI(imguiを使用)の更新
//              各種デバッグオブジェクトの保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "Debugger.h"
#include "DebugObjectBase.h"
#include "DebugObjectPool.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
Debugger::Debugger(DEBUG_STYLE _debugStyle)
	:m_debugStyle(_debugStyle)
	,m_debugWindow(NULL)
	,m_windowH(1280)
	,m_windowW(720)
	,m_debugWidth(480)
	,m_debugHeight(270)
	,m_debugObjects(nullptr)
	,m_actorDebugObjects(nullptr)
	,m_uiDebugObjects(nullptr)
	,m_isShow(true)
{
	m_debugObjects = new DebugObjectPool();
	m_actorDebugObjects = new DebugObjectPool();
	m_uiDebugObjects = new DebugObjectPool();
	CreateDebugBuffers();
}

/// <summary>
/// デストラクタ
/// </summary>
Debugger::~Debugger()
{
}

/// <summary>
/// 初期化処理
/// ※RendererでSDLWindowを作成している必要がある
/// </summary>
/// <returns> 初期化失敗時にfalseを返す </returns>
bool Debugger::Initialize()
{

	//--------------------------------------+
    // デバッグウィンドウ定義
    //--------------------------------------+
	if (m_debugStyle == DEBUG_STYLE::ANOTHER_WINDOW)
	{
		// ウィンドウ作成
		m_debugWindow = SDL_CreateWindow
		(
			"Debug_TPL",                                 // ウィンドウの名称
			0,                                                // x座標のウィンドウ描画原点
			0,                                                // y座標のウィンドウ描画原点
			m_windowW,                                    // 画面の横幅
			m_windowH,                                   // 画面の縦幅
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		// ウィンドウ作成失敗時
		if (!m_debugWindow)
		{
			std::cout << "Failed : Create Debugger Window" << std::endl;
			return false;
		}

		// デバッグウィンドウとコンテキストのリンク (レンダラーと共通のコンテキストを使用)
		SDL_GL_MakeCurrent(m_debugWindow, RENDERER->GetSDLGLContext());

		// SDLレンダラーの作成
		m_debugRenderer = SDL_CreateRenderer(m_debugWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	}
	else
	{
		// メインウィンドウと同じウィンドウをセット
		m_debugWindow = RENDERER->GetSDLWindow();

		// デバッグウィンドウとコンテキストのリンク (レンダラーと共通のコンテキストを使用)
		SDL_GL_MakeCurrent(m_debugWindow, RENDERER->GetSDLGLContext());

		// SDLレンダラーの取得
		m_debugRenderer = RENDERER->GetSDLRenderer();
	}

	if (!m_debugRenderer)
	{
		std::cout << "Failed : Create Debugger Renderer" << std::endl;
		return false;
	}

	//----------------------------------------+
    // imguiセットアップ
    //----------------------------------------+
    // imguiコンテキスト作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imguiのカラースタイルを設定
	ImGui::StyleColorsDark();         // ダークスタイル
	//ImGui::StyleColorsClassic();    // クラシックスタイル
	// 日本語フォントに設定
	io.Fonts->AddFontFromFileTTF("Data/Fonts/Debug/mplus-1c-medium.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// GLとGLSLにバインド
	// ver420を使用時でも、410 core としてセット
	const char* glsl_version = "#version 410 core";
	ImGui_ImplSDL2_InitForOpenGL(m_debugWindow, RENDERER->GetSDLGLContext());
	ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

/// <summary>
/// 解放処理
/// </summary>
void Debugger::Delete()
{
	// デバッグオブジェクトのプールを解放
	delete m_debugObjects;
	delete m_actorDebugObjects;
	delete m_uiDebugObjects;

	// imguiのクリーンアップ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	// SDLのクリーンアップ
	if (m_debugStyle == DEBUG_STYLE::ANOTHER_WINDOW)
	{
		SDL_DestroyWindow(m_debugWindow);
	}

}

/// <summary>
/// ImGuiの更新処理
/// デバッグクラス群の更新を行う予定
/// </summary>
void Debugger::UpdateImGui(float _deltaTime)
{

	// "LALT"押下中+"Dキー"でデバッグ表示のオンオフ切り替え
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LALT) && INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_D))
	{
		if (m_isShow == true)
		{
			m_isShow = false;
		}
		else
		{
			m_isShow = true;
		}
	}

	// 表示モードoffの場合は更新しない
	if (!m_isShow)
	{
		return;
	}


	// imguiの新規フレームを開始する
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_debugWindow);
	ImGui::NewFrame();

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 1 (システム関連)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.0f, 0.7f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.4f, 0.0f, 0.4f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.0f, 0.3f, 0.6f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// ウィンドウタイトル
	// ※日本語フォントはu8リテラルで正しく表示できる
	ImGui::Begin("DEBUG WINDOW : System");
	//フレームレートを表示
	ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FrameRate >> %.1f FPS", ImGui::GetIO().Framerate);
	// デバッグオブジェクト更新処理
	m_debugObjects->UpdateObjects(_deltaTime);
	// システムウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 2 (アクター関連)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.2f, 0.7f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.4f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.08f, 0.2f, 0.6f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// ウィンドウタイトル
	ImGui::Begin("DEBUG WINDOW : Actor");
	// デバッグオブジェクト更新処理
	m_actorDebugObjects->UpdateObjects(_deltaTime);
	// アクターウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	//-------------------------------------------------------------------------------+
    // デバッグウィンドウ 3 (インターフェース関連)
    //-------------------------------------------------------------------------------+
    // タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.3f, 0.5f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.4f, 0.4f, 0.6f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.6f, 0.8f, 0.6f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(620, 20), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// ウィンドウタイトル
	ImGui::Begin("DEBUG WINDOW : Interface");
	// デバッグオブジェクト更新処理
	m_uiDebugObjects->UpdateObjects(_deltaTime);
	// アクターウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 4 (ゲーム設定)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.4f, 0.1f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.08f, 0.6f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(620, 320), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	// メニュータブを備えたウィンドウにする
	ImGui::Begin(u8"デバッグ画面 (環境設定)", nullptr, ImGuiWindowFlags_MenuBar);
	// メニュー階層処理
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Load"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	// アクターウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


}



/// <summary>
/// 描画処理
/// デバッグウィンドウに対して描画を行う
/// </summary>
void Debugger::RenderImGui()
{

	if (m_isShow)
	{
		// コンテキスト関連付け (レンダラーと共通)
		SDL_GL_MakeCurrent(m_debugWindow, GAME_INSTANCE.GetRenderer()->GetSDLGLContext());
		// フレームバッファサイズの取得
		SDL_GetWindowSize(m_debugWindow, &m_windowW, &m_windowH);
		// ビューポートの設定
		glViewport(0, 0, m_windowW, m_windowH);

		// ImGuiレンダリング開始
		ImGui::Render();

		// フレームバッファにImGuiの描画結果を書き出す
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 別ウィンドウに表示している時のみ
		// 新しいカラーバッファを古いバッファと交換し、画面に表示
		if (m_debugStyle == DEBUG_STYLE::ANOTHER_WINDOW)
		{
			SDL_GL_SwapWindow(m_debugWindow);

		}
	}

}


void Debugger::AddDebugObject(DebugObjectBase* _debugObj, DEBUG_TAG _tag)
{

	if (_tag == DEBUG_TAG::SYSTEM)
	{
		m_debugObjects->AddObject(_debugObj);
		return;
	}

	if(_tag == DEBUG_TAG::ACTOR)
	{
		m_actorDebugObjects->AddObject(_debugObj);
		return;
	}

	if (_tag == DEBUG_TAG::INTERFACE)
	{
		m_uiDebugObjects->AddObject(_debugObj);
		return;
	}
}


void Debugger::DeleteDebugObject(DebugObjectBase* _debugObj, DEBUG_TAG _tag)
{
	if (_tag == DEBUG_TAG::SYSTEM)
	{
		m_debugObjects->DeleteObject(_debugObj);
		return;
	}
	
	if(_tag == DEBUG_TAG::ACTOR)
	{
		m_actorDebugObjects->DeleteObject(_debugObj);
		return;
	}

	if (_tag == DEBUG_TAG::INTERFACE)
	{
		m_uiDebugObjects->DeleteObject(_debugObj);
		return;
	}
}

/// <summary>
/// デバッグ用フレームバッファの生成処理
/// </summary>
/// <param name="_width"> 横幅 </param>
/// <param name="_height"> 縦幅 </param>
void Debugger::CreateDebugBuffers()
{

	// カラーバッファ (テクスチャ)
	glGenTextures(1, &m_debugCB);
	glBindTexture(GL_TEXTURE_2D, m_debugCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_debugWidth, m_debugHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// レンダーバッファ
	glGenRenderbuffers(1, &m_debugRB);
	glBindRenderbuffer(GL_RENDERBUFFER, m_debugRB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_debugWidth, m_debugHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// フレームバッファ
	glGenFramebuffers(1, &m_debugFB);
	glBindFramebuffer(GL_FRAMEBUFFER, m_debugFB);
	// テクスチャをカラーバッファとして結合
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_debugCB, 0);
	// レンダーバッファを深度バッファとして結合
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_debugRB);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


