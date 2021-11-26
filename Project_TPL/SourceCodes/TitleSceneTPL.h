#pragma once
#include "SceneBase.h"


class TitleSceneTPL : public SceneBase
{

public:

	enum class SCENE_STATE
	{
		INTRO_FADEIN = 0,
		SELECT,
		OUTRO_FADEOUT,
	};

	enum class SELECT_STATE
	{
		NONE = 0,
		PRESS_ANY_KEY,
		GAME_START,
		QUIT
	};

	TitleSceneTPL();
	~TitleSceneTPL();

	void Initialize() override;

	SceneBase* Update(float _deltaTime) override;

	SceneBase* UpdateSceneState(float _deltaTime);

	void UpdateSelectState();

	void Draw() override;

	SCENE_STATE GetSceneState() { return m_sceneState; }
	SELECT_STATE GetSelectState() { return m_selectState; }

private:


	SCENE_STATE m_sceneState;
	SELECT_STATE m_selectState;

	class CanvasTitleSceneTPL* m_canvas;     // タイトルシーン用UIキャンバス
	class WorldTitleSceneTPL* m_world;
	
	float m_fadeInSpeed;

};