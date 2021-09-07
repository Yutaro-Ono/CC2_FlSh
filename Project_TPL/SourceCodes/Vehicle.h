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
#include "Tag.h"
#include "Math.h"


class Vehicle
{

public:

	// 一般的な乗り物の状態
	enum class VEHICLE_STATE
	{
		STATE_ACTIVE = 0,
		STATE_PAUSED,
		STATE_DESTROY
	};

	Vehicle(OBJECT_TAG _tag);
	virtual ~Vehicle();

	void Update(float _deltaTime);                          // 更新処理総合
	void UpdateComponents(float _deltaTime);                // このクラスを親とするコンポーネントの更新処理
	virtual void UpdateVehicle(float _deltaTime) = 0;       // 継承先固有の更新処理(純粋仮想関数)
	void ProcessInput(float _deltaTime);                    // コンポーネント関連の入力情報更新

	void AddComponent(class Component* _comp);              // コンポーネントの追加処理
	void RemoveComponent(class Component* _comp);           // コンポーネントの削除処理

	void RotateToNewForward(const Vector3& _forward);       // 前進ベクトルの更新

	//-----------------------------------------------------------------------------------+
	// Getter / Setter
	//-----------------------------------------------------------------------------------+
	// ポジション
	const Vector3& GetPosition() const { return m_position; }
	void SetPosition(const Vector3& _pos) { m_position = _pos; m_recomputeWorldTransform = true; }
	// スケール
	const Vector3& GetScale() const { return m_scale; }
	void SetScale(float _scale) { m_scale = Vector3(_scale, _scale, _scale); m_recomputeWorldTransform = true; }
	void SetScale(const Vector3& _scalev3) { m_scale = _scalev3; m_recomputeWorldTransform = true; }
	// ローテーション
	const Quaternion& GetRotation() const { return m_rotation; }
	void SetRotation(const Quaternion& _rotate) { m_rotation = _rotate; m_recomputeWorldTransform = true; }

	// ワールド変換行列(※注意：セット関数を使用した場合、適切な変換計算が行われなくなります)
	void SetWorldTransform(const Matrix4& _worldMat) { m_worldTransform = _worldMat; }
	const Matrix4& GetWorldTransform() const { return m_worldTransform; }
	// ベクトル
	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitX, m_rotation); }     // 前進
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitY, m_rotation); }       // 右方向
	// ステート
	VEHICLE_STATE GetState() const { return m_state; }
	void SetState(VEHICLE_STATE _state) { m_state = _state; }


private:

	void ComputeWorldTransform();                    // ワールド変換行列の再計算処理



	VEHICLE_STATE   m_state;                         // 乗り物の状態
	OBJECT_TAG      m_tag;                           // このクラスのタグ

	// 座標系
	Vector3         m_scale;                         // 拡大率
	Quaternion      m_rotation;                      // 回転
	Vector3         m_position;                      // ワールド座標
	Matrix4         m_worldTransform;                // ワールド変換行列
	bool            m_recomputeWorldTransform;       // ワールド変換行列の再計算フラグ(負荷軽減策)

	bool            m_accel;                         // アクセルペダルを踏んでいるか
	bool            m_brake;                         // ブレーキペダルを踏んでいるか
	float           m_speed;                         // 乗り物のスピード
	float           m_velocity;                      // 加速度
	float           m_weight;                        // 乗り物の重量
	float           m_friction;                      // 摩擦係数

	bool            m_isRide;                        // プレイヤーに乗られているかどうか


};