#include "TitleSceneTPL.h"
#include "GameMain.h"
#include "WorldTitleSceneTPL.h"
#include "CanvasTitleSceneTPL.h"
#include "RenderBloom.h"
#include "LoadScreen.h"
#include "GameSceneTPL.h"
#include "AudioManager.h"
#include "ParticleManager.h"

TitleSceneTPL::TitleSceneTPL()
	:m_sceneState(SCENE_STATE::INTRO_FADEIN)
	,m_selectState(SELECT_STATE::NONE)
	,m_canvas(nullptr)
	,m_world(nullptr)
	,m_fadeInSpeed(0.15f)
{
}

TitleSceneTPL::~TitleSceneTPL()
{
	delete m_world;
	delete m_canvas;
	// 全てのアクターを削除
	GAME_INSTANCE.DeadAllActor();
	// 全てのUIをCloseに設定
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// パーティクルを全て削除
	RENDERER->GetParticleManager()->AllDeadParticle();
	// 音楽を停止
	AUDIO->StopMusic();
}

void TitleSceneTPL::Initialize()
{
	// ロード画面の有効化
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}


	// タイトルシーン用ワールドの生成
	m_world = new WorldTitleSceneTPL(this);
	m_world->Load();

	// UIキャンバスの生成
	m_canvas = new CanvasTitleSceneTPL(m_world, this);
	m_canvas->Load();


	// ロード演出(残り分)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}


	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}


SceneBase* TitleSceneTPL::Update(float _deltaTime)
{
	m_world->Update(_deltaTime);
	m_world->UpdateWorld(_deltaTime);

	m_canvas->Update(_deltaTime);

	return UpdateSceneState(_deltaTime);
}

/// <summary>
/// シーンステートの更新処理
/// </summary>
/// <param name="_deltaTime"> デルタタイム </param>
/// <returns> 次のシーンまたはこのシーンクラスのポインタ </returns>
SceneBase* TitleSceneTPL::UpdateSceneState(float _deltaTime)
{
	// フェードイン
	if (m_sceneState == SCENE_STATE::INTRO_FADEIN)
	{
		// フェードイン処理
		if (RENDERER->GetBloom()->FadeIn(m_fadeInSpeed, _deltaTime))
		{
			// 終了したら選択画面を開始
			m_sceneState = SCENE_STATE::SELECT;
			m_selectState = SELECT_STATE::PRESS_ANY_KEY;
			// UIのタイトル演出
			m_canvas->EnterTitle();
		}

		return this;
	}

	// 選択画面
	if (m_sceneState == SCENE_STATE::SELECT)
	{
		UpdateSelectState();

		return this;
	}

	// フェードアウト
	if (m_sceneState == SCENE_STATE::OUTRO_FADEOUT)
	{
		// フェードアウト処理
		if (RENDERER->GetBloom()->FadeOut(0.7f, _deltaTime))
		{
			// 終了したら次のシーンへ移行
			GameSceneTPL* gameScene = new GameSceneTPL();

			return gameScene;
		}

		return this;
	}

	return this;
}

/// <summary>
/// 選択画面時、選択項目ステートの更新関数
/// </summary>
void TitleSceneTPL::UpdateSelectState()
{

	// 決定キーの取得(押して離したか)
	bool enter = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_E) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START));

	// キャンセルキー
	bool back = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B) ||
		CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X));

	// 何も選択されていない(Press Any Key)
	if (m_selectState == SELECT_STATE::PRESS_ANY_KEY)
	{
		if (enter)
		{
			m_selectState = SELECT_STATE::GAME_START;

			m_canvas->EnterSelect();

			return;
		}
	}

	if (m_selectState == SELECT_STATE::GAME_START)
	{
		// 上下キー入力の取得
		bool up = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP));
		bool down = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN));

		// 上キー入力を検知したら、一つ上の選択項目へ
		if (up)
		{
			m_selectState = SELECT_STATE::QUIT;
			return;
		}

		// 下キー入力を検知したら、一つ下の選択項目へ
		if (down)
		{
			m_selectState = SELECT_STATE::QUIT;
			return;
		}

		// 決定キー押されたらフェードアウト処理→ゲームシーン
		if (enter)
		{
			m_sceneState = SCENE_STATE::OUTRO_FADEOUT;
			return;
		}
	}

	if (m_selectState == SELECT_STATE::QUIT)
	{
		// 上下キー入力の取得
		bool up = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP));
		bool down = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN));

		// 上キー入力を検知したら、一つ上の選択項目へ
		if (up)
		{
			m_selectState = SELECT_STATE::GAME_START;
		}

		// 下キー入力を検知したら、一つ下の選択項目へ
		if (down)
		{
			m_selectState = SELECT_STATE::GAME_START;
		}

		// 決定キー押されたらゲーム終了
		if (enter)
		{
			GAME_INSTANCE.SetShutDown();
			return;
		}
	}

	// キャンセルキーで最初に戻る
	if (back)
	{
		m_sceneState = SCENE_STATE::INTRO_FADEIN;
		m_selectState = SELECT_STATE::PRESS_ANY_KEY;

		m_canvas->BackTitle();

		m_fadeInSpeed = 0.4f;

		return;
	}
}

void TitleSceneTPL::Draw()
{
	RENDERER->Draw();
}
