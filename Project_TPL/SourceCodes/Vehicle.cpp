#include "Vehicle.h"
#include "Component.h"

Vehicle::Vehicle(OBJECT_TAG _tag)
	:m_tag(_tag)
{
}

Vehicle::~Vehicle()
{
}

void Vehicle::Update(float _deltaTime)
{
}

void Vehicle::UpdateComponents(float _deltaTime)
{
}

void Vehicle::ProcessInput(float _deltaTime)
{
}

void Vehicle::AddComponent(Component* _comp)
{
}

void Vehicle::RemoveComponent(Component* _comp)
{
}

void Vehicle::RotateToNewForward(const Vector3& _forward)
{
}

void Vehicle::ComputeWorldTransform()
{
}
