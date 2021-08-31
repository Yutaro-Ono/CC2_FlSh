//----------------------------------------------------------------------------------+
// @file        WorldTitleScene.h
// @brief       �^�C�g���V�[���̃��[���h
// @note        WorldBase���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   �V�K�쐬
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