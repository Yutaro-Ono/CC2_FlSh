//----------------------------------------------------------------------------------+
// @file        CanvasGameSceneTPL.h
// @brief       �Q�[���V�[����UI�L�����o�X�N���X
// @note        Canvas�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 11/26   �V�K�쐬
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