#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "ThirdPersonCamera.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "BoxColliderComponent.h"
#include "WallColliderComponent.h"
#include "LineColliderComponent.h"
#include "GLSLprogram.h"

// コンストラクタ
PhysicsWorld::PhysicsWorld()
	:m_boolDebugMode(false)
{
	std::cout << "CREATE : PhysicsWorld class Instance" << std::endl;

	CreateLineColors();


}

PhysicsWorld::~PhysicsWorld()
{
	std::cout << "DELETE : PhysicsWorld class Instance" << std::endl;
	//m_bgBoxes.clear();
}

/// <summary>
/// 当たり判定の可視化
/// </summary>
void PhysicsWorld::DebugVisualizeCollisions(GLSLprogram* _shader)
{
	if (!m_boolDebugMode)
	{
		return;
	}

	// 当たり判定ボックス描画 tag毎に色を変えてすべてのリスト表示
	int colorCount = 0;
	size_t colorNum = m_lineColors.size();
	for (auto t = OBJECT_TAG::BEGIN; t != OBJECT_TAG::END; ++t)
	{
		DrawCollisions(_shader, m_colliderComps[t], m_lineColors[colorCount % colorNum]);
		colorCount++;
	}
}

/// <summary>
/// 当たり判定の描画処理
/// </summary>
/// <param name="_colliderComps"> 描画する当たり判定 </param>
/// <param name="_visualColor"> 描画カラー </param>
void PhysicsWorld::DrawCollisions(GLSLprogram* _shader, const std::vector<class ColliderComponent*>& _colliderComps, const Vector3& _visualColor)
{
	// プロジェクション * ビュー 合成行列
	Matrix4 view, proj, projView;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	projView = proj * view;

	// ワールド変換行列作成用
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	// 描画用シェーダーの有効化
	_shader->UseProgram();
	_shader->SetUniform("u_projView", projView);
	_shader->SetUniform("u_color", _visualColor);

	for (auto item : _colliderComps)
	{
		// Boxだった場合の描画
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_BOX)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxColliderComponent*>(item)->GetWorldBox();

			// ボックスのスケールと位置を取得
			min = box.m_min;
			max = box.m_max;
			scale = max - min;
			pos = min;

			scaleMat = Matrix4::CreateScale(scale);
			posMat = Matrix4::CreateTranslation(pos);

			worldMat = scaleMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			RENDERER->SetActiveBoxVAO();
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		}
		// Wallだった場合の描画 
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_WALL||
			item->GetColliderType() == COLLIDER_TYPE::TYPE_SLOPE)
		{
			WallColliderComponent* wallcol;
			Wall walldata;

			Vector3 scale; // 描画スケーリング係数
			Vector3 pos; // 描画位置
			Vector3 normal; // 壁法線

			// WallColliderと壁データ取得
			wallcol = dynamic_cast<WallColliderComponent*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.m_normal;
			// 4点の中点を求める
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.m_wallVertices[i];
			}
			pos = pos * 0.25f;

			// 行列
			scaleMat = Matrix4::CreateScale(walldata.m_scale);
			rotMat = Matrix4::CreateRotationZ(walldata.m_zRotate);
			posMat = Matrix4::CreateTranslation(pos);
			slopeRot = Matrix4::CreateRotationY(walldata.m_slopeAngle);

			worldMat = scaleMat * slopeRot * rotMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			RENDERER->SetActiveSquareVAO();
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);
		}
		// LINEだった場合
		if (item->GetColliderType() == COLLIDER_TYPE::TYPE_LINE)
		{
			LineColliderComponent* linecol;
			Line line;

			linecol = dynamic_cast<LineColliderComponent*>(item);

			Vector3 scale;  // 描画スケーリング係数
			Vector3 pos;    // 描画位置
			line = linecol->GetLine();

			// 線分ベクトルと正規化線分方向ベクトル求める
			Vector3 lineVec = line.m_endPoint - line.m_startPoint;
			Vector3 lineDir = lineVec;
			lineDir.Normalize();

			//線分長からスケーリング行列作成
			float len = lineVec.Length();
			Matrix4 scaleMat = Matrix4::CreateScale(len, 0, 0);

			// 線分から回転軸と回転角を求めクオータニオン作成
			Vector3 rotAxis;
			rotAxis = Vector3::Cross(lineDir, Vector3(1, 0, 0));
			float rotAngle = -1.0f * acosf(Vector3::Dot(rotAxis, lineDir));

			// クオータニオンから線分の方向に回転する行列を作成
			Quaternion q(rotAxis, rotAngle);
			Matrix4 rotMat = Matrix4::CreateFromQuaternion(q);

			// 平行移動成分
			Matrix4 posMat = Matrix4::CreateTranslation(line.m_startPoint);


			worldMat = scaleMat * rotMat * posMat;
			_shader->SetUniform("u_worldTransform", worldMat);

			//描画
			RENDERER->SetActiveLineVAO();
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		}
	}
}


/// <summary>
/// 当たり判定コンポーネントの追加処理
/// </summary>
/// <param name="_collComp"> 追加する当たり判定コンポーネント </param>
void PhysicsWorld::AddColliderComponent(ColliderComponent* _collComp)
{
	// 指定したタグ番目にコンポーネントを追加
	OBJECT_TAG tag = _collComp->GetOwnerTag();
	m_colliderComps[tag].emplace_back(_collComp);
}

