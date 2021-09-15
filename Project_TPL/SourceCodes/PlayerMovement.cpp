#include "PlayerMovement.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "Player.h"
#include "ThirdPersonCamera.h"

const float PlayerMovement::PLAYER_SPEED = 45.0f;
const float PlayerMovement::SPEED_WALK = 45.0f;
const float PlayerMovement::SPEED_JOG = 80.0f;
const float PlayerMovement::SPEED_SPRINT = 120.0f;


// コンストラクタ
PlayerMovement::PlayerMovement(Player* _player)
	:MoveComponent(_player)
	,m_player(_player)
	,m_velocity(SPEED_WALK)
{
	
}

// デストラクタ
PlayerMovement::~PlayerMovement()
{
}

// 更新処理
void PlayerMovement::Update(float in_deltaTime)
{
	if (m_isActive)
	{
		if (m_padIsActive)
		{
			MovementByController(in_deltaTime);
		}
		else
		{
			MovementByKeyboard(in_deltaTime);
		}
	}

	// 重力処理
	//m_owner->SetPosition(m_owner->GetPosition() - Vector3(0.0f, 0.0f, 2.0f));
}


// プレイヤーの移動処理(コントローラ)
void PlayerMovement::MovementByController(float in_deltaTime)
{
	// 左スティック入力値の取得
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	//std::cout << "StickL::x = " << axisL.x << "::  ::y = " << axisL.y << std::endl;
	float axisLength = axisL.LengthSq();
	// 入力閾値
	float axisThreshold = 0.01f;

	//-------------------------------------------------------------------------------+
	// 「Sprint」トグル制御
	//-------------------------------------------------------------------------------+
	// 「走る」ボタンが押されているかを取得
	// 通常はLBボタン
	bool toggleSprint = m_player->GetToggleSprint();
	if (!toggleSprint)
	{
		toggleSprint = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

		// trueなら走り状態へ/しゃがみ状態を解除
		if (toggleSprint)
		{
			m_player->SetToggleCrouch(false);
			m_player->SetToggleSprint(true);
		}
		
	}
	// 走り状態時、入力閾値が一定以下になったら走りトグル解除
	if (toggleSprint && axisLength < axisThreshold && axisLength > -axisThreshold)
	{
		toggleSprint = false;
		m_player->SetToggleSprint(toggleSprint);
	}

	//--------------------------------------------------------------------------------------+
    // 「Crouch」トグル制御
    //--------------------------------------------------------------------------------------+
	bool toggleCrouch = m_player->GetToggleCrouch();
	// しゃがみ状態の時左Ctrlで解除
	if (toggleCrouch)
	{
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// 右スティック押し込みでcrouch状態へ/走り状態解除
	if (!toggleCrouch)
	{
		toggleCrouch = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_RIGHTSTICK);

		if (toggleCrouch)
		{
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}
		
	}

	//-------------------------------------------------------------------------------+
    // 移動速度更新
    //-------------------------------------------------------------------------------+
	// 「走る」ボタンが押されていた時、同時に左スティック入力もされていた場合
	if (toggleSprint && CONTROLLER_INSTANCE.GetIsInputAxisL())
	{
		m_velocity = SPEED_SPRINT;
	}
	else if(axisLength >= 0.55f || axisLength <= -0.55f)
	{
		m_velocity = SPEED_JOG;
	}
	else
	{
		m_velocity = SPEED_WALK;
	}


	// プレイヤーの前進・右方向ベクトル定義 (カメラ基準)
	//Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 forwardVec = GAME_INSTANCE.GetViewTarget() - GAME_INSTANCE.GetViewPos();
	forwardVec.z = 0.0f;
	forwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向ベクトルからカメラ回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, charaRightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, charaRightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;
	Vector3 charaForwardVec = m_owner->GetForward(); // キャラの前進ベクトル

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	axisL = Vector2::Transform(axisL, rot);


	// 前進ベクトルと右方向ベクトルから移動量を算出
	Vector3 moveVec = Vector3::Zero;
	moveVec += Vector3(axisL.x, axisL.y, 0.0f);

	// 入力キーの総和
	if (moveVec.LengthSq() > axisThreshold)
	{
		// 方向キー入力
		charaForwardVec = moveVec;
		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		m_owner->RotateToNewForward(charaForwardVec);
	}


	// プレイヤーの現在位置から、定義した方向へ速度分を加算
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// 最終的なプレイヤーの座標をセット
	m_owner->SetPosition(resultPos);
}

