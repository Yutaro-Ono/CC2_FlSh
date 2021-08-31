//----------------------------------------------------------------------------------+
// @file        CanvasGameScene.h
// @brief       ゲームシーンのUIキャンバスクラス
// @note        Canvasクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Canvas.h"


class CanvasGameScene : public Canvas
{

public:

	CanvasGameScene(class WorldGameScene* _world);
	~CanvasGameScene();

	void Update(float _deltaTime) override;


};