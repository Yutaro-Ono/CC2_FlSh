#include "PlayerState_Run.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Run::PlayerState_Run()
{
	m_animSpeed = 15.0f;
}

PlayerState_Run::~PlayerState_Run()
{
}

PLAYER_STATE PlayerState_Run::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// 走りステートから、条件ごとに他のステートへ移行する
	//---------------------------------------------------------------------+

	// コントローラ接続時
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// ジャンプ処理への移行
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_A))
		{
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// 待機状態か(移動ボタンが押されているか)
		Vector2 move = CONTROLLER_INSTANCE.GetLAxisVec();
		float inputVal = move.LengthSq();

		// 走り状態の時のみ、パッドの入力値が閾値を下回ったら走りを解除
		if (inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_IDLE;
		}

		std::cout << "ToggleRun = " << (int)m_toggleRun << std::endl;

	}

	// コントローラ未接続時
	else
	{
		// ジャンプ処理への移行
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE))
		{
			//return PLAYER_STATE::STATE_JUMP_START;
		}

		// 待機状態か(移動キーが押されているか)
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
			INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

		// いずれかの移動キーが入力+左シフトが押されていたら、走り状態へ移行
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT) && !isIdle)
		{
			return PLAYER_STATE::STATE_RUN;
		}
		// いずれかの移動キーが押されていたら、歩き状態へ
		else if (!isIdle)
		{
			return PLAYER_STATE::STATE_WALK;
		}
	}

	return PLAYER_STATE::STATE_RUN;
}

void PlayerState_Run::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_RUN), m_animSpeed * _deltaTime);
}
