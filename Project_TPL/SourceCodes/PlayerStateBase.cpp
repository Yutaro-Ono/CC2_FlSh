#include "PlayerStateBase.h"

// �������ԂƔ��肷�鑬�x�̃��C��
const float PlayerStateBase::JOG_SPEED_LINE = 0.45f;
// ������ԂƔ��肷�鑬�x�̃��C��
const float PlayerStateBase::WALK_SPEED_LINE = 0.05f;

PlayerStateBase::PlayerStateBase()
	:m_animSpeed(1.0f)
{
}