//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       ゲームループに関わるインスタンスを生成・保持
//              更新処理も行う
// @note        シングルトン化し、includeすればどこからでもアクセス可能
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   新規作成
//
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
#include <unordered_map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Math.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "Input.h"
#include "Mouse.h"
#include "InputController.h"

class GameMain
{


public:

	// ゲームの状態管理
	enum GameState
	{
		PLAYING,
		PAUSED,
		QUIT
	};

	// シングルトン(静的インスタンスのゲッター)
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();                                                                    // デストラクタ

	bool   Initialize();                                                            // 各種初期化処理
	void   Delete();                                                                // 各種解放処理

	void   RunLoop();                                                               // ゲームのメインループ
	void   CloseGame();                                                             // ゲームの終了処理

	void   SetFirstScene(class SceneBase* _scene);                                // 開始シーンのセット

	void   SetShutDown() { m_isRunning = false; }                                     // シャットダウン処理

	void   AddActor(class Actor* _actor);                                           // アクターの追加
	void   RemoveActor(class Actor* _actor);                                        // アクターの削除
	void   DeadAllActor();                                                            // アクター全削除


    //--------------------------------------------------------------------//
	// Getter/Setter
	//-------------------------------------------------------------------//
	class GameConfig* GetConfig() const { return m_config; }                         // ゲーム設定クラスのゲッター
	class Renderer* GetRenderer() const { return m_renderer; }                       // レンダラー取得
	class Debugger* GetDebugger() const { return m_debugger; }                       // デバッガ―取得
	class AudioManager* const GetAudio() { return m_audio; }                         // オーディオのゲッター
	class PhysicsWorld* const GetPhysics() { return m_physicsWorld; }                // 当たり判定クラスのゲッター
	class LoadScreen* const GetLoadScreen() { return m_loadScreen; }                 // ロードスクリーンクラスのゲッター
	class ActorPool* const GetActorPool() { return m_actorPool; }

	SDL_Renderer* GetSDLRenderer();                                                  // 2D用SDLレンダラーの取得
	// カメラ
	void SetCamera(class Camera* _camera);                                         // カメラをシステムに登録
	void SetCamera(class CameraComponent* _camera);                                // カメラをシステムに登録
	class CameraComponent* GetCamera() { return m_activeCamera; }                    // アクティブなカメラポインタの取得

	void InActiveCamera(class Camera* _activeCam);                                 // カメラの登録を解除
	void InActiveCamera(class CameraComponent* _activeCam);                        // カメラの登録を解除
	// 行列取得
	const Matrix4& GetViewMatrix() { return m_viewMatrix; };                         // ビュー行列のゲッター
	const Vector3& GetViewVector();                                                  // ビュー座標のゲッター
	// フレーム計測
	const int GetFrame() { return m_frame; }                                         // フレームのゲッター
	// デルタタイム取得
	const float GetDeltaTime() { return m_deltaTime; }                               // デルタタイムの取得
	// デバッグ用視覚化ボックス
	void SetDrawDebugBox(struct OBB _box);
	void DrawDebugBoxes();
	// UI
	const std::vector<class UIScreen*>& GetUIStack() { return m_uiStack; }            // UIスタックのゲッター
	void AddUI(class UIScreen* _screen);
	void SwapPauseUI();                                                               // ポーズ画面を最前面に持ってくる
	// フォント
	class Font* GetFont(const std::string& _fileName);
	const std::string& GetFontPath() { return FONT_FILE_PATH; }
	// テキスト
	void LoadText(const std::string& _fileName);
	const std::string& GetText(const std::string& _key);

private:

	GameMain();                                                              // コンストラクタ

	void Input();                                                            // 入力処理

	int   UpdateGame();                                                      // ゲームの更新処理
	void UpdateActor();                                                      // アクターの更新処理                                                   // アクターリスト表示(デバッグ用)

	void Draw();                                                             // 描画処理

	// ゲームループ制御と描画用のヘルパー関数
	void LoopHelper();
	void DebugRenderer();

	GameState    m_state;                                                    // ゲームの状態(プレイ中かポーズ中か終了か)
	
	bool m_isRunning;                                                        // ゲームループを回し続ける(続行する)かどうか
	bool m_isPauseMode;                                                      // ポーズモード

	int          m_frame;                                                    // フレーム計測用(パーティクルの生成に用いる)
	float        m_deltaTime;                                                // 1フレームの経過時間(秒)
	Uint32       m_ticksCount;                                               // ミリ秒単位の経過時間

	Matrix4      m_viewMatrix;

	class GameConfig* m_config;
	class Renderer* m_renderer;                                              // レンダリングクラス
	class SceneBase*   m_nowScene;                                           // 現在のシーン

	class CameraComponent* m_activeCamera;                                   // アクティブなカメラ

	// 物理法則関連
	class PhysicsWorld* m_physicsWorld;                                      // 当たり判定システム
	class DebugBox* m_debugBox;                                              // デバッグ用ボックス
	
    // 音楽
	class AudioManager* m_audio;                                             // オーディオ管理

	// UI関連
	std::vector<class UIScreen*> m_uiStack;                                  // UIスタック
	std::unordered_map<std::string, class Font*> m_fonts;                    // フォントマップ (キー : ttfファイルパス)
	std::unordered_map<std::string, std::string> m_text;                     // テキスト文字列 (キー : テキスト文字列
	class PauseScreen* m_pauseScreen;                                        // ポーズ画面
	class LoadScreen* m_loadScreen;                                          // ロード画面

	// 各種オブジェクトプール群
	class ActorPool* m_actorPool;
	class MeshPool* m_meshPool;
	class TexturePool* m_texturePool;

	// デバッグクラス
	class Debugger* m_debugger;

	static const std::string FONT_FILE_PATH;                                 // 標準のフォントパス

};

// ゲッター
#define GAME_INSTANCE GameMain::GetInstance()
// 各クラスポインタのゲッターdefine化
#define RENDERER GameMain::GetInstance().GetRenderer()
#define GAME_CONFIG GameMain::GetInstance().GetConfig()
#define DEBUGGER GameMain::GetInstance().GetDebugger()
#define AUDIO GameMain::GetInstance().GetAudio()
#define ACTOR_POOL GameMain::GetInstance().GetActorPool()