// プレイヤーの移動処理(キーボード)
void PlayerMovement::MovementByKeyboard(float in_deltaTime)
{
	// キー入力値
	Vector2 inputAxis = Vector2::Zero;
	// 移動キーの入力状態を取得
	bool pressW = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W);
	bool pressA = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A);
	bool pressS = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S);
	bool pressD = INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D);

	//-------------------------------------------------------------------------------+
    // 「Sprint」トグル制御
    //-------------------------------------------------------------------------------+
	// 走り状態時、入力されていなかったら走りトグル解除
	bool toggleSprint = m_player->GetToggleSprint();
	if (toggleSprint && !(pressW || pressA || pressS || pressD))
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LSHIFT);

		// trueなら走り状態へ/しゃがみ状態を解除
		if (toggleSprint)
		{
			m_player->SetToggleCrouch(false);
			m_player->SetToggleSprint(true);
		}
	}
	// 「走る」ボタンが押されているかを取得
	// 左シフト
	if (!toggleSprint)
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT);
		m_player->SetToggleSprint(toggleSprint);
	}

	//-------------------------------------------------------------------------------+
    // 「Walk」トグル制御
    //-------------------------------------------------------------------------------+
	// 歩き状態のトグル更新
	// LeftAltで更新
	bool toggleWalk = m_player->GetToggleWalk();
	bool key = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LALT);
	if (key)
	{
		if (!toggleWalk)
		{
			toggleWalk = true;
			m_player->SetToggleWalk(toggleWalk);
		}
		else
		{
			toggleWalk = false;
			m_player->SetToggleWalk(toggleWalk);
		}
		
	}

	//--------------------------------------------------------------------------------------+
    // 「Crouch」トグル制御
    //--------------------------------------------------------------------------------------+
	bool toggleCrouch = m_player->GetToggleCrouch();
	// しゃがみ状態の時左Ctrlで解除
	if (toggleCrouch)
	{
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// 右スティック押し込みでcrouch状態へ/走り状態解除
	if (!toggleCrouch)
	{
		toggleCrouch = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL);

		if (toggleCrouch)
		{
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}

	}

	//-------------------------------------------------------------------------------+
    // 移動速度更新
    //-------------------------------------------------------------------------------+
	// 「走る」ボタンが押されていた時、同時に左スティック入力もされていた場合
	if (toggleSprint)
	{
		m_velocity = SPEED_SPRINT;
	}
	else if (!toggleWalk)
	{
		m_velocity = SPEED_JOG;
	}
	else
	{
		m_velocity = SPEED_WALK;
	}


	// キー入力WASDによる移動処理
	if (pressW)
	{
		inputAxis.y += -1.0f;
	}
	if (pressS)
	{
		inputAxis.y += 1.0f;
	}
	if (pressA)
	{
		inputAxis.x += -1.0f;
	}
	if (pressD)
	{
		inputAxis.x += 1.0f;
	}


	// プレイヤーの前進・右方向ベクトル定義 (カメラ基準)
	Vector3 forwardVec = GAME_INSTANCE.GetViewTarget() - GAME_INSTANCE.GetViewPos();
	forwardVec.z = 0.0f;
	forwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向ベクトルからカメラ回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, charaRightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, charaRightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;
	Vector3 charaForwardVec = m_owner->GetForward(); // キャラの前進ベクトル

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	inputAxis = Vector2::Transform(inputAxis, rot);


	// 前進ベクトルと右方向ベクトルから移動量を算出
	Vector3 moveVec = Vector3::Zero;
	moveVec += Vector3(inputAxis.x, inputAxis.y, 0.0f);

	// 入力キーの総和
	if (moveVec.LengthSq() > 0.5f)
	{
		// 方向キー入力
		charaForwardVec = moveVec;
		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		m_owner->RotateToNewForward(charaForwardVec);
	}


	// プレイヤーの現在位置から、定義した方向へ速度分を加算
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// 最終的なプレイヤーの座標をセット
	m_owner->SetPosition(resultPos);
}
