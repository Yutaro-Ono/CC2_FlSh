#include "MotorBikeParent.h"

MotorBikeParent::MotorBikeParent()
	:Actor(OBJECT_TAG::VEHICLE)
{
	// 各パーツの生成
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
