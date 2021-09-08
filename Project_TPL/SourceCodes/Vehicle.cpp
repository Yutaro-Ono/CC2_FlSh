#include "Vehicle.h"

Vehicle::Vehicle(OBJECT_TAG _tag)
	:Actor(_tag)
	,m_rideActor(nullptr)
	,m_rideOffset(Vector3::Zero)
	,m_accel(false)
	,m_brake(false)
	,m_velocity(1.0f)
	,m_weight(1.0f)
	,m_friction(1.0f)
	,m_isRide(false)
{

}

Vehicle::~Vehicle()
{
}

/// <summary>
/// ��ԃ��[�h�̃A�N�e�B�x�[�g�Ə�Ԃ����A�N�^�[�Ƃ̃����N
/// </summary>
/// <param name="_rideActor"> ���̏�蕨�ɏ�Ԃ����A�N�^�[ </param>
void Vehicle::ActivateRideMode(Actor* _rideActor)
{
	m_isRide = true;
	m_rideActor = _rideActor;
}

/// <summary>
/// ��ԃ��[�h�̉����Ə�Ԓ��A�N�^�[�Ƃ̃����N������
/// </summary>
void Vehicle::DeactivateRideMode()
{
	m_isRide = false;
	m_rideActor = nullptr;
}
