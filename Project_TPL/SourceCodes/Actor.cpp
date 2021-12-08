//-----------------------------------------------------------------------+
// アクタークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Debugger.h"
#include "Component.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "ActorDebugObject.h"
#include <algorithm>
#include <typeinfo>

int Actor::m_globalActorNo = 0;

// コンストラクタ
Actor::Actor(OBJECT_TAG _tag)
	:m_tag(_tag)
	,m_state(STATE_ACTIVE)
	,m_position(Vector3::Zero)
	,m_rotation(Quaternion::Identity)
	,m_scale(Vector3(1.0f, 1.0f, 1.0f))
	,m_health(10)
	,m_speed(0.0f)
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
	,m_debugObj(nullptr)
	,m_meshComp(nullptr)
	,m_skelComp(nullptr)
{
	// ゲームメインにこのアクターを追加登録
	GAME_INSTANCE.AddActor(this);
	// ゲーム全体のアクター番号を更新
	m_globalActorNo++;

	// デバッグオブジェクトの生成
#ifdef _DEBUG

	m_debugObj = new ActorDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, Debugger::DEBUG_TAG::ACTOR);

#endif
}

// デストラクタ
Actor::~Actor()
{
	// アクターが所持するコンポーネントの削除
	while (!m_components.empty())
	{
		delete m_components.back();
	}

	// ゲームメインにこのアクターの削除を依頼
	GAME_INSTANCE.RemoveActor(this);
}

// 更新処理 (Gameからコール)
// 引数 : _deltaTime => 1フレーム分の経過時間
void Actor::Update(float _deltaTime)
{
	// アクターがアクティブなら
	if (m_state == STATE_ACTIVE)
	{
		// ワールド変換行列を計算
		ComputeWorldTransform();

		// コンポーネントの更新処理
		UpdateComponents(_deltaTime);
		// アクター固有の更新処理
		UpdateActor(_deltaTime);

		// ワールド変換行列の再計算
		ComputeWorldTransform();
	}
}

// 更新処理 (コンポーネント)
// 引数 : _deltaTime => 1フレーム分の経過時間
void Actor::UpdateComponents(float _deltaTime)
{
	for (auto comp : m_components)
	{
		comp->Update(_deltaTime);
	}
}

// 更新処理 (アクター固有処理:オーバーライド可能)
// 引数 : _deltaTime => 1フレーム分の経過時間
void Actor::UpdateActor(float _deltaTime)
{
}

// 入力処理
void Actor::ProcessInput(float _deltaTime)
{
	if (m_state == STATE_ACTIVE)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : m_components)
		{
			comp->ProcessInput(_deltaTime);
		}
	}
}

/// <summary>
/// (当たり判定有効時)衝突判定時のリアクション処理:各クラスで必要時にオーバーライドして使用
/// </summary>
/// <param name="_ownCollComp"> このアクターが持つ当たり判定コンポーネント </param>
/// <param name="_otherCollComp"> 他アクターの当たり判定コンポーネント </param>
void Actor::OnCollisionEnter(ColliderComponent* _ownCollComp, ColliderComponent* _otherCollComp)
{
	// 処理なし(継承先でオーバーライドして使用)
}

// ワールド変換行列を計算
void Actor::ComputeWorldTransform()
{
	// ワールド変換の再計算が必要なら実行
	if (m_recomputeWorldTransform)
	{

		// スケーリング→回転→平行移動となるように変換行列を作成
		m_worldTransform = Matrix4::CreateScale(m_scale);
		m_worldTransform *= Matrix4::CreateFromQuaternion(m_rotation);
		m_worldTransform *= Matrix4::CreateTranslation(m_position);

		// アクターが持っている全コンポーネントの変換を行う
		for (auto comp : m_components)
		{
			comp->OnUpdateWorldTransform();
		}

		m_recomputeWorldTransform = false;
	}
}


// 前進ベクトルの更新
// _forward => 向かせたい前方方向ベクトル
void Actor::RotateToNewForward(const Vector3 & _forward)
{
	// X軸ベクトル(1, 0, 0)とforward間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, _forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.99f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.99f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸を求めて回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, _forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}


// コンポーネントの追加
void Actor::AddComponent(Component * _comp)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = _comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// 要素を見つけたポイントの手前に挿入する
	m_components.insert(iter, _comp);
}

// コンポーネントの削除
void Actor::RemoveComponent(Component * _comp)
{
	// 引数のコンポーネントをコンポーネント配列から検索し、そのイテレータを削除
	auto iter = std::find(m_components.begin(), m_components.end(), _comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}

/// <summary>
/// スケルタルメッシュコンポーネントのゲッター
/// </summary>
/// <returns> スケルタルメッシュが生成されていたらポインタを返す </returns>
SkeletalMeshComponent* Actor::GetSkelComp()
{
	if (m_skelComp == nullptr)
	{
		std::cout << "WARNING::Not Create::SkeletalMeshComponent" << std::endl;
		return nullptr;
	}

	return m_skelComp;
}

/// <summary>
/// 体力の回復
/// </summary>
/// <param name="_recoverVal"> 回復量 </param>
void Actor::RecoverHealth(unsigned int _recoverVal)
{
	unsigned int recover = _recoverVal;

	while (recover > 0)
	{
		if (m_health <= 4294967295)
		{
			break;
		}

		// 1ずつ回復
		m_health++;
		// 回復量を減らす
		recover--;
	}
}

/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="_damage">ダメージ量</param>
void Actor::ReduceHealth(unsigned int _damage)
{
	unsigned int damage = _damage;

	while (damage > 0)
	{

		if (m_health <= 0)
		{
			break;
		}

		// 1ずつ減らす
		m_health--;

		// ダメージ総量を減らす
		damage--;
	}
}
