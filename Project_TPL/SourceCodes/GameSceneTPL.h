#pragma once
#include "SceneBase.h"


class GameSceneTPL : public SceneBase
{

public:

	enum class SCENE_STATE
	{
		FADE_IN,

		IN_GAME,

		FADE_OUT
	};

	GameSceneTPL();
	~GameSceneTPL();

	void Initialize() override;

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;

private:

	SCENE_STATE m_sceneState;

	class WorldGameSceneTPL* m_world;
	class CanvasGameSceneTPL* m_canvas;

};