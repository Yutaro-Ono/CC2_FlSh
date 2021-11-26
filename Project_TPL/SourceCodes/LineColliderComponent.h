//----------------------------------------------------------------------------------+
// @file        LineColliderComponent.h
// @brief       線分当たり判定コンポーネントクラス
// @note        ColliderComponentクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/11/26   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "ColliderComponent.h"


class LineColliderComponent : public ColliderComponent
{

public:

	LineColliderComponent(class Actor* _owner, int _updateOrder = 100);
	~LineColliderComponent();


	void         SetLine(const Line& line) { m_localLine = line; }
	const Line& GetLine() const { return m_worldLine; }

	void SetArrowRotate(bool value) { m_worldLine.m_rotatable = value; }
	void OnUpdateWorldTransform() override;
	bool CollisionDetection(ColliderComponent* _other) override;

protected:


	bool         Check(class BoxColliderComponent* _other) override;
	bool         Check(class WallColliderComponent* _other) override;



	Line         m_localLine;              // ローカル位置
	Line         m_worldLine;              // 線分


	friend class PhysicsWorld;
};