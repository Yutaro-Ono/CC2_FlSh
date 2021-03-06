//----------------------------------------------------------------------------------+
// @file        WeaponAR4.h
// @brief       武器アクター：AR4
// @note        AR4の情報を持つ
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 10/12   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "WeaponBase.h"

class WeaponAR4 : public WeaponBase
{

public:

	WeaponAR4();
	WeaponAR4(class Actor* _owner);
	WeaponAR4(class Player* _owner);
	~WeaponAR4();

	void AdjustWorldMatToOwnerBone(const Matrix4& _boneWorldMat, float _deltaTime) override;

private:

	void Initialize();

	Vector3 m_adjustHolsteredIdlePos;          // 武器携行/待機時の調整座標
	Vector3 m_adjustUnholsteredIdlePos;        // 武器出し/待機時の調整座標
	Vector3 m_adjustUnholsteredMovePos;        // 武器出し/移動時の調整座標
	Vector3 m_adjustAimPos;                    // エイム時の調整座標

	Quaternion m_holsterIdleRot;
	float m_holsterRadianY;

	Quaternion m_unholsterIdleRot;
	float m_unholsterIdleRadianX;
	float m_unholsterIdleRadianY;
	float m_unholsterIdleRadianZ;

	Quaternion m_unholsterMoveRot;
	float m_unholsterMoveRadianX;
	float m_unholsterMoveRadianY;
	float m_unholsterMoveRadianZ;


	friend class DebugWeaponAR4;

protected:


	class FirstPersonCameraComponent* m_fpsCamera;

	static const unsigned int SOCKET_NUM_SPINE;
	static const unsigned int SOCKET_NUM_RIGHTHAND;
	static const unsigned int SOCKET_NUM_LEFTHAND;

	static const Vector3 ADJUST_POS_BASIC;
	static const Vector3 ADJUST_POS_IDLE_WEAPONOUT;
	static const Vector3 ADJUST_POS_MOVE_WEAPONOUT;

	static const std::string AR4_MESH_PATH;

};