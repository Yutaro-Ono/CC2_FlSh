#include "TitleSceneTPL.h"
#include "WorldTitleSceneTPL.h"
#include "GameMain.h"
#include "RenderBloom.h"

TitleSceneTPL::TitleSceneTPL()
	:m_sceneState(SCENE_STATE::INTRO_FADEIN)
	,m_selectState(SELECT_STATE::NONE)
	,m_canvas(nullptr)
	,m_world(nullptr)
{
}

TitleSceneTPL::~TitleSceneTPL()
{
}

void TitleSceneTPL::Initialize()
{
	// タイトルシーン用ワールドの生成
	m_world = new WorldTitleSceneTPL(this);
	m_world->Load();

}


SceneBase* TitleSceneTPL::Update(float _deltaTime)
{
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
		if (RENDERER->GetBloom()->FadeIn(0.15f, _deltaTime))
		{
			// 終了したら選択画面へ移行
			m_sceneState = SCENE_STATE::SELECT;
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


			//return gameScene;
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
	// 何も選択されていない(Press Any Key)
	if (m_selectState == SELECT_STATE::NONE)
	{
		// 決定キーの取得(押して離したか)
		bool enter = (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)|| INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_E) ||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)||
			          CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START));

		if (enter)
		{
			m_selectState == SELECT_STATE::GAME_START;
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
		}

		// 下キー入力を検知したら、一つ下の選択項目へ
		if (down)
		{
			m_selectState = SELECT_STATE::QUIT;
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
	}
}

void TitleSceneTPL::Draw()
{
}
