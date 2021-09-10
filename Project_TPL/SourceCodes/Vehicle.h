//----------------------------------------------------------------------------------+
// @file        Vehicle.h
// @brief       乗り物ベースクラスクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 9/ 7   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include "Tag.h"
#include "Math.h"
#include <vector>

class Vehicle : public Actor
{

public:

	Vehicle(OBJECT_TAG _tag);
	virtual ~Vehicle();

	void ActivateRideMode(class Actor* _rideActor);
	void DeactivateRideMode();

protected:



	class Actor*    m_rideActor;                     // 乗っているアクター
	Vector3         m_rideOffset;                    // 乗っているアクターの座標調整用ベクトル

	bool            m_accel;                         // アクセルペダルを踏んでいるか
	bool            m_brake;                         // ブレーキペダルを踏んでいるか
	float           m_speed;                         // 乗り物のスピード
	float           m_velocity;                      // 加速度
	float           m_weight;                        // 乗り物の重量
	float           m_friction;                      // 摩擦係数

	bool            m_isRide;                        // プレイヤーに乗られているかどうか

};