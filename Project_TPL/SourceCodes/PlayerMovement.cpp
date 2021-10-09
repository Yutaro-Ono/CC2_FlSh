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
const float PlayerMovement::SPEED_SPRINT = 160.0f;

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

	// プレイヤー状態の取得
	bool toggleSprint = m_player->GetToggleSprint();
	bool toggleCrouch = m_player->GetToggleCrouch();
	//-------------------------------------------------------------------------------+
	// 「Sprint」トグル制御
	//-------------------------------------------------------------------------------+
	// 通常はLBボタンで走りへ移行/しゃがみ状態の時は無効
	if (!toggleSprint && !toggleCrouch)
	{
		toggleSprint = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

		// trueなら走り状態へ
		// 移動速度を走りに設定
		if (toggleSprint)
		{
			m_velocity = SPEED_SPRINT;
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

		// 移動速度を歩きに設定
		if (toggleCrouch)
		{
			m_velocity = SPEED_WALK;
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}
		
	}

	//-------------------------------------------------------------------------------+
    // 移動速度更新
    //-------------------------------------------------------------------------------+
	// 通常移動速度にセット
	if(!toggleSprint && !toggleCrouch && (axisLength >= 0.55f || axisLength <= -0.55f))
	{
		m_velocity = SPEED_JOG;
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
		//charaForwardVec = moveVec;
		moveVec.Normalize();

		// 進行方向に向けて回転(ラープ処理)
		if (charaForwardVec.y - moveVec.y >= 1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		if (charaForwardVec.y - moveVec.y <= -1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		moveVec = Vector3::Lerp(charaForwardVec, moveVec, 0.35f);

		printf("chara = x:%f | y:%f | z:%f\n", charaForwardVec.x, charaForwardVec.y, charaForwardVec.z);
		printf("moveV = x:%f | y:%f | z:%f\n", moveVec.x, moveVec.y, moveVec.z);

		m_owner->RotateToNewForward(moveVec);
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


	bool toggleWalk = m_player->GetToggleWalk();
	bool toggleSprint = m_player->GetToggleSprint();
	bool toggleCrouch = m_player->GetToggleCrouch();
	//-------------------------------------------------------------------------------+
    // 「Sprint」トグル制御
    //-------------------------------------------------------------------------------+
	// 走り状態時、入力されていなかったら走りトグル解除
	if (toggleSprint && !(pressW || pressA || pressS || pressD))
	{
		m_player->SetToggleSprint(false);
	}
	// 「走る」ボタン=左SHIFTが押されているかを取得
	// しゃがみ状態時は走りへ移行しない
	// 移動速度を走りに設定
	if (!toggleSprint && !toggleCrouch)
	{
		toggleSprint = INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_LSHIFT);

		if (toggleSprint)
		{
			m_velocity = SPEED_SPRINT;
			m_player->SetToggleSprint(toggleSprint);
		}
	}

	//-------------------------------------------------------------------------------+
    // 「Walk」トグル制御
    //-------------------------------------------------------------------------------+
	// 歩き状態のトグル更新
	// Xキーで切り替え
	bool key = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_X);
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
	// しゃがみ状態の時左Ctrlで解除
	if (toggleCrouch)
	{
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL))
		{
			m_player->SetToggleCrouch(false);
		}
	}
	// 左Ctrl押下でしゃがみ状態へ移行
	if (!toggleCrouch)
	{
		toggleCrouch = INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LCTRL);

		// 走り状態を解除
		// 移動速度を歩きに設定
		if (toggleCrouch)
		{
			m_velocity = SPEED_WALK;
			m_player->SetToggleSprint(false);
			m_player->SetToggleCrouch(true);
		}

	}

	//-------------------------------------------------------------------------------+
    // 移動速度更新
    //-------------------------------------------------------------------------------+
	// 歩き状態でない、しゃがみ状態でない、いずれかに移動している場合
	// 移動速度をジョグ(小走り)に設定
	if(!toggleSprint && !toggleCrouch && !(pressW || pressA || pressS || pressD))
	{
		m_velocity = SPEED_JOG;
	}


	// キー入力WASDによる移動処理
	if (pressW)
	{
		inputAxis.y -= 1.0f;
	}
	if (pressS)
	{
		inputAxis.y += 1.0f;
	}
	if (pressA)
	{
		inputAxis.x -= 1.0f;
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
	angleSign = (tmpVec.z > 0.0f) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	inputAxis = Vector2::Transform(inputAxis, rot);

	// 前進ベクトルと右方向ベクトルから移動量を算出
	Vector3 moveVec = Vector3::Zero;
	moveVec = Vector3(inputAxis.x, inputAxis.y, 0.0f);

	// プレイヤーの武器出し状態の取得
	bool toggleWeaponOut = m_player->GetToggleWeaponOut();

	// 入力キーの総和が一定以上であれば、キャラを入力方向に回転させる
	// 武器出し状態のときはキャラの向きをカメラに固定させるため処理を分岐
	if (!toggleWeaponOut && (moveVec.LengthSq() >= 0.5f))
	{

		// キャラの前進ベクトル
	    Vector3 charaForwardVec = m_owner->GetForward();
		//charaForwardVec.Normalize();

		// 方向キー入力
		//charaForwardVec = moveVec;
	    moveVec.Normalize();

		// 進行方向に向けて回転(Lerp処理)
		if (charaForwardVec.y - moveVec.y >= 1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		if (charaForwardVec.y - moveVec.y <= -1.01f)
		{
			moveVec.x = 1.0f;
			moveVec.y = 0.0f;
		}
		charaForwardVec = Vector3::Lerp(charaForwardVec, moveVec, 0.51f);

		printf("chara = x:%f | y:%f | z:%f\n", charaForwardVec.x, charaForwardVec.y, charaForwardVec.z);
		printf("moveV = x:%f | y:%f | z:%f\n", moveVec.x, moveVec.y, moveVec.z);

		// rotationを更新
		m_owner->RotateToNewForward(charaForwardVec);
	}
	else if (toggleWeaponOut)
	{

		// キャラの前進ベクトルをカメラと同期
		Vector3 charaForwardVec = m_owner->GetForward();
		
		charaForwardVec = Vector3::Lerp(charaForwardVec, forwardVec, 0.51f);

		// rotationを更新
		m_owner->RotateToNewForward(charaForwardVec);
	}
	

	// プレイヤーの現在位置から、定義した方向へ速度分を加算
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += moveVec * m_velocity * in_deltaTime;

	// 最終的なプレイヤーの座標をセット
	m_owner->SetPosition(resultPos);
}
