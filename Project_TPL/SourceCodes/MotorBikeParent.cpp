#include "MotorBikeParent.h"

MotorBikeParent::MotorBikeParent()
	:Actor(OBJECT_TAG::VEHICLE)
{
	// �e�p�[�c�̐���
	for (int i = 0; i < static_cast<int>(MOTORBIKE_PARTS::ALL); i++)
	{

	}
}

MotorBikeParent::~MotorBikeParent()
{
}

void MotorBikeParent::UpdateActor(float _deltaTime)
{
}
