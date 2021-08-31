//-----------------------------------------------------------------------+
// �X�s�[�h���[�^�[UI�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class SpeedMeterUI : public UIScreen
{


public:

	SpeedMeterUI(class Player* _target);
	~SpeedMeterUI();

	void Update(float _deltaTime) override;

	void Draw(GLSLprogram* _shader) override;


private:

	class Player* m_player;

	class Texture* m_speedTex;          // �X�s�[�h�̐��l

	class Texture* m_gaugeTex[20];          // �X�s�[�h���[�^�[�Q�[�W
	class Texture* m_gaugeShadowTex;    // �X�s�[�h���[�^�[�Q�[�W�̉e
	int m_currentSpeedRate;                 // ���݂̑��x�ƍő呬�x�Ƃ̔䗦(�Q�[�W�̕`��ɗp����)


	Vector2 m_speedTexPos;

	int m_fontSize;

};