#include "PlayerStateBase.h"

// 小走り状態と判定する速度のライン
const float PlayerStateBase::JOG_SPEED_LINE = 0.55f;
// 歩き状態と判定する速度のライン
const float PlayerStateBase::WALK_SPEED_LINE = 0.05f;

PlayerStateBase::PlayerStateBase()
	:m_animSpeed(10.0f)
{
}
