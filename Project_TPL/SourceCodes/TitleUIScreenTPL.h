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

	TitleUIScreenTPL(class TitleSceneTPL* _titleScene);
	~TitleUIScreenTPL();

	void Update(float _deltaTime) override;

	void EnterTitle();
	void EnterSelect();
	void BackTitle();

	void SetCursolPosition(TITLEUI_CATEGORY _uiCategory);

	void SetUIPosition(TITLEUI_CATEGORY _uiCategory, const Vector2& _pos);
	void SetUIScale(TITLEUI_CATEGORY _uiCategory, float _scale);

	void Draw(GLSLprogram* _shader) override;


private:

	class TitleSceneTPL* m_titleScene;

	std::unordered_map<TITLEUI_CATEGORY, class Texture*> m_uiTextures;
	std::unordered_map<TITLEUI_CATEGORY, Vector2> m_uiPos;
	std::unordered_map<TITLEUI_CATEGORY, float> m_uiScale;
	std::unordered_map<TITLEUI_CATEGORY, bool> m_uiVisible;

	class TitleUIScreenTPLDebugObject* m_debugObj;

	friend class TitleUIScreenTPLDebugObject;
};