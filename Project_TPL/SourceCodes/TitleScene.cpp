//-----------------------------------------------------------------------+
// タイトルシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TitleScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "WorldTitleScene.h"
#include "TitleScreen.h"
#include "GameScene.h"
#include "LoadScreen.h"
#include "RenderBloom.h"
#include "TutorialScene.h"
#include "ActorPool.h"

const int TitleScene::STAGE_ALL_NUM = 1;

// コンストラクタ
TitleScene::TitleScene()
	:m_state(FADE_IN)
	,m_selectedStage(0)
	,m_world(nullptr)
{
}

// デストラクタ
TitleScene::~TitleScene()
{
	GAME_INSTANCE.DeadAllActor();
}

// 初期化処理
void TitleScene::Initialize()
{
	// ロード画面の有効化
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}


	// ワールド情報のロード
	m_world = new WorldTitleScene();
	m_world->Load();

	//--------------------------------------------------------------------------------------------+
	// 音楽関連
	// SE
	m_sound["Enter"] = "Data/Music/SE/System/Enter/decide13.wav";
	AUDIO->GetSound(m_sound["Enter"]);                                       // 決定音
	m_sound["Select"] = "Data/Music/SE/System/Select/decide14.wav";
	AUDIO->GetSound(m_sound["Select"]);                                      // 選択音
	// BGM
	m_sound["BGM"] = "Data/Music/BGM/Title/01_njol.wav";
	AUDIO->GetMusic(m_sound["BGM"]);

	// ロード演出(残り分)
	for (int i = 0; i < GAME_INSTANCE.GetLoadScreen()->GetGaugeAllNum(); i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// タイトル用UI
	//UIScreen* hud = new TitleScreen(this);

	// BGM再生開始
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// 更新処理
SceneBase * TitleScene::Update(float _deltaTime)
{

	// ワールドの更新
	m_world->UpdateWorld(_deltaTime);

	return this;


	// タイトルメニュー
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case FADE_IN:

		if (RENDERER->GetBloom()->FadeIn(0.15f, _deltaTime))
		{
			m_state = PRESS_ANY_KEY;
		}

		break;

	//----------------------------------------------------------------------+
    // "FADE OUT"
    //----------------------------------------------------------------------+
	case FADE_OUT:

		if (RENDERER->GetBloom()->FadeOut(0.7f, _deltaTime))
		{

            // 全てのUIをCloseに設定
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// 全てのアクターを削除
			for (auto actor : ACTOR_POOL->GetPool())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// 次のシーンを返す
			return new GameScene(m_selectedStage);
			//return new TutorialScene();
		}
		
		break;


	//----------------------------------------------------------------------+
	// "PRESS ANY KEY"
	//----------------------------------------------------------------------+
	case PRESS_ANY_KEY:


		// いずれかのボタンで次へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Select"]);
			m_state = GAME_START;
		}

		break;

	//----------------------------------------------------------------------+
	// "GAME START"
	//----------------------------------------------------------------------+
	case GAME_START:

		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}

		// 下キーかDPAD下で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = GAME_QUIT;
		}

		//---------------------------------------------------------------------------+
        // シーン遷移処理
		//---------------------------------------------------------------------------+
        // SPACEかENTER、Aボタンを押したら次のシーンへ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// 音楽をフェードアウト
			AUDIO->FadeOutMusic(3000.0f);

			// フェードアウト処理に移行
			m_state = FADE_OUT;
		}

		break;


	//-----------------------------------------------------------------------------------+
	// "STAGE SELECT"
	//-----------------------------------------------------------------------------------+
	case STAGE_SELECT:

		//---------------------------------------------------------------------+
		// 「戻る」処理
		//---------------------------------------------------------------------+
		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// "GAME_START"へ
			m_state = GAME_START;

			break;
		}


		//-----------------------------------------------------------------------+
		// ステージ項目の移動時処理
		//-----------------------------------------------------------------------+
		// ↑キーかDPAD↑で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// ステージ1でなければ選択中のステージをカウントダウン
			if (m_selectedStage != 0)
			{
				m_selectedStage--;
			}

			break;
		}

		// ↓キーかDPAD↓で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			// ステージ数の上限でなければ選択中のステージ番号をカウントアップ
			if (m_selectedStage < STAGE_ALL_NUM - 1)
			{
				m_selectedStage++;
			}

			break;
		}


		//---------------------------------------------------------------------------+
		// シーン遷移処理
		//---------------------------------------------------------------------------+
		// SPACEかENTER、Aボタンを押したら次のシーンへ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// 音楽をフェードアウト
			AUDIO->FadeOutMusic(1.0f);

			// プレイヤーのサウンドを停止
			//m_player->AllStopSound();


			// 全てのUIをCloseに設定
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// 全てのアクターを削除
			for (auto actor : ACTOR_POOL->GetPool())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// フェードアウト処理に移行
			m_state = FADE_OUT;
		}

		break;

	//----------------------------------------------------------------------+
	// "QUIT TO DESKTOP"
	//----------------------------------------------------------------------+
	case GAME_QUIT:


		// SPACEかENTER、Aボタンを押したらシャットダウン
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// 上キーかDPAD上で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = GAME_START;
		}

		break;

	default:

		break;

	}

	
	return this;

}


// 描画処理
void TitleScene::Draw()
{
	RENDERER->Draw();
}
