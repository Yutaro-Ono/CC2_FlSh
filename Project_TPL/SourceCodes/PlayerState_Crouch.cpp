#include "PlayerState_Crouch.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"

PlayerState_Crouch::PlayerState_Crouch()
{
}

PlayerState_Crouch::~PlayerState_Crouch()
{
}

PLAYER_STATE PlayerState_Crouch::Update(Player* _player, float _deltaTime)
{

	//---------------------------------------------------------------------+
    // かがみステートから、条件ごとに他のステートへ移行する
    //---------------------------------------------------------------------+

	// バインドしたプレイヤーから走り/歩き状態の取得
	bool toggleSprint = _player->GetToggleSprint();
	bool toggleWalk = _player->GetToggleWalk();

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
		// 入力値が歩き入力値以上でかがみ歩き状態
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
		// いずれかの入力キーが押されていたら歩き状態へ
		if (!isIdle)
		{
			return PLAYER_STATE::STATE_CROUCH_MOVE;
		}

	}





	return PLAYER_STATE::STATE_CROUCH;
}

void PlayerState_Crouch::EnterState(Player* _player, float _deltaTime)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_CROUCH), m_animSpeed * _deltaTime);
}
