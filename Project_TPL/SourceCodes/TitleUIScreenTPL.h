#pragma once
#include "UIScreen.h"

class TitleUIScreenTPL : public UIScreen
{

public:

	enum class TITLEUI_CATEGORY
	{
		LOGO,
		PRESS_ANY_KEY,
		CURSOL,
		GAME_START,
		GAME_QUIT
	};

	TitleUIScreenTPL();
	~TitleUIScreenTPL();

	void Update(float _deltaTime) override;

	void Draw(GLSLprogram* _shader) override;


private:

	std::unordered_map<TITLEUI_CATEGORY, class Texture*> m_uiTextures;
	std::unordered_map<TITLEUI_CATEGORY, Vector2> m_uiPos;
	std::unordered_map<TITLEUI_CATEGORY, float> m_uiScale;

};