#include "PlayerState_WeaponOut_MoveForward.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"
#include "PlayerMovement.h"
PlayerState_WeaponOut_MoveForward::PlayerState_WeaponOut_MoveForward()
{
	m_animSpeed = 0.35f;
}

PlayerState_WeaponOut_MoveForward::~PlayerState_WeaponOut_MoveForward()
{
}

PLAYER_STATE PlayerState_WeaponOut_MoveForward::Update(Player* _player, float _deltaTime)
{
	// バインドしたプレイヤーから状態の取得
	bool toggleWalk = _player->GetToggleWalk();              // 歩行状態
	bool toggleCrouch = _player->GetToggleCrouch();          // しゃがみ状態
	bool toggleWeaponOut = _player->GetToggleWeaponOut();    // 武器出し状態

	// コントローラ接続時
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// 待機状態か(移動ボタンが押されているか)
		Vector2 move = CONTROLLER_INSTANCE.GetLAxisVec();
		float inputVal = move.LengthSq();

		// 武器出し状態
		if (toggleWeaponOut)
		{
			// しゃがみ状態
			if (toggleCrouch)
			{
				//return PLAYER_STATE::STATE_WEAPONOUT_CROUCH;
			}

			// 一定以上の入力値+前方入力時に、前方移動状態へ移行
			if ((inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE) && move.y > 0.0f)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// 一定以上の入力値+後方入力時に、後方移動状態へ移行
			if ((inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE) && move.y < 0.0f)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD;
			}

			// 歩き状態
			if (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// 入力がない場合は待機状態へ
			return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
		}
		else
		{
			// ジャンプ処理への移行
			if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y))
			{
				//return PLAYER_STATE::STATE_JUMP_START;
			}

			// しゃがみ状態
			if (toggleCrouch)
			{
				//return PLAYER_STATE::STATE_WEAPONOUT_CROUCH;
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

			// 入力がない場合は待機状態へ
			return PLAYER_STATE::STATE_IDLE;
		}
	}

	// コントローラ未接続時
	else
	{
		// 武器出し状態
		if (toggleWeaponOut)
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

			// 上下左右の入力によって
			// 各方向の移動モーションへ遷移
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}
			// 歩きトグル有効かつSキーが押されていたら後方移動状態へ
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEBWD;
			}
			// 歩きトグル有効かつAキーが押されていたら左移動状態へ
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVELEFT;
			}
			// 歩きトグル有効かつDキーが押されていたら右状態へ
			if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVERIGHT;
			}



			// 移動している場合、小走り
			if (!isIdle)
			{
				return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;
			}

			// しゃがみ状態
			if (toggleCrouch)
			{
				return PLAYER_STATE::STATE_CROUCH;
			}
			// 入力がない場合は待機状態へ
			return PLAYER_STATE::STATE_WEAPONOUT_IDLE;
		}

		// 武器収め状態
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

			// 歩きトグル有効かついずれかの入力キーが押されていたら歩き状態へ
			if (toggleWalk && !isIdle)
			{
				return PLAYER_STATE::STATE_WALK;
			}
			// 移動している場合、小走り
			if (!isIdle)
			{
				return PLAYER_STATE::STATE_JOG;
			}

			// 入力がない場合は待機状態へ
			return PLAYER_STATE::STATE_IDLE;
		}
	}

	return PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD;

}

void PlayerState_WeaponOut_MoveForward::EnterState(Player* _player)
{
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	// 歩行状態かどうかでモーションを変更
	if (_player->GetToggleWalk())
	{
		m_animSpeed = 0.65f;

		// 歩行状態の時
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_WALKFWD), m_animSpeed);
	}
	else
	{
		m_animSpeed = 0.35f;

		// 通常走り状態
		skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_WEAPONOUT_MOVEFWD), m_animSpeed);
	}
}
