#include "PlayerState_Walk.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Walk::PlayerState_Walk()
{
	m_animSpeed = 0.5f;
}

PlayerState_Walk::~PlayerState_Walk()
{
}

PLAYER_STATE PlayerState_Walk::Update(Player* _player, float _deltaTime)
{

	// バインドしたプレイヤーから走り/歩き状態の取得
	bool toggleSprint = _player->GetToggleSprint();
	bool toggleWalk = _player->GetToggleWalk();
	bool toggleCrouch = _player->GetToggleCrouch();

	//---------------------------------------------------------------------+
	// 歩きステートから、条件ごとに他のステートへ移行する
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

		// 走り状態
		if (toggleSprint)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}

		// しゃがみ状態
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

		// 一定以上の入力値で小走り状態へ移行
		if (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// 歩き状態
		if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_WALK;
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

		// いずれかの移動キーが入力+左シフトが押されていたら、走り状態へ移行
		if (toggleSprint && !isIdle)
		{
			return PLAYER_STATE::STATE_SPRINT;
		}

		// しゃがみ状態
		if (toggleCrouch)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

		// 移動している場合、小走り
		if (!isIdle && !toggleWalk)
		{
			return PLAYER_STATE::STATE_JOG;
		}
		// 歩きトグル有効かついずれかの入力キーが押されていたら歩き状態へ
		if (toggleWalk && !isIdle)
		{
			return PLAYER_STATE::STATE_WALK;
		}

	}

	return PLAYER_STATE::STATE_IDLE;
}

void PlayerState_Walk::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WALK), m_animSpeed);
}
