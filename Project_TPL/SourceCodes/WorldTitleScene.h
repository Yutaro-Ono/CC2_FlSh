//----------------------------------------------------------------------------------+
// @file        WorldTitleScene.h
// @brief       タイトルシーンのワールド
// @note        WorldBaseを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "WorldBase.h"


class WorldTitleScene : public WorldBase
{

public:

	WorldTitleScene();
	~WorldTitleScene();

	bool Load() override;

	void UpdateWorld(float _deltaTime) override;


private:


	

};