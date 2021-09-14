//-----------------------------------------------------------------------+
// アクタークラス
// 機能：アクター(基本的にワールド空間に座標を持つもの)のベースクラス
// 特性：ベースクラス
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>
#include "Tag.h"
#include "ActorDebugObject.h"

class Actor
{

public:


	// アクターのステート(状態)
	enum ACTOR_STATE
	{
		STATE_ACTIVE,            // アクティブである
		STATE_PAUSED,            // 停止中である
		STATE_DEAD,              // 削除判定を行う
		STATE_ALLSTATE           // 全ステート
	};

	Actor(OBJECT_TAG _tag);
	virtual ~Actor();     // 仮想デストラクタ

	void Update(float _deltaTime);                  // 更新処理(総合)
	void UpdateComponents(float _deltaTime);        // 更新処理 (コンポーネント)
	virtual void UpdateActor(float _deltaTime);     // 更新処理 (アクター固有処理:オーバーライド可能)
	void ProcessInput(float _deltaTime);            // 入力処理

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
	void SetRotation(const Quaternion& _rotation) { m_rotation = _rotation; m_recomputeWorldTransform = true; }

	// ワールド変換行列
	void ComputeWorldTransform();
	void SetWorldTransform(const Matrix4& _mat) { m_worldTransform = _mat; }
	const Matrix4& GetWorldTransform() const { return m_worldTransform; }
	// ベクトル
	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitX, m_rotation); }     // 前進
	void RotateToNewForward(const Vector3& _forward);                                              // 前進ベクトルのセット
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitY, m_rotation); }       // 右方向
	// スピード
	float GetSpeed() const { return m_speed; }
	void SetSpeed(float _speed) { m_speed = _speed; }
	// ステート
	Actor::ACTOR_STATE GetState() const { return m_state; }
	void SetState(Actor::ACTOR_STATE _state) { m_state = _state; }
	// 管理ID
	int GetID() { return m_ID; }
	// コンポーネント
	void AddComponent(class Component* _comp);
	void RemoveComponent(class Component* _comp);
	// メッシュコンポーネント
	class MeshComponent* GetMeshComponent() { return m_meshComp; }

	// 体力
	void Sethealth(unsigned int _health) { m_health = _health; }
	const unsigned int& GetHealth() const { return m_health; }
	void RecoverHealth(unsigned int _recoverVal);
	void ReduceHealth(unsigned int _damage);


protected:


	Actor::ACTOR_STATE m_state;                           // ステート(状態)
	OBJECT_TAG m_tag;                               // ベースクラス識別タグ

	Vector3 m_scale;                                // スケール(拡大・縮小率)
	Quaternion m_rotation;                          // 回転角
	Vector3 m_position;                             // ポジション
	Matrix4 m_worldTransform;                       // ワールド変換行列

	float m_radian;

	unsigned int m_health;                          // 体力

	float m_speed;                                  // 移動速度

	bool m_recomputeWorldTransform;                 // ワールド座標変換を行うか

	class MeshComponent* m_meshComp;                // メッシュコンポーネントのポインタ

	std::vector<class Component*> m_components;     // コンポーネント配列(アクターの持つコンポーネントを格納)

	int m_ID;                                       // アクターID
	static int m_globalActorNo;                     // ゲーム全体でのアクターの番号


	class ActorDebugObject* m_debugObj;
	// アクター用デバッグクラスのフレンド宣言
	friend class ActorDebugObject;

};