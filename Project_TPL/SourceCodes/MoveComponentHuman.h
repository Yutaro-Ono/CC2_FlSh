#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class Actor* in_owner);     // コンストラクタ
	~MoveComponentHuman();                               // デストラクタ


	void Update(float in_deltaTime) override;                      // コンポーネントの更新

	void ProcessInput(float in_deltaTime) override {}                                    // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	bool m_toggleRun;                     // 走りボタンが押されたか

	float m_velocity;                     // 移動時の加速度

	static const float PLAYER_SPEED;
	static const float SPEED_WALK;
	static const float SPEED_JOG;
	static const float SPEED_RUN;

};