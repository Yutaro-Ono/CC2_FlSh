//----------------------------------------------------------------------------------+
// @file        CanvasTitleScene.h
// @brief       �^�C�g���V�[����UI�L�����o�X�N���X
// @note        Canvas�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   �V�K�쐬
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