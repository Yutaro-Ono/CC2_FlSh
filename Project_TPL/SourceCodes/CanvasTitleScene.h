//----------------------------------------------------------------------------------+
// @file        CanvasTitleScene.h
// @brief       タイトルシーンのUIキャンバスクラス
// @note        Canvasクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Canvas.h"


class CanvasTitleScene : public Canvas
{

public:

	CanvasTitleScene(class WorldTitleScene* _world);
	~CanvasTitleScene();


	bool Load() override;

	void Update(float _deltaTime) override;



};