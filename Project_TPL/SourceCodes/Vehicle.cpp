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
/// 乗車モードのアクティベートと乗車したアクターとのリンク
/// </summary>
/// <param name="_rideActor"> この乗り物に乗車したアクター </param>
void Vehicle::ActivateRideMode(Actor* _rideActor)
{
	m_isRide = true;
	m_rideActor = _rideActor;
}

/// <summary>
/// 乗車モードの解除と乗車中アクターとのリンクを解除
/// </summary>
void Vehicle::DeactivateRideMode()
{
	m_isRide = false;
	m_rideActor = nullptr;
}
