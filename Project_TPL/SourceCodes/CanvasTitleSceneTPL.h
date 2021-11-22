//----------------------------------------------------------------------------------+
// @file        CanvasTitleSceneTPL.h
// @brief       タイトルシーンのUIキャンバスクラス
// @note        Canvasクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Canvas.h"
#include <unordered_map>

class CanvasTitleSceneTPL : public Canvas
{

public:

	CanvasTitleSceneTPL(class WorldTitleSceneTPL* _world, class TitleSceneTPL* _titleScene);
	~CanvasTitleSceneTPL();

	bool Load() override;

	void Update(float _deltaTime) override;



private:



};