/// <summary>
/// 指定した当たり判定コンポーネントの削除
/// </summary>
/// <param name="_collComp"> 削除したい当たり判定コンポーネント </param>
void PhysicsWorld::RemoveColliderComponent(ColliderComponent* _collComp)
{
	OBJECT_TAG tag = _collComp->GetOwnerTag();
	// タグによって絞り込んだ連想配列内を検索して取得
	std::vector<ColliderComponent*>::iterator itr = std::find(m_colliderComps[tag].begin(), m_colliderComps[tag].end(), _collComp);
	// 配列内に存在していた場合、コンポーネントを消去
	if (itr != m_colliderComps[tag].end())
	{
		m_colliderComps[tag].erase(itr);
		return;
	}
}

/// <summary>
/// 片方のみリアクションする当たり判定ペアの登録
/// </summary>
/// <param name="_noReactType"> リアクションを返さない方のタグ </param>
/// <param name="_reactType"> もう片方からの影響を受ける方のタグ </param>
void PhysicsWorld::SetOneSideReactionColliderPair(OBJECT_TAG _noReactType, OBJECT_TAG _reactType)
{
	ColliderPairs pair;

	pair.pair1 = _noReactType;
	pair.pair2 = _reactType;

	m_oneSideReactions.emplace_back(pair);
}

/// <summary>
/// 双方向でリアクションする当たり判定ペアの登録
/// </summary>
/// <param name="_react1"> 当たり判定1のタグ </param>
/// <param name="_react2"> 当たり判定2のタグ </param>
void PhysicsWorld::SetDualReactionColliderPair(OBJECT_TAG _react1, OBJECT_TAG _react2)
{
	ColliderPairs pair;

	pair.pair1 = _react1;
	pair.pair2 = _react2;

	m_dualReactions.emplace_back(pair);
}

/// <summary>
/// 同グループ内で当たり判定処理するタグの登録
/// </summary>
/// <param name="_selfReact"> 当たり判定するタグ </param>
void PhysicsWorld::SetSelfReactionCollider(OBJECT_TAG _selfReact)
{
	m_selfReactions.emplace_back(_selfReact);
}


/// <summary>
/// 当たり判定の更新処理総合
/// </summary>
void PhysicsWorld::UpdateCollision()
{
	// 片方のみリアクションする当たり判定
	for (auto oneReactPair : m_oneSideReactions)
	{
		OneReactionMatch(oneReactPair);
	}

	// 双方向リアクションする当たり判定
	for (auto twoSideReactPair : m_dualReactions)
	{
		DualReactionMatch(twoSideReactPair);
	}

	// 同じリストの当たり判定
	for (auto selfReact : m_selfReactions)
	{
		SelfReactionMatch(selfReact);
	}

}

void PhysicsWorld::DebugShowBox()
{
}

/// <summary>
/// 片方のみリアクションの当たり判定ペアの衝突判定処理
/// </summary>
/// <param name="_pair"> 片方リアクションペア </param>
void PhysicsWorld::OneReactionMatch(ColliderPairs _pair)
{
	for (auto noreactColl : m_colliderComps[_pair.pair1])
	{
		for (auto reactColl : m_colliderComps[_pair.pair2])
		{
			if (noreactColl->CollisionDetection(reactColl))
			{
				reactColl->GetOwner()->OnCollisionEnter(reactColl, noreactColl);
			}
		}
	}
}

/// <summary>
/// 双方向リアクションの当たり判定ペアの衝突判定処理
/// </summary>
/// <param name="_pair"> 双方向リアクションペア </param>
void PhysicsWorld::DualReactionMatch(ColliderPairs _pair)
{
	for (auto coll1 : m_colliderComps[_pair.pair1])
	{
		for (auto coll2 : m_colliderComps[_pair.pair2])
		{
			if (coll1->CollisionDetection(coll2))
			{
				coll1->GetOwner()->OnCollisionEnter(coll1, coll2);
			}
		}
	}
}

/// <summary>
/// 同グループ内の衝突判定処理
/// </summary>
/// <param name="_tag"> 判定処理を行うグループタグ </param>
void PhysicsWorld::SelfReactionMatch(OBJECT_TAG _tag)
{
	// 同タグの当たり判定の登録数を保持
	size_t size = m_colliderComps[_tag].size();

	for (int i = 0; i < size; i++)
	{
		// 自分以外の当たり判定を総当たりでチェック
		for (int j = i + 1; j < size; i++)
		{
			if (m_colliderComps[_tag][i]->CollisionDetection(m_colliderComps[_tag][j]))
			{
				Actor* actor1 = m_colliderComps[_tag][i]->GetOwner();
				actor1->OnCollisionEnter(m_colliderComps[_tag][i], m_colliderComps[_tag][j]);
			}
		}
	}
}




/// <summary>
/// デバッグ用当たり判定ボックスカラーのセット
/// </summary>
void PhysicsWorld::CreateLineColors()
{
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 1.0f));
	m_lineColors.emplace_back(Vector3(1.0f, 0.0f, 0.0f));       // RED
	m_lineColors.emplace_back(Vector3(0.0f, 1.0f, 0.0f));       // GREEN
	m_lineColors.emplace_back(Vector3(0.0f, 0.0f, 1.0f));       // BLUE
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 0.0f));       // YELLOW
	m_lineColors.emplace_back(Vector3(1.0f, 0.71f, 0.76f));     // PINK
	m_lineColors.emplace_back(Vector3(0.56f, 0.93f, 0.56f));    // LIGHT GREEN
	m_lineColors.emplace_back(Vector3(1.0f, 1.0f, 0.88f));      // LIGHT YELLOW
	m_lineColors.emplace_back(Vector3(0.68f, 0.85f, 0.9f));     // LIGHT BLUE
}