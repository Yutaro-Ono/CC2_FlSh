#pragma once
#include "UIScreen.h"

class PlayerHUD : public UIScreen
{

public:

	PlayerHUD(class Player* _player);
	~PlayerHUD();

	void Update(float in_deltaTime) override;               // XVˆ—

	void Draw(GLSLprogram* _shader) override;               // •`‰æˆ—

private:

	void Initialize();

	std::vector<class Texture*> m_hpGauges;
	class Texture* m_hpFrame;

	class Texture* m_rightLine;

	class Texture* m_tutorialPanel;

	class Texture* m_stealthGauge;
	class Texture* m_stealthGaugeBack;

	class Player* m_player;

	static const int HP_GAUGE_NUM;
};