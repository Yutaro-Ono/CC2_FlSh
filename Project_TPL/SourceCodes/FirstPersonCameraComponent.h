#pragma once
#include "CameraComponent.h"


class FirstPersonCameraComponent : public CameraComponent
{
public:
	FirstPersonCameraComponent(class Actor* _owner);
	~FirstPersonCameraComponent();
	void Update(float _deltaTime) override;

	void ProcessInput(float _deltaTime) override;

	void UpdateMouse(const float _rotate, float _deltaTime);
	void UpdateKeyBoard(const float _rotate, float _deltaTime);
	void UpdateGamePad(const float _rotate, float _deltaTime);

	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }
	void SetYawAngle(float _angle) { m_yaw = _angle; }
	void SetPitchAngle(float _angle) { m_pitch = _angle; }
	Vector3& GetCameraForwardVec() { return m_cameraForwardVec; }

	void SetHeadBoneNum(unsigned int _headBoneNum) { m_ownerHeadBoneNum = _headBoneNum; }

private:

	bool m_isActivePad;                 // パッドが有効かどうか
	float m_yaw;                        // カメラヨー(Z軸回転値)
	float m_pitch;                      // カメラピッチ(Y軸回転値)
	float m_roll;                       // カメラロール(X軸回転値)
	Vector3 m_cameraForwardVec;         // 前方方向ベクトル
	Vector3 m_viewTarget;               // カメラがターゲットする座標
	float m_cameraSensitivity;          // カメラ入力時の反応速度
	unsigned int m_ownerHeadBoneNum;    // ヘッドボーンの番号(座標をプレイヤーの頭に紐付ける際に使用)

	static const float MAX_PITCH_ANGLE;     // ピッチの最大角度
	static const float MIN_PITCH_ANGLE;     // ピッチの最小角度
	static const float CAMERA_SENSITIVITY;  // カメラの反応速度

};