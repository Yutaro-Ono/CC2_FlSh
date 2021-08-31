//----------------------------------------------------------------------------------+
// @file        WorldBase.h
// @brief       ���[���h�x�[�X�N���X(�Q�[���t�B�[���h�̃I�u�W�F�N�g�Ǘ�)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 8/20   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once



class WorldBase
{

public:

	WorldBase();
	virtual ~WorldBase();

	virtual bool Load() = 0;

	virtual void UpdateWorld(float _deltaTime) = 0;



protected:



	class Environment* m_environment;     // ���N���X

	class Canvas* m_canvas;               // UI�L�����o�X

};