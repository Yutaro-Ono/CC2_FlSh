#include "PlayerState_Jog.h"
#include "GameMain.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"


PlayerState_Jog::PlayerState_Jog()
{
	m_animSpeed = 15.0f;
}

PlayerState_Jog::~PlayerState_Jog()
{
}

PLAYER_STATE PlayerState_Jog::Update(Player* _player, float _deltaTime)
{

	//---------------------------------------------------------------------+
    // 小走りステートから、条件ごとに他のステートへ移行する
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
		// 走り状態出ない場合のみ、「走る」ボタンの状態を取得
		if (!m_toggleRun)
		{
			m_toggleRun = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		}

		// 走り状態の時のみ、パッドの入力値が閾値を下回ったら走りを解除
		if (m_toggleRun && inputVal < WALK_SPEED_LINE && inputVal > -WALK_SPEED_LINE)
		{
			m_toggleRun = false;
		}

		std::cout << "ToggleRun = " << (int)m_toggleRun << std::endl;


		// 走りフラグがtrueかつ「WALK」以上の入力値で走り状態へ移行
		if (m_toggleRun && (inputVal >= WALK_SPEED_LINE || inputVal <= -WALK_SPEED_LINE))
		{
			return PLAYER_STATE::STATE_RUN;
		}

		// 一定以上の入力値で小走り状態へ移行
		if (inputVal >= JOG_SPEED_LINE || inputVal <= -JOG_SPEED_LINE)
		{
			return PLAYER_STATE::STATE_JOG;
		}

		// 小走りの基準を満たしていないものの、一定以上の入力値がある場合
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

	m_toggleRun = false;
	return PLAYER_STATE::STATE_IDLE;
}

void PlayerState_Jog::EnterState(Player* _player, float _deltaTime)
{
	m_toggleRun = false;
	SkeletalMeshComponent* skel = _player->GetSkelMesh();
	skel->PlayAnimation(_player->GetAnim(PLAYER_STATE::STATE_JOG), m_animSpeed * _deltaTime);
}
