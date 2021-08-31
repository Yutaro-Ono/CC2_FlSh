//----------------------------------------------------------------------------------+
// @file        CanvasGameScene.h
// @brief       �Q�[���V�[����UI�L�����o�X�N���X
// @note        Canvas�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   �V�K�쐬
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