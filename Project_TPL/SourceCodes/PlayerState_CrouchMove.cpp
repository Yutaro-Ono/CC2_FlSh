#include "PlayerState_CrouchMove.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_CrouchMove::PlayerState_CrouchMove()
{
}

PlayerState_CrouchMove::~PlayerState_CrouchMove()
{
}

PLAYER_STATE PlayerState_CrouchMove::Update(Player* _player, float _deltaTime)
{
	//---------------------------------------------------------------------+
	// かがみステートから、条件ごとに他のステートへ移行する
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

		// しゃがみトグル解除かつ小走り以上のスピード
		if (!toggleCrouch && !toggleWalk && (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// しゃがみトグル解除かつ歩き以上のスピード
		if (!toggleCrouch && (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_WALK;
		}

		// 入力値が歩き入力値以上でかがみ歩き状態
		if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
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

		// しゃがみトグル無効・待機状態でない・歩きトグルON
		if (!toggleCrouch && !isIdle && toggleWalk)
		{
			return PLAYER_STATE::STATE_WALK;
		}
		// しゃがみトグル無効・待機状態でない
		if (!toggleCrouch && !isIdle)
		{
			return PLAYER_STATE::STATE_JOG;
		}

		// いずれかの入力キーが押されていたら歩き状態へ
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

	}

	return PLAYER_STATE::STATE_CROUCH;
}

void PlayerState_CrouchMove::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_CROUCH_MOVE), m_animSpeed * _deltaTime);
}
