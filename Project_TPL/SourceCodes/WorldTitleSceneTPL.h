#pragma once
#include "WorldBase.h"


class WorldTitleSceneTPL : public WorldBase
{

public:

	WorldTitleSceneTPL(class TitleSceneTPL* _titleScene);
	~WorldTitleSceneTPL();

	bool Load() override;

	void Update(float _deltaTime);
	void UpdateWorld(float _deltaTime) override;

private:

	class TitleSceneTPL* m_titleScene;

};