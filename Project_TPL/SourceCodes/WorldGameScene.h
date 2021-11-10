//---------------------------------------------------------+
//
// ゲームワールド統括クラス
//
//---------------------------------------------------------+
#pragma once
#include <vector>
#include "WorldBase.h"
#include "Environment.h"

class WorldGameScene : public WorldBase
{

public:


	WorldGameScene();             // コンストラクタ
	~WorldGameScene();            // デストラクタ

	bool Load() override;

	void Update(float in_deltaTime);
	void UpdateWorld(float _deltaTime) override;

	void ImGuiDebugWorld();

	// プレイヤーのゲッター
	class Player* GetPlayer() { return m_player; }
	// 地形マネージャのポインタゲッター
	class LevelManager* GetLevelManager() { return m_level; }

private:

	class Player* m_player;
	class LevelManager* m_level;

};