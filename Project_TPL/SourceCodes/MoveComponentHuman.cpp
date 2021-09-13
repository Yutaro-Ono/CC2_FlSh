#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "ThirdPersonCamera.h"

const float MoveComponentHuman::PLAYER_SPEED = 45.0f;
const float MoveComponentHuman::SPEED_WALK = 45.0f;
const float MoveComponentHuman::SPEED_JOG = 80.0f;
const float MoveComponentHuman::SPEED_RUN = 120.0f;


// コンストラクタ
MoveComponentHuman::MoveComponentHuman(Actor* in_owner)
	:MoveComponent(in_owner)
	,m_toggleRun(false)
	,m_velocity(SPEED_WALK)
{
	
}

// デストラクタ
MoveComponentHuman::~MoveComponentHuman()
{
}

// 更新処理
void MoveComponentHuman::Update(float in_deltaTime)
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
void MoveComponentHuman::MovementByController(float in_deltaTime)
{
	// 左スティック入力値の取得
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	//std::cout << "StickL::x = " << axisL.x << "::  ::y = " << axisL.y << std::endl;
	float axisLength = axisL.LengthSq();
	// 入力閾値
	float axisThreshold = 0.01f;

	// 走り状態時、入力閾値が一定以下になったら走りトグル解除
	if (m_toggleRun && axisLength < axisThreshold && axisLength > -axisThreshold)
	{
		m_toggleRun = false;
	}
	// 「走る」ボタンが押されているかを取得
	// 通常はLBボタン
	if (!m_toggleRun)
	{
		m_toggleRun = CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	}


	
	// 「走る」ボタンが押されていた時、同時に左スティック入力もされていた場合
	if (m_toggleRun && CONTROLLER_INSTANCE.GetIsInputAxisL())
	{
		m_velocity = SPEED_RUN;
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
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{
	// キー入力値
	Vector2 inputAxis = Vector2::Zero;

	// キー入力WASDによる移動処理
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
	{
		inputAxis.x += 1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
	{
		inputAxis.x += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
	{
		inputAxis.y += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
	{
		inputAxis.y += 1.0f;
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
	resultPos += moveVec * PLAYER_SPEED * in_deltaTime;

	// 最終的なプレイヤーの座標をセット
	m_owner->SetPosition(resultPos);
}
