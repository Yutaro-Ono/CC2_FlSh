//----------------------------------------------------------------------------------+
// @file        CanvasGameSceneTPL.h
// @brief       ゲームシーンのUIキャンバスクラス
// @note        Canvasクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 11/26   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Canvas.h"


class CanvasGameSceneTPL : public Canvas
{

public:

	CanvasGameSceneTPL(class WorldGameSceneTPL* _world, class GameSceneTPL* _scene);
	~CanvasGameSceneTPL();

	bool Load() override;

	void Update(float _deltaTime) override;

	class WorldGameSceneTPL* GetWorld() { return m_world; }


private:

	class GameSceneTPL* m_scene;
	class WorldGameSceneTPL* m_world;


};