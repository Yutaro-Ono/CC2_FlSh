#include "PatrolPoint.h"

// コンストラクタ
PatrolPoint::PatrolPoint(const Vector3& in_pos)
	:Actor(OBJECT_TAG::STATIC_OBJECT)
	,m_isSelected(false)
{
	m_position = in_pos;
}

// デストラクタ
PatrolPoint::~PatrolPoint()
{
}
