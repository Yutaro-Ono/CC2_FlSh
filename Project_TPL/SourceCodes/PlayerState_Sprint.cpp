#include "PlayerState_Sprint.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Sprint::PlayerState_Sprint()
{
	m_animSpeed = 0.5f;
}

PlayerState_Sprint::~PlayerState_Sprint()
{
}

PLAYER_STATE PlayerState_Sprint::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// 走りステートから、条件ごとに他のステートへ移行する
	//---------------------------------------------------------------------+

	// バインドしたプレイヤーから走り/歩き状態の取得
	bool toggleSprint = _player->GetToggleSprint();
	bool toggleWalk = _player->GetToggleWalk();
	bool toggleCrouch = _player->GetToggleCrouch();

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

		// しゃがみ状態
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// 走り状態の時のみ、パッドの入力値が閾値を下回ったら走りを解除
		if (toggleSprint && inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_IDLE;
		}

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
		bool isIdle = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
			          INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D);

		// しゃがみ状態
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH;
		}

		// いずれかの移動キーが入力+走り状態で、走り継続
		if (toggleSprint && !isIdle)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}
		// いずれかの移動キーが押されていた、歩きトグルが入っていたら、歩き状態へ
		if (!isIdle && toggleWalk)
		{
			return PLAYER_STATE::STATE_WALK;
		}
		// 移動キーが押されているが歩きトグルが入っていない場合は、小走り状態へ
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// 何も押されていなかったら待機状態
		if (isIdle)
		{
			return PLAYER_STATE::STATE_IDLE;
		}

	}

	return PLAYER_STATE::STATE_SPRINT;
}

void PlayerState_Sprint::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_SPRINT), m_animSpeed);
}
