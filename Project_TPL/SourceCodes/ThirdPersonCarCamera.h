#pragma once
#include "CameraComponent.h"


class ThirdPersonCarCamera : public CameraComponent
{
public:

	ThirdPersonCarCamera(class PlayerCar* in_owner);
	~ThirdPersonCarCamera();

	void Update(float in_deltaTime) override;           // 更新処理

	void ProcessInput(float in_deltaTime) override;     // 入力処理

	//-----------------------------------------------------------+
	// Getter/Setter
	//-----------------------------------------------------------+
	float GetPitch() const { return m_pitch; }          // ピッチのゲッター
	float GetYaw() const { return m_yaw; }              // ヨーのゲッター
	void SetDistance(float in_dist);                    // カメラ距離のセッター
	// カメラ追従処理を行うかどうかのセッター
	void SetChaseOwnerForward(bool in_chase) { m_chaseOwnerForward = in_chase; }
	// カメラ前進ベクトルのゲッター・セッター
	const Vector3& GetForward() { return m_forwardVec; }
	void SetCameraForward(const Vector3& in_forward) { m_forwardVec = in_forward; }

private:

	// オーナーの後方から一定距離にカメラ位置を調整し、その座標を返す
	const Vector3& ComputeCameraPos() const;

	Vector3 m_offset;                              // 三人称視点時ターゲットからの距離オフセット

	Vector3 m_forwardVec;                          // カメラの前進ベクトル
	Vector3 m_upVec;                               // カメラの上方ベクトル

	Vector3 m_velocity;                            // カメラ加速度

	Vector3 m_ownerForward;                        // オーナーアクタの移動時に使用する前進ベクトル(カメラ基準)

	float m_pitch;                                 // ピッチ関連

	float m_yaw;                                   // ヨー速度(/秒)

	float m_distance;                              // ターゲットまでの距離

	// カメラの前進ベクトルをオーナーアクタの前進ベクトルへ追従させるか (デフォルト：OFF)
	bool m_chaseOwnerForward;

	class PlayerCar* m_playerCar;                  // ターゲットアクタ(乗り物)

	// マウス入力
	Vector2 m_mousePos;                            // マウスの座標
	Vector2 m_frameMousePos;                       // マウスの前フレームの座標

	static const float CAMERA_SENSITIVITY;
	static const Vector3 DEFAULT_DISTANCE_OFFSET;
	static const float MIN_TARGET_DISTANCE;        // ターゲットまでの最短距離
	static const float MAX_TARGET_DISTANCE;        // ターゲットまでの最大距離
};