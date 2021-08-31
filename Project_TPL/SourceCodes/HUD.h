//-----------------------------------------------------------------------+
// HUD�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{


public:

	HUD(class WorldGameScene* _world);
	~HUD();

	void Update(float _deltaTime) override;

	void Draw(GLSLprogram* _shader) override;






private:

	class Texture* m_dangerCall;                             // �w���Ɍ�����������DANGER�\��
	class Texture* m_speedTexture;

	class Texture* m_impactTexture[2];                       // �W����

	class WorldGameScene* m_world;                         // ���[���h�ւ̃|�C���^

	std::vector<class Texture*> m_flameGauge;

	std::vector<Vector2> m_flameGaugePos;
	
	bool m_isImpact;                                  // �W�������o�����o���Ȃ���

	Vector2 m_speedTexPos;

	float m_dispAccel;                                // �A�N�Z���\���p

};