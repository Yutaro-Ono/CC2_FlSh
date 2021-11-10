//-----------------------------------------------------------------------+
// ‘€ìà–¾UIƒNƒ‰ƒX
//-----------------------------------------------------------------------+

#pragma once
#include "UIScreen.h"



class PlayerControlUI : public UIScreen
{
public:

	PlayerControlUI(class WorldGameScene* _world);
	~PlayerControlUI();

	void Update(float in_deltaTime) override;

	void Draw(GLSLprogram* _shader) override;


private:

};

