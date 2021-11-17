#pragma once
#include "GameMain.h"
#include "Tag.h"
#include <unordered_map>
#include <vector>

class PhysicsWorld
{
public:


	struct ColliderPairs
	{
		OBJECT_TAG pair1;
		OBJECT_TAG pair2;
	};


	PhysicsWorld();
	~PhysicsWorld();

	void DebugVisualizeCollisions(class GLSLprogram* _shader);
	void DrawCollisions(class GLSLprogram* _shader, const std::vector<class ColliderComponent*>& _colliderComps, const Vector3& _visualColor);

	// 当たり判定追加/削除関数
	void AddColliderComponent(class ColliderComponent* _collComp);
	void RemoveColliderComponent(class ColliderComponent* _collComp);

	void SetOneSideReactionColliderPair(OBJECT_TAG _noReactType, OBJECT_TAG _reactType);
	void SetDualReactionColliderPair(OBJECT_TAG _react1, OBJECT_TAG _react2);
	void SetSelfReactionCollider(OBJECT_TAG _selfReact);


	void UpdateCollision();                                                         // コリジョン
	
	void DebugShowBox();                                                      // デバッグ用ボックス表示(未実装)
	void ToggleDebugMode() { m_boolDebugMode = !m_boolDebugMode; }            // デバッグモード

private:

	void OneReactionMatch(ColliderPairs _pair);
	void DualReactionMatch(ColliderPairs _pair);
	void SelfReactionMatch(OBJECT_TAG _tag);



	void CreateLineColors();





	// 当たり判定コンポーネント格納用
	std::unordered_map<OBJECT_TAG, std::vector<class ColliderComponent*>> m_colliderComps;

	std::vector<ColliderPairs> m_oneSideReactions;        // 片方のみリアクションを行う当たり判定ペアのリスト
	std::vector<ColliderPairs> m_dualReactions;           // 双方向でリアクションを行う当たり判定ペアのリスト
	std::vector<OBJECT_TAG> m_selfReactions;              // 同グループ内での処理を行う当たり判定リスト

	std::vector<Vector3> m_lineColors;                    // 当たり判定ボックスのカラー(Debug時に使用)

	bool m_boolDebugMode;

	void PlayerAndBGTest();                                                   // プレイヤーと壁の当たり判定テスト

};