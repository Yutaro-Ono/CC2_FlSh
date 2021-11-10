//-----------------------------------------------------------------------+
// HUDクラス.
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



	void SetActiveDanger(bool _flag) { m_isDanger = _flag; }
	void SetActiveImpact(bool _flag) { m_isImpact = _flag; }

private:

	class Texture* m_dangerCall;                             // ヘリに見つかった時のDANGER表示
	class Texture* m_speedTexture;

	class Texture* m_impactTexture[2];                       // 集中線

	class WorldGameScene* m_world;                         // ワールドへのポインタ

	std::vector<class Texture*> m_flameGauge;

	std::vector<Vector2> m_flameGaugePos;
	
	bool m_isDanger;                                 // "Danger"表示するかしないか
	bool m_isImpact;                                  // 集中線を出すか出さないか

	Vector2 m_speedTexPos;

	float m_dispAccel;                                // アクセル表示用

};