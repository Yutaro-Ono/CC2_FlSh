//----------------------------------------------------------------------------------+
// @file        CanvasTitleSceneTPL.h
// @brief       �^�C�g���V�[����UI�L�����o�X�N���X
// @note        Canvas�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/31   �V�K�쐬
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

	void EnterTitle();
	void EnterSelect();
	void BackTitle();

private:

	class TitleSceneTPL* m_titleScene;        // �R�Â���ꂽ�^�C�g���V�[��

	class TitleUIScreenTPL* m_titleUI;

};