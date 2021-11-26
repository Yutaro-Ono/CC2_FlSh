#pragma once
#include "WorldBase.h"


class WorldGameSceneTPL : public WorldBase
{
public:


	WorldGameSceneTPL(class GameSceneTPL* _scene);             // コンストラクタ
	~WorldGameSceneTPL();            // デストラクタ

	bool Load() override;

	void Update(float in_deltaTime);
	void UpdateWorld(float _deltaTime) override;

	void ImGuiDebugWorld();

	// プレイヤーのゲッター
	class Player* GetPlayer() { return m_player; }


private:

	class GameSceneTPL* m_scene;
	class Player* m_player;

};