#pragma once
#include "MoveComponent.h"

class PlayerMovement : public MoveComponent
{

public:

	PlayerMovement(class Player* _player);                      // コンストラクタ
	~PlayerMovement();                                          // デストラクタ


	void Update(float in_deltaTime) override;                      // コンポーネントの更新

	void ProcessInput(float in_deltaTime) override {}              // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用


	void MovementByController(float in_deltaTime) override;
	void MovementByKeyboard(float in_deltaTime) override;

protected:

	class Player* m_player;               // プレイヤーへのポインタ

	float m_velocity;                     // 移動時の加速度

	static const float PLAYER_SPEED;
	static const float SPEED_WALK;
	static const float SPEED_JOG;
	static const float SPEED_SPRINT;

};