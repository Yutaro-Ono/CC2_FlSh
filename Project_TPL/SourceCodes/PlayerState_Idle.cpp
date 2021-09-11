#include "PlayerState_Idle.h"
#include "GameMain.h"


PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

PLAYER_STATE PlayerStateIdle::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// 待機ステートから、条件ごとに他のステートへ移行する
	//---------------------------------------------------------------------+

	// コントローラ接続時
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// ジャンプ処理への移行
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_A))
		{
			return PLAYER_STATE::STATE_JUMP_START;
		}

		// 待機状態か(移動ボタンが押されているか)
		bool isIdle = CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_) &
			          CONTROLLER_INSTANCE.IsKeyOff(SDL_CONTROLLER_);

		if (!isIdle && )
		{
			return PLAYER_STATE::STATE_WALK;
		}
	}

	else
	{
		// ジャンプ処理への移行
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE))
		{
			return PLAYER_STATE::STATE_JUMP_START;
		}

		// 待機状態か(移動キーが押されているか)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);
	}




	return PLAYER_STATE::STATE_IDLE;
}

void PlayerStateIdle::EnterState(Player* _player, float _deltaTime)
{
}
