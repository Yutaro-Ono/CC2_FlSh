#include "ShootComponent.h"
#include "WeaponBase.h"

ShootComponent::ShootComponent(WeaponBase* _weapon)
	:Component(_weapon)
{
}

ShootComponent::~ShootComponent()
{
}

void ShootComponent::Update(float _deltaTime)
{
